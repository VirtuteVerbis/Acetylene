// Acetylene ~ Acetylene Header
// Record 01-0000
// 2024.03.14 @ 16.08

// File:	Acetylene.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.08
// Purpose:	Declare the main Scene and App classes of the Acetylene framework.

#ifndef AC_ACETYLENE_H
#define AC_ACETYLENE_H

#include "Log.h"
#include "Audio.h"
#include "Graphics.h"
#include "UI.h"

namespace Acetylene {
	// Abstract scene of an app.
	class Scene : public InputListener {
	protected:
		// Allow the UIGroup class to access protected functions.
		friend class UIGroup;

		/*
		Initialize this scene's memory.
		@return bool: Whether this scene was successfully initialized.
		*/
		virtual bool init() = 0;
		/*
		Enter this scene from another.
		@param Scene&: A reference to the previous scene or this one if it is
		the first one in the app.
		*/
		virtual void enter(Scene&) = 0;
		/*
		Render this scene's graphics.
		*/
		virtual void draw() = 0;
		/*
		Process user input to this scene.
		@return bool: Whether the app should continue running on this scene.
		*/
		virtual bool processInput() = 0;
		/*
		Process a UI component event on this scene.
		@param int: The ID number of the UI group the event occurred in.
		@param int: The ID number of the UI component the event occurred on.
		@param int: The ID number of the event type.
		*/
		virtual void componentEvent(int, int, int) = 0;
		/*
		Update this scene's physics.
		@param float: The number of frames elapsed since the last update.
		*/
		virtual void update(float) = 0;
		/*
		Leave this scene for another.
		@param Scene&: A reference to the next scene or this one if it is the
		last one in the app.
		*/
		virtual void leave(Scene&) = 0;
		/*
		Free this scene's memory.
		*/
		virtual void destroy() = 0;

	private:
		// Allow the app to access this scene's memory.
		friend class App;

		// Whether this scene's init function has been called.
		bool m_initialized = false;

		/*
		Set the initialized flag and call init.
		@return bool: Whether this scene is initialized.
		*/
		bool initScene();
		/*
		Reset the initialized flag and call destroy.
		*/
		void destroyScene();
	};

	// The main app class of the Acetylene library.
	class App {
	public:
		// The log for the app.
		static LogManager Log;
		// The window manager for the app.
		static WindowManager Window;
		// The user input manager for the app.
		static InputManager Input;
		// The audio management system for the app.
		static AudioManager Audio;
		// The graphics renderer for the app.
		static GraphicsManager Graphics;

		// The configuration of an app's settings.
		class Configuration {
		public:
			// The settings for the logging system.
			struct Log {
				// Whether to write logs.
				bool enabled = true;
				// The paths of the files for the log to write to.
				std::vector<std::string> fileNames = { "Acetylene.log" };
				// The format for the log to print dates in.
				std::string timestampFmt = "%Y.%m.%d @ %H.%M.%S";
			} log;
			// The settings for the window the app appears in.
			struct Window {
				// The initial dimensions of the window.
				glm::ivec2 dimensions = glm::ivec2(800, 600);
				// The initial title of the window.
				std::string title = "Acetylene";
				// Whether the window will appear in fullscreen mode.
				bool fullscreen = false;
			} window;
			// The settings for the audio manager for the app.
			struct Audio {
				// The initial volume for sound effects and music.
				float volume = 1.0f;
			} audio;
			// The settings for the renderer for the app.
			struct Graphics {
				// The background color to draw in the window.
				glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);
			} graphics;
			// The physics update settings for the app.
			struct Physics {
				// The target number of updates per second.
				float targetUpdatesPerSecond = 60.0f;
				// The maximum number of updates per rendering frame.
				unsigned int maxUpdatesPerFrame = 10;
			} physics;
			// The initial scene to be set in the app.
			Scene& initScene;
			
			/*
			Initialize the app's configuration with an initial scene.
			@param Scene& initScene: A reference to the initial scene to run
			the app with.
			*/
			Configuration(Scene&);
		};

		/*
		Initialize the app's memory with a configuration.
		@param const Configuration& configuration: The initialization
		configuration.
		@return bool: Whether the app could be initialized.
		*/
		static bool Init(const Configuration&);
		/*
		Start the main loop of the app.
		*/
		static void Run();
		/*
		Change the current scene in the app.
		@param Scene& nextScene: A reference to the scene to switch the app to.
		*/
		static void SetScene(Scene&);
		/*
		Free a not currently active scene's memory.
		@param Scene& scene: A reference to the scene to destroy.
		@return bool: Whether the scene was in memory and not active.
		*/
		static bool DestroyScene(Scene&);
		/*
		Free the app's memory.
		*/
		static void Destroy();

	private:
		// The set of scenes already initialized by the app.
		static std::vector<Scene*> Scenes;
		// The current scene in the app.
		static Scene* CurrentScene;
		// The target number of physics updates per second.
		static float TargetUpdatesPerSecond;
		// The maximum number of physics updates per rendering frame.
		static unsigned int MaxUpdatesPerFrame;

		/*
		Do not allow the app class to be instantiated.
		*/
		App() = delete;
		/*
		Do not allow the app class to be copied.
		@param const App&: The app not to copy.
		*/
		App(const App&) = delete;
		/*
		Do not allow the app class to be assigned.
		@param const App&: The app not to copy.
		*/
		void operator = (const App&) = delete;
	};
}

#endif