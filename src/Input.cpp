// Acetylene ~ Input Implementation
// Record 01-0007
// 2024.03.14 @ 16.56

// File:	Input.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.57
// Purpose:	Implement functions declared in Input.h.

#include "Acetylene.h"

namespace Acetylene {
	// Implement InputListener functions.

	void InputListener::keyPressed(KeyID key) {

	}

	void InputListener::keyReleased(KeyID key) {

	}

	void InputListener::charTyped(char c) {

	}

	void InputListener::mouseMoved(const glm::vec2& move) {

	}

	void InputListener::mouseButtonPressed(MouseButtonID button) {

	}

	void InputListener::mouseButtonReleased(MouseButtonID button) {

	}

	void InputListener::mouseScrolled(const glm::vec2& scroll) {

	}

	void InputListener::controllerConnected(ControllerID controller) {

	}

	void InputListener::controllerDisconnected(ControllerID controller) {

	}

	void InputListener::controllerButtonPressed(ControllerID controller,
		ControllerButtonID button) {

	}

	void InputListener::controllerButtonReleased(ControllerID controller,
		ControllerButtonID button) {

	}

	void InputListener::controllerAxisMoved(ControllerID controller,
		ControllerAxisID axis, float move) {

	}

	// Implement InputManager functions.

	void InputManager::init() {
		glfwSetKeyCallback(App::Window.getHandle(), KeyEvent);
		glfwSetCharCallback(App::Window.getHandle(), CharEvent);
		glfwSetCursorPosCallback(App::Window.getHandle(), MousePositionEvent);
		glfwSetMouseButtonCallback(App::Window.getHandle(), MouseButtonEvent);
		glfwSetScrollCallback(App::Window.getHandle(), MouseScrollEvent);
		glfwSetJoystickCallback(ControllerEvent);
		ACLOG(Input, Message, "Added GLFW callback functions.");
		unsigned int controllerCount = 0;
		for (int j = 0; j < AC_CONTROLLER_LAST; j++) {
			if (glfwJoystickPresent(j) == GLFW_TRUE) {
				connectController((ControllerID)j);
				controllerCount++;
			}
		}
		ACLOG(Input, Message, "Added ", controllerCount,
			" pre-existing controllers.");
	}

	void InputManager::update() {
		for (std::pair<const KeyID, std::pair<bool, bool>>& k : m_keys) {
			k.second.second = k.second.first;
		}
		m_mousePosition.second = m_mousePosition.first;
		for (std::pair<const MouseButtonID, std::pair<bool, bool>>& b
			: m_mouseButtons) {
			b.second.second = b.second.first;
		}
		m_mouseScroll.second = m_mouseScroll.first;
		m_mouseScroll.first = glm::vec2(0.0f, 0.0f);
		glfwPollEvents();
		std::scoped_lock(m_removedControllersMutex);
		while (!m_removedControllers.empty()) {
			ControllerID rid = m_removedControllers.back();
			m_removedControllers.pop_back();
			std::map<ControllerID,
				std::pair<GLFWgamepadstate, GLFWgamepadstate>>::iterator it
				= m_controllers.find(rid);
			if (it != m_controllers.end()) {
				m_controllers.erase(it);
			}
		}
		for (std::pair<const ControllerID,
			std::pair<GLFWgamepadstate, GLFWgamepadstate>>& c
			: m_controllers) {
			if (!glfwJoystickPresent((int)c.first)) {
				continue;
			}
			c.second.second = c.second.first;
			glfwGetGamepadState(c.first, &c.second.first);
			for (int b = 0; b <= AC_CB_LAST; b++) {
				if (c.second.first.buttons[b] != c.second.second.buttons[b]) {
					if (c.second.first.buttons[b] == GLFW_PRESS) {
						for (InputListener* listener : m_listeners) {
							listener->controllerButtonPressed(c.first,
								(ControllerButtonID)b);
						}
					}
					else {
						for (InputListener* listener : m_listeners) {
							listener->controllerButtonReleased(c.first,
								(ControllerButtonID)b);
						}
					}
				}
			}
			for (int a = 0; a <= AC_CA_LAST; a++) {
				if (c.second.first.axes[a] != c.second.second.axes[a]) {
					for (InputListener* listener : m_listeners) {
						listener->controllerAxisMoved(c.first,
							(ControllerAxisID)a,
							c.second.first.axes[a] - c.second.second.axes[a]);
					}
				}
			}
		}
	}

