#pragma once

#include "common.h"
#include <string>
#include "include/http/curl.h"
#include <functional>
#include <thread>

using namespace std;

/** httpGet请求 **/
LIB_COMMON_EXPORT string httpGet(const string& url);

/** httpPost请求 **/
LIB_COMMON_EXPORT string httpPost(const string& url, const string& params);

/** http异步请求 **/
LIB_COMMON_EXPORT void httpGet(const string& url, function<int(string)>& callback);