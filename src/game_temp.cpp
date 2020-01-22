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

// Headers
#include "game_temp.h"
#include "transition.h"

int Game_Temp::battle_troop_id;
std::string Game_Temp::battle_background;
int Game_Temp::battle_formation;
int Game_Temp::battle_escape_mode;
int Game_Temp::battle_defeat_mode;
bool Game_Temp::battle_first_strike;
int Game_Temp::battle_result;
bool Game_Temp::battle_random_encounter;

void Game_Temp::Init() {
	battle_troop_id = 0;
	battle_background = {};
	battle_formation = 0;
	battle_escape_mode = -1;
	battle_defeat_mode = 0;
	battle_first_strike = false;
	battle_result = BattleAbort;
	battle_random_encounter = false;
}
