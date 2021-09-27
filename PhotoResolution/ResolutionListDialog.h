#pragma once
#include "..\DuiLib\UIlib.h"

using namespace DuiLib;

class ResolutionListDialog :public WindowImplBase
{
public:
	ResolutionListDialog() {};
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;

	void Init();

	CControlUI* CreateImgItem(const string& name, int i);
	void CreateImgList();


	//°´Å¥wrapper
	CVerticalLayoutUI* m_pImageSizeList;


	int m_pButtonHeight;
	int m_pButtonWidth;
	int m_pMargin;
};

