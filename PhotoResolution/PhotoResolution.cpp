// PhotoResolution.cpp : 定义应用程序的入口点。
//
#include "..\DuiLib\UIlib.h"
#include "ResolutionListDialog.h"
#include "resource.h"

using namespace DuiLib;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);

    ResolutionListDialog* pFrame = new ResolutionListDialog();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("List"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 200, 50);
    //创建窗口，包括各种窗口属性，和扩展属性

    pFrame->CenterWindow();//居中窗口位置

    ::ShowWindow(*pFrame, SW_SHOW);//显示窗口

    CPaintManagerUI::MessageLoop();//开启duilib的消息循环，通用做法，Windows是基于消息机制的操作系统

    return 0;
}