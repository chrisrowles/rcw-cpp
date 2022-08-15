// RacingWheelTrackerDlg.h : header file
//

#pragma once
#define USE_INIT_WITH_WINDOW

#include "SteeringWheelInputCalibrateDlg.h"

#include "LogitechSteeringWheelLib.h"
#pragma comment(lib, "LogitechSteeringWheelLib.lib")

#include "map"

class CRacingWheelTrackerDlg : public CDialog
{
public:
	CRacingWheelTrackerDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_RACINGWHEELTRACKER_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy( );
	DECLARE_MESSAGE_MAP()
public:
	bool m_isTimerActive;
	int m_calibrateDevice;
	DIJOYSTATE2 *m_DIJoyState2Device[2];
	CSteeringWheelInputCalibrateDlg *m_pWheelInputCalibrateDlg;

	//Maps an action (key) to a control (value).
	std::map<int, int> m_controlMap[2];
	std::map<int, int> m_tempMap[2];

	afx_msg void OnBnClickedInit();
	afx_msg void OnBnClickedCalibrate();
	afx_msg void OnBnClickedShutdown();
	afx_msg LRESULT OnCalibrateInput(WPARAM wParam, LPARAM lParam);

	void ShowInputCalibrate(int device);
	long GetControlValue(int device, int control);
};
