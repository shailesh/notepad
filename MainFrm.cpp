// MainFrm.cpp

#include "MainFrm.h"
#include "Paper.h"
#include "Toolbox.h"
#include "Pen.h"
#include "Clipboard.h"

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
	this->row = 0;
	this->column = 0;
	this->paper = 0;
	this->label = 0;
	this->ptStart.x = -1;
	this->ptStart.y = -1;
	this->toolbox = 0;
	this->pen = 0;
	this->clipboard = 0;
}

CMainFrame::~CMainFrame(){}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->paper = new Paper;
	this->label = new Label;
	this->pen = new Pen;
	this->toolbox = new Toolbox;
	this->clipboard = new Clipboard;

	CClientDC dc (this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	this->cxChar = tm.tmAveCharWidth/4;
	this->cyChar = tm.tmHeight;	

	ptTextOrigin.x = cxChar;
	ptTextOrigin.y = cyChar/4;
	m_ptCaretPos = ptTextOrigin;

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
	if(this->toolbox != 0)
	{
		delete this->toolbox;
	}
	if(this->clipboard != 0)
	{
		delete this->clipboard;
	}

	CFrameWnd::OnClose();
}

void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_RETURN:
		if(this->toolbox->GetShift() == false)
		{
			this->pen->OnEnterKeyDown(&(this->paper), &(this->label), this->column);
			this->row++;
			this->column = 0;
		}
		break;
	
	// Backspace Key
	case VK_BACK:
		// Delete not selected, just cursor
		if(this->ptStart.x < 0) 
		{
			this->pen->OnBackspaceKeyDown(&(this->paper), &(this->label), &(this->column), &(this->row));
		} 
		// Delete selected section : shift key pressed & Backspace key
		else
		{
			//this->pen->OnBackspaceKeyDown(&(this->paper), &(this->label), &(this->column), &(this->row), this->ptStart);
			if(this->ptStart.y == this->row)
			{
				if(this->label == 0)
				{
					this->label = new Label(this->paper->Detach());
				}

				if(this->ptStart.x < this->column)
				{
					this->column = this->label->Erase(ptStart.x, this->column-this->ptStart.x);
				}
				else if(this->ptStart.x > this->column)
				{
					this->column = this->label->Erase(this->column, this->ptStart.x-this->column);
				}
			}
			else
			{
				CPoint ptFirst(0, 0);
				CPoint ptLast(0, 0);
				CString strFirst, strLast;
				
				Label lbStart(this->paper->GetAt(this->ptStart.y));
				Label lbCurrent(*(this->paper->GetCurrent()));

				if(this->ptStart.y < this->row)
				{
					ptFirst.Offset(this->ptStart);
					ptLast.Offset(this->column, this->row);
					strFirst = (char*)(lbStart);
					strLast = (char*)(lbCurrent);
				}
				else
				{
					ptFirst.Offset(this->column, this->row);
					ptLast.Offset(this->ptStart);
					strFirst = (char*)(lbCurrent);
					strLast = (char*)(lbStart);
				}

				this->label = new Label(this->paper->Detach(&(this->paper->GetAt(ptLast.y))));

				char* buffer = this->label->Copy(ptLast.x, this->label->GetLength()-ptLast.x);
				delete this->label;

				for(int i = ptLast.y-1; i > ptFirst.y; i--)
				{
					this->label = new Label(this->paper->Detach(&(this->paper->GetAt(i))));
					delete this->label;
				}
				this->label = new Label(this->paper->Detach(&(this->paper->GetAt(ptFirst.y))));
				this->column = this->label->Erase(ptFirst.x, this->label->GetLength()-ptFirst.x);
				this->label->Store(buffer);
				delete buffer;
				this->row = ptFirst.y;
			}
			this->toolbox->ShiftUp();
			this->ptStart.x = -1;
			this->ptStart.y = -1;
		}
		break;

	default:
		if((nChar>='0' && nChar<='9') || (nChar>='A' && nChar<='z') || nChar == VK_SPACE)
		{
			if(this->label == 0)
			{
				//this->label = new Label(this->paper->Detach());
				this->label = new Label(this->paper->Detach(this->paper->GetCurrent()));
			}
			this->column = this->label->Write(nChar);
		}
		break;
	}// switch case
	
	//if(GetKeyState(VK_CONTROL) & 0x8000)
	if(this->toolbox->GetControl() && nFlags == 46)
	{
		if(this->ptStart.x > -1)
		{
			if(this->ptStart.y == this->row)
			{
				int index, count;

				this->clipboard = new Clipboard(1);
				if(this->ptStart.x < this->column)
				{
					index = this->ptStart.x;
					count = this->column - this->ptStart.x;
				}
				else if(this->ptStart.x > this->column)
				{
					index = this->column;
					count = this->ptStart.x - this->column;
				}
				Label label;
				if(this->label != 0)
				{
					label = *(this->label);
				}
				else
				{
					label = *(this->paper->GetCurrent());
				}
				char* copy = label.Copy(index, count);
				this->clipboard->Record(copy);
				//cstring.Format("%s", this->clipboard->GetAt(0).c_str());
				cstring = this->clipboard->GetAt(0);
			}
			else
			{
				CPoint first;
				CPoint last;

				if(this->ptStart.y < this->row)
				{
					first = this->ptStart;
					last = CPoint(this->column, this->row);
				}
				else
				{
					first = CPoint(this->column, this->row);
					last =  this->ptStart;
				}

				this->clipboard = new Clipboard(last.y - first.y);
			}
		}
	}

	Invalidate();
	CWnd::OnChar(nChar,nRepCnt,nFlags);
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this);

	HideCaret();
	DrawInputText (&dc);
	if(this->ptStart.x > -1)
	{
		DrawBackground(&dc);
	}
	PositionCaret(&dc);
	ShowCaret();
}

