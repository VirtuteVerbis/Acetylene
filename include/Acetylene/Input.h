// Acetylene ~ Input Header
// Record 01-0006
// 2024.03.14 @ 16.56

// File:	Input.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.56
// Purpose:	Declare the user-input processing system for Acetylene apps.

#ifndef AC_INPUT_H
#define AC_INPUT_H

#include "Window.h"

#include <map>
#include <vector>
#include <deque>
#include <mutex>

namespace Acetylene {
	// Enumeration of all keyboard keys overloaded from GLFW.
	enum KeyID {
		AC_UNKNOWN = GLFW_KEY_UNKNOWN,

		AC_SYM_GRAVE = GLFW_KEY_GRAVE_ACCENT, AC_SYM_MINUS = GLFW_KEY_MINUS,
		AC_SYM_EQUALS = GLFW_KEY_EQUAL,
		AC_SYM_L_BRACKET = GLFW_KEY_LEFT_BRACKET,
		AC_SYM_R_BRACKET = GLFW_KEY_RIGHT_BRACKET,
		AC_SYM_BACKSLASH = GLFW_KEY_BACKSLASH,
		AC_SYM_SEMICOLON = GLFW_KEY_SEMICOLON,
		AC_SYM_APOSTROPHE = GLFW_KEY_APOSTROPHE,
		AC_SYM_COMMA = GLFW_KEY_COMMA, AC_SYM_PERIOD = GLFW_KEY_PERIOD,
		AC_SYM_SLASH = GLFW_KEY_SLASH, AC_SYM_SPACE = GLFW_KEY_SPACE,
		AC_SYM_WORLD_1 = GLFW_KEY_WORLD_1, AC_SYM_WORLD_2 = GLFW_KEY_WORLD_2,
		AC_SYM_KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
		AC_SYM_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
		AC_SYM_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
		AC_SYM_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		AC_SYM_KP_ADD = GLFW_KEY_KP_ADD, AC_SYM_KP_ENTER = GLFW_KEY_KP_ENTER,
		AC_SYM_KP_EQUALS = GLFW_KEY_KP_EQUAL,

		AC_KEY_0 = GLFW_KEY_0, AC_KEY_1 = GLFW_KEY_1, AC_KEY_2 = GLFW_KEY_2,
		AC_KEY_3 = GLFW_KEY_3, AC_KEY_4 = GLFW_KEY_4, AC_KEY_5 = GLFW_KEY_5,
		AC_KEY_6 = GLFW_KEY_6, AC_KEY_7 = GLFW_KEY_7, AC_KEY_8 = GLFW_KEY_8,
		AC_KEY_9 = GLFW_KEY_9,

		AC_KEY_KP_0 = GLFW_KEY_KP_0, AC_KEY_KP_1 = GLFW_KEY_KP_1,
		AC_KEY_KP_2 = GLFW_KEY_KP_2, AC_KEY_KP_3 = GLFW_KEY_KP_3,
		AC_KEY_KP_4 = GLFW_KEY_KP_4, AC_KEY_KP_5 = GLFW_KEY_KP_5,
		AC_KEY_KP_6 = GLFW_KEY_KP_6, AC_KEY_KP_7 = GLFW_KEY_KP_7,
		AC_KEY_KP_8 = GLFW_KEY_KP_8, AC_KEY_KP_9 = GLFW_KEY_KP_9,

