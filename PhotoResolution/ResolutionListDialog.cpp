#include "ResolutionListDialog.h"

vector<string> nameList;

CDuiString ResolutionListDialog::GetSkinFolder() {
	return _T("skin\\ResolutionList\\");
}

CDuiString ResolutionListDialog::GetSkinFile()
{
	return _T("skin.xml");
}

LPCTSTR ResolutionListDialog::GetWindowClassName() const
{
	return _T("ResolutionList");
};


void ResolutionListDialog::OnFinalMessage(HWND hWnd)
{

};


void ResolutionListDialog::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		MessageBox(GetHWND(), msg.pSender->GetText(), "分辨率", MB_OK);
	}
};

LRESULT ResolutionListDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
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
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}


LRESULT ResolutionListDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDialogBuilder builder;

	CDuiString strResourcePath = m_PaintManager.GetResourcePath();
	if (strResourcePath.IsEmpty())
	{
		strResourcePath = m_PaintManager.GetInstancePath();
		strResourcePath += GetSkinFolder().GetData();
	}
	m_PaintManager.SetResourcePath(strResourcePath.GetData());

	//init
	m_PaintManager.Init(m_hWnd);

	//xml
	CControlUI* pRoot = builder.Create(GetSkinFile().GetData(), 0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);

	m_PaintManager.AddNotifier(this);

	//init
	Init();

	return 0;
}

LRESULT ResolutionListDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	ShowWindow(true);
	DestroyWindow(this->GetHWND());
	return 0;
}

LRESULT ResolutionListDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

//shield system bar
LRESULT ResolutionListDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT ResolutionListDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void ResolutionListDialog::Init() {

	m_pImageSizeList = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl("img_size_list"));

	//按钮数据
	nameList.push_back("1920*1080");
	nameList.push_back("1280*960");
	nameList.push_back("960*540");

	m_pButtonHeight = 50;
	m_pButtonWidth = 170;
	m_pMargin = 10;

	CreateImgList();
}

CControlUI* ResolutionListDialog::CreateImgItem(string name, int i) {
	//水平layout
	CHorizontalLayoutUI* layout = new CHorizontalLayoutUI;
	layout->SetFixedHeight(m_pButtonHeight + m_pMargin);
	CDuiString attribute;
	attribute.Format("padding=\"0, %d, 0, 0\"", m_pMargin);
	layout->SetAttributeList(attribute.GetData());
	//左边空白
	CControlUI* left = new CControlUI;

	layout->Add(left);

	//中间按钮
	CButtonUI* button = new CButtonUI;
	button->SetText(name.c_str());
	button->SetName(_T("img_size_btn_" + i));
	button->SetAttributeList("font=\"1\" textcolor=\"#FF000000\" hottextcolor=\"#FFFFFFFF\" pushedtextcolor=\"#FFFFFFFF\" focusedtextcolor=\"#FFFFFFFF\"");	button->SetFixedWidth(m_pButtonWidth);
	button->SetFixedHeight(m_pButtonHeight);
	button->SetNormalImage("btn_normal.png");
	button->SetHotImage("btn_press.png");
	button->SetPushedImage("btn_press.png");
	button->SetFocusedImage("btn_press.png");

	layout->Add(button);

	//右边空白
	CControlUI* right = new CControlUI;
	layout->Add(right);
	return layout;
}

void ResolutionListDialog::CreateImgList() {
	auto iterator = nameList.begin();
	int i = 0;
	// 循环添加按钮
	while (iterator != nameList.end()) {
		string name = *iterator;

		this->m_pImageSizeList->Add(CreateImgItem(name, i));

		iterator++;
		i++;
	}
	ResizeClient(m_pButtonWidth + 2 * m_pMargin, (m_pButtonHeight + m_pMargin) * nameList.size());
}