void CMainFrame::DrawInputText (CDC* pDC)
{
	CString str;
	int i;
	
	if(this->label == 0)
	{
		for(i = 0; i < this->paper->GetLength(); i++)
		{
			Label label = this->paper->GetAt(i);
			str = (char*)(label);
			pDC->TextOut(0, this->cyChar*i, str);
		}
	}
	else  
	{
		for(i = 0; i < this->row; i++)
		{
			Label label = this->paper->GetAt(i);
			str = (char*)(label);
			pDC->TextOut(0, cyChar*i, str);
		}

		str = (char*)(*(this->label));
		pDC->TextOut(0, this->cyChar*this->row, str);

		for(i = this->row; i < this->paper->GetLength(); i++)
		{
			Label label = this->paper->GetAt(i);
			str = (char*)(label);
			pDC->TextOut(0, this->cyChar*(i+1), str);
		}
	}

	pDC->TextOut(100, 100, this->cstring);
}

void CMainFrame::DrawBackground(CDC* pDC)
{
	CString str;
	pDC->SetBkColor(RGB(192, 192, 192));

	if(this->ptStart.y == this->row)
	{
		if(this->label != 0)
		{
			str = (char*)(*(this->label));
		}
		else
		{
			Label label = *(this->paper->GetCurrent());
			str = (char*)(label);
		}

		if(this->ptStart.x < this->column)
		{
			CString left = str.Left(this->ptStart.x);
			pDC->TextOut((pDC->GetTextExtent(left, left.GetLength())).cx, this->cyChar*this->row, str.Mid(this->ptStart.x, this->column-this->ptStart.x));
		}
		else if(this->ptStart.x > this->column)
		{
			CString left = str.Left(this->column);
			pDC->TextOut((pDC->GetTextExtent(left, left.GetLength())).cx, this->cyChar*this->row, str.Mid(this->column, this->ptStart.x-this->column));
		}
	}
	else
	{
		CPoint first;
		CPoint last;
		CString strFirst, strLast;
		
		Label lbStart(this->paper->GetAt(this->ptStart.y));
		Label lbCurrent(*(this->paper->GetCurrent()));

		if(this->ptStart.y < this->row)
		{
			first = this->ptStart;
			last = CPoint(this->column, this->row);
			strFirst = (char*)(lbStart);
			strLast = (char*)(lbCurrent);
		}
		else
		{
			first = CPoint(this->column, this->row);
			last = this->ptStart;
			strFirst = (char*)(lbCurrent);
			strLast = (char*)(lbStart);	
		}

		CString left = strFirst.Left(first.x);
		pDC->TextOut((pDC->GetTextExtent(left, left.GetLength())).cx, this->cyChar*first.y, strFirst.Right(strFirst.GetLength()-first.x));

		for(int i = first.y+1; i < last.y; i++)
		{
			Label label = this->paper->GetAt(i);
			str = (char*)(label);
			pDC->TextOut(0, this->cyChar*i, str);
		}
		left = strLast.Left(last.x);
		pDC->TextOut(0, this->cyChar*last.y, left);
	}
}

void CMainFrame::PositionCaret (CDC* pDC)
{
	CString str;
	CPoint point;

	if(this->label != 0)
	{
		str = (char*)(*(this->label));
	}
	else
	{
		Label label = *(this->paper->GetCurrent());
		str = (char*)(label);
	}

	CString string = str.Left(this->column);

	point.x = (pDC->GetTextExtent(string, string.GetLength())).cx;
	point.y = this->cyChar * this->row;
	SetCaretPos(point);
}

