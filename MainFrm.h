// MainFrm.h

#ifndef _MAINFRM_H
#define _MAINFORM_H

#include <afxwin.h>

class Paper;
class Label;
class CMainFrame:public CFrameWnd
{
public:
	CMainFrame();
	~CMainFrame();

public:
	Paper* paper;
	Label* label;
	int row;
	int column;

	int m_cxChar;                // Average character width
	int m_cyChar;                // Character height
	int m_nTextPos;              // Index of current character in text box
	CPoint m_ptTextOrigin;       // Origin for drawing input text
	CPoint m_ptCaretPos;         // Current caret position
	CString m_strInputText;                // Input text

protected:
	void DrawInputText (CDC* pDC);
	void PositionCaret (CDC* pDC = NULL);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus (CWnd* pWnd);
	afx_msg void OnKillFocus (CWnd* pWnd);

	DECLARE_MESSAGE_MAP()
};

#endif //_MAINFORM_H