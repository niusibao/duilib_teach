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
* ��¼��Ϣ
*/
struct LoginParam {
	HWND hWnd;
	CDuiString username;
	CDuiString password;
};

/*
* ��¼�û�
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

	/*** ϵͳ��Ϣ ***/
	UINT GetClassStyle() const;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND /*hWnd*/);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Notify(TNotifyUI& msg);


	/***�Զ���***/
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
	//�ͷ�new���ڴ�ռ䣬������Ӳ��������������ﴦ��
	void FreeParam();

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;  //�˴���Ա������m_ ��ͷ�������׼�أ������Ⱪ¶�ĳ�Աһ����private����
	//��С��
	CButtonUI* m_pMinBtn;
	//��¼
	CButtonUI* m_pLoginBtn;
	//�û���
	CEditUI* m_pUserNameUI;
	//�û�����
	CEditUI* m_pPasswordUI;
	//��ס����
	CCheckBoxUI* m_pSavePassword;
	//�Զ���¼
	CCheckBoxUI* m_pAutoLogin;
	//�û�json
	map<string, LoginUser*> m_pUserMap;
	//�û��б�
	vector<LoginUser*> m_pUserList;
	//��С����ֵ
	int m_pMinSort;

	static string m_pUserFile;
};
