#include "CircleImageUI.h"
#include "StdAfx.h"
#include "atlstr.h"

CircleImageUI::CircleImageUI()
{
	m_sDefaultBkImage = _T("qq_main.png");
	m_bAutoPenColor = false;
	m_dwPenColor = Color(255, 255, 255, 255).GetValue();
	m_nPenWidth = 2;
}

LPCTSTR CircleImageUI::GetClass() const
{
	return _T("CircleImageUI");
}

LPVOID CircleImageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("CircleImage")) == 0) return static_cast<CircleImageUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CircleImageUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("defaultbkimage")) == 0) SetDefaultBkImage(pstrValue);
	else if (_tcscmp(pstrName, _T("bkimage")) == 0) SetBkImage(pstrValue);
	else if (_tcscmp(pstrName, _T("pencolor")) == 0) {
		while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetPenColor(clrColor);
	}
	else if (_tcscmp(pstrName, _T("autopencolor")) == 0) SetAutoPenColor(_tcscmp(pstrValue, _T("true")) == 0);
	else if (_tcscmp(pstrName, _T("penwidth")) == 0) SetPenWidth(_ttoi(pstrValue));
	else return CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CircleImageUI::PaintBkImage(HDC hDC)
{
	//����
	POINT    pt = { m_rcItem.left, m_rcItem.top };

	//��С
	SIZE    sz = { m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top };

	Graphics    graphics(hDC);
	if (graphics.GetLastStatus() != Ok)
		return;

	//�������
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	GraphicsPath graphicspath;
	if (graphicspath.GetLastStatus() != Ok)
		return;

	graphicspath.AddEllipse(pt.x, pt.y, sz.cx, sz.cy);

	//���òü�Բ
	graphics.SetClip(&graphicspath, CombineModeReplace);
	WCHAR avatar[11] = L"avatar.jpg";
	Image image(avatar);
	if (image.GetLastStatus() != Ok)
		return;

	//����ͼ��
	graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);

	//����һ��1���ؿ�ȵ�Բ�Σ������������
	Pen    myPen(GetPenColor(hDC), GetPenWidth());
	if (myPen.GetLastStatus() != Ok)
		return;

	graphics.DrawEllipse(&myPen, pt.x, pt.y, sz.cx, sz.cy);
}

DWORD CircleImageUI::GetPenColor(HDC hDC)
{
	if (IsAutoPenColor())
	{
		//����ֵ��ɫȡ�㣨 pt.x + 1, pt.y + 1����ֵ
		RECT rc = GetPos();
		COLORREF color = GetPixel(hDC, rc.left + 1, rc.top + 1);

		BYTE r = GetRValue(color);
		BYTE g = GetGValue(color);
		BYTE b = GetBValue(color);

		return Color(255, r, g, b).GetValue();
	}

	return m_dwPenColor;
}

bool CircleImageUI::IsHeadImageExist(LPCTSTR pStrImage)
{
	return GetFileAttributes(pStrImage) == -1 ? false : true;
}