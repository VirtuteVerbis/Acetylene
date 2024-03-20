// Acetylene ~ Test Driver Options Scene Implementation
// Record 01-0014
// 2024.03.17 @ 20.37

// File:	OptionsScene.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.17 @ 20.37
// Purpose:	Implement the functions declared in Scenes.h for the options scene
//			of the TestDriver program.

#include "TestDriver.h"

#include <sstream>

// Declare static OptionsScene variables.

OptionsScene::Options OptionsScene::OPTIONS;

// Implement Options class functions.

bool OptionsScene::Options::loadOptions() {
	std::ifstream optionsFile("options.txt");
	if (!optionsFile.good()) {
		ACLOG(Options, Warning, "Failed to read options file.");
		return false;
	}
	std::vector<std::string> flags = {
		"Resolution: ", "Fullscreen: ", "Volume: ", "Username: " };
	std::string line;
	while (std::getline(optionsFile, line)) {
		if (line.substr(0, flags.at(0).length()) == flags.at(0)) {
			std::string opt = line.substr(flags.at(0).length());
			ACLOG(Options, Message, "Read resolution: \"", opt, "\".");
			std::vector<std::string> res = { "", "" };
			int i = 0;
			for (char c : opt) {
				if (c == 'x') {
					i++;
				}
				else {
					res[i] += c;
				}
			}
			resolution.x = std::atoi(res[0].c_str());
			resolution.y = std::atoi(res[1].c_str());
			ACLOG(Options, Message, "Set resolution (", resolution.x, ", ",
				resolution.y, ").");
		}
		else if (line.substr(0, flags.at(1).length()) == flags.at(1)) {
			std::string opt = line.substr(flags.at(1).length());
			ACLOG(Options, Message, "Read fullscreen: \"", opt, "\".");
			fullscreen = (bool)std::atoi(opt.c_str());
			ACLOG(Options, Message, "Set fullscreen ", fullscreen, ".");
		}
		else if (line.substr(0, flags.at(2).length()) == flags.at(2)) {
			std::string opt = line.substr(flags.at(2).length());
			ACLOG(Options, Message, "Read volume: \"", opt, "\".");
			volume = (float)std::atof(opt.c_str());
			if (volume < 0.0f) {
				volume = 0.0f;
			}
			else if (volume > 1.0f) {
				volume = 1.0f;
			}
			ACLOG(Options, Message, "Set volume ", volume, ".");
		}
		else if (line.substr(0, flags.at(3).length()) == flags.at(3)) {
			std::string opt = line.substr(flags.at(3).length());
			ACLOG(Options, Message, "Read username: \"", opt, "\".");
			username = opt;
		}
	}
	optionsFile.close();
	ACLOG(Options, Message, "Read options file.");
	return true;
}

bool OptionsScene::Options::saveOptions() {
	std::ofstream optionsFile("options.txt");
	if (!optionsFile.good()) {
		ACLOG(Options, Error, "Failed to write options file.");
		return false;
	}
	optionsFile << "Resolution: " << resolution.x << "x" << resolution.y
		<< std::endl << "Fullscreen: " << fullscreen << std::endl << "Volume: "
		<< volume << std::endl << "Username: " << username << std::endl;
	optionsFile.close();
	return true;
}

// Implement OptionsScene class functions.

bool OptionsScene::init() {
	m_UIShader = App::Graphics.getDefaultShader();
	m_UICamera.init();
	m_UIShader.setCamera(m_UICamera);
	m_mainUI.init(*this, 0,
		"Assets/fonts/Work_Sans/static/WorkSans-Regular.ttf", 0.25f,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_mainUI.setShader(m_UIShader);
	m_backButton.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.1f),
		"Assets/textures/UI/button.png", "BACK");
	m_mainUI.addComponent(m_backButton);
	m_defaultsButton.init(glm::vec3(0.5f - (0.25f / 2.0f), 0.0f, 0.0f),
		glm::vec2(0.25f, 0.1f), "Assets/textures/UI/button.png", "DEFAULTS");
	m_mainUI.addComponent(m_defaultsButton);
	m_saveButton.init(glm::vec3(1.0f - 0.25f, 0.0f, 0.0f),
		glm::vec2(0.25f, 0.1f), "Assets/textures/UI/button.png", "SAVE");
	m_mainUI.addComponent(m_saveButton);
	m_optionsUI.init(*this, 1,
		"Assets/fonts/Work_Sans/static/WorkSans-Regular.ttf", 0.25f,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_optionsUI.setShader(m_UIShader);
	m_resolutionCarousel.init(glm::vec3(0.01f, 0.85f, 0.0f),
		glm::vec2(0.48f, 0.1f), "Assets/textures/UI/carousel.png", "Resolution",
		{ "800x600", "1600x900", "1920x1080" }, "800x600",
		"Assets/textures/UI/carousel_button.png");
	m_optionsUI.addComponent(m_resolutionCarousel);
	m_fullscreenSwitch.init(glm::vec3(0.01f, 0.7f, 0.0f),
		glm::vec2(0.48f, 0.1f), "Assets/textures/UI/switch.png", "Fullscreen");
	m_optionsUI.addComponent(m_fullscreenSwitch);
	m_volumeSlider.init(glm::vec3(0.01f, 0.55f, 0.0f), glm::vec2(0.48f, 0.1f),
		"Assets/textures/UI/slider.png", glm::vec2(10.0f, 0.08f),
		"Assets/textures/UI/cursor.png", "Volume");
	m_optionsUI.addComponent(m_volumeSlider);
	m_usernameBox.init(glm::vec3(0.51f, 0.85f, 0.0f), glm::vec2(0.48f, 0.1f),
		"Assets/textures/UI/text_box.png", "Username", glm::vec2(1.0f, 0.08f),
		"Assets/textures/UI/cursor.png", "");
	m_optionsUI.addComponent(m_usernameBox);
	ACLOG(OptionsScene, Message, "Initialized options scene.");
	return true;
}

