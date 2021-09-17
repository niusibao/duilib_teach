#pragma once

#include "..\DuiLib\UIlib.h"

using namespace DuiLib;

class CircleImageUI: public CButtonUI
{
public:

    CircleImageUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void PaintBkImage(HDC hDC);

    void SetDefaultBkImage(LPCTSTR pStrImage) { m_sDefaultBkImage = pStrImage; }
    CDuiString GetDefaultBkImage() { return m_sDefaultBkImage; }
    void SetAutoPenColor(bool bAuto) { m_bAutoPenColor = bAuto; }
    bool IsAutoPenColor() { return m_bAutoPenColor; }
    void SetPenColor(DWORD dwColor) { m_dwPenColor = dwColor; }
    DWORD GetPenColor(HDC hDC);
    void SetPenWidth(int nPenWidth) { m_nPenWidth = nPenWidth; }
    int GetPenWidth() { return m_nPenWidth; }

    bool IsHeadImageExist(LPCTSTR pStrImage);

private:

    CDuiString m_sDefaultBkImage;
    bool m_bAutoPenColor;
    DWORD m_dwPenColor;
    int m_nPenWidth;
};

