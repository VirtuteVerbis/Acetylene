// Acetylene ~ Test Driver Title Scene Implementation
// Record 01-0013
// 2024.03.17 @ 20.34

// File:	TitleScene.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.17 @ 20.34
// Purpose:	Implement the functions declared in Scenes.h for the title scene of
//			the TestDriver program.

#include "TestDriver.h"

#include <random>

// Implement TitleScene class functions.

bool TitleScene::init() {
	m_UIShader = App::Graphics.getDefaultShader();
	m_UICamera.init();
	m_UIShader.setCamera(m_UICamera);
	m_UI.init(*this, 0, "Assets/fonts/Work_Sans/static/WorkSans-Regular.ttf",
		0.25f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_UI.setShader(m_UIShader);
	m_startButton.init(glm::vec3(0.25f, 0.5f, 0.0f), glm::vec2(0.5f, 0.1f),
		"Assets/textures/UI/button.png", "START GAME");
	m_UI.addComponent(m_startButton);
	m_optionsButton.init(glm::vec3(0.25f, 0.35f, 0.0f), glm::vec2(0.5f, 0.1f),
		"Assets/textures/UI/button.png", "OPTIONS");
	m_UI.addComponent(m_optionsButton);
	m_exitButton.init(glm::vec3(0.25f, 0.2f, 0.0f), glm::vec2(0.5f, 0.1f),
		"Assets/textures/UI/button.png", "EXIT");
	m_UI.addComponent(m_exitButton);
	ACLOG(TitleScene, Message, "Initialized title scene.");
	return true;
}

void TitleScene::enter(Scene& prev) {
	glm::ivec2 wd = App::Window.getDimensions();
	m_UICamera.position = glm::vec3((float)wd.x / 2.0f,
		(float)wd.y / 2.0f, 0.0f);
	ACLOG(TitleScene, Message, "Camera pos: (", m_UICamera.position.x, ", ",
		m_UICamera.position.y, ").");
	App::Audio.playMusic("Assets/audio/low_droning.wav");
	ACLOG(TitleScene, Message, "Entered title scene from ", &prev, ".");
}

void TitleScene::draw() {
	m_UI.draw();
	App::Graphics.submit(m_embers, m_UIShader);
}

bool TitleScene::processInput() {
	m_UI.processInput();
	return m_running;
}

void TitleScene::componentEvent(int gID, int cID, int eID) {
	if (gID == m_UI.getID()) {
		if (cID == m_startButton.getID()) {
			if (eID == Button::ClickedEvent) {
				ACLOG(TitleScene, Message, "Start button clicked.");
				App::Audio.playEffect("Assets/audio/click.wav");
				App::SetScene(TestDriver::GAME);
			}
		}
		else if (cID == m_optionsButton.getID()) {
			if (eID == Button::ClickedEvent) {
				ACLOG(TitleScene, Message, "Options button clicked.");
				App::Audio.playEffect("Assets/audio/click.wav");
				App::SetScene(TestDriver::OPTIONS);
			}
		}
		else if (cID == m_exitButton.getID()) {
			if (eID == Button::ClickedEvent) {
				ACLOG(TitleScene, Message, "Exit button clicked.");
				App::Audio.playEffect("Assets/audio/click.wav");
				m_running = false;
			}
		}
	}
}

void TitleScene::update(float dt) {
	m_UICamera.update(dt);
	m_UI.update(dt);
	for (Ember& e : m_embers) {
		e.update(dt);
	}
	std::vector<Ember>::iterator rit = m_embers.begin();
	while (rit != m_embers.end()) {
		if (!rit->isAlive()) {
			rit = m_embers.erase(rit);
		}
		else {
			rit++;
		}
	}
	if (m_timer > 7.0f) {
		m_embers.push_back(Ember());
		m_embers.back().init(
			glm::vec3((float)(rand() % App::Window.getDimensions().x), -10.0f,
				-0.1f));
		m_timer = 0.0f;
	}
	m_timer += dt;
}

void TitleScene::leave(Scene& next) {
	for (Ember& e : m_embers) {
		e.destroy();
	}
	m_embers.clear();
	ACLOG(TitleScene, Message, "Left title scene for ", &next, ".");
}

void TitleScene::destroy() {
	m_UICamera.destroy();
	m_UIShader.destroy();
	m_UI.destroy();
	for (Ember& e : m_embers) {
		e.destroy();
	}
	m_embers.clear();
	ACLOG(TitleScene, Message, "Destroyed title scene.");
}