// MainFrm.cpp

#include "MainFrm.h"
#include "Paper.h"

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
	this->row = 0;
	this->column = 0;
	this->paper = 0;
	this->label = 0;
}

CMainFrame::~CMainFrame(){}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->paper = new Paper;
	this->label = new Label;

	CClientDC dc (this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	this->m_cxChar = tm.tmAveCharWidth/4;
	this->m_cyChar = tm.tmHeight;	

	m_ptTextOrigin.x = m_cxChar;
	m_ptTextOrigin.y = m_cyChar/4;
	m_ptCaretPos = m_ptTextOrigin;

	return 0;
}

void CMainFrame::OnClose()
{
	if(this->paper != 0)
	{
		delete this->paper;
	}
	if(this->label != 0) 
	{
		delete this->label;
	}
	CFrameWnd::OnClose();
}

void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 1. 문자(숫자, spacebar)키를 누르면
	if((nChar>='0' && nChar<='9') || (nChar>='A' && nChar<='z') || nChar == VK_SPACE)
	{
		// 라벨이 없으면 종이에서 라벨을 뗀다
		if(this->label == 0)
		{
			Label detachedLabel = this->paper->Detach();
			this->label = new Label(detachedLabel);
		}
		this->column = this->label->Write(nChar); // 문자를 라벨에 적는다
	} 

	// 2. press the Enter key
	else if(nChar == VK_RETURN) 
	{
		// 1. 뗀 라벨이 없으면 종이에서 라벨을 뗀다
		if(this->label == 0)
		{
			Label detachedLabel = this->paper->Detach();
			this->label = new Label(detachedLabel);
		}

		// 2.1 1st column에 있는 경우, 빈 라벨을 넣는다
		if(this->column == 0)
		{
			Label newLabel;
			this->paper->Attach(newLabel);
		}
		// 2-2. 중간에 있다
		else if(this->column < this->label->GetLength())
		{
			char* buffer = this->label->Copy(0, this->column);
			Label prevLabel;
			prevLabel.Write(buffer);
			this->paper->Attach(prevLabel);
			delete buffer;

			buffer = this->label->Copy(this->column, this->label->GetLength()-this->column);
			delete this->label;
			this->label = new Label;
			this->label->Write(buffer);
			delete buffer;
		}
		else // 2.3 last column에 있는 경우, 새 라벨을 만들다
		{
			this->paper->Attach(*(this->label)); // 라벨을 종이에 붙이다

			delete this->label; // 라벨을 지우다
			this->label = new Label; // 새 라벨을 만들다
		}

		this->row++;
		this->column = 0;
	}
	// End [2. Enter Key]

	// Start [3. Press the Backspace key]
	else if(nChar == VK_BACK) 
	{ 
		// 라벨이 없으면 종이에서 라벨을 뗀다
		if(this->label == 0)
		{
			Label detachedLabel = this->paper->Detach();
			this->label = new Label(detachedLabel);
		}

		if(this->column != 0) // 현재 위치가 1st 칸이 아니면
		{
			this->column = this->label->Erase(this->label->GetCurrent()-1, 1);
		}
		// 현재 위치가 1st 칸이고 1st 줄이 아니면
		else if(this->row != 0)			
		{
			// 지금 라벨에 복사할 문자열이 있을 때
			if(this->label->GetLength() > 0)
			{
				char *buffer = this->label->Copy();

				delete this->label; // 라벨을 삭제하다	

				Label dlabel = this->paper->Detach(); // 라벨을 종이에서 떼다
				this->label = new Label(dlabel);
				this->row--;
				this->column = this->label->MoveEnd(); // 라벨의 마지막 칸으로 이동하다

				this->label->Write(buffer);
				delete buffer;

				this->label->Move(this->column);
			}
			else // 복사할 문자열이 없을 때
			{
				delete this->label; // 라벨을 삭제하다	

				Label dlabel = this->paper->Detach(); // 라벨을 종이에서 떼다
				this->label = new Label(dlabel);
				this->row--;
				this->column = this->label->MoveEnd(); // 라벨의 마지막 칸으로 이동하다
			}
		}
	}
	// End [Backspace key]

	// Start [press the Delete key]
	else if(nChar == VK_DELETE) 
	{ 
		// 1. 라벨이 없으면 종이에서 라벨을 뗀다
		if(this->label == 0)
		{
			this->label = new Label(this->paper->Detach());
		}

		// 2.1. 현재 마지막 칸이 아니면
		if(this->column < this->label->GetLength()) 
		{
			// 2.1.1. 현재 칸을 지우다
			this->column = this->label->Erase(this->label->GetCurrent(), 1);
		}
		// 2.2 현재 마지막 칸이고 마지막 줄이 아니면
		else if(this->row < this->paper->GetLength())
		{
			// 2.2.1. 지금 라벨에 복사할 문자열이 있을 때
			if(this->label->GetLength() > 0)
			{
				char *buffer = this->label->Copy();
				
				// 2.2.1.1. 라벨을 삭제하다
				delete this->label; 
				this->label = new Label(this->paper->Detach());

				this->row--;
				// 2.2.1.2. 라벨의 마지막 칸으로 이동하다
				this->column = this->label->MoveEnd(); 
							
				this->label->Write(buffer);
				delete buffer;

				this->label->Move(this->column);
			}
			else // 2.2.2. 복사할 문자열이 없을 때
			{
				delete this->label; 
				this->label = new Label(this->paper->Detach());

				this->row--;
				this->column = this->label->MoveEnd(); // 라벨의 마지막 칸으로 이동하다
			}
		}
	}
	// End [Delete key]


	Invalidate(); // 클라이언트 영역을 무효화하다

	CWnd::OnChar(nChar,nRepCnt,nFlags);
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this);

	HideCaret();
	DrawInputText (&dc);
	PositionCaret(&dc);
	ShowCaret();
}

