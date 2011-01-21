// Notepad.cpp

#include "Notepad.h"
#include "MainFrm.h"
#include "Label.h"

CNotepadApp notepadApp;

BOOL CNotepadApp::InitInstance() 
{
	CMainFrame* mainFrame = new CMainFrame;
	mainFrame->Create(NULL, _T("¸Þ¸ðÀå"));
	mainFrame->ShowWindow(SW_SHOW);
	mainFrame->UpdateWindow();
	this->m_pMainWnd = mainFrame;

	return TRUE;
}

