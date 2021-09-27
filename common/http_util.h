#pragma once

#include "common.h"
#include <string>
#include "include/http/curl.h"
#include <functional>
#include <thread>

using namespace std;

class HttpParams {
public:
	string url;
	function<int(string)> callback;

	HttpParams(const string& url, const function<int(string)>& callback) {
		this->url = url;
		this->callback = callback;
	}
};

/** httpGet���� **/
LIB_COMMON_EXPORT string httpGet(const string& url);

/** httpPost���� **/
LIB_COMMON_EXPORT string httpPost(const string& url, const string& params);

/** http�첽���� **/
LIB_COMMON_EXPORT void httpGet(const string& url, const function<int(string)>& callback);