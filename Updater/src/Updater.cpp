#include "Updater.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	
	if (Updater::isCancelled)
		return 0;
	
	return written;
}

static size_t write_data_to_string(char *contents, size_t size, size_t nmemb, void *userp) {
	((std::string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

static int progress_function(void *ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) {
	Updater::Progress = (int)((NowDownloaded / TotalToDownload) * 100);
	return 0;
}

namespace Updater {
	std::string GetVersion() {
		CURL *curl;

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		std::string readBuffer;

		curl_easy_setopt(curl, CURLOPT_URL, VERSION_URL.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_string);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		
		int result = (int) curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return readBuffer;
    }

	int DownloadUpdate(const std::string &path) {
		CURL *curl;
		FILE *file;

		Progress = 0;
		isCancelled = false;

		fs::create_directory(fs::path(path).parent_path());

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, SETUP_URL.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_function);

		fopen_s(&file, path.c_str(), "wb");
		if (!file) {
			return -1;
		}

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
		int result = (int) curl_easy_perform(curl);
		fclose(file);

		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return result;
    }
}