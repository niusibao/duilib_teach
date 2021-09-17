#include "HttpHelper.h"

CURLcode _curl_easy_perform(void* curl) {
	CURLcode res = CURL_LAST;
	__try {
		res = ::curl_easy_perform(curl);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		
	}

	return res;
}

int HttpGetCallback(char* data, size_t size, size_t nmemb, string* result)
{
	result->append(data, size * nmemb);
	return size * nmemb;
}

int HttpGet(const string & url, string & response)
{
	CURL* curl;
	curl = curl_easy_init();

	char szErrBuf[CURL_ERROR_SIZE] = { 0 };
	{
		curl_slist* plist = NULL;
		::curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		::curl_easy_setopt(curl, CURLOPT_URL, url.data());
		::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);	// 防止阿里云文件重定向

		::curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, szErrBuf);
		::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpGetCallback);
		::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

		::curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		::curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);	// 超时时间10秒

#ifdef SKIP_PEER_HOSTNAME_VERIFICATION
		::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
#endif

		plist = ::curl_slist_append(NULL, "Accept-Language: zh-CN;q=0.8");
		::curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
	}

	//CURLcode res = ::curl_easy_perform(curl);
	CURLcode res = _curl_easy_perform(curl);
	::curl_easy_cleanup(curl);

	return res;
}