	bool InputManager::addListener(InputListener& listener) {
		if (std::find(m_listeners.begin(), m_listeners.end(), &listener)
			!= m_listeners.end()) {
			return false;
		}
		m_listeners.push_back(&listener);
		ACLOG(Input, Message, "Added input listener at ", &listener, ".");
		return true;
	}

	bool InputManager::removeListener(InputListener& listener) {
		std::vector<InputListener*>::iterator it = std::find(
			m_listeners.begin(), m_listeners.end(), &listener);
		if (it == m_listeners.end()) {
			return false;
		}
		m_listeners.erase(it);
		ACLOG(Input, Message, "Removed input listener at ", &listener, ".");
		return true;
	}

	void InputManager::destroy() {
		ACLOG(Input, Message, "Removing ", m_listeners.size(),
			" input listeners.");
		m_listeners.clear();
		m_keys.clear();
		m_mouseEnabled = true;
		m_mouseVisible = true;
		m_mousePosition = std::pair<glm::vec2, glm::vec2>();
		m_mouseButtons.clear();
		m_mouseScroll = std::pair<glm::vec2, glm::vec2>();
		m_controllers.clear();
		std::scoped_lock(m_removedControllersMutex);
		m_removedControllers.clear();
	}

	bool InputManager::isKeyDown(KeyID key) const {
		if (m_keys.find(key) == m_keys.end()) {
			return false;
		}
		return m_keys.at(key).first;
	}

	bool InputManager::isKeyPressed(KeyID key) const {
		return isKeyDown(key) && !wasKeyDown(key);
	}

	bool InputManager::isKeyReleased(KeyID key) const {
		return !isKeyDown(key) && wasKeyDown(key);
	}

	bool InputManager::isMouseEnabled() const {
		return m_mouseEnabled;
	}

