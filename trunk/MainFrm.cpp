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
	// 1. ����(����, spacebar)Ű�� ������
	if((nChar>='0' && nChar<='9') || (nChar>='A' && nChar<='z') || nChar == VK_SPACE)
	{
		// ���� ������ ���̿��� ���� ����
		if(this->label == 0)
		{
			Label detachedLabel = this->paper->Detach();
			this->label = new Label(detachedLabel);
		}
		this->column = this->label->Write(nChar); // ���ڸ� �󺧿� ���´�
	} 

	// 2. press the Enter key
	else if(nChar == VK_RETURN) 
	{
		// 1. �� ���� ������ ���̿��� ���� ����
		if(this->label == 0)
		{
			Label detachedLabel = this->paper->Detach();
			this->label = new Label(detachedLabel);
		}

		// 2.1 1st column�� �ִ� ���, �� ���� �ִ´�
		if(this->column == 0)
		{
			Label newLabel;
			this->paper->Attach(newLabel);
		}
		// 2-2. �߰��� �ִ�
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
		else // 2.3 last column�� �ִ� ���, �� ���� �����
		{
			this->paper->Attach(*(this->label)); // ���� ���̿� ���̴�

			delete this->label; // ���� �����
			this->label = new Label; // �� ���� �����
		}

		this->row++;
		this->column = 0;
	}
	// End [2. Enter Key]

	// Start [3. Press the Backspace key]
	else if(nChar == VK_BACK) 
	{ 
		// ���� ������ ���̿��� ���� ����
		if(this->label == 0)
		{
			Label detachedLabel = this->paper->Detach();
			this->label = new Label(detachedLabel);
		}

		if(this->column != 0) // ���� ��ġ�� 1st ĭ�� �ƴϸ�
		{
			this->column = this->label->Erase(this->label->GetCurrent()-1, 1);
		}
		// ���� ��ġ�� 1st ĭ�̰� 1st ���� �ƴϸ�
		else if(this->row != 0)			
		{
			// ���� �󺧿� ������ ���ڿ��� ���� ��
			if(this->label->GetLength() > 0)
			{
				char *buffer = this->label->Copy();

				delete this->label; // ���� �����ϴ�	

				Label dlabel = this->paper->Detach(); // ���� ���̿��� ����
				this->label = new Label(dlabel);
				this->row--;
				this->column = this->label->MoveEnd(); // ���� ������ ĭ���� �̵��ϴ�

				this->label->Write(buffer);
				delete buffer;

				this->label->Move(this->column);
			}
			else // ������ ���ڿ��� ���� ��
			{
				delete this->label; // ���� �����ϴ�	

				Label dlabel = this->paper->Detach(); // ���� ���̿��� ����
				this->label = new Label(dlabel);
				this->row--;
				this->column = this->label->MoveEnd(); // ���� ������ ĭ���� �̵��ϴ�
			}
		}
	}
	// End [Backspace key]

	// Start [press the Delete key]
	else if(nChar == VK_DELETE) 
	{ 
		// 1. ���� ������ ���̿��� ���� ����
		if(this->label == 0)
		{
			this->label = new Label(this->paper->Detach());
		}

		// 2.1. ���� ������ ĭ�� �ƴϸ�
		if(this->column < this->label->GetLength()) 
		{
			// 2.1.1. ���� ĭ�� �����
			this->column = this->label->Erase(this->label->GetCurrent(), 1);
		}
		// 2.2 ���� ������ ĭ�̰� ������ ���� �ƴϸ�
		else if(this->row < this->paper->GetLength())
		{
			// 2.2.1. ���� �󺧿� ������ ���ڿ��� ���� ��
			if(this->label->GetLength() > 0)
			{
				char *buffer = this->label->Copy();
				
				// 2.2.1.1. ���� �����ϴ�
				delete this->label; 
				this->label = new Label(this->paper->Detach());

				this->row--;
				// 2.2.1.2. ���� ������ ĭ���� �̵��ϴ�
				this->column = this->label->MoveEnd(); 
							
				this->label->Write(buffer);
				delete buffer;

				this->label->Move(this->column);
			}
			else // 2.2.2. ������ ���ڿ��� ���� ��
			{
				delete this->label; 
				this->label = new Label(this->paper->Detach());

				this->row--;
				this->column = this->label->MoveEnd(); // ���� ������ ĭ���� �̵��ϴ�
			}
		}
	}
	// End [Delete key]


	Invalidate(); // Ŭ���̾�Ʈ ������ ��ȿȭ�ϴ�

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

	// last column�� Label�̸�
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

	// last column�� Label�� �ƴϸ�
	// this->row < length
	else  
	{
		// 1. this->row �� �� ���
		for(int i = 0; i < this->row; i++)
		{
			Label label = this->paper->GetAt(i);
			CString str = (char*)(label);
			pDC->TextOut(m_ptTextOrigin.x, m_cyChar*i, str);
		}

		// 2.1 ���� ���
		if(label != 0)
		{
			// 2.1.1 this->row �� ���
			m_strInputText = (char*)(*(this->label));
			pDC->TextOut(m_ptTextOrigin.x, this->m_cyChar*this->row, m_strInputText);

			// 2.1.2 this->row �� �� ���
			for(int i = this->row; i < length; i++)
			{
				Label label = this->paper->GetAt(i);
				CString str = (char*)(label);
				pDC->TextOut(m_ptTextOrigin.x, m_cyChar*(i+1), str);
			}
		}
		// 2.2 ���� �ƴ� ���
		else
		{
			// 2.2.1 this->row �� ���
			Label label = this->paper->GetAt(i); // i = this->row
			m_strInputText = (char*)(label);
			pDC->TextOut(m_ptTextOrigin.x, m_cyChar*i, m_strInputText);

			// 2.2.2 this->row �� �� ���
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
		// 1st column: 1st row(0)�� �ƴϸ�(���� ũ��) previous row�� last column���� �̵���
		else if(this->row > 0) 
		{
			// ������ �� ���� ���̰� �̵��Ѵ�
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

		// Start [������ Ű�� ������ ��]
	case VK_RIGHT:

		// 1.1 label�� �ִٸ�
		if(this->label != 0) 
		{
			// 1.1.1.  ������ ĭ�� �ƴϸ�, ���� ĭ���� �̵��ϴ�
			if(this->column < this->label->GetLength())
			{
				this->column = this->label->MoveRight();
			}
			// 1.1.2. ������ ĭ�̸�
			else
			{
				// 1.1.2.1. ���� ���� ������
				if(this->row < this->paper->GetLength())
				{
					this->paper->Attach(*(this->label));

					// ���� �����ϴ�
					delete label; label = 0;

					this->paper->MoveDown();
					this->row++;
					this->column = this->paper->GetCurrent()->MoveHome(); // 0
				}
			}
		}
		// 1.2 ���� ������
		else 
		{
			// 1.2.1.  ������ ĭ�� �ƴϸ�, ���� ĭ���� �̵��ϴ�
			if(this->column < this->paper->GetCurrent()->GetLength())
			{
				this->column = this->paper->GetCurrent()->MoveRight();
			}
			// 2.2. ������ ĭ�̸�
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

		// ���� ���� Ű�� ���� ���
	case VK_UP:
		// 1. first line(0 row)�� �ƴϸ�
		if(this->row > 0)
		{
			// 1.1. ���� ������ if(label != 0)
			if(this->label != 0)
			{
				this->paper->Attach(*(this->label));
				// 1.1.2. ���� �����ϴ�
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
		// 1. ���� ���̸� ���ϴ� 
		int last = this->paper->GetLength();
		// 2. ���� ������ ���� ���� 
		if(this->label == 0) 
		{
			last--;
		}
		// 3. last line�� �ƴϸ� 
		if(this->row < last)
		{
			// 3.1 ���� ������
			if(this->label != 0)
			{
				// 3.1.1. ���� ���̴�
				this->paper->Attach(*(this->label));
				// 3.1.2. ���� �����ϴ�
				delete label; label = 0;
			}
			// 3.2. ���� �ٷ� �̵��ϴ� 
			this->paper->MoveDown();
			// 3.3. �� ����
			this->row++;
			// 3.3. x��° ĭ���� �̵��ϴ�
			this->column = this->paper->GetCurrent()->Move(this->column);
		}
		break;
	}

	Invalidate(); // Ŭ���̾�Ʈ ������ ��ȿȭ�ϴ�
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
