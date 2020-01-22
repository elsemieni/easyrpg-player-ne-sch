/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

//netherware

#include "system.h"

#ifdef HAVE_FLUIDSYNTH

// Headers
#include <cassert>
#include "audio_decoder.h"
#include "output.h"
#include "decoder_fluidsynth.h"
#include "main_data.h"

#include <fluidsynth.h>

FluidSynthDecoder::FluidSynthDecoder() {
	music_type = "midi";

	settings = new_fluid_settings();
	fluid_settings_setstr(settings, "player.timing-source", "sample");
	fluid_settings_setint(settings, "synth.lock-memory", 0);

	fluid_settings_setnum(settings, "synth.gain", 0.6);
	fluid_settings_setint(settings, "synth.polyphony", 32);

	synth = new_fluid_synth(settings);
	//netherware fix, if it's osx load it from SDL defined directory.
#if defined(__APPLE__)
    char soundfontpath[512];
    sprintf(soundfontpath, "%s%s", Main_Data::GetProjectPath().c_str(), "easyrpg.soundfont");
	if (fluid_synth_sfload(synth, soundfontpath, 1) == FLUID_FAILED) {
		error_message = "Could not load soundfont (OSX way).";
		return;
	}
#else
	if (fluid_synth_sfload(synth, "easyrpg.soundfont", 1) == FLUID_FAILED) {
		error_message = "Could not load soundfont (otherOS way).";
		return;
	}
#endif

	fluid_synth_set_interp_method(synth, -1, 7);

	double sample_rate = 0;
	fluid_settings_getnum(settings, "synth.sample-rate", &sample_rate);
	assert(sample_rate != 0);
	frequency = sample_rate;

	seq = new_fluid_sequencer2(false);
	if (fluid_sequencer_register_fluidsynth(seq, synth) == FLUID_FAILED) {
		error_message = "Could not initialize MIDI playback.";
		return;
	}

	init = true;
}

FluidSynthDecoder::~FluidSynthDecoder() {
	if (player)
		delete_fluid_player(player);
	if (seq)
		delete_fluid_sequencer(seq);
	if (synth)
		delete_fluid_synth(synth);
	if (settings)
		delete_fluid_settings(settings);
}

bool FluidSynthDecoder::WasInited() const {
	return init;
}

bool FluidSynthDecoder::Open(FILE* file) {
	if (!init)
		return false;

	if (player) {
		fluid_player_stop(player);
		delete_fluid_player(player);
	}

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char*) malloc(size);
	if (!buffer) {
		error_message = "Not enough memory to load MIDI file";
		return false;
	}

	if(fread(buffer, size, 1, file) == 1) {

		player = new_fluid_player(synth);

		if (fluid_player_add_mem(player, buffer, size) == FLUID_OK) {
			free(buffer);

			if (fluid_player_play(player) == FLUID_FAILED) {
				error_message = "Could not start MIDI playback.";
				return false;
			}
			fluid_player_set_loop(player, -1);

			fclose(file);
			return true;
		} else {
			error_message = "FluidSynth failed to load in-memory song";
		}
	}
	free(buffer);

	return false;
}

bool FluidSynthDecoder::Seek(size_t offset, Origin origin) {
	// FIXME: only restart allowed for now
	if (offset == 0 && origin == Origin::Begin) {
		if (init) {
			fluid_player_stop(player);
			fluid_player_set_loop(player, -1);
			fluid_player_play(player);
		}
		return true;
	}

	return false;
}

bool FluidSynthDecoder::IsFinished() const {
	return fluid_player_get_status(player) != FLUID_PLAYER_PLAYING;
}

void FluidSynthDecoder::GetFormat(int& freq, AudioDecoder::Format& format, int& chans) const {
	freq = frequency;
	format = Format::S16;
	chans = 2;
}

bool FluidSynthDecoder::SetFormat(int freq, AudioDecoder::Format format, int chans) {
	if (freq != frequency || chans != 2 || format != Format::S16)
		return false;

	return true;
}

bool FluidSynthDecoder::SetPitch(int pitch) {
	if (pitch != 100) {
		return false;
	}

	return true;
}

int FluidSynthDecoder::FillBuffer(uint8_t* buffer, int length) {
	if(!init)
		return 0;

	if (fluid_synth_write_s16(synth, length / 4, buffer, 0, 2, buffer, 1, 2) == FLUID_FAILED) {
		error_message = "Fluidsynth error: %s", fluid_synth_error(synth);
	}

	return length;
}

int FluidSynthDecoder::GetTicks() const {
	if (!init)
		return 0;

	return fluid_sequencer_get_tick(seq);
}

#endif