// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
// Добавьте сюда заголовочные файлы для предварительной компиляции
#include "framework.h"
#include <windows.h>
#include <string>
#include <lmcons.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tlhelp32.h>
#include <iomanip>
#include <stdexcept>
#include <ctime>
#include <thread>
#include <chrono>
#include <optional>
#include <Shobjidl.h>
#include <codecvt>
#include <shellapi.h>
#include <commdlg.h>
#include <shtypes.h>
#include <shobjidl_core.h>
#include <shlobj_core.h>
#include <mutex>
// 
#include <json/single_include/nlohmann/json.hpp>
#include <simpleini/SimpleIni.h>
#endif //PCH_H

