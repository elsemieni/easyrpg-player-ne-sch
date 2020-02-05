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
#include <sstream>
#include <chrono>
#include <array>

#include "graphics.h"
#include "cache.h"
#include "output.h"
#include "player.h"
#include "fps_overlay.h"
#include "message_overlay.h"
#include "transition.h"
#include "scene.h"
#include "drawable.h"
#include "drawable_mgr.h"
#include "baseui.h"
#include "game_clock.h"

using namespace std::chrono_literals;

namespace Graphics {
	void UpdateTitle();
	void LocalDraw(Bitmap& dst, int priority = Priority::Priority_Maximum);
	void GlobalDraw(Bitmap& dst, int priority = Priority::Priority_Maximum);

	int framerate;

	Game_Clock::time_point next_fps_time;

	std::shared_ptr<Scene> current_scene;

	std::unique_ptr<MessageOverlay> message_overlay;
	std::unique_ptr<FpsOverlay> fps_overlay;
}

unsigned SecondToFrame(float const second) {
	return(second * Graphics::framerate);
}

void Graphics::Init() {
	Scene::Push(std::make_shared<Scene>());
	current_scene = Scene::instance;

	message_overlay.reset(new MessageOverlay());
	fps_overlay.reset(new FpsOverlay());

	next_fps_time = {};
}

void Graphics::Quit() {
	DrawableMgr::GetGlobalList().Clear();

	fps_overlay.reset();
	message_overlay.reset();

	Cache::Clear();
}

void Graphics::Update() {
	//FPS:
	auto now = Game_Clock::now();
	if (next_fps_time == Game_Clock::time_point()) {
		next_fps_time = now + 1s;
	}

	BitmapRef disp = DisplayUi->GetDisplaySurface();

	if (now >= next_fps_time) {
		next_fps_time += 1s;

		if (fps_overlay->GetFps() == 0) {
			Output::Debug("Framerate is 0 FPS!");
			Draw(*disp);
			Player::FrameReset(now);
		} else {
			next_fps_time = now + 1s;
			fps_overlay->ResetCounter();
		}

		UpdateTitle();
	}

	//Update Graphics:
	fps_overlay->Update();
	fps_overlay->AddUpdate();
	message_overlay->Update();
	Transition::instance().Update();
}

void Graphics::UpdateTitle() {
	if (DisplayUi->IsFullscreen()) return;
#ifdef EMSCRIPTEN
	return;
#endif

	std::stringstream title;
	if (!Player::game_title.empty()) {
		title << Player::game_title << "";
	}
	title << GAME_TITLE;

	if (Player::fps_flag) {
		title << " - " << fps_overlay->GetFpsString();
	}

	DisplayUi->SetTitle(title.str());
}

void Graphics::Draw(Bitmap& dst) {
	fps_overlay->AddFrame();

	BitmapRef disp = DisplayUi->GetDisplaySurface();

	if (Transition::instance().IsErased()) {
		DisplayUi->CleanDisplay();
		GlobalDraw(dst);
		DisplayUi->UpdateDisplay();
		return;
	}
	LocalDraw(dst);
	GlobalDraw(dst);
	DisplayUi->UpdateDisplay();
}

void Graphics::LocalDraw(Bitmap& dst, int priority) {
	auto& drawable_list = DrawableMgr::GetLocalList();

	if (!drawable_list.empty()) {
		current_scene->DrawBackground(dst);
	}

	drawable_list.Draw(dst, priority);
}

void Graphics::GlobalDraw(Bitmap& dst, int priority) {
	auto& drawable_list = DrawableMgr::GetGlobalList();

	drawable_list.Draw(dst, priority);
}


BitmapRef Graphics::SnapToBitmap(int priority) {
	BitmapRef disp = DisplayUi->GetDisplaySurface();
	LocalDraw(*disp, priority);
	GlobalDraw(*disp, priority);
	return DisplayUi->CaptureScreen();
}

void Graphics::FrameReset(Game_Clock::time_point now) {
	next_fps_time = now + 1s;
	fps_overlay->ResetCounter();
}

void Graphics::UpdateSceneCallback() {
	current_scene = Scene::instance;
	if (current_scene) {
		DrawableMgr::SetLocalList(&current_scene->GetDrawableList());
	} else {
		DrawableMgr::SetLocalList(nullptr);
	}
}

MessageOverlay& Graphics::GetMessageOverlay() {
	return *message_overlay;
}

