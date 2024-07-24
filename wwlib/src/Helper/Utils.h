#pragma once

#ifndef HELPER_H
#define HELPER_H

#include <pch.h>
#include <json/single_include/nlohmann/json.hpp>

namespace Utils {

	inline std::string configName = "config.json";
	std::string getCurrentUserName();
	std::string getUserName();
	std::string generateLogDirectory();
	std::string getCurrentPath();
	bool SavePathToJson(const std::string&);
	std::string GetPathFromJson(nlohmann::json& jsonPath);
	bool ConfigExists(const std::string& configName);
	bool CreateConfig(const std::string& configName);
	bool DeleteConfig(const std::string& configName);
	std::string GetModulePath(HMODULE);
	std::optional<std::string> SelectDirectory(const char*);
	std::optional<std::string> GetOrSelectPath(nlohmann::json& json, const char* section, const char* name, const char* friendName, const char* filter);
	bool isProcessRunning(const wchar_t*);
	bool GetProcessIdByName(const wchar_t* wszProcessName, DWORD& dwProcID);
	bool KillProcessByName(const wchar_t* wszProcessName);
	bool KillProcessByPID(DWORD dwProcessID);

	std::string GetLastErrorAsString(DWORD errorId /*= 0*/);

	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
		auto size = static_cast<size_t>(size_s);
		auto buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}
}

#endif // !