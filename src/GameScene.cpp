// Acetylene ~ Test Driver Game Scene
// Record 01-0015
// 2024.03.18 @ 14.14

// File:	GameScene.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.18 @ 14.14
// Purpose:	Implement the functions declared in Scenes.h for the game scene
//			of the TestDriver program.

#include "TestDriver.h"

// Implement GameScene class functions.
 
bool GameScene::init() {
	m_UIShader = App::Graphics.getDefaultShader();
	m_UICamera.init();
	m_UIShader.setCamera(m_UICamera);
	glm::ivec2 wd = App::Window.getDimensions();
	m_UICamera.position = glm::vec3((float)wd.x / 2.0f,
		(float)wd.y / 2.0f, 0.0f);
	m_pauseUI.init(*this, 0,
		"Assets/fonts/Work_Sans/static/WorkSans-Regular.ttf", 0.25f,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pauseUI.setShader(m_UIShader);
	m_continueButton.init(glm::vec3(0.5f - (0.33f / 2.0f), 0.5f, 0.1f),
		glm::vec2(0.33f, 0.08f), "Assets/textures/UI/button.png", "CONTINUE");
	m_pauseUI.addComponent(m_continueButton);
	m_optionsButton.init(glm::vec3(0.5f - (0.33f / 2.0f), 0.4f, 0.1f),
		glm::vec2(0.33f, 0.08f), "Assets/textures/UI/button.png", "OPTIONS");
	m_pauseUI.addComponent(m_optionsButton);
	m_quitButton.init(glm::vec3(0.5f - (0.33f / 2.0f), 0.3f, 0.1f),
		glm::vec2(0.33f, 0.08f), "Assets/textures/UI/button.png", "QUIT");
	m_pauseUI.addComponent(m_quitButton);
	m_pauseUI.setEnabled(false);
	m_pauseUI.setVisible(false);
	m_walls.resize(4);
	m_walls[0].init(glm::vec3(-1536.0f, -1536.0f, 0.0f),
		glm::vec2(1536.0f, 3328.0f), "Assets/textures/wall.png");
	m_walls[1].init(glm::vec3(0.0f, -1536.0f, 0.0f),
		glm::vec2(256.0f, 1536.0f), "Assets/textures/wall.png");
	m_walls[2].init(glm::vec3(256.0f, -1536.0f, 0.0f),
		glm::vec2(1536.0f, 3328.0f), "Assets/textures/wall.png");
	m_walls[3].init(glm::vec3(0.0f, 256.0f, 0.0f),
		glm::vec2(256.0f, 1536.0f), "Assets/textures/wall.png");
	for (size_t i = 0; i < 5; i++) {
		m_coins.push_back(Coin());
		m_coins.back().init(
			glm::vec3((rand() % 16) * 16.0f, (rand() % 16) * 16.0f, 0.0f));
	}
	m_player.init(glm::vec3(0.0f, 0.0f, 0.01f), *this);
	App::Graphics.getDefaultCamera().scale.scale = 2.0f;
	App::Graphics.getDefaultCamera().scale.velocity = 0.0f;
	ACLOG(GameScene, Message, "Initialized game scene.");
	return true;
}

void GameScene::enter(Scene& prev) {
	App::Audio.playMusic("Assets/audio/high_droning.wav");
	ACLOG(GameScene, Message, "Entered game scene from ", &prev, ".");
}

void GameScene::draw() {
	m_pauseUI.draw();
	App::Graphics.submit(m_walls);
	App::Graphics.submit(m_coins);
	m_player.draw();
}

bool GameScene::processInput() {
	m_pauseUI.processInput();
	if (App::Input.isKeyPressed(AC_CTL_ESC)) {
		m_pauseUI.setEnabled(!m_pauseUI.isEnabled());
		m_pauseUI.setVisible(m_pauseUI.isEnabled());
	}
	if (!m_pauseUI.isEnabled()) {
		m_player.processInput();
	}
	return true;
}

void GameScene::componentEvent(int gID, int cID, int eID) {
	if (gID == m_pauseUI.getID()) {
		if (cID == m_continueButton.getID()) {
			if (eID == Button::ClickedEvent) {
				App::Audio.playEffect("Assets/audio/click.wav");
				m_pauseUI.setEnabled(false);
				m_pauseUI.setVisible(false);
			}
		}
		else if (cID == m_optionsButton.getID()) {
			if (eID == Button::ClickedEvent) {
				App::Audio.playEffect("Assets/audio/click.wav");
				App::SetScene(TestDriver::OPTIONS);
			}
		}
		else if (cID == m_quitButton.getID()) {
			if (eID == Button::ClickedEvent) {
				App::Audio.playEffect("Assets/audio/click.wav");
				App::SetScene(TestDriver::TITLE);
			}
		}
	}
}

void GameScene::update(float dt) {
	m_pauseUI.update(dt);
	m_UICamera.update(dt);
	if (!m_pauseUI.isEnabled()) {
		if (!m_coins.empty()) {
			m_player.update(dt);
			App::Graphics.getDefaultCamera().position = m_player.getPosition();
			std::vector<Coin>::iterator rit = m_coins.begin();
			while (rit != m_coins.end()) {
				if (rit->isCollected()) {
					rit = m_coins.erase(rit);
				}
				else {
					rit++;
				}
			}
		}
		else {
			App::Graphics.getDefaultCamera().scale.velocity = -0.01f;
		}
	}
	else {
		App::Graphics.getDefaultCamera().scale.velocity = 0.0f;
	}
}

void GameScene::leave(Scene& next) {
	ACLOG(GameScene, Message, "Left game scene for ", &next, ".");
}

void GameScene::destroy() {
	m_pauseUI.destroy();
	m_UIShader.destroy();
	m_UICamera.destroy();
	for (Sprite& w : m_walls) {
		w.destroy();
	}
	m_walls.clear();
	for (Coin& c : m_coins) {
		c.destroy();
	}
	m_coins.clear();
	m_player.destroy();
	ACLOG(GameScene, Message, "Destroyed game scene.");
}