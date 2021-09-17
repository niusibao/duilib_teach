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
	//坐标
	POINT    pt = { m_rcItem.left, m_rcItem.top };

	//大小
	SIZE    sz = { m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top };

	Graphics    graphics(hDC);
	if (graphics.GetLastStatus() != Ok)
		return;

	//消除锯齿
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	GraphicsPath graphicspath;
	if (graphicspath.GetLastStatus() != Ok)
		return;

	graphicspath.AddEllipse(pt.x, pt.y, sz.cx, sz.cy);

	//设置裁剪圆
	graphics.SetClip(&graphicspath, CombineModeReplace);
	WCHAR avatar[11] = L"avatar.jpg";
	Image image(avatar);
	if (image.GetLastStatus() != Ok)
		return;

	//绘制图像
	graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);

	//绘制一个1像素宽度的圆形，用于消除锯齿
	Pen    myPen(GetPenColor(hDC), GetPenWidth());
	if (myPen.GetLastStatus() != Ok)
		return;

	graphics.DrawEllipse(&myPen, pt.x, pt.y, sz.cx, sz.cy);
}

DWORD CircleImageUI::GetPenColor(HDC hDC)
{
	if (IsAutoPenColor())
	{
		//像素值颜色取点（ pt.x + 1, pt.y + 1）的值
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