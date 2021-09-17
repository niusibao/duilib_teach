#pragma once
#include <objbase.h>
#include <zmouse.h>
#include <exdisp.h>
#include <comdef.h>
#include <vector>
#include <sstream>
#include <string>

#include "..\DuiLib\UIlib.h"

using namespace DuiLib;

#define WM_LOGIN_RESULT  WM_USER + 50

struct LoginParam {
	HWND hWnd;
	CDuiString username;
	CDuiString password;
};

struct LoginResult {
	string result;
};

class QQLoginForm : public CWindowWnd, public INotifyUI
{
public:
	QQLoginForm() {
	};

	/*** 系统消息 ***/
	UINT GetClassStyle() const;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND /*hWnd*/);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Notify(TNotifyUI& msg);


	/***自定义***/
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLoginResult(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	void OnClick(TNotifyUI& msg);
	void OnTextChange(TNotifyUI& msg);

	void Init();
	void toLogin();
	static DWORD WINAPI httpLogin(LPVOID params);

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;  //此处成员变量用m_ 开头，请务必准守，不对外暴露的成员一律用private修饰
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pLoginBtn;
	CEditUI* m_pUserNameUI;
	CEditUI* m_pPasswordUI;


};
