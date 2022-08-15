#include "stdafx.h"
#include "RacingWheelTracker.h"
#include "RacingWheelTrackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CRacingWheelTrackerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CRacingWheelTrackerApp::CRacingWheelTrackerApp()
{

}

CRacingWheelTrackerApp theApp;

BOOL CRacingWheelTrackerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CRacingWheelTrackerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}
