#include <pch.h>
#include "Utils.h"
#include "Logger/Logger.h"

namespace fs = std::filesystem;

#ifdef _DEBUG
//std::string Logger::logDirectory = Utils::generateLogDirectory();
#endif // _DEBUG

std::string Utils::getCurrentUserName()
{
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);
    return std::string(username);
}

std::string Utils::getUserName()
{
#ifdef _WIN32
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);
    return std::string(username);
#else
    char* username = getenv("USER");
    if (username == nullptr) {
        username = getenv("LOGNAME");
    }
    return username ? std::string(username) : "unknown";
#endif
}

std::string Utils::generateLogDirectory()
{
    std::string username = getUserName();
    return "C:/Users/" + username + "/Documents/chadlrnsn/logs";
}

std::string Utils::getCurrentPath()
{
    fs::path path = fs::current_path();
    std::string path_string{ path.string() };
    return path_string;
}

bool Utils::SavePathToJson(const std::string& exePath) {
    nlohmann::json jsonPath;
    jsonPath["exe_path"] = exePath;

    std::ofstream file("path.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    file << jsonPath.dump(4);
    file.close();

    return true;
}

std::string Utils::GetPathFromJson(nlohmann::json& jsonPath)
{
    if (jsonPath.contains("exe_path")) {
        return jsonPath["exe_path"].get<std::string>();
    }
    else {
        return "";
    }
}

bool Utils::ConfigExists(const std::string& configName)
{
    ////std::string exePath = getExePath();
    //std::filesystem::path fullPath = std::filesystem::path(exePath).parent_path() / configName;

    //return std::filesystem::exists(fullPath);
    return true;
}

bool Utils::CreateConfig(const std::string& configName)
{
    std::fstream file(configName, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    file.write("", 0);
    file.close();
    return true;
}

bool Utils::DeleteConfig(const std::string& configName)
{
    //std::string exePath = getExePath();
    //std::filesystem::path fullPath = std::filesystem::path(exePath).parent_path() / configName;
    //if (std::filesystem::exists(fullPath)) {
    //    std::filesystem::remove(fullPath);
    //    return true;
    //}
    return false;
}

std::optional<std::string> Utils::SelectDirectory(const char* title)
{
    auto currPath = std::filesystem::current_path();

    if (!SUCCEEDED(CoInitialize(nullptr)))
        return {};

    IFileDialog* pfd;
    if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
        return {};

    const size_t titleSize = strlen(title) + 1;
    wchar_t* wcTitle = new wchar_t[titleSize];
    mbstowcs(wcTitle, title, titleSize);

    DWORD dwOptions;
    IShellItem* psi;
    if (!SUCCEEDED(pfd->GetOptions(&dwOptions)) ||
        !SUCCEEDED(pfd->SetOptions(dwOptions | FOS_PICKFOLDERS)) ||
        !SUCCEEDED(pfd->SetTitle(wcTitle)) ||
        !SUCCEEDED(pfd->Show(NULL)) ||
        !SUCCEEDED(pfd->GetResult(&psi)))
    {
        pfd->Release();
        return {};
    }

    WCHAR* folderName;
    if (!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &folderName)))
    {
        pfd->Release();
        psi->Release();
        return {};
    }

    pfd->Release();
    psi->Release();

    std::filesystem::current_path(currPath);

    std::u16string u16(reinterpret_cast<const char16_t*>(folderName));
    std::string result = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16);
    return std::optional<std::string>(result);
}

std::string Utils::GetModulePath(HMODULE hModule /*= nullptr*/)
{
    char pathOut[MAX_PATH] = {};
    GetModuleFileNameA(hModule, pathOut, MAX_PATH);

    return std::filesystem::path(pathOut).parent_path().string();
}

std::optional<std::string> Utils::GetOrSelectPath(nlohmann::json& json, const char* section, const char* name, const char* friendName, const char* filter)
{
    //auto savedPath = ini.GetValue(section, name);
    //if (savedPath != nullptr)
    //    return std::string(savedPath);

    //LOG_DEBUG("%s path not found. Please point to it manually.", friendName);
    //printf("%s path not found. Please point to it manually.\n", friendName);

    //auto titleStr = string_format("Select %s", friendName);
    //auto selectedPath = filter == nullptr ? SelectDirectory(titleStr.c_str()) : SelectFile(filter, titleStr.c_str());
    //if (!selectedPath)
    //    return {};

    //ini.SetValue(section, name, selectedPath->c_str());
    return "";
}

bool Utils::isProcessRunning(const wchar_t* processName)
{
    DWORD PID = 0;

	if (Utils::GetProcessIdByName(processName, PID)) {
		return true;
	}

    return false;
}

bool Utils::GetProcessIdByName(const wchar_t* wszProcessName, DWORD& dwProcID)
{
    HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwProcID);
    PROCESSENTRY32 pe = { sizeof(pe) };
    BOOL bOk = ::Process32First(hSnapShot, &pe);
    while (bOk)
    {
        if (wcsstr(pe.szExeFile, wszProcessName) != NULL)
        {
            dwProcID = pe.th32ProcessID;
            return TRUE;
        }

        bOk = ::Process32Next(hSnapShot, &pe);
    }

    ::CloseHandle(hSnapShot);

    return FALSE;
}

bool Utils::KillProcessByName(const wchar_t* wszProcessName)
{
    DWORD PID = 0;

    if (Utils::GetProcessIdByName(wszProcessName, PID)) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
        if (hProcess != NULL) {
            BOOL bTerminated = TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
            return bTerminated == TRUE;
        }
    }

    return false;
}

bool Utils::KillProcessByPID(DWORD dwProcessID)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
	if (hProcess != NULL) {
		BOOL bTerminated = TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		return bTerminated == TRUE;
	}
}

std::string Utils::GetLastErrorAsString(DWORD errorId /*= 0*/)
{
    //Get the error message ID, if any.
    DWORD errorMessageID = errorId == 0 ? ::GetLastError() : errorId;
    if (errorMessageID == 0)
    {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}
