#include "pch.h"
#include "http_util.h"
#include "string_util.h"


int HttpGetCallback(char* data, size_t size, size_t nmemb, string* result)
{
	string gbk = Utf8ToGbk(data);
	result->append(gbk);
	return size * nmemb;
}

string httpGet(const string& url) {
	string response;
	CURL* curl;
	curl = curl_easy_init();

	char szErrBuf[CURL_ERROR_SIZE] = { 0 };
	{
		curl_slist* plist = NULL;
		::curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		::curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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

	CURLcode res = curl_easy_perform(curl);
	::curl_easy_cleanup(curl);

	return response;
}

void httpGetThread(const string url, function<int(string)> const& callback) {
	string response = httpGet(url);
	callback(response);
}

void httpGet(const string url, function<int(string)> const &callback) {
	std::thread t(httpGetThread, url, callback);

	t.join();
}

size_t HttpPostCallback(void* ptr, size_t size, size_t nmemb, void* stream)
{
	string gbk = Utf8ToGbk((char*)ptr);

	string* str = (string*)stream;

	str->append(gbk);

	return size * nmemb;
}

string httpPost(const string& url, const string& params) {
	string response;
	CURL* curl;
	CURLcode res = CURLE_GOT_NOTHING;
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded");
	headers = curl_slist_append(headers, "X-Requested-With:XMLHttpRequest");

	::curl_global_init(CURL_GLOBAL_ALL);
	curl = ::curl_easy_init();

	char szErrBuf[CURL_ERROR_SIZE] = { 0 };
	{
		::curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		::curl_easy_setopt(curl, CURLOPT_USERAGENT, "android");
		::curl_easy_setopt(curl, CURLOPT_URL, url.data());
		::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);	// 防止阿里云文件重定向
		::curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.data());
		::curl_easy_setopt(curl, CURLOPT_POST, 1);
		::curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, szErrBuf);
		::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpPostCallback);
		::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
		::curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		::curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);	// 超时时间10秒

#ifdef SKIP_PEER_HOSTNAME_VERIFICATION
		::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
#endif
	}

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return response;
}