// Acetylene ~ Test Driver Header
// Record 01-0010
// 2024.03.17 @ 15.59

// File:	TestDriver.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.17 @ 15.59
// Purpose:	Declare the main class of the TestDriver program.

#ifndef TD_TEST_DRIVER_H
#define TD_TEST_DRIVER_H

#include "Scenes.h"

// The main class of the TestDriver program, containing all three scenes.
class TestDriver {
public:
	// The title scene of the TestDriver program.
	static TitleScene TITLE;
	// The options editing scene of the TestDriver program.
	static OptionsScene OPTIONS;
	// The main game scene of the TestDriver program.
	static GameScene GAME;
};

#endif