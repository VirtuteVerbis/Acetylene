// Acetylene ~ Log Implementation
// Record 01-0003
// 2024.03.14 @ 16.18

// File:	Log.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.18
// Purpose:	Implement functions declared in Log.h.

#include "Log.h"

namespace Acetylene {
	// Implement LogManager class functions.

	bool LogManager::init(bool enabled,
		const std::vector<std::string>& fileNames,
		const std::string& timestampFormat) {
		m_enabled = enabled;
		for (const std::string& fileName : fileNames) {
			m_files.push_back(std::ofstream(fileName));
			if (!m_files.back().good()) {
				return false;
			}
		}
		m_timestampFormat = timestampFormat;
		if (getTimestamp().empty()) {
			return false;
		}
		return true;
	}

	bool LogManager::isEnabled() const {
		return m_enabled;
	}

	void LogManager::setEnabled(bool enabled) {
		m_enabled = enabled;
	}

	const std::string LogManager::getTimestamp() const {
		struct tm t;
		__time64_t lt;
		char buffer[64];
		errno_t err;
		_time64(&lt);
		err = _localtime64_s(&t, &lt);
		if (err) {
			return "";
		}
		strftime(buffer, sizeof(buffer), m_timestampFormat.c_str(), &t);
		return std::string(buffer);
	}

	void LogManager::destroy() {
		m_enabled = false;
		for (std::ofstream& of : m_files) {
			of.close();
		}
		m_files.clear();
		m_timestampFormat = "";
	}
}