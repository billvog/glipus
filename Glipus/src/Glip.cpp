#include "Glip.h"

namespace Glip {
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
        for (size_t i = 0; i < length; i++) {
            output << PossibleCString.at(rand() % PossibleCString.length());
        }

        return output.str();
    }

    std::string HashPwd(const std::string &password) {
        std::string digest;
        CryptoPP::SHA256 hash;

        CryptoPP::StringSource foo(password, true,
            new CryptoPP::HashFilter(hash, new CryptoPP::StringSink(digest))
        );

        return digest;
    }

    bool CheckPassword(const std::string &file, const std::string &password) {
        std::fstream input_s(file, std::ios::in | std::ios::out | std::ios::binary);
        if (!input_s.is_open()) {
            return false;
        }

        std::string HashedPassword = HashPwd(password);

        std::string str;
        CryptoPP::FileSource Stream(input_s, false, new CryptoPP::StringSink(str));
        
        // Skip embedded IV
        Stream.Pump(16);
        str.clear();

        Stream.Pump(HashedPassword.length());
        input_s.close();

        return (str == HashedPassword);
    }

    int Encrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log, int *progress, double *processedBytes) {
        std::fstream input_s(file, std::ios::in | std::ios::binary);
        if (!input_s.is_open()) {
            *log = "File \"" + file + "\" cannot be opened";
            return -1;
        }

        std::fstream output_s(output, std::ios::out | std::ios::binary);
        if (!output_s.is_open()) {
            *log = "Can't export file to \"" + output + "\"";
            return -2;
        }
        
        CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
        
        std::string iv(RandPassword(16, true, true, false));

        CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
        hkdf.DeriveKey(key, key.size(), (const CryptoPP::byte *)password.data(), password.size(), (const CryptoPP::byte *)iv.data(), iv.size(), NULL, 0);

        CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryption;
        encryption.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH);

        CryptoPP::StringSource IVStream(iv, true, new CryptoPP::FileSink(output_s));

        CryptoPP::StringSource HeaderStream(HashPwd(password), true, new CryptoPP::FileSink(output_s));

        CryptoPP::StreamTransformationFilter *OutFilter = new CryptoPP::StreamTransformationFilter(
            encryption, new CryptoPP::FileSink(output_s)
        );

        CryptoPP::FileSource Stream(input_s, false, OutFilter);

        double FileSize = (double) fs::file_size(file);
        double ProcessedBytes = 0;
        
        while (FileSize > ProcessedBytes && !Stream.GetStream()->eof()) {
            if (*log == "cmd-cancel") {
                return 1;
            }
            else if (*log == "cmd-pause") {
                continue;
            }

            Stream.Pump(GLP_READ_BLOCK_SIZE);
            OutFilter->Flush(false);

            ProcessedBytes += GLP_READ_BLOCK_SIZE;
            *processedBytes += GLP_READ_BLOCK_SIZE;

            int TmpProgress = (int) ((ProcessedBytes / FileSize) * 100);
            if (TmpProgress < 0 || TmpProgress > 100)
                TmpProgress = 100;

            *progress = TmpProgress;
        }

        input_s.close();
        output_s.close();

        *log = "Finished";
        return 0;
    }

    int Decrypt(const std::string &file, const std::string &password, const std::string &output, std::string *log, int *progress, double *processedBytes) {
        std::fstream input_s(file, std::ios::in | std::ios::binary);
        if (!input_s.is_open()) {
            *log = "File \"" + file + "\" cannot be opened";
            return -1;
        }

        std::fstream output_s(output, std::ios::out | std::ios::binary);
        if (!output_s.is_open()) {
            *log = "Can't export file to \"" + output + "\"";
            return -2;
        }

        CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);

        std::string iv;

        CryptoPP::FileSource Stream(input_s, false, new CryptoPP::StringSink(iv));
        Stream.Pump(16);

        CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
        hkdf.DeriveKey(key, key.size(), (const CryptoPP::byte *)password.data(), password.size(), (const CryptoPP::byte *)iv.data(), iv.size(), NULL, 0);

        CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH);

        CryptoPP::StreamTransformationFilter *OutFilter = new CryptoPP::StreamTransformationFilter(
            decryption, new CryptoPP::FileSink(output_s)
        );

        Stream.Attach(OutFilter);

        double FileSize = (double) fs::file_size(file);
        double ProcessedBytes = 16;
        ProcessedBytes += HashPwd(password).length();

        input_s.seekg(ProcessedBytes, input_s._Seekbeg);

        while (FileSize > ProcessedBytes && !Stream.GetStream()->eof()) {
            if (*log == "cmd-cancel") {
                return 1;
            }
            else if (*log == "cmd-pause") {
                continue;
            }

            Stream.Pump(GLP_READ_BLOCK_SIZE);
            OutFilter->Flush(false);

            ProcessedBytes += GLP_READ_BLOCK_SIZE;
            *processedBytes += GLP_READ_BLOCK_SIZE;

            int TmpProgress = (int) ((ProcessedBytes / FileSize) * 100);
            if (TmpProgress < 0 || TmpProgress > 100)
                TmpProgress = 100;

            *progress = TmpProgress;
        }

        input_s.close();
        output_s.close();

        *log = "Finished";
        return 0;
    }
}