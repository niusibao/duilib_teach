// QQLogin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "..\DuiLib\UIlib.h"
#include "QQLoginForm.h"
#include "resource.h"

using namespace DuiLib;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	QQLoginForm* pFrame = new QQLoginForm();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("QQ登录"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 500);
	//创建窗口，包括各种窗口属性，和扩展属性
	
	pFrame->SetIcon(IDI_FAVICON);

	pFrame->CenterWindow();//居中窗口位置

	::ShowWindow(*pFrame, SW_SHOW);//显示窗口

	CPaintManagerUI::MessageLoop();//开启duilib的消息循环，通用做法，Windows是基于消息机制的操作系统

	return 0;
}