	void InputManager::setMouseEnabled(bool enabled) {
		if (enabled && !m_mouseEnabled) {
			if (m_mouseVisible) {
				glfwSetInputMode(App::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(App::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_HIDDEN);
			}
		}
		else if (!enabled && m_mouseEnabled) {
			glfwSetInputMode(App::Window.getHandle(), GLFW_CURSOR,
				GLFW_CURSOR_DISABLED);
		}
		m_mouseEnabled = enabled;
	}

	bool InputManager::isMouseVisible() const {
		return m_mouseVisible;
	}

	void InputManager::setMouseVisible(bool visible) {
		if (visible && !m_mouseVisible) {
			if (m_mouseEnabled) {
				glfwSetInputMode(App::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(App::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_DISABLED);
			}
		}
		else if (!visible && m_mouseVisible) {
			if (m_mouseEnabled) {
				glfwSetInputMode(App::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_HIDDEN);
			}
			else {
				glfwSetInputMode(App::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_DISABLED);
			}
		}
		m_mouseVisible = visible;
	}

	bool InputManager::isMouseMoved() const {
		return m_mousePosition.first != m_mousePosition.second;
	}

	const glm::vec2& InputManager::getMousePosition() const {
		return m_mousePosition.first;
	}

	glm::vec2 InputManager::getMousePosition(Camera& camera) const {
		glm::vec2 mp = getMousePosition();
		glm::vec2 wd((float)App::Window.getDimensions().x,
			(float)App::Window.getDimensions().y);
		mp.x -= wd.x / 2.0f;
		mp.y -= wd.y / 2.0f;
		mp /= camera.scale.scale;
		mp.x += camera.position.x;
		mp.y += camera.position.y;
		return mp;
	}

	const glm::vec2& InputManager::getPrevMousePosition() const {
		return m_mousePosition.second;
	}

	glm::vec2 InputManager::getPrevMousePosition(Camera& camera) const {
		glm::vec2 mp = getPrevMousePosition();
		glm::vec2 wd((float)App::Window.getDimensions().x,
			(float)App::Window.getDimensions().y);
		mp.x -= wd.x / 2.0f;
		mp.y -= wd.y / 2.0f;
		mp /= camera.scale.scale;
		mp.x += camera.position.x;
		mp.y += camera.position.y;
		return mp;
	}

	bool InputManager::isMouseButtonDown(MouseButtonID button) const {
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			return false;
		}
		return m_mouseButtons.at(button).first;
	}

	bool InputManager::isMouseButtonPressed(MouseButtonID button) const {
		return isMouseButtonDown(button) && !wasMouseButtonDown(button);
	}

	bool InputManager::isMouseButtonReleased(MouseButtonID button) const {
		return !isMouseButtonDown(button) && wasMouseButtonDown(button);
	}

	bool InputManager::isMouseScrolled() const {
		return (m_mouseScroll.first != m_mouseScroll.second)
			&& (m_mouseScroll.first != glm::vec2(0.0f, 0.0f));
	}

	const glm::vec2& InputManager::getMouseScroll() const {
		return m_mouseScroll.first;
	}

	std::vector<ControllerID> InputManager::getControllerIDs() const {
		std::vector<ControllerID> controllers;
		for (const std::pair<const ControllerID,
			std::pair<GLFWgamepadstate, GLFWgamepadstate>>& c
			: m_controllers) {
			controllers.push_back(c.first);
		}
		return controllers;
	}

	bool InputManager::isControllerConnected(ControllerID controller) const {
		return m_controllers.find(controller) != m_controllers.end();
	}

	bool InputManager::isControllerButtonDown(ControllerID controller,
		ControllerButtonID button) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return false;
		}
		return m_controllers.at(controller).first.buttons[(int)button]
			== GLFW_PRESS;
	}

	bool InputManager::isControllerButtonPressed(ControllerID controller,
		ControllerButtonID button) const {
		return isControllerButtonDown(controller, button)
			&& !wasControllerButtonDown(controller, button);
	}

	bool InputManager::isControllerButtonReleased(ControllerID controller,
		ControllerButtonID button) const {
		return !isControllerButtonDown(controller, button)
			&& wasControllerButtonDown(controller, button);
	}

	bool InputManager::isControllerAxisMoved(ControllerID controller,
		ControllerAxisID axis) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return false;
		}
		return m_controllers.at(controller).first.axes[(int)axis]
			!= m_controllers.at(controller).second.axes[(int)axis];
	}

	float InputManager::getControllerAxisPosition(ControllerID controller,
		ControllerAxisID axis) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return 0.0f;
		}
		return m_controllers.at(controller).first.axes[(int)axis];
	}

	float InputManager::getPrevControllerAxisPosition(ControllerID controller,
		ControllerAxisID axis) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return 0.0f;
		}
		return m_controllers.at(controller).second.axes[(int)axis];
	}

	bool InputManager::wasKeyDown(KeyID key) const {
		if (m_keys.find(key) == m_keys.end()) {
			return false;
		}
		return m_keys.at(key).second;
	}

	bool InputManager::wasMouseButtonDown(MouseButtonID button) const {
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			return false;
		}
		return m_mouseButtons.at(button).second;
	}

	bool InputManager::wasControllerButtonDown(ControllerID controller,
		ControllerButtonID button) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return false;
		}
		return m_controllers.at(controller).second.buttons[(int)button]
			== GLFW_PRESS;
	}

	void InputManager::pressKey(KeyID key) {
		if (m_keys.find(key) == m_keys.end()) {
			m_keys[key] = std::pair<bool, bool>(true, false);
		}
		m_keys[key].first = true;
		for (InputListener* listener : m_listeners) {
			listener->keyPressed(key);
		}
	}

	void InputManager::releaseKey(KeyID key) {
		if (m_keys.find(key) == m_keys.end()) {
			m_keys[key] = std::pair<bool, bool>(false, true);
		}
		m_keys[key].first = false;
		for (InputListener* listener : m_listeners) {
			listener->keyReleased(key);
		}
	}

	void InputManager::typeChar(char c) {
		for (InputListener* listener : m_listeners) {
			listener->charTyped(c);
		}
	}

	void InputManager::moveMouse(const glm::vec2& position) {
		if (!m_mouseEnabled) {
			return;
		}
		m_mousePosition.first = position;
		for (InputListener* listener : m_listeners) {
			listener->mouseMoved(position - m_mousePosition.second);
		}
	}

	void InputManager::pressMouseButton(MouseButtonID button) {
		if (!m_mouseEnabled) {
			return;
		}
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			m_mouseButtons[button] = std::pair<bool, bool>(true, false);
		}
		m_mouseButtons[button].first = true;
		for (InputListener* listener : m_listeners) {
			listener->mouseButtonPressed(button);
		}
	}

	void InputManager::releaseMouseButton(MouseButtonID button) {
		if (!m_mouseEnabled) {
			return;
		}
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			m_mouseButtons[button] = std::pair<bool, bool>(false, true);
		}
		m_mouseButtons[button].first = false;
		for (InputListener* listener : m_listeners) {
			listener->mouseButtonReleased(button);
		}
	}

	void InputManager::scrollMouse(const glm::vec2& scroll) {
		if (!m_mouseEnabled) {
			return;
		}
		m_mouseScroll.first = scroll;
		for (InputListener* listener : m_listeners) {
			listener->mouseScrolled(scroll);
		}
	}

	void InputManager::connectController(ControllerID controller) {
		if (!glfwJoystickIsGamepad((int)controller)) {
			return;
		}
		if (m_controllers.find(controller) != m_controllers.end()) {
			return;
		}
		ACLOG(Input, Message, "Connecting controller ", (int)controller, ".");
		m_controllers[controller]
			= std::pair<GLFWgamepadstate, GLFWgamepadstate>();
		glfwGetGamepadState((int)controller, &m_controllers[controller].first);
		glfwGetGamepadState((int)controller,
			&m_controllers[controller].second);
		for (InputListener* listener : m_listeners) {
			listener->controllerConnected(controller);
		}
	}

	void InputManager::disconnectController(ControllerID controller) {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return;
		}
		ACLOG(Input, Message, "Disconnecting controller ", (int)controller,
			".");
		for (InputListener* listener : m_listeners) {
			listener->controllerDisconnected(controller);
		}
		std::scoped_lock(m_removedControllersMutex);
		m_removedControllers.push_back(controller);
	}

	void InputManager::KeyEvent(GLFWwindow* window, int key, int scancode,
		int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			App::Input.pressKey((KeyID)key);
			break;
		case GLFW_RELEASE:
			App::Input.releaseKey((KeyID)key);
			break;
		}
	}

	void InputManager::CharEvent(GLFWwindow* window, unsigned int codepoint) {
		App::Input.typeChar((char)codepoint);
	}

	void InputManager::MousePositionEvent(GLFWwindow* window, double x,
		double y) {
		App::Input.moveMouse(glm::vec2(x, App::Window.getDimensions().y - y));
	}

	void InputManager::MouseButtonEvent(GLFWwindow* window, int button,
		int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			App::Input.pressMouseButton((MouseButtonID)button);
			break;
		case GLFW_RELEASE:
			App::Input.releaseMouseButton((MouseButtonID)button);
			break;
		}
	}

	void InputManager::MouseScrollEvent(GLFWwindow* window, double x,
		double y) {
		App::Input.scrollMouse(glm::vec2(x, y));
	}

	void InputManager::ControllerEvent(int jid, int event) {
		if (event == GLFW_CONNECTED) {
			App::Input.connectController((ControllerID)jid);
		}
		else if (event == GLFW_DISCONNECTED) {
			App::Input.disconnectController((ControllerID)jid);
		}
	}
}