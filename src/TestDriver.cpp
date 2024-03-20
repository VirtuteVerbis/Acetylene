// Acetylene ~ Test Driver Implementation
// Record 01-0012
// 2024.03.17 @ 20.30

// File:	TestDriver.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.17 @ 20.30
// Purpose:	Implement the main entry point to the TestDriver program.

#include "TestDriver.h"

// Declare static TestDriver variables.

TitleScene TestDriver::TITLE;
OptionsScene TestDriver::OPTIONS;
GameScene TestDriver::GAME;

// Implement TestDriver main entry point.

int main(int argc, char** argv) {
	srand((unsigned int)time(nullptr));
	if (!OptionsScene::OPTIONS.loadOptions()) {
		if (!OptionsScene::OPTIONS.saveOptions()) {
			return EXIT_FAILURE;
		}
	}
	App::Configuration config(TestDriver::TITLE);
	config.window.title = "TestDriver";
	config.window.dimensions = OptionsScene::OPTIONS.resolution;
	config.window.fullscreen = OptionsScene::OPTIONS.fullscreen;
	config.audio.volume = OptionsScene::OPTIONS.volume;
	if (!App::Init(config)) {
		return EXIT_FAILURE;
	}
	App::Run();
	App::Destroy();
	return EXIT_SUCCESS;
}