#pragma once
#include <future>
#include <random>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/sha.h>
#include <cryptopp/files.h>

#define GLP_READ_BLOCK_SIZE 4096

namespace Glip {
	std::string RandPassword(int length, bool letters, bool numbers, bool special);
	bool CheckPassword(const std::string &file, const std::string &password);
	int Encrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log, int *progress, double *processedBytes);
	int Decrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log, int *progress, double *processedBytes);
}