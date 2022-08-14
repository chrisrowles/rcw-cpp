// MonkaSteerSDK.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MonkaSteerSDK.h"
#include "MonkaSteerSDKDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMonkaSteerSDKApp

BEGIN_MESSAGE_MAP(CMonkaSteerSDKApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMonkaSteerSDKApp construction

CMonkaSteerSDKApp::CMonkaSteerSDKApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMonkaSteerSDKApp object

CMonkaSteerSDKApp theApp;


// CMonkaSteerSDKApp initialization

BOOL CMonkaSteerSDKApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CMonkaSteerSDKDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}