void OptionsScene::enter(Scene& prev) {
	glm::ivec2 wd = App::Window.getDimensions();
	m_UICamera.position = glm::vec3((float)wd.x / 2.0f,
		(float)wd.y / 2.0f, 0.0f);
	std::ostringstream oss;
	oss << OPTIONS.resolution.x << "x" << OPTIONS.resolution.y;
	m_resolutionCarousel.setValue(oss.str());
	m_fullscreenSwitch.setOn(OPTIONS.fullscreen);
	m_volumeSlider.setValue(OPTIONS.volume);
	m_usernameBox.setText(OPTIONS.username);
	m_prevScene = &prev;
	App::Audio.playMusic("Assets/audio/low_droning.wav");
	ACLOG(OptionsScene, Message, "Entered options scene from ", &prev, ".");
}

void OptionsScene::draw() {
	m_mainUI.draw();
	m_optionsUI.draw();
	App::Graphics.submit(m_embers, m_UIShader);
}

bool OptionsScene::processInput() {
	m_mainUI.processInput();
	m_optionsUI.processInput();
	return true;
}

void OptionsScene::componentEvent(int gID, int cID, int eID) {
	if (gID == m_mainUI.getID()) {
		if (cID == m_backButton.getID()) {
			if (eID == Button::ClickedEvent) {
				ACLOG(OptionsScene, Message, "Back button clicked.");
				App::Audio.playEffect("Assets/audio/click.wav");
				App::SetScene(*m_prevScene);
			}
		}
		else if (cID == m_defaultsButton.getID()) {
			if (eID == Button::ClickedEvent) {
				ACLOG(OptionsScene, Message, "Defaults button clicked.");
				App::Audio.playEffect("Assets/audio/click.wav");
				m_resolutionCarousel.setValue("800x600");
				m_fullscreenSwitch.setOn(false);
				m_volumeSlider.setValue(1.0f);
				m_usernameBox.setText("VirtuteVerbis");
			}
		}
		else if (cID == m_saveButton.getID()) {
			if (eID == Button::ClickedEvent) {
				ACLOG(OptionsScene, Message, "Save button clicked.");
				App::Audio.playEffect("Assets/audio/click.wav");
				std::vector<std::string> res = { "", "" };
				int i = 0;
				for (char c : m_resolutionCarousel.getValue()) {
					if (c == 'x') {
						i++;
					}
					else {
						res[i] += c;
					}
				}
				OPTIONS.resolution.x = std::atoi(res[0].c_str());
				OPTIONS.resolution.y = std::atoi(res[1].c_str());
				App::Window.setDimensions(OPTIONS.resolution);
				OPTIONS.fullscreen = m_fullscreenSwitch.isOn();
				App::Window.setFullscreen(OPTIONS.fullscreen);
				OPTIONS.volume = m_volumeSlider.getValue();
				App::Audio.setVolume(OPTIONS.volume);
				OPTIONS.username = m_usernameBox.getText();
				OPTIONS.saveOptions();
			}
		}
	}
}

void OptionsScene::update(float dt) {
	m_UICamera.update(dt);
	m_mainUI.update(dt);
	m_optionsUI.update(dt);
	m_fullscreenSwitch.setLabelText(m_fullscreenSwitch.isOn()
		? "Fullscreen: On" : "Fullscreen: Off");
	std::ostringstream oss;
	oss << (int)(m_volumeSlider.getValue() * 100.0f) << "%";
	m_volumeSlider.setLabelText("Volume: " + oss.str());
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

void OptionsScene::leave(Scene& next) {
	for (Ember& e : m_embers) {
		e.destroy();
	}
	m_embers.clear();
	ACLOG(OptionsScene, Message, "Left options scene for ", &next, ".");
}

void OptionsScene::destroy() {
	m_UICamera.destroy();
	m_UIShader.destroy();
	m_mainUI.destroy();
	m_optionsUI.destroy();
	for (Ember& e : m_embers) {
		e.destroy();
	}
	m_embers.clear();
	ACLOG(OptionsScene, Message, "Destroyed options scene.");
}