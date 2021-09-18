#pragma once
#include <objbase.h>
#include <zmouse.h>
#include <exdisp.h>
#include <comdef.h>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <curl/curl.h>
#include <json/json.h>
#include <algorithm>

#include "..\DuiLib\UIlib.h"

using namespace DuiLib;

#define WM_LOGIN_RESULT  WM_USER + 50

/*
* 登录信息
*/
struct LoginParam {
	HWND hWnd;
	CDuiString username;
	CDuiString password;
};

/*
* 登录用户
*/
struct LoginUser {
	string username;
	string password;
	bool autoLogin;
	int sort;
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
	void InitUsers();
	void SaveUsers();
	void ToLogin();
	static DWORD WINAPI httpLogin(LPVOID params);
	//释放new的内存空间，这个例子不能在析构函数里处理
	void FreeParam();

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;  //此处成员变量用m_ 开头，请务必准守，不对外暴露的成员一律用private修饰
	//最小化
	CButtonUI* m_pMinBtn;
	//登录
	CButtonUI* m_pLoginBtn;
	//用户名
	CEditUI* m_pUserNameUI;
	//用户密码
	CEditUI* m_pPasswordUI;
	//记住密码
	CCheckBoxUI* m_pSavePassword;
	//自动登录
	CCheckBoxUI* m_pAutoLogin;
	//用户json
	map<string, LoginUser*> m_pUserMap;
	//用户列表
	vector<LoginUser*> m_pUserList;
	//最小排序值
	int m_pMinSort;

	static string m_pUserFile;
};
