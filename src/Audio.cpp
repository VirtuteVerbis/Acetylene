// Acetylene ~ Audio Implementation
// Record 01-0009
// 2024.03.14 @ 16.59

// File:	Audio.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.59
// Purpose:	Implement functions declared in Audio.h.

#include "Acetylene.h"

namespace Acetylene {
	// Implement Audio class functions.

	bool AudioManager::init(float volume) {
		if (m_audio.init() != SoLoud::SO_NO_ERROR) {
			ACLOG(Audio, Error, "Failed to initialize SoLoud library.");
			return false;
		}
		ACLOG(Audio, Message, "Initialized SoLoud library.");
		setVolume(volume);
		return true;
	}

	bool AudioManager::playEffect(const std::string& fileName) {
		if (m_effects.find(fileName) == m_effects.end()) {
			if (!loadEffect(fileName)) {
				ACLOG(Audio, Warning, "Failed to load audio file \"", fileName,
					"\".");
				return false;
			}
		}
		m_audio.play(m_effects.at(fileName));
		return true;
	}

	bool AudioManager::playMusic(const std::string& fileName) {
		if (fileName.empty()) {
			if (m_music == 0) {
				return false;
			}
			if (!m_audio.getPause(m_music)) {
				return false;
			}
			m_audio.setPause(m_music, false);
			ACLOG(Audio, Message, "Resumed music track \"", fileName, "\".");
		}
		else {
			if (m_effects.find(fileName) == m_effects.end()) {
				if (!loadEffect(fileName)) {
					ACLOG(Audio, Warning, "Failed to load audio file \"",
						fileName, "\".");
					return false;
				}
			}
			stopMusic();
			m_music = m_audio.playBackground(m_effects.at(fileName));
			m_audio.setLooping(m_music, true);
			ACLOG(Audio, Message, "Playing music track \"", fileName, "\".");
		}
		m_musicPaused = false;
		return true;
	}

	bool AudioManager::pauseMusic() {
		if (m_music == 0 || m_musicPaused) {
			return false;
		}
		m_audio.setPause(m_music, true);
		m_musicPaused = true;
		ACLOG(Audio, Message, "Paused music track.");
		return true;
	}

	bool AudioManager::isMusicPaused() const {
		return m_musicPaused;
	}

	void AudioManager::stopMusic() {
		if (m_music != 0) {
			m_audio.stop(m_music);
		}
		m_music = 0;
		m_musicPaused = false;
		ACLOG(Audio, Message, "Stopped music track.");
	}

	void AudioManager::destroy() {
		m_audio.stopAll();
		m_effects.clear();
		m_music = 0;
		m_musicPaused = false;
		m_audio.deinit();
		ACLOG(Audio, Message, "Destroyed audio manager.");
	}

	float AudioManager::getVolume() const {
		return m_audio.getGlobalVolume();
	}

	void AudioManager::setVolume(float volume) {
		if (volume < 0.0f) {
			volume = 0.0f;
		}
		else if (volume > 1.0f) {
			volume = 1.0f;
		}
		m_audio.setGlobalVolume(volume);
	}

	bool AudioManager::loadEffect(const std::string& fileName) {
		ACLOG(Audio, Message, "Loading sound effect \"", fileName, "\".");
		m_effects[fileName] = SoLoud::Wav();
		if (m_effects[fileName].load(fileName.c_str())
			!= SoLoud::SO_NO_ERROR) {
			ACLOG(Audio, Warning, "Failed to load audio file \"", fileName,
				"\".");
			m_effects.erase(m_effects.find(fileName));
			return false;
		}
		return true;
	}
}