		AC_KEY_A = GLFW_KEY_A, AC_KEY_B = GLFW_KEY_B, AC_KEY_C = GLFW_KEY_C,
		AC_KEY_D = GLFW_KEY_D, AC_KEY_E = GLFW_KEY_E, AC_KEY_F = GLFW_KEY_F,
		AC_KEY_G = GLFW_KEY_G, AC_KEY_H = GLFW_KEY_H, AC_KEY_I = GLFW_KEY_I,
		AC_KEY_J = GLFW_KEY_J, AC_KEY_K = GLFW_KEY_K, AC_KEY_L = GLFW_KEY_L,
		AC_KEY_M = GLFW_KEY_M, AC_KEY_N = GLFW_KEY_N, AC_KEY_O = GLFW_KEY_O,
		AC_KEY_P = GLFW_KEY_P, AC_KEY_Q = GLFW_KEY_Q, AC_KEY_R = GLFW_KEY_R,
		AC_KEY_S = GLFW_KEY_S, AC_KEY_T = GLFW_KEY_T, AC_KEY_U = GLFW_KEY_U,
		AC_KEY_V = GLFW_KEY_V, AC_KEY_W = GLFW_KEY_W, AC_KEY_X = GLFW_KEY_X,
		AC_KEY_Y = GLFW_KEY_Y, AC_KEY_Z = GLFW_KEY_Z,

		AC_CTL_ESC = GLFW_KEY_ESCAPE, AC_CTL_ENTER = GLFW_KEY_ENTER,
		AC_CTL_TAB = GLFW_KEY_TAB, AC_CTL_BACKSPACE = GLFW_KEY_BACKSPACE,
		AC_CTL_INSERT = GLFW_KEY_INSERT, AC_CTL_DELETE = GLFW_KEY_DELETE,
		AC_CTL_LEFT = GLFW_KEY_LEFT, AC_CTL_RIGHT = GLFW_KEY_RIGHT,
		AC_CTL_UP = GLFW_KEY_UP, AC_CTL_DOWN = GLFW_KEY_DOWN,
		AC_CTL_PG_UP = GLFW_KEY_PAGE_UP, AC_CTL_PG_DOWN = GLFW_KEY_PAGE_DOWN,
		AC_CTL_HOME = GLFW_KEY_HOME, AC_CTL_END = GLFW_KEY_END,
		AC_CTL_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
		AC_CTL_PRINTSC = GLFW_KEY_PRINT_SCREEN, AC_CTL_PAUSE = GLFW_KEY_PAUSE,
		AC_CTL_F1 = GLFW_KEY_F1, AC_CTL_F2 = GLFW_KEY_F2,
		AC_CTL_F3 = GLFW_KEY_F3, AC_CTL_F4 = GLFW_KEY_F4,
		AC_CTL_F5 = GLFW_KEY_F5, AC_CTL_F6 = GLFW_KEY_F6,
		AC_CTL_F7 = GLFW_KEY_F7, AC_CTL_F8 = GLFW_KEY_F8,
		AC_CTL_F9 = GLFW_KEY_F9, AC_CTL_F10 = GLFW_KEY_F10,
		AC_CTL_F11 = GLFW_KEY_F11, AC_CTL_F12 = GLFW_KEY_F12,
		AC_CTL_F13 = GLFW_KEY_F13, AC_CTL_F14 = GLFW_KEY_F14,
		AC_CTL_F15 = GLFW_KEY_F15, AC_CTL_F16 = GLFW_KEY_F16,
		AC_CTL_F17 = GLFW_KEY_F17, AC_CTL_F18 = GLFW_KEY_F18,
		AC_CTL_F19 = GLFW_KEY_F19, AC_CTL_F20 = GLFW_KEY_F20,
		AC_CTL_F21 = GLFW_KEY_F21, AC_CTL_F22 = GLFW_KEY_F22,
		AC_CTL_F23 = GLFW_KEY_F23, AC_CTL_F24 = GLFW_KEY_F24,
		AC_CTL_F25 = GLFW_KEY_F25, AC_CTL_L_SHIFT = GLFW_KEY_LEFT_SHIFT,
		AC_CTL_L_CTRL = GLFW_KEY_LEFT_CONTROL,
		AC_CTL_L_ALT = GLFW_KEY_LEFT_ALT, AC_CTL_L_SUPER = GLFW_KEY_LEFT_SUPER,
		AC_CTL_R_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		AC_CTL_R_CTRL = GLFW_KEY_RIGHT_CONTROL,
		AC_CTL_R_ALT = GLFW_KEY_RIGHT_ALT,
		AC_CTL_R_SUPER = GLFW_KEY_RIGHT_SUPER, AC_CTL_MENU = GLFW_KEY_MENU,

