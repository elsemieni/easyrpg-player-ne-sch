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

#ifndef EP_DECODER_FLUIDSYNTH_H
#define EP_DECODER_FLUIDSYNTH_H

// Headers
#include <string>
#include <memory>
#ifdef HAVE_FLUIDSYNTH
#include <fluidsynth.h>
#endif
#include "audio_decoder.h"

/**
 * Audio decoder for MIDI powered by FluidSynth
 */
class FluidSynthDecoder : public AudioDecoder {
public:
    FluidSynthDecoder();

    ~FluidSynthDecoder();

    bool WasInited() const override;

    // Audio Decoder interface
    bool Open(FILE* file) override;

    bool Seek(size_t offset, Origin origin) override;

    bool IsFinished() const override;

    void GetFormat(int& frequency, AudioDecoder::Format& format, int& channels) const override;

    bool SetFormat(int frequency, AudioDecoder::Format format, int channels) override;

    bool SetPitch(int pitch) override;

    int GetTicks() const override;
private:
    int FillBuffer(uint8_t* buffer, int length) override;

#ifdef HAVE_FLUIDSYNTH
    fluid_settings_t *settings = NULL;
	fluid_synth_t *synth = NULL;
	fluid_player_t *player = NULL;
	fluid_sequencer_t *seq = NULL;
#endif

    int frequency = 44100;
    bool init = false;
};

#endif