#include "stdafx.h"
#include "MonkaSteerSDK.h"
#include "MonkaSteerSDKDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMonkaSteerSDKApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMonkaSteerSDKApp::CMonkaSteerSDKApp()
{

}

CMonkaSteerSDKApp theApp;

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