		AC_CTL_LAST = GLFW_KEY_LAST,
	};

	// Enumeration of all mouse buttons overloaded from GLFW.
	enum MouseButtonID {
		AC_MB_UNKNOWN = GLFW_KEY_UNKNOWN,

		AC_MB_1 = GLFW_MOUSE_BUTTON_1, AC_MB_2 = GLFW_MOUSE_BUTTON_2,
		AC_MB_3 = GLFW_MOUSE_BUTTON_3, AC_MB_4 = GLFW_MOUSE_BUTTON_4,
		AC_MB_5 = GLFW_MOUSE_BUTTON_5, AC_MB_6 = GLFW_MOUSE_BUTTON_6,
		AC_MB_7 = GLFW_MOUSE_BUTTON_7, AC_MB_8 = GLFW_MOUSE_BUTTON_8,

		AC_MB_LEFT = GLFW_MOUSE_BUTTON_LEFT,
		AC_MB_MID = GLFW_MOUSE_BUTTON_MIDDLE,
		AC_MB_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,

		AC_MB_LAST = GLFW_MOUSE_BUTTON_LAST,
	};

	// Enumeration of all game controller ID numbers overloaded from GLFW.
	enum ControllerID {
		AC_CONTROLLER_UNKNOWN = GLFW_KEY_UNKNOWN,

		AC_CONTROLLER_1 = GLFW_JOYSTICK_1,
		AC_CONTROLLER_2 = GLFW_JOYSTICK_2,
		AC_CONTROLLER_3 = GLFW_JOYSTICK_3,
		AC_CONTROLLER_4 = GLFW_JOYSTICK_4,
		AC_CONTROLLER_5 = GLFW_JOYSTICK_5,
		AC_CONTROLLER_6 = GLFW_JOYSTICK_6,
		AC_CONTROLLER_7 = GLFW_JOYSTICK_7,
		AC_CONTROLLER_8 = GLFW_JOYSTICK_8,
		AC_CONTROLLER_9 = GLFW_JOYSTICK_9,
		AC_CONTROLLER_10 = GLFW_JOYSTICK_10,
		AC_CONTROLLER_11 = GLFW_JOYSTICK_11,
		AC_CONTROLLER_12 = GLFW_JOYSTICK_12,
		AC_CONTROLLER_13 = GLFW_JOYSTICK_13,
		AC_CONTROLLER_14 = GLFW_JOYSTICK_14,
		AC_CONTROLLER_15 = GLFW_JOYSTICK_15,
		AC_CONTROLLER_16 = GLFW_JOYSTICK_16,

		AC_CONTROLLER_LAST = GLFW_JOYSTICK_LAST,
	};

	// Enumeration of all game controller buttons overloaded from GLFW.
	enum ControllerButtonID {
		AC_CB_UNKNOWN = GLFW_KEY_UNKNOWN,

		AC_CB_A = GLFW_GAMEPAD_BUTTON_A, AC_CB_B = GLFW_GAMEPAD_BUTTON_B,
		AC_CB_X = GLFW_GAMEPAD_BUTTON_X, AC_CB_Y = GLFW_GAMEPAD_BUTTON_Y,

		AC_CB_CROSS = GLFW_GAMEPAD_BUTTON_CROSS,
		AC_CB_CIRCLE = GLFW_GAMEPAD_BUTTON_CIRCLE,
		AC_CB_SQUARE = GLFW_GAMEPAD_BUTTON_SQUARE,
		AC_CB_TRIANGLE = GLFW_GAMEPAD_BUTTON_TRIANGLE,

		AC_CB_LEFT_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
		AC_CB_RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		AC_CB_LEFT_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
		AC_CB_RIGHT_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,

