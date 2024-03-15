// Acetylene ~ Acetylene Implementation
// Record 01-0001
// 2024.03.14 @ 16.09

// File:	Acetylene.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.09
// Purpose:	Implement functions declared in Acetylene.h.

#include "Acetylene.h"

namespace Acetylene {
	// Implement Scene class functions.

	bool Scene::initScene() {
		if (!m_initialized) {
			m_initialized = init();
		}
		return m_initialized;
	}

	void Scene::destroyScene() {
		if (m_initialized) {
			destroy();
		}
		m_initialized = false;
	}

	// Implement App Config class functions.
	
	App::Configuration::Configuration(Scene& initScene)
		: initScene(initScene) {
		
	}

	// Initialize App class memory.

	LogManager App::Log;
	WindowManager App::Window;
	InputManager App::Input;
	AudioManager App::Audio;
	GraphicsManager App::Graphics;
	std::vector<Scene*> App::Scenes;
	Scene* App::CurrentScene = 0;
	float App::TargetUpdatesPerSecond = 60.0f;
	unsigned int App::MaxUpdatesPerFrame = 10;

	// Implement App class functions.

	bool App::Init(const Configuration& configuration) {
		if (!Log.init(configuration.log.enabled, configuration.log.fileNames,
			configuration.log.timestampFmt)) {
			return false;
		}
		ACLOG(App, Message, "Initializing app.");
		ACLOG(App, Message, "Initialized logging system.");
		ACLOG(App, Message, "Initializing app window.");
		if (!Window.init(configuration.window.dimensions,
			configuration.window.title, configuration.window.fullscreen)) {
			ACLOG(App, Error, "Failed to initialize app window.");
			return false;
		}
		Input.init();
		ACLOG(App, Message, "Initialized user input manager.");
		if (!Audio.init(configuration.audio.volume)) {
			ACLOG(App, Error, "Failed to initialize audio manager.");
			return false;
		}
		ACLOG(App, Message, "Initialized audio manager.");
		if (!Graphics.init(configuration.graphics.backgroundColor)) {
			ACLOG(App, Error, "Failed to initialize the renderer.");
			return false;
		}
		ACLOG(App, Message, "Initialized the renderer.");
		TargetUpdatesPerSecond = configuration.physics.targetUpdatesPerSecond;
		MaxUpdatesPerFrame = configuration.physics.maxUpdatesPerFrame;
		ACLOG(App, Message, "Set physics configuration.");
		SetScene(configuration.initScene);
		ACLOG(App, Message, "Set initial scene, done initializing app.");
		return true;
	}

	void App::Run() {
		ACLOG(App, Message, "Running main app loop.");
		double start = glfwGetTime();
		double now = glfwGetTime();
		double elapsed = 0.0;
		double dt = 0.0;
		unsigned int updates = 0;
		while (Window.update()) {
			Graphics.begin();
			CurrentScene->draw();
			Graphics.end();
			Input.update();
			if (!CurrentScene->processInput()) {
				break;
			}
			now = glfwGetTime();
			elapsed = now - start;
			start = now;
			dt = elapsed * TargetUpdatesPerSecond;
			while (dt > 1.0 && updates < MaxUpdatesPerFrame) {
				CurrentScene->update(1.0);
				Graphics.update(1.0);
				dt -= 1.0;
				updates++;
			}
			CurrentScene->update((float)dt);
			Graphics.update((float)dt);
			updates = 0;
		}
		ACLOG(App, Message, "Main app loop finished.");
	}

	void App::SetScene(Scene& nextScene) {
		ACLOG(App, Message, "Setting new scene at ", &nextScene, ".");
		if (std::find(Scenes.begin(), Scenes.end(), &nextScene)
			== Scenes.end()) {
			Scenes.push_back(&nextScene);
		}
		if (CurrentScene != 0) {
			Input.removeListener(*CurrentScene);
			CurrentScene->leave(nextScene);
		}
		if (&nextScene == CurrentScene) {
			CurrentScene = 0;
			return;
		}
		if (nextScene.initScene()) {
			if (CurrentScene != 0) {
				nextScene.enter(*CurrentScene);
			}
			else {
				nextScene.enter(nextScene);
			}
			Input.addListener(nextScene);
			CurrentScene = &nextScene;
		}
	}

	bool App::DestroyScene(Scene& scene) {
		ACLOG(App, Message, "Destroy scene at ", &scene, ".");
		if (&scene == CurrentScene) {
			ACLOG(App, Warning, "Cannot destroy the current scene.");
			return false;
		}
		std::vector<Scene*>::iterator it = std::find(Scenes.begin(),
			Scenes.end(), &scene);
		if (it != Scenes.end()) {
			ACLOG(App, Message, "Removing scene from memory.");
			Scenes.erase(it);
		}
		else {
			return false;
		}
		ACLOG(App, Message, "Calling destroy on the scene.");
		scene.destroyScene();
		return true;
	}

	void App::Destroy() {
		ACLOG(App, Message, "Destroying Acetylene app.");
		SetScene(*CurrentScene);
		for (Scene* scene : Scenes) {
			DestroyScene(*scene);
		}
		ACLOG(App, Message, "Destroyed all scenes.");
		TargetUpdatesPerSecond = 60.0f;
		MaxUpdatesPerFrame = 10;
		ACLOG(App, Message, "Reset physics configuration.");
		ACLOG(App, Message, "Destroying the renderer.");
		Graphics.destroy();
		ACLOG(App, Message, "Destroying audio manager.");
		Audio.destroy();
		ACLOG(App, Message, "Destroying user input manager.");
		Input.destroy();
		ACLOG(App, Message, "Destroying app window.");
		Window.destroy();
		ACLOG(App, Message, "Destroying logging system.");
		Log.destroy();
	}
}