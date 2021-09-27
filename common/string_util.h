#pragma once
#include "pch.h"
#include "common.h"
#include <string>
#include <iostream>
#include <windows.h>

using namespace std;

LIB_COMMON_EXPORT string GbkToUtf8(const char* src_str);

LIB_COMMON_EXPORT string Utf8ToGbk(const char* src_str);

LIB_COMMON_EXPORT string gb2312ToUtf8(const std::string& gb2312);

LIB_COMMON_EXPORT string helloWord();