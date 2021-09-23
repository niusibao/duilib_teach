// HttpTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>

#include "include\curl.h"
#include <tchar.h>

using namespace std;

string UtfToGbk(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}


string UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	WCHAR* wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(LPCTSTR)strUTF8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

int HttpGetCallback(char* data, size_t size, size_t nmemb, string* result)
{
	string gbk = UtfToGbk(data);
	result->append(gbk);
	return size * nmemb;
}

int httpLogin()
{
	string response;

	string url = "http://localhost:8081/login?username=niusibao&pwd=111111";

	CURL* curl;
	curl = curl_easy_init();

	char szErrBuf[CURL_ERROR_SIZE] = { 0 };
	{
		curl_slist* plist = NULL;
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);	// 防止阿里云文件重定向

		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, szErrBuf);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpGetCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);	// 超时时间10秒

#ifdef SKIP_PEER_HOSTNAME_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
#endif

		plist = curl_slist_append(NULL, "Accept-Language: zh-CN;q=0.8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
	}

	//CURLcode res = ::curl_easy_perform(curl);
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	cout << response.c_str() << endl;
	return 0;
}

int main()
{
	httpLogin();
}