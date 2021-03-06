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

#ifndef EP_OPTIONS_H
#define EP_OPTIONS_H

/** Window title to show. */
#define GAME_TITLE ""

/** Targeted screen default width. */
#define SCREEN_TARGET_WIDTH 320

/** Targeted screen default height. */
#define SCREEN_TARGET_HEIGHT 240

/** Working with hi resolutions. default 16 */
#define TILE_SIZE 16

/** Targeted screen bits per pixel. */
#define SCREEN_TARGET_BPP 32

/** Allows user to change fullscreen state. */
#define ALLOW_FULLSCREEN_TOGGLE 1

/** Run game in fullscreen mode. */
#define RUN_FULLSCREEN 0

/** Run game with this magnification factor. */
#define RUN_ZOOM 3

/**
 * Pause the game process when the player window
 * looses its focus.
 */
#define PAUSE_GAME_WHEN_FOCUS_LOST 1

/**
 * Pause the audio process when the player window
 * looses its focus.
 */
#define PAUSE_AUDIO_WHEN_FOCUS_LOST 1

/** INI configuration filename. */
#define INI_NAME "RPG_RT.ini"

/** Database filename. */
#define DATABASE_NAME "RPG_RT.ldb"
#define DATABASE_NAME_EASYRPG "EASY_RT.edb"

/** Map tree filename. */
#define TREEMAP_NAME "RPG_RT.lmt"
#define TREEMAP_NAME_EASYRPG "EASY_RT.emt"

/** File name for additional metadata, such as multi-game save imports. */
#define META_NAME "easyrpg.ini"

/**
 * RPG_RT.exe (official engine) filename.
 * Not used by emscripten.
 */
#define EXE_NAME "RPG_RT.exe"

/** Default fps rate. */
#define DEFAULT_FPS 60

/** Enables or disables font smoothing. */
#define FONT_SMOOTHING 0

/** Netherware fix: RPG_RT.ldb checksum */
#define MHW_CUNE_RPGRTLDB_CHECKSUM 168
#define MHW_CUNE_LOBBY_MAPID 71
#define MHW_CUNE_CLEAR_PASSWORD 7732

// OUTPUT_TYPE
//		OUTPUT_NONE - no output
//		OUTPUT_CONSOLE - print to console
//		OUTPUT_FILE - write to file
//		OUTPUT_MSGBOX - create pop up message box
//		OUTPUT_SCREEN - write to screen
//

/** Name of the file for output. */
#ifndef OUTPUT_FILENAME
#  define OUTPUT_FILENAME "sch_log.txt"
#endif

#define USE_KEYBOARD
#define USE_MOUSE
#define USE_JOYSTICK
#define USE_JOYSTICK_HAT
#define USE_JOYSTICK_AXIS
#define USE_TOUCH

//#define USE_FIXED_TIMESTEP_FPS

#endif
