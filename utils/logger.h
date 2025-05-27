#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <format>

using namespace std;


class Logger {
private:
	ofstream logFile;

	Logger() {
		on();
	}

public:
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	static Logger& getInstance() {
		static Logger instance; // Guaranteed to be created once
		return instance;
	}

	~Logger() {
		if (logFile.is_open()) {
			logFile.close();
		}
	}

	void on() {
		string filename = "clace_" + getDate() + ".log";
		logFile.open(filename, ios::app);

		if (!logFile) {
			cerr << "Failed to open the log file: " << filename << "\n";
		}
	}

	void off() {
		logFile.close();
	}

	void log(const string& message) {
		if (logFile) {
			logFile << "[" << getTimestamp() << "] " << message << endl;
		}
	}

	string getTimestamp() {
		auto now = chrono::system_clock::now();
		time_t now_time = chrono::system_clock::to_time_t(now);
		auto duration = now.time_since_epoch();
		auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration) % 1000;
		tm local_time{};
		localtime_r(&now_time, &local_time); // Thread-safe localtime
		return format(
				"{}{:02}{:02}-{:02}:{:02}:{:02}.{:03}",
				local_time.tm_year + 1900,  // Year
				local_time.tm_mon + 1,      // Month (0-based)
				local_time.tm_mday,         // Day
				local_time.tm_hour,         // Hour
				local_time.tm_min,          // Minute
				local_time.tm_sec,          // Second
				milliseconds.count()        // Milliseconds
		);

	}

	string getDate() {
		auto now = chrono::system_clock::now();
		time_t now_time = chrono::system_clock::to_time_t(now);
		tm* local_time = localtime(&now_time);
		return format("{:04}{:02}{:02}",
					  local_time->tm_year + 1900,
					  local_time->tm_mon + 1,
					  local_time->tm_mday);
	}
};