void CMainFrame::OnSetFocus(CWnd* pWnd)
{
	CreateSolidCaret(this->cxChar, this->cyChar);
	SetCaretPos(this->m_ptCaretPos);
	ShowCaret();
}

void CMainFrame::OnKillFocus (CWnd* pWnd)
{
	//
	// Hide the caret when the VisualKB window loses the input focus.
	//
	HideCaret ();
	m_ptCaretPos = GetCaretPos();
	::DestroyCaret();
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar) {

	case VK_SHIFT:
		if(this->toolbox->GetShift() == false)
		{
			this->toolbox->ShiftDown();
			if(this->ptStart.x == -1)
			{
				this->ptStart.x = this->column;
				this->ptStart.y = this->row;
			}
		}
		break;

	case VK_CONTROL:
		if(this->toolbox->GetControl() == false)
		{
			this->toolbox->ControlDown();
		}
		//if(GetKeyState (VK_CONTROL) & 0x8000){}

		break;

	case VK_LEFT:
		if(this->column > 0) 
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
		else if(this->row > 0) 
		{
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

		if(this->toolbox->GetShift() == false)
		{
			this->ptStart.x = -1;
			this->ptStart.y = -1;
		}

		break;

	case VK_RIGHT:
		if(this->label != 0) 
		{
			if(this->column < this->label->GetLength())
			{
				this->column = this->label->MoveRight();
			}
			else
			{
				if(this->row < this->paper->GetLength())
				{
					this->paper->Attach(*(this->label));

					delete this->label; 
					this->label = 0;

					this->paper->MoveDown();
					this->row++;
					this->column = this->paper->GetCurrent()->MoveHome();
				}
			}
		}
		else 
		{
			if(this->column < this->paper->GetCurrent()->GetLength())
			{
				this->column = this->paper->GetCurrent()->MoveRight();
			}
			else
			{
				if(this->row < this->paper->GetLength()-1)
				{
					this->paper->MoveDown();
					this->row++;
					this->column = this->paper->GetCurrent()->MoveHome(); 
				}
			}			
		}

		if(this->toolbox->GetShift() == false)
		{
			this->ptStart.x = -1;
			this->ptStart.y = -1;
		}
		break;

	case VK_UP:

		if(this->row > 0)
		{
			if(this->label != 0)
			{
				this->paper->Attach(*(this->label));
				delete this->label; 
				this->label = 0;
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

		if(this->toolbox->GetShift() == false)
		{
			this->ptStart.x = -1;
			this->ptStart.y = -1;
		}
		break;
	
	case VK_DOWN:

		if(this->label != 0)
		{
			if(this->row < this->paper->GetLength())
			{
				this->paper->Attach(*(this->label));
				delete this->label; 
				this->label = 0;

				this->paper->MoveDown();
				this->row++;
				this->column = this->paper->GetCurrent()->Move(this->column);
			}
		}
		else if(this->row < this->paper->GetLength()-1)
		{
			this->paper->MoveDown();
			this->row++;
			this->column = this->paper->GetCurrent()->Move(this->column);
		}

		if(this->toolbox->GetShift() == false)
		{
			this->ptStart.x = -1;
			this->ptStart.y = -1;
		}
		break;
	
	case VK_HOME:
		if(this->label != 0)
		{
			this->column = this->label->MoveHome();
		}
		else
		{
			this->column = this->paper->GetCurrent()->MoveHome();
		}

		if(this->toolbox->GetShift() == false)
		{
			this->ptStart.x = -1;
			this->ptStart.y = -1;
		}
		break;

	case VK_END:
		if(this->label != 0)
		{
			this->column = this->label->MoveEnd();
		}
		else
		{
			this->column = this->paper->GetCurrent()->MoveEnd();
		}

		if(this->toolbox->GetShift() == false)
		{
			this->ptStart.x = -1;
			this->ptStart.y = -1;
		}
		break;

	case VK_DELETE:

		if(this->label == 0)
		{
			this->label = new Label(this->paper->Detach());
		}

		if(this->column < this->label->GetLength()) 
		{
			this->column = this->label->Erase(this->label->GetCurrent(), 1);
		}
		else if(this->row < this->paper->GetLength())
		{
			if(this->label->GetLength() > 0)
			{
				char *buffer = this->label->Copy();
				this->paper->MoveDown();
				delete this->label;
				this->label = new Label(this->paper->Detach());
				this->label->MoveHome();

				this->column = this->label->Write(buffer);
				delete buffer;
			}
			else 
			{
				this->paper->MoveDown();
				delete this->label;
				this->label = new Label(this->paper->Detach());
				this->column = this->label->MoveHome();
			}
		}
		break;
	}

	Invalidate(); 
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_SHIFT:
		this->toolbox->ShiftUp();
		break;
	case VK_CONTROL:
		this->toolbox->ControlUp();
		break;
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}