#include "Glip.h"

namespace Glip {
    inline bool ends_with(const std::string &value, const std::string &ending) {
        if (ending.size() > value.size()) return false;
        return equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    std::string FormatBytes(long bytes) {
        if (bytes > 1000000000) {
            return std::to_string(bytes / 1000000000) + " GB";
        }
        else if (bytes > 1000000) {
            return std::to_string(bytes / 1000000) + " MB";
        }
        else if (bytes > 1000) {
            return std::to_string(bytes / 1000) + " KB";
        }
        else {
            return std::to_string(bytes) + " Bytes";
        }
    }

    std::string RandPassword(int length, bool letters, bool numbers, bool special) {
        std::stringstream PossibleCharacters;

        if (letters)
            PossibleCharacters << "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        if (numbers)
            PossibleCharacters << "0123456789";

        if (special)
            PossibleCharacters << "~!@#$%^&*()_+{}[]?<>,.;:";

        std::string PossibleCString = PossibleCharacters.str();

        if (PossibleCString.length() <= 0) {
            return std::string();
        }

        std::stringstream output;
        for (int i = 0; i < length; i++) {
            output << PossibleCString.at(rand() % PossibleCString.length());
        }

        return output.str();
    }

    std::string STR_TO_HEX(const std::string &input) {
        static const char *const lut = "0123456789ABCDEF";
        size_t len = input.length();

        std::string output;
        output.reserve(2 * len);
        for (size_t i = 0; i < len; ++i) {
            const unsigned char c = input[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        }

        return output;
    }

    std::string HEX_TO_STR(const std::string &input) {
        std::basic_string<uint8_t> bytes;

        for (size_t i = 0; i < input.length(); i += 2) {
            uint16_t byte;
            std::string nextbyte = input.substr(i, 2);
            std::istringstream(nextbyte) >> std::hex >> byte;
            bytes.push_back(static_cast<uint8_t>(byte));
        }

        std::string result(begin(bytes), end(bytes));
        return result;
    }

    std::string XOR(const std::string &data, const char *key, int length) {
        int KeyIndex = 0;
        std::stringstream res;

        for (size_t i = 0; i < (int) data.length(); i++) {
            res << std::hex << std::setfill('0') << std::setw(2) << (data[i] ^ key[KeyIndex]);

            if (++KeyIndex >= length)
                KeyIndex = 0;
        }

        return res.str();
    }

    std::string DEXOR(const std::string &data, const char *key, int length) {
        std::string decoded;

        for (size_t i = 0; i < (int) data.length() - 1; i += 2) {
            std::string output = data.substr(i, 2);
            int decimal = strtol(output.c_str(), nullptr, 16);
            decoded += (char) decimal;
        }

        int KeyIndex = 0;
        std::stringstream res;

        for (size_t i = 0; i < (char) decoded.length(); i++) {
            res.put((char) (decoded[i] ^ key[KeyIndex]));

            if (++KeyIndex >= length)
                KeyIndex = 0;
        }

        return res.str();
    }

    bool CheckPassword(const std::string &file, const std::string &password) {
        std::fstream input(file, std::ios::in | std::ios::binary);
        if (input.is_open()) {
            
            std::string EncryptedPwd = XOR(STR_TO_HEX(password), password.c_str(), (int)password.length());

            char c;
            std::stringstream res;
            while (input >> std::noskipws >> c) {
                if (res.str().length() >= EncryptedPwd.length())
                    break;

                res.put(c);
            }

            std::string OriginalPassword = res.str();
            return OriginalPassword._Equal(EncryptedPwd);
        }

        input.close();
        return false;
    }

    int Encrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log) {
        std::fstream input_s(file, std::ios::in | std::ios::binary);
        if (!input_s.is_open()) {
            *log = "File \"" + file + "\" cannot be opened";
            return -1;
        }

        input_s.seekg(0, input_s.end);
        long FileSize = (long)input_s.tellg();
        input_s.seekg(0, input_s.beg);

        std::fstream output_s(output, std::ios::out | std::ios::binary);
        if (!output_s.is_open()) {
            *log = "Can't export file to \"" + output + "\"";
            return -2;
        }

        // Remove password-check to the encrypted file
        output_s << XOR(STR_TO_HEX(password), password.c_str(), (int)password.length());

        int kCount = 0;
        char c;
        long cProgress = 0;

        std::string FormatedSize = FormatBytes(FileSize);

        while (input_s >> std::noskipws >> c) {
            if (*log == "c-1") {
                return 1;
            }

            *log = FormatBytes(++cProgress) + " of " + FormatedSize;

            output_s.put(c ^ password[kCount]);

            if (++kCount >= (int)password.length())
                kCount = 0;
        }

        output_s.close();
        *log = "Finished";
        return 0;
    }

    int Decrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log) {
        std::fstream input_s(file, std::ios::in | std::ios::binary);
        if (!input_s.is_open()) {
            *log = "File \"" + file + "\" cannot be opened";
            return -1;
        }

        input_s.seekg(0, input_s.end);
        long FileSize = (long)input_s.tellg();
        input_s.seekg(0, input_s.beg);

        std::fstream output_s(output, std::ios::out | std::ios::binary);
        if (!output_s.is_open()) {
            *log = "Can't export file to \"" + output + "\"";
            return -2;
        }

        int kCount = 0, KeyLength = XOR(STR_TO_HEX(password), password.c_str(), (int)password.length()).length();
        char c;
        int cProgress = 0;

        std::string FormatedSize = FormatBytes(FileSize);

        while (input_s >> std::noskipws >> c) {
            if (*log == "c-1") {
                return 1;
            }

            *log = FormatBytes(++cProgress) + " of " + FormatedSize;

            // Remove password-check from the decrypted file
            if (KeyLength-- > 0) {
                continue;
            }

            output_s.put(c ^ password[kCount]);

            if (++kCount >= (int)password.length())
                kCount = 0;
        }

        output_s.close();
        *log = "Finished";
        return 0;
    }
}