		AC_CB_DP_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP,
		AC_CB_DP_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
		AC_CB_DP_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
		AC_CB_DP_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,

		AC_CB_BACK = GLFW_GAMEPAD_BUTTON_BACK,
		AC_CB_START = GLFW_GAMEPAD_BUTTON_START,
		AC_CB_GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE,

		AC_CB_LAST = GLFW_GAMEPAD_BUTTON_LAST,
	};

	// Enumeration of all game controller axes overloaded from GLFW.
	enum ControllerAxisID {
		AC_CA_UNKNOWN = GLFW_KEY_UNKNOWN,

		AC_CA_LEFT_STICK_X = GLFW_GAMEPAD_AXIS_LEFT_X,
		AC_CA_LEFT_STICK_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
		AC_CA_RIGHT_STICK_X = GLFW_GAMEPAD_AXIS_RIGHT_X,
		AC_CA_RIGHT_STICK_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,

		AC_CA_LEFT_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
		AC_CA_RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,

		AC_CA_LAST = GLFW_GAMEPAD_AXIS_LAST,
	};

	// Abstract class for input event callbacks.
	class InputListener {
	public:
		/*
		A keyboard key has been pressed.
		@param KeyID key: The ID number of the key.
		*/
		virtual void keyPressed(KeyID);
		/*
		A keyboard key has been released.
		@param KeyID key: The ID number of the key.
		*/
		virtual void keyReleased(KeyID);
		/*
		A character has been typed on the keyboard.
		@param char c: The character typed.
		*/
		virtual void charTyped(char);
		/*
		The mouse cursor has been moved on the window.
		@param const glm::vec2& move: The distance moved horizontally and
		vertically.
		*/
		virtual void mouseMoved(const glm::vec2&);
		/*
		A mouse button has been pressed.
		@param MouseButtonID button: The ID number of the button.
		*/
		virtual void mouseButtonPressed(MouseButtonID);
		/*
		A mouse button has been released.
		@param MouseButtonID button: The ID number of the button.
		*/
		virtual void mouseButtonReleased(MouseButtonID);
		/*
		The mouse's scroll wheel has been moved.
		@param const glm::vec2& scroll: The distance scrolled horizontally and
		vertically.
		*/
		virtual void mouseScrolled(const glm::vec2&);
		/*
		A game controller has been connected.
		@param ControllerID controller: The ID number of the controller.
		*/
		virtual void controllerConnected(ControllerID);
		/*
		A game controller has been disconnected.
		@param ControllerID controller: The ID number of the controller.
		*/
		virtual void controllerDisconnected(ControllerID);
		/*
		A game controller button has been pressed.
		@param ControllerID controller: The ID number of the controller the
		button was pressed on.
		@param ControllerButtonID button: The ID number of the button.
		*/
		virtual void controllerButtonPressed(ControllerID, ControllerButtonID);
		/*
		A game controller button has been released.
		@param ControllerID controller: The ID number of the controller the
		button was released on.
		@param ControllerButtonID button: The ID number of the button.
		*/
		virtual void controllerButtonReleased(ControllerID,
			ControllerButtonID);
		/*
		A game controller axis has moved.
		@param ControllerID controller: The ID number of the controller the
		axis was moved on.
		@param ControllerAxisID axis: The ID number of the axis.
		@param float move: The distance moved.
		*/
		virtual void controllerAxisMoved(ControllerID, ControllerAxisID,
			float);
	};

