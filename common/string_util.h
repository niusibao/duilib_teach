#pragma once
#include "pch.h"
#include "common.h"
#include <string>
#include <windows.h>

using namespace std;

LIB_COMMON_EXPORT string GbkToUtf8(const char* src_str);

LIB_COMMON_EXPORT string Utf8ToGbk(const char* src_str);