void CMainFrame::DrawInputText (CDC* pDC)
{
	int length = this->paper->GetLength();

	// last column이 Label이면
	if(this->row == length)
	{
		for(int i = 0; i < length; i++)
		{
			Label label = this->paper->GetAt(i);
			CString str = (char*)(label);
			pDC->TextOut(m_ptTextOrigin.x, m_cyChar*i, str);
		}
		m_strInputText = (char*)(*(this->label));
		pDC->TextOut(m_ptTextOrigin.x, this->m_cyChar*this->row, m_strInputText);
	}

	// last column이 Label이 아니면
	// this->row < length
	else  
	{
		// 1. this->row 앞 줄 출력
		for(int i = 0; i < this->row; i++)
		{
			Label label = this->paper->GetAt(i);
			CString str = (char*)(label);
			pDC->TextOut(m_ptTextOrigin.x, m_cyChar*i, str);
		}

		// 2.1 라벨인 경우
		if(label != 0)
		{
			// 2.1.1 this->row 라벨 출력
			m_strInputText = (char*)(*(this->label));
			pDC->TextOut(m_ptTextOrigin.x, this->m_cyChar*this->row, m_strInputText);

			// 2.1.2 this->row 뒤 줄 출력
			for(int i = this->row; i < length; i++)
			{
				Label label = this->paper->GetAt(i);
				CString str = (char*)(label);
				pDC->TextOut(m_ptTextOrigin.x, m_cyChar*(i+1), str);
			}
		}
		// 2.2 라벨이 아닌 경우
		else
		{
			// 2.2.1 this->row 줄 출력
			Label label = this->paper->GetAt(i); // i = this->row
			m_strInputText = (char*)(label);
			pDC->TextOut(m_ptTextOrigin.x, m_cyChar*i, m_strInputText);

			// 2.2.2 this->row 뒤 줄 출력
			for(int i = this->row+1; i < length; i++)
			{
				Label label = this->paper->GetAt(i);
				CString str = (char*)(label);
				pDC->TextOut(m_ptTextOrigin.x, m_cyChar*i, str);
			}
		}
	}
}

void CMainFrame::PositionCaret (CDC* pDC)
{/*
 BOOL bRelease = FALSE;

 if (pDC == NULL) {
 pDC = GetDC();
 bRelease = TRUE;
 }
 */
	CPoint point = m_ptTextOrigin;
	//CString string = m_strInputText.Left(m_nTextPos);
	CString string = m_strInputText.Left(this->column);
	point.x += (pDC->GetTextExtent(string, string.GetLength())).cx;
	point.y = this->m_cyChar * this->row;
	SetCaretPos(point);

	//
	// Release the device context if it was created inside this function.
	//
	//    if (bRelease)
	//        ReleaseDC (pDC);
}

