// Acetylene ~ Test Driver Scenes Header
// Record 01-0011
// 2024.03.17 @ 16.03

// File:	Scenes.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.17 @ 16.03
// Purpose:	Declare all the scenes for the TestDriver Acetylene app.

#ifndef TD_SCENES_H
#define TD_SCENES_h

#include "Sprites.h"

// The opening title scene for the TestDriver program.
class TitleScene : public Scene {
public:
	/*
	Initialize the title scene's memory.
	@return bool: Whether the title scene could be initialized.
	*/
	bool init() override;
	/*
	Enter the title scene from another scene.
	@param Scene& prev: A reference to the previous scene.
	*/
	void enter(Scene&) override;
	/*
	Draw the title scene's graphics.
	*/
	void draw() override;
	/*
	Process user input to the title scene.
	@return bool: Whether to continue running the app.
	*/
	bool processInput() override;
	/*
	Process UI component events on the title scene.
	@param int gID: The ID of the UI group that triggered the event.
	@param int cID: The ID of the UI component that triggered the event.
	@param int eID: The ID of the event type.
	*/
	void componentEvent(int, int, int) override;
	/*
	Update the title scene's physics.
	@param float dt: The number of frames elapsed since the last update.
	*/
	void update(float) override;
	/*
	Leave the title scene for another scene.
	@param Scene& next: A reference to the next scene.
	*/
	void leave(Scene&) override;
	/*
	Free the title scene's memory.
	*/
	void destroy() override;

private:
	// The shader to use when drawing UI components.
	Shader m_UIShader;
	// The camera to use when drawing UI components.
	Camera m_UICamera;
	// The main UI group on the title scene.
	UIGroup m_UI;
	// The start game button.
	Button m_startButton;
	// The edit options button.
	Button m_optionsButton;
	// The exit program button.
	Button m_exitButton;
	// Whether the app should continue running with the title scene.
	bool m_running = true;
	// The set of embers currently on the title scene.
	std::vector<Ember> m_embers;
	// Timer for spawning new embers on the title scene.
	float m_timer = 0.0f;
};

// The options editing scene for the TestDriver program.
class OptionsScene : public Scene {
public:
	// Class containing the values of all the options editable by the options
	// scene.
	static class Options {
	public:
		// The dimensions of the app's window.
		glm::ivec2 resolution = glm::ivec2(800, 600);
		// Whether the app should appear in fullscreen mode.
		bool fullscreen = false;
		// The audio volume for the app.
		float volume = 1.0f;
		// The username of the player in the game scene.
		std::string username = "VirtuteVerbis";

		/*
		Load the options from the options.txt file.
		@return bool: Whether the options.txt file could be read.
		*/
		bool loadOptions();
		/*
		Save the options to the options.txt file.
		@return bool: Whether the options.txt file could be written.
		*/
		bool saveOptions();
	} OPTIONS;

	/*
	Initialize the options scene's memory.
	@return bool: Whether the options scene could be initialized.
	*/
	bool init() override;
	/*
	Enter the options scene from another scene.
	@param Scene& prev: A reference to the previous scene.
	*/
	void enter(Scene&) override;
	/*
	Draw the options scene's graphics.
	*/
	void draw() override;
	/*
	Process user input to the options scene.
	@return bool: Whether to continue running the app.
	*/
	bool processInput() override;
	/*
	Process UI component events on the options scene.
	@param int gID: The ID of the UI group which triggered the event.
	@param int cID: The ID of the UI group which triggered the event.
	@param int eID: The ID of the event type.
	*/
	void componentEvent(int, int, int) override;
	/*
	Update the options scene's physics.
	@param float dt: The number of frames elapsed since the last update.
	*/
	void update(float) override;
	/*
	Leave the options scene for another scene.
	@param Scene& next: A reference to the next scene.
	*/
	void leave(Scene&) override;
	/*
	Free the options scene's memory.
	*/
	void destroy() override;

private:
	// A pointer to the previous scene which switched to the options scene.
	Scene* m_prevScene = 0;
	// The shader to use when drawing UI components.
	Shader m_UIShader;
	// The camera to use when drawing UI components.
	Camera m_UICamera;
	// The UI group of main actions to appear on the bottom of the options
	// scene.
	UIGroup m_mainUI;
	// The back to previous scene button.
	Button m_backButton;
	// The reset options to defaults button.
	Button m_defaultsButton;
	// The save options button.
	Button m_saveButton;
	// The UI group of components for editing options.
	UIGroup m_optionsUI;
	// The carousel to change the resolution of the window.
	Carousel m_resolutionCarousel;
	// The switch to change the fullscreen mode of the window.
	Switch m_fullscreenSwitch;
	// The slider to change the audio volume.
	Slider m_volumeSlider;
	// The text box to edit the username of the player on the game scene.
	TextBox m_usernameBox;
	// The set of embers currently on the options scene.
	std::vector<Ember> m_embers;
	// The timer to spawn new embers.
	float m_timer = 0.0f;
};

// The main game scene of the TestDriver program.
class GameScene : public Scene {
public:
	/*
	Initialize the game scene's memory.
	@return bool: Whether the game scene could be initialized.
	*/
	bool init() override;
	/*
	Enter the game scene from another scene.
	@param Scene& prev: A reference to the previous scene.
	*/
	void enter(Scene&) override;
	/*
	Draw the game scene's graphics.
	*/
	void draw() override;
	/*
	Process user input to the game scene.
	@return bool: Whether to continue running the app.
	*/
	bool processInput() override;
	/*
	Process UI component events on the game scene.
	@param int gID: The ID of the UI group which triggered the event.
	@param int cID: The ID of the UI component which triggered the event.
	@param int eID: The ID of the event type.
	*/
	void componentEvent(int, int, int) override;
	/*
	Update the game scene's physics.
	@param float dt: The number of frames elapsed since the last update.
	*/
	void update(float) override;
	/*
	Leave the game scene for another scene.
	@param Scene& next: A reference to the next scene.
	*/
	void leave(Scene&) override;
	/*
	Free the game scene's memory.
	*/
	void destroy() override;

private:
	// Allow the Player class to access the game scene's memory.
	friend class Player;
	// The shader to use when drawing UI components.
	Shader m_UIShader;
	// The camera to use when drawing UI components.
	Camera m_UICamera;
	// The UI group of buttons appearing when the game is paused.
	UIGroup m_pauseUI;
	// Button to continue the game from its paused state.
	Button m_continueButton;
	// Button to open the options editor from the game scene.
	Button m_optionsButton;
	// Button to quit to the title scene.
	Button m_quitButton;
	// The walls surrounding the player on the game scene.
	std::vector<Sprite> m_walls;
	// The set of coins appearing on the game scene.
	std::vector<Coin> m_coins;
	// The player sprite controlled by the user on the game scene.
	Player m_player;
};

#endif