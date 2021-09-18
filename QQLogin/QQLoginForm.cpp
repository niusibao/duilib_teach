#include "StdAfx.h"
#include "QQLoginForm.h"
#include "CircleImageUI.h"
//#include "httplib.h"
#include "string_util.h"


using namespace DuiLib;


CDuiString QQLoginForm::GetSkinFolder()
{

	return _T("skin\\QQLogin\\");
}


CDuiString QQLoginForm::GetSkinFile()
{
	return _T("login.xml");
}

LPCTSTR QQLoginForm::GetWindowClassName() const
{
	return _T("QQLogin");
};

UINT QQLoginForm::GetClassStyle() const
{
	return CS_DBLCLKS;
};

void QQLoginForm::OnFinalMessage(HWND /*hWnd*/) {
	::PostQuitMessage(0L);
	//delete this;
}

void QQLoginForm::Init() {
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_min")));
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_close")));
	m_pLoginBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_login")));
	m_pUserNameUI = static_cast<CEditUI*>(m_pm.FindControl(_T("input_username")));
	m_pPasswordUI = static_cast<CEditUI*>(m_pm.FindControl(_T("input_password")));
	m_pAutoLogin = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("check_box_auto_login")));
	m_pSavePassword = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("check_box_save_password")));


	this->InitUsers();
}

inline bool file_exists(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}

string QQLoginForm::m_pUserFile = "users.json";

void QQLoginForm::InitUsers() {

	if (file_exists(m_pUserFile)) {
		Json::Value root;
		Json::Reader reader;

		std::ifstream config_doc(m_pUserFile, std::ifstream::binary);
		reader.parse(config_doc, root);
		config_doc.close();

		if (root.isArray()) {
			for (int i = 0;i < root.size();i++) {
				Json::Value user = root[i];
				LoginUser* loginUser = new LoginUser();
				loginUser->username = user["username"].asString();
				loginUser->password = user["password"].asString();
				loginUser->autoLogin = user["autoLogin"].asBool();
				loginUser->sort = i;

				this->m_pUserList.push_back(loginUser);
				this->m_pUserMap[loginUser->username] = loginUser;
			}
		}
	}
}

void QQLoginForm::SaveUsers() {
	Json::Value root;

	CDuiString username = m_pUserNameUI->GetText();
	CDuiString password = m_pPasswordUI->GetText();

	LoginUser* currentUser = m_pUserMap.at(username.GetData());

	if (!currentUser) {
		currentUser = new LoginUser();
	}

	Json::Value userObj;
	userObj["username"] = username.GetData();
	if (m_pSavePassword->GetCheck()) {
		userObj["password"] = password.GetData();
	}
	userObj["autoLogin"] = m_pAutoLogin->GetCheck();
	root.append(userObj);

	for (int i = 0;i < m_pUserList.size();i++) {
		LoginUser* user = m_pUserList[i];
		if (user == currentUser) {
			continue;
		}
		Json::Value userObj;
		userObj["username"] = user->username;
		userObj["password"] = user->password;
		userObj["autoLogin"] = user->autoLogin;
		root.append(userObj);
		delete user;
	}
	
	delete currentUser;

	m_pUserList.clear();
	m_pUserMap.clear();

	Json::StyledWriter sw;

	//输出到文件
	ofstream os;
	os.open(m_pUserFile);
	os << sw.write(root);
	os.close();

	this->InitUsers();
}

int HttpGetCallback(char* data, size_t size, size_t nmemb, string* result)
{
	result->append(data, size * nmemb);
	return size * nmemb;
}

/// <summary>
/// 接口线程登录
/// </summary>
/// <param name="params"></param>
/// <returns></returns>
DWORD WINAPI QQLoginForm::httpLogin(LPVOID params)
{
	LoginParam* param = (struct LoginParam*)params;
	string response;

	CDuiString url;
	url.Format(_T("http://localhost:8081/login?username=%s&pwd=%s"), param->username.GetData(), param->password.GetData());

	CURL* curl;
	curl = curl_easy_init();

	char szErrBuf[CURL_ERROR_SIZE] = { 0 };
	{
		curl_slist* plist = NULL;
		::curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		::curl_easy_setopt(curl, CURLOPT_URL, url.GetData());
		::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);	// 防止阿里云文件重定向

		::curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, szErrBuf);
		::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpGetCallback);
		::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

		::curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		::curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);	// 超时时间10秒

#ifdef SKIP_PEER_HOSTNAME_VERIFICATION
		::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
#endif

		plist = ::curl_slist_append(NULL, "Accept-Language: zh-CN;q=0.8");
		::curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
	}

	//CURLcode res = ::curl_easy_perform(curl);
	CURLcode res = curl_easy_perform(curl);
	::curl_easy_cleanup(curl);
	// 接口内容为utf-8,需转换成gbk
	string result = Utf8ToGbk(response.data());

	//在堆内存创建字符串，否则内容会丢失
	string* loginInfo = new string(result);
	::PostMessage(param->hWnd, WM_LOGIN_RESULT, 0L, (LPARAM)loginInfo);

	delete params;

	return 0;
}

