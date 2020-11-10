/*
	Glipus Library made by BILLVOG

	This is the base Glip. This is the library Glipus stands on. With this library
	you can enc/Decrypt files using XOR cipher easily with a password. Feel free to
	use it "AS IS" for your own purpose as long as you refer the of the name of the
	library.

	DISCLAIMER: XOR cipher is not so secure. Someone capable (but not everyone) can
	Decrypt	your files.
*/

#pragma once
#include <time.h>
#include <random>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string.h>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

namespace Glip {
	std::string RandPassword(int length, bool letters, bool numbers, bool special);
	std::string STR_TO_HEX(const std::string &input);
	std::string HEX_TO_STR(const std::string &input);
	std::string XOR(const std::string &data, const char *key, int length);
	bool CheckPassword(const std::string &file, const std::string &password);
	int Encrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log);
	int Decrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log);
}