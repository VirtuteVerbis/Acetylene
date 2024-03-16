// Acetylene ~ Audio Header
// Record 01-0008
// 2024.03.14 @ 16.58

// File:	Audio.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.58
// Purpose:	Declare the sound-effect and music system for Aceytlene apps.

#ifndef AC_AUDIO_H
#define AC_AUDIO_H

#include <SoLoud/soloud.h>
#include <SoLoud/soloud_wav.h>
#include <map>
#include <string>

namespace Acetylene {
	// App utility for loading and playing sound effects and background music.
	class AudioManager {
	public:
		/*
		Initialize the SoLoud library and the audio manager's memory.
		@param float volume: The initial volume to set.
		@return bool: Whether the SoLoud library could be initialized.
		*/
		bool init(float);
		/*
		Play a sound effect.
		@param const std::string& fileName: The file name of the sound effect.
		@return bool: Whether the sound effect could be played.
		*/
		bool playEffect(const std::string&);
		/*
		Play a music track.
		@param const std::string& fileName: The file name of the music track or
		empty string to play current track.
		@return bool: Whether the music track could be played.
		*/
		bool playMusic(const std::string& = "");
		/*
		Pause the currently playing music track.
		@return bool: Whether there was a music track playing.
		*/
		bool pauseMusic();
		/*
		Test if the currently music track is paused.
		@return Whether the music is paused.
		*/
		bool isMusicPaused() const;
		/*
		Stop the currently playing or paused music track if any.
		*/
		void stopMusic();
		/*
		Free the audio manager's memory.
		*/
		void destroy();
		/*
		Get the current volume of audio.
		@return float: The current volume (0.0 - 1.0).
		*/
		float getVolume() const;
		/*
		Set the volume of audio.
		@param float volume: The new volume (0.0 - 1.0).
		*/
		void setVolume(float);

	private:
		// Instance of the SoLoud library for this audio manager.
		SoLoud::Soloud m_audio;
		// Set of sound effects and music tracks mapped to their file names.
		std::map<std::string, SoLoud::Wav> m_effects;
		// Handle for the currently playing music track.
		SoLoud::handle m_music = 0;
		// Whether the music track is currently paused.
		bool m_musicPaused = false;

		/*
		Load a WAV file via the SoLoud library.
		@param const std::string& fileName: The file name of the sound effect.
		@return bool: Whether the sound effect could be loaded.
		*/
		bool loadEffect(const std::string&);
	};
}

#endif