void QQLoginForm::ToLogin() {
	m_pLoginBtn->SetEnabled(false);

	CDuiString username = m_pUserNameUI->GetText();
	CDuiString password = m_pPasswordUI->GetText();

	if (username.IsEmpty()) {
		MessageBox(this->m_hWnd, _T("用户名不能为空"), _T("提示"), MB_OK);
	}
	else
		if (password.IsEmpty()) {
			MessageBox(this->m_hWnd, _T("密码不能为空"), _T("提示"), MB_OK);
			return;
		}
		else {
			//传递登录需要的参数
			struct LoginParam* param = new LoginParam;

			param->hWnd = this->m_hWnd;
			param->username = username;
			param->password = password;
			DWORD dwThreadID = 0;

			//开启线程调用接口登录
			HANDLE hThread = CreateThread(NULL, 0, &QQLoginForm::httpLogin, (LPVOID)param, 0, &dwThreadID);

			return;
		}

	m_pLoginBtn->SetEnabled(true);
}



/// <summary>
/// 消息通知
/// </summary>
/// <param name="msg"></param>
void QQLoginForm::Notify(TNotifyUI& msg) {
	if (msg.sType == _T("click")) {
		this->OnClick(msg);
	}
	else if (msg.sType == _T("textchanged")) {
		this->OnTextChange(msg);
	}
}

/// <summary>
/// 点击事件
/// </summary>
/// <param name="msg"></param>
void QQLoginForm::OnClick(TNotifyUI& msg) {
	if (msg.pSender == m_pCloseBtn)
	{
		this->Close();
	}
	else if (msg.pSender == m_pMinBtn) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else if (msg.pSender == m_pLoginBtn) {
		this->ToLogin();
	}
}

/// <summary>
/// 文字变化
/// </summary>
/// <param name="msg"></param>
void QQLoginForm::OnTextChange(TNotifyUI& msg) {
	if (msg.pSender == m_pUserNameUI)
	{
		int i = 0;
	}
}



LRESULT QQLoginForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_LOGIN_RESULT:  lRes = OnLoginResult(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcscmp(pstrClass, _T("CircleImage")) == 0) return new CircleImageUI;
		return NULL;
	}
};

/// <summary>
/// 登录接口消息处理
/// </summary>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <param name="bHandled"></param>
/// <returns></returns>
LRESULT QQLoginForm::OnLoginResult(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	string* result = (string*)lParam;
	Json::Reader reader;
	Json::Value root;

	if (reader.parse(*result, root))
	{
		int code = root["code"].asInt();

		//登录成功
		if (code == 1) {
			string tips = root["username"].asString() + "，欢迎登录";
			MessageBox(this->GetHWND(), _T(tips.data()), _T("登录成功"), MB_OK);
			this->SaveUsers();
		}
		//登录失败
		else {
			string msg = root["msg"].asString();
			MessageBox(this->GetHWND(), _T(msg.data()), _T("提示"), MB_OK);
		}
	}

	delete result;


	m_pLoginBtn->SetEnabled(true);

	return 0;
}
LRESULT  QQLoginForm::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	GetLastError();
	CDialogBuilder builder;
	CDuiString strResourcePath = m_pm.GetResourcePath();
	if (strResourcePath.IsEmpty())
	{
		strResourcePath = m_pm.GetInstancePath();
		strResourcePath += GetSkinFolder().GetData();
	}
	m_pm.SetResourcePath(strResourcePath.GetData());

	m_pm.Init(m_hWnd);
	CDialogBuilderCallbackEx cb;

	CControlUI* pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();

	return 0;
}


LRESULT QQLoginForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//注意触发顺序，OnClose OnDestroy OnFinalMessage
	bHandled = TRUE;
	ShowWindow(true);
	this->FreeParam();
	DestroyWindow(this->m_hWnd);
	return 0;
}

LRESULT QQLoginForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

LRESULT QQLoginForm::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT QQLoginForm::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT QQLoginForm::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}


LRESULT  QQLoginForm::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this)) {
		RECT rcSizeBox = m_pm.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top) {
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
			_tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
			_tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

/**
*大小改变的回调函数，可以尝试注释，修改代码 看效果
*/
LRESULT  QQLoginForm::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
		HRGN hRgn1 = ::CreateRectRgnIndirect(&rc);
		HRGN hRgn2 = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
		::CombineRgn(hRgn1, hRgn1, hRgn2, RGN_OR);
		::SetWindowRgn(*this, hRgn1, TRUE);
		::DeleteObject(hRgn1);
		::DeleteObject(hRgn2);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT  QQLoginForm::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT  QQLoginForm::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

void QQLoginForm::FreeParam() {
	int i = 0;
}