	// User input handler for Acetylene apps.
	class InputManager {
	public:
		/*
		Initialize this input manager's memory and set GLFW callbacks.
		*/
		void init();
		/*
		Update this input manager's logic and state.
		*/
		void update();
		/*
		Add an input listener to this input manager.
		@param InputListener& listener: A reference to the input listener to
		add.
		@return bool: Whether the input listener was successfully added or
		already present.
		*/
		bool addListener(InputListener&);
		/*
		Remove an input listener from this input manager.
		@param InputListener& listener: A reference to the input listener to
		remove.
		@return bool: Whether the input listener was successfully removed or
		not present.
		*/
		bool removeListener(InputListener&);
		/*
		Free this input manager's memory.
		*/
		void destroy();
		/*
		Test whether a key is currently down on the keyboard.
		@param KeyID key: The ID number of the key to test.
		@return bool: Whether the key is currently down.
		*/
		bool isKeyDown(KeyID) const;
		/*
		Test whether a key has just been pressed on the keyboard.
		@param KeyID key: The ID number of the key to test.
		@return bool: Whether the key is currently down and was not down in the
		last update.
		*/
		bool isKeyPressed(KeyID) const;
		/*
		Test whether a key has just been released on the keyboard.
		@param KeyID key: The ID number of the key to test.
		@return bool: Whether the key is not currently down and was down in the
		last update.
		*/
		bool isKeyReleased(KeyID) const;
		/*
		Test whether the mouse cursor is enabled on the window.
		@return Whether the mouse is enabled.
		*/
		bool isMouseEnabled() const;
		/*
		Set whether the mouse cursor is enabled on the window.
		@param bool enabled: The new enabledness state for the mouse.
		*/
		void setMouseEnabled(bool);
		/*
		Test whether the mouse cursor is visible on the window.
		@return bool: Whether the mouse is visible.
		*/
		bool isMouseVisible() const;
		/*
		Set whether the mouse cursor is visible on the window.
		@param bool visible: The new visibility state for the mouse.
		*/
		void setMouseVisible(bool);
		/*
		Test whether the mouse cursor moved since the last update.
		@return bool: Whether the mouse has moved.
		*/
		bool isMouseMoved() const;
		/*
		Get the current position of the mouse cursor on the window.
		@return const glm::vec2&: The current position of the mouse in
		window-space.
		*/
		const glm::vec2& getMousePosition() const;
		/*
		Get the current position of the mouse cursor in camera space.
		@param Camera& camera: The camera to project through.
		@return glm::vec2: The current position of the mouse in the space of
		the given camera.
		*/
		glm::vec2 getMousePosition(Camera&) const;
		/*
		Get the position of the mouse cursor in the last update.
		@return const glm::vec2&: The previous position of the mouse cursor in
		window-space.
		*/
		const glm::vec2& getPrevMousePosition() const;
		/*
		Get the position of the mouse cursor in camera space in the last
		update.
		@param Camera& camera: The camera to project through.
		@return glm::vec2: The previous mouse position in the given camera's
		space.
		*/
		glm::vec2 getPrevMousePosition(Camera&) const;
		/*
		Test whether a mouse button is currently down.
		@param MouseButtonID button: The ID number of the button to test.
		@return bool: Whether the button is currently down.
		*/
		bool isMouseButtonDown(MouseButtonID) const;
		/*
		Test whether a mouse button has just been pressed.
		@param MouseButtonID button: The ID number of the button to test.
		@return bool: Whether the button is currently down and was not down in
		the last update.
		*/
		bool isMouseButtonPressed(MouseButtonID) const;
		/*
		Test whether a mouse button has just been released.
		@param MouseButtonID button: The ID number of the button to test.
		@return bool: Whether the button is not currently down and was down in
		the last update.
		*/
		bool isMouseButtonReleased(MouseButtonID) const;
		/*
		Test whether the mouse scroll wheel has moved since the last update.
		@return bool: Whether the mouse has scrolled.
		*/
		bool isMouseScrolled() const;
		/*
		Get the scroll distance of the mouse on this update.
		@return const glm::vec2&: The horizontal and vertical mouse scroll
		distance.
		*/
		const glm::vec2& getMouseScroll() const;
		/*
		Get the ID numbers of all currently connected game controllers.
		@return std::vector<ControllerID>: The set of connected controllers'
		IDs.
		*/
		std::vector<ControllerID> getControllerIDs() const;
		/*
		Test whether a game controller is connected.
		@param ControllerID controller: The ID number of the controller to
		test.
		@return bool: Whether the given controller is currently connected.
		*/
		bool isControllerConnected(ControllerID) const;
		/*
		Test whether a game controller button is currently down.
		@param ControllerID controller: The ID number of the controller to test
		the buttons of.
		@param ControllerButtonID button: The ID number of the button to test.
		@return bool: Whether the button is down on the given controller.
		*/
		bool isControllerButtonDown(ControllerID, ControllerButtonID) const;
		/*
		Test whether a game controller button has just been pressed.
		@param ControllerID controller: The ID number of the controller to test
		the buttons of.
		@param ControllerButtonID button: The ID number of the button to test.
		@return bool: Whether the button is currently down and was not down in
		the last update on the given controller.
		*/
		bool isControllerButtonPressed(ControllerID, ControllerButtonID) const;
		/*
		Test whether a game controller button has just been released.
		@param ControllerID controller: The ID number of the controller to test
		the buttons of.
		@param ControllerButtonID button: The ID number of the button to test.
		@return bool: Whether the button is not currently down and was down in
		the last update on the given controller.
		*/
		bool isControllerButtonReleased(ControllerID,
			ControllerButtonID) const;
		/*
		Test whether a game controller axis has been moved since the last
		update.
		@param ControllerID controller: The ID number of the controller to test
		the axes of.
		@param ControllerAxisID axis: The ID number of the axis to test.
		@return bool: Whether the axis has moved on the given controller since
		the last update.
		*/
		bool isControllerAxisMoved(ControllerID, ControllerAxisID) const;
		/*
		Get the current position of a game controller axis.
		@param ControllerID controller: The ID number of the controller to test
		the axes of.
		@param ControllerAxisID axis: The ID number of the axis to test.
		@return float: The current position of the axis on the given
		controller.
		*/
		float getControllerAxisPosition(ControllerID, ControllerAxisID) const;
		/*
		Get the position of a game controller axis in the last update.
		@param ControllerID controller: The ID number of the controller to test
		the axes of.
		@param ControllerAxisID axis: The ID number of the axis to test.
		@return float: The position of the axis on the given controller in the
		last update.
		*/
		float getPrevControllerAxisPosition(ControllerID,
			ControllerAxisID) const;