void CMainFrame::OnSetFocus(CWnd* pWnd)
{
	CreateSolidCaret(m_cxChar, m_cyChar);
	SetCaretPos(m_ptCaretPos);
	ShowCaret();
}

void CMainFrame::OnKillFocus (CWnd* pWnd)
{
	//
	// Hide the caret when the VisualKB window loses the input focus.
	//
	HideCaret ();
	m_ptCaretPos = GetCaretPos();
	::DestroyCaret ();
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar) {
	case VK_LEFT:
		// is not the 1st column
		if(this->column != 0) 
		{
			if(label != 0) 
			{
				this->column = this->label->MoveLeft();
			}
			else if(label == 0)
			{
				this->column = this->paper->GetCurrent()->MoveLeft();
			}
		}
		// 1st column: 1st row(0)가 아니면(보다 크면) previous row의 last column으로 이동함
		else if(this->row > 0) 
		{
			// 지금이 라벨 위면 붙이고 이동한다
			if(label != 0) 
			{
				this->paper->Attach(*(this->label));
				delete this->label;
				this->label = 0;
			}

			this->paper->MoveUp();
			this->row--;
			this->column = this->paper->GetCurrent()->MoveEnd();
		}

		break;

		// Start [오른쪽 키를 눌렀을 때]
	case VK_RIGHT:

		// 1.1 label이 있다면
		if(this->label != 0) 
		{
			// 1.1.1.  마지막 칸이 아니면, 다음 칸으로 이동하다
			if(this->column < this->label->GetLength())
			{
				this->column = this->label->MoveRight();
			}
			// 1.1.2. 마지막 칸이면
			else
			{
				// 1.1.2.1. 다음 줄이 있으면
				if(this->row < this->paper->GetLength())
				{
					this->paper->Attach(*(this->label));

					// 라벨을 삭제하다
					delete label; label = 0;

					this->paper->MoveDown();
					this->row++;
					this->column = this->paper->GetCurrent()->MoveHome(); // 0
				}
			}
		}
		// 1.2 라벨이 없으면
		else 
		{
			// 1.2.1.  마지막 칸이 아니면, 다음 칸으로 이동하다
			if(this->column < this->paper->GetCurrent()->GetLength())
			{
				this->column = this->paper->GetCurrent()->MoveRight();
			}
			// 2.2. 마지막 칸이면
			else
			{
				if(this->row < this->paper->GetLength()-1)
				{
					this->paper->MoveDown();
					this->row++;
					this->column = this->paper->GetCurrent()->MoveHome(); // 0
				}
			}			
		}

		break;

		// 위로 가기 키를 누른 경우
	case VK_UP:
		// 1. first line(0 row)이 아니면
		if(this->row > 0)
		{
			// 1.1. 라벨이 있으면 if(label != 0)
			if(this->label != 0)
			{
				this->paper->Attach(*(this->label));
				// 1.1.2. 라벨을 삭제하다
				delete label; label = 0;
			}
			this->paper->MoveUp();
			this->row--;

			this->column = this->paper->GetCurrent()->Move(this->column);
			/*
			if(this->column <= this->paper->GetCurrent()->GetLength())
			this->column = this->paper->GetCurrent()->Move(this->column);
			else
			this->column = this->paper->GetCurrent()->MoveEnd();
			*/
		}
		break;

	case VK_DOWN:
		// 1. 줄의 길이를 구하다 
		int last = this->paper->GetLength();
		// 2. 라벨이 없으면 길이 감소 
		if(this->label == 0) 
		{
			last--;
		}
		// 3. last line이 아니면 
		if(this->row < last)
		{
			// 3.1 라벨이 있으면
			if(this->label != 0)
			{
				// 3.1.1. 라벨을 붙이다
				this->paper->Attach(*(this->label));
				// 3.1.2. 라벨을 삭제하다
				delete label; label = 0;
			}
			// 3.2. 다음 줄로 이동하다 
			this->paper->MoveDown();
			// 3.3. 줄 증가
			this->row++;
			// 3.3. x번째 칸으로 이동하다
			this->column = this->paper->GetCurrent()->Move(this->column);
		}
		break;
	}

	Invalidate(); // 클라이언트 영역을 무효화하다
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
