// HttpTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>

#include <tchar.h>
#include "../common/http_util.h"
#include "../common/string_util.h"

using namespace std;
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <process.h>

void f1(int n)
{
	for (int i = 0; i < 5; ++i) {
		std::cout << "Thread " << n << " executing\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
	}
}

void f2(int& n)
{
	for (int i = 0; i < 5; ++i) {
		std::cout << "Thread 2 executing\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
	}
}

void httpGetThread(const string url, function<int(string)> const& callback) {
	this_thread::sleep_for(std::chrono::milliseconds(1000));
	string response = httpGet(url);
	callback(response);
}

void httpGetTest(const string& url, const function<int(string)> const& callback) {
	std::thread t(httpGetThread, url, callback);

	t.join();
}

void test0(void*) {
	cout << "test0" << endl;
}

void test1(void*) {
	cout << "test1" << endl;
}

//int main()
//{
//
//	cout << "start" << endl;
//	_beginthread(test0, 0, NULL);
//	_beginthread(test1, 0, NULL);
//	cout << "over" << endl;
//	//system("pause");
//	return 0;
//}

void test() {
	httpGet("http://localhost:8081/login?username=niusibao&pwd=111111", [](string result) {
		cout << result << endl;
		return 0;
		});
}

int main()
{
	test();
	cout << helloWord() << endl;
	cout << "end" << endl;

	system("pause");
}