	private:
		// The set of currently active input listeners.
		std::vector<InputListener*> m_listeners;
		// The set of keyboard input codes mapped to their current and
		// previous states.
		std::map<KeyID, std::pair<bool, bool>> m_keys;
		// Whether the mouse cursor is currently enabled.
		bool m_mouseEnabled = true;
		// Whether the mouse cursor is currently visible.
		bool m_mouseVisible = true;
		// The current and previous update's position of the mouse cursor.
		std::pair<glm::vec2, glm::vec2> m_mousePosition;
		// The set of mouse buttons mapped to their current and previous
		// update's states.
		std::map<MouseButtonID, std::pair<bool, bool>> m_mouseButtons;
		// The current and previous update's positions of the mouse's scroll
		// wheel.
		std::pair<glm::vec2, glm::vec2> m_mouseScroll;
		// The set of connected game controllers mapped to their current and
		// previous states.
		std::map<ControllerID, std::pair<GLFWgamepadstate, GLFWgamepadstate>>
			m_controllers;
		// Queue of controllers removed since the last update.
		std::deque<ControllerID> m_removedControllers;
		// Mutex for locking queue of removed controllers.
		std::mutex m_removedControllersMutex;
		
		/*
		Test whether a key was down on the keyboard in the last update.
		@param KeyID key: The ID number of the key to test.
		@return bool: Whether the key was down in the last update.
		*/
		bool wasKeyDown(KeyID) const;
		/*
		Test whether a mouse button was down in the last update.
		@param MouseButtonID button: The ID number of the button to test.
		@return bool: Whether the button was down in the last update.
		*/
		bool wasMouseButtonDown(MouseButtonID) const;
		/*
		Test whether a game controller button was down in the last update.
		@param ControllerID controller: The ID number of the controller to test
		the buttons of.
		@param ControllerButtonID button: The ID number of the button to test.
		@return bool: Whether the button was down on the given controller in
		the last update.
		*/
		bool wasControllerButtonDown(ControllerID, ControllerButtonID) const;
		/*
		Set a keyboard key's state to pressed and notify input listeners.
		@param KeyID key: The ID number of the key to set to its pressed state.
		*/
		void pressKey(KeyID);
		/*
		Set a keyboard key's state to released and notify input listeners.
		@param KeyID key: The ID number of the key to set to its released
		state.
		*/
		void releaseKey(KeyID);
		/*
		Notify input listeners that a character has been typed on the keyboard.
		@param char c: The character typed.
		*/
		void typeChar(char);
		/*
		Update the mouse cursor's position and notify input listeners.
		@param const glm::vec2& position: The new position of the mouse.
		*/
		void moveMouse(const glm::vec2&);
		/*
		Set a mouse button's state to pressed.
		@param MouseButtonID button: The ID number of the button to set to its
		pressed state.
		*/
		void pressMouseButton(MouseButtonID);
		/*
		Set a mouse button's state to released.
		@param MouseButtonID button: The ID number of the button to set to its
		released state.
		*/
		void releaseMouseButton(MouseButtonID);
		/*
		Change the position of the mouse scroll wheel.
		@param const glm::vec2& scroll: The new scroll position of the mouse.
		*/
		void scrollMouse(const glm::vec2&);
		/*
		Connect a game controller.
		@param ControllerID controller: The ID number of the controller to add
		to the set of connected controllers.
		*/
		void connectController(ControllerID);
		/*
		Disconnect a game controller.
		@param ControllerID controller: The ID number of the controller to
		remove from the set of connected controllers.
		*/
		void disconnectController(ControllerID);
		/*
		GLFW callback function for keyboard events.
		@param GLFWwindow* window: The handle for the window on which the event
		occurred.
		@param int key: The ID number of the key state changed.
		@param int scancode: The data of the key event.
		@param int action: The ID number of the event type.
		@param int mods: Modifier bits for the scancode.
		*/
		static void KeyEvent(GLFWwindow*, int, int, int, int);
		/*
		GLFW callback function for typed characters.
		@param GLFWwindow* window: The handle for the window on which the event
		occurred.
		@param unsigned int codepoint: The data of the typed character.
		*/
		static void CharEvent(GLFWwindow*, unsigned int);
		/*
		GLFW callback function for mouse movements.
		@param GLFWwindow* window: The handle for the window on which the event
		occurred.
		@param double x: The horizontal position of the mouse cursor in window
		space.
		@param double y: The vertical position of the mouse cursor in window
		space.
		*/
		static void MousePositionEvent(GLFWwindow*, double, double);
		/*
		GLFW callback function for mouse button events.
		@param GLFWwindow* window: The handle for the window on which the event
		occurred.
		@param int button: The ID number of the button state changed.
		@param int action: The ID number of the event type.
		@param int mods: Modifier bits for the button.
		*/
		static void MouseButtonEvent(GLFWwindow*, int, int, int);
		/*
		GLFW callback function for mouse scroll wheel events.
		@param GLFWwindow* window: The handle for the window on which the event
		occurred.
		@param double x: The horizontal distance the scroll wheel moved.
		@param double y: The vertical distance the scroll wheel moved.
		*/
		static void MouseScrollEvent(GLFWwindow*, double, double);
		/*
		GLFW callback function for game controller connection events.
		@param int jid: The ID number of the controller state changed.
		@param int event: The ID number of the event type.
		*/
		static void ControllerEvent(int, int);
	};
}

#endif