#pragma once
#include <string>
#include "..\libcURL\include\curl.h"

using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "../libcURL/lib/libcurld.lib")
#else
#pragma comment(lib, "../libcURL/lib/libcurl.lib")
#endif


int HttpGet(const string& url, string& response);