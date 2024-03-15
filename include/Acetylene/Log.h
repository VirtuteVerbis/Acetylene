// Acetylene ~ Log Header
// Record 01-0002
// 2024.03.14 @ 16.17

// File:	Log.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 16.17
// Purpose:	Declare the logging system for Acetylene apps.

#ifndef AC_LOGGING_H
#define AC_LOGGING_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Preprocessor definition for easy logging within the framework.
#define ACLOG(SRC, LVL, ...) App::Log, App::Log.getTimestamp(), ": [", #SRC, \
	"] [", #LVL, "] ", __VA_ARGS__, "\n"

namespace Acetylene {
	// Class for writing logs to the console and multiple files.
	class LogManager {
	public:
		/*
		Initialize the log's memory.
		@param bool enabled: Whether to write logs.
		@param const std::vector<std::string>& fileNames: The set of file names
		to write logs to from this log.
		@param const std::string& timestampFormat: The initial format to
		display timestamps in on logs.
		@return bool: Whether this log could be initialized.
		*/
		bool init(bool, const std::vector<std::string>&, const std::string&);
		/*
		Write data to this log.
		@param const T& data: The object to write.
		*/
		template <typename T>
		void write(const T& data) {
			if (m_enabled) {
#ifdef _DEBUG
				std::cout << data;
#endif
				for (std::ofstream& of : m_files) {
					if (of.good()) {
						of << data;
					}
				}
			}
		}
		/*
		Test whether this log is enabled.
		@return bool: This log's enabledness state.
		*/
		bool isEnabled() const;
		/*
		Set whether this log is enabled.
		@param bool: Whether to enable this log.
		*/
		void setEnabled(bool);
		/*
		Get the current date and time in the timestamp format.
		@return const std::string: The timestamp.
		*/
		const std::string getTimestamp() const;
		/*
		Free this log's memory.
		*/
		void destroy();

	private:
		// Whether to write logs.
		bool m_enabled = true;
		// The set of files to write logs to.
		std::vector<std::ofstream> m_files;
		// The format to write timestamps in.
		std::string m_timestampFormat = "";
	};

	/*
	Overload the , operator to write data to the log.
	@param LogManager& log: A reference to the log to write to.
	@param const T& data: The object to write.
	@return LogManager&: The reference to the log.
	*/
	template <typename T>
	LogManager& operator , (LogManager& log, const T& data) {
		log.write(data);
		return log;
	}
}

#endif