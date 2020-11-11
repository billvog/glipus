/*
	Glipus Updater made by BILLVOG

	This project needs libcurl installed. I've installed it using Microsoft's
	vcpkg (C++ package manager).
*/

#pragma once

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <curl/curl.h>

namespace fs = std::filesystem;

#ifdef GL_PLATFORM_WINDOWS
	#ifdef GL_BUILD_DLL
		#define DLL __declspec(dllexport)
	#else
		#define DLL __declspec(dllimport)
	#endif
#endif


namespace Updater {
	std::string VERSION_URL = "https://billvog.ga/downloads/get.php?file=glipus/version";
	
#ifdef GL_ARCH_64
	std::string SETUP_URL = "https://billvog.ga/downloads/get.php?file=glipus/windows/64/Glipus_x64.exe";
#else
	std::string SETUP_URL = "https://billvog.ga/downloads/get.php?file=glipus/windows/86/Glipus_x64.exe";
#endif

	std::string DLL GetVersion();
	int DLL DownloadUpdate(const std::string &path);
}