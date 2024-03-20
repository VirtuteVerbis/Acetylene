// Acetylene ~ Window Implementation
// Record 01-0005
// 2024.03.14 @ 16.55

// File:	Window.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.55
// Purpose:	Implement functions declared in Window.h.

#include "Acetylene.h"

namespace Acetylene {
	// Implement WindowManager class functions.

	bool WindowManager::init(const glm::ivec2& dimensions,
		const std::string& title, bool fullscreen) {
		if (m_handle != 0) {
			ACLOG(Window, Warning, "Window already initialized.");
			return false;
		}
		if (!glfwInit()) {
			ACLOG(Window, Error, "Failed to initialize GLFW library.");
			return false;
		}
		ACLOG(Window, Message, "Initialized GLFW library.");
		m_monitor = glfwGetPrimaryMonitor();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		if (!fullscreen) {
			m_handle = glfwCreateWindow(dimensions.x, dimensions.y,
				title.c_str(), 0, 0);
		}
		else {
			const GLFWvidmode* vm = glfwGetVideoMode(m_monitor);
			m_handle = glfwCreateWindow(vm->width, vm->height, title.c_str(),
				m_monitor, 0);
		}
		if (!m_handle) {
			ACLOG(Window, Error, "Failed to open GLFW window.");
			return false;
		}
		ACLOG(Window, Message, "Created GLFW window.");
		setFullscreen(fullscreen);
		setDimensions(dimensions);
		setTitle(title);
		glfwMakeContextCurrent(m_handle);
		center();
		return true;
	}

	bool WindowManager::update() {
		glfwSwapBuffers(m_handle);
		return !glfwWindowShouldClose(m_handle);
	}

	void WindowManager::destroy() {
		if (!m_handle) {
			ACLOG(Window, Warning, "Window already destroyed.");
			return;
		}
		glfwDestroyWindow(m_handle);
		ACLOG(Window, Message, "Destroyed window.");
		glfwTerminate();
		ACLOG(Window, Message, "Terminated GLFW library.");
		m_dimensions = glm::ivec2();
		m_windowedDimensions = glm::ivec2();
		m_title = "";
		m_fullscreen = false;
		m_monitor = 0;
		m_handle = 0;
	}

	const glm::ivec2& WindowManager::getDimensions() const {
		return m_dimensions;
	}

	void WindowManager::setDimensions(const glm::ivec2& dimensions) {
		m_windowedDimensions = dimensions;
		ACLOG(Window, Message, "Updated window dimensions (", dimensions.x,
			", ", dimensions.y, ").");
		if (m_fullscreen) {
			return;
		}
		m_dimensions = dimensions;
		glfwSetWindowSize(m_handle, dimensions.x, dimensions.y);
		glViewport(0, 0, dimensions.x, dimensions.y);
		center();
	}

	const std::string& WindowManager::getTitle() const {
		return m_title;
	}

	void WindowManager::setTitle(const std::string& title) {
		m_title = title;
		glfwSetWindowTitle(m_handle, title.c_str());
		ACLOG(Window, Message, "Set window title \"", title, "\".");
	}

	bool WindowManager::isFullscreen() const {
		return m_fullscreen;
	}

	void WindowManager::setFullscreen(bool fullscreen) {
		if (fullscreen && !m_fullscreen) {
			ACLOG(Window, Message, "Setting window to fullscreen mode.");
			const GLFWvidmode* vm = glfwGetVideoMode(m_monitor);
			glfwSetWindowMonitor(m_handle, m_monitor, 0, 0, vm->width,
				vm->height, GLFW_DONT_CARE);
			glViewport(0, 0, vm->width, vm->height);
			m_windowedDimensions = m_dimensions;
			m_dimensions = glm::ivec2(vm->width, vm->height);
		}
		else if (!fullscreen && m_fullscreen) {
			ACLOG(Window, Message, "Setting window to windowed mode.");
			glfwSetWindowMonitor(m_handle, 0, 0, 0, m_windowedDimensions.x,
				m_windowedDimensions.y, GLFW_DONT_CARE);
			glfwSetWindowAttrib(m_handle, GLFW_DECORATED, GLFW_TRUE);
			ACLOG(Window, Message, "Windowed dims: (", m_windowedDimensions.x,
				", ", m_windowedDimensions.y, ").");
			glViewport(0, 0, m_windowedDimensions.x, m_windowedDimensions.y);
			m_dimensions = m_windowedDimensions;
			center();
		}
		m_fullscreen = fullscreen;
	}

	GLFWmonitor* WindowManager::getMonitor() const {
		return m_monitor;
	}

	void WindowManager::setMonitor(GLFWmonitor* monitor) {
		m_monitor = monitor;
	}

	GLFWwindow* WindowManager::getHandle() const {
		return m_handle;
	}

	void WindowManager::center() {
		const GLFWvidmode* vm = glfwGetVideoMode(m_monitor);
		glfwSetWindowPos(m_handle, (vm->width - m_dimensions.x) / 2,
			(vm->height - m_dimensions.y) / 2);
	}
}