// Acetylene ~ Window Header
// Record 01-0004
// 2024.03.14 @ 16.54

// File:	Window.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.54
// Purpose:	Declare the windowing system for Acetylene apps.

#ifndef AC_WINDOWING_H
#define AC_WINDOWING_H

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Acetylene {
	// GLFW window creation utility for Acetylene apps.
	class WindowManager {
	public:
		/*
		Initialize and open a GLFW window.
		@param const glm::ivec2& dimensions: The dimensions (width and height)
		of the window in pixels.
		@param const std::string& title: The title to appear at the top of the
		window in windowed mode.
		@param bool fullscreen: Whether the window should initially appear in
		fullscreen mode.
		@return bool: Whether the GLFW library was successfully initialized and
		the window was opened.
		*/
		bool init(const glm::ivec2&, const std::string&, bool);
		/*
		Swap this windows frame buffer.
		@return bool: Whether the window should remain open after this update.
		*/
		bool update();
		/*
		Free this window's memory.
		*/
		void destroy();
		/*
		Get the current dimensions of this window.
		@return const glm::ivec2&: The current dimensions in pixels.
		*/
		const glm::ivec2& getDimensions() const;
		/*
		Set the dimensions of this window for windowed mode.
		@param const glm::ivec2& dimensions: The dimensions to apply next time
		the window enters windowed mode.
		*/
		void setDimensions(const glm::ivec2&);
		/*
		Get the title of this window.
		@return const std::string&: The title.
		*/
		const std::string& getTitle() const;
		/*
		Set the title of this window.
		@param const std::string& title: The new title.
		*/
		void setTitle(const std::string&);
		/*
		Test whether this window is in fullscreen mode.
		@return bool: Whether this window is in fullscreen mode.
		*/
		bool isFullscreen() const;
		/*
		Set this window's fullscreen mode.
		@param bool fullscreen: Whether to set this window to fullscreen mode.
		*/
		void setFullscreen(bool);
		/*
		Get the monitor this window is currently appearing on.
		@return GLFWmonitor*: The GLFW monitor this window appears on.
		*/
		GLFWmonitor* getMonitor() const;
		/*
		Set the monitor for this window to appear on.
		@param GLFWmonitor* monitor: The new monitor for this window to appear
		on.
		*/
		void setMonitor(GLFWmonitor*);
		/*
		Get the GLFW window handle for this window.
		@return GFLWwndow*: The GLFW window handle for this window.
		*/
		GLFWwindow* getHandle() const;

	private:
		// The current dimensions of this window.
		glm::ivec2 m_dimensions = glm::ivec2();
		// The dimensions of this window for windowed mode.
		glm::ivec2 m_windowedDimensions = glm::ivec2();
		// The title of this window.
		std::string m_title = "";
		// Whether this window is currently in fullscreen mode.
		bool m_fullscreen = false;
		// The current monitor for this window to appear on.
		GLFWmonitor* m_monitor = 0;
		// The GLFW window handle for this window.
		GLFWwindow* m_handle = 0;

		/*
		Center this window on the monitor.
		*/
		void center();
	};
}

#endif