#include "stdafx.h"
#include "MonkaSteerSDK.h"
#include "MonkaSteerSDKDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifndef TRACE
#define TRACE LOGIWHEELTRACE
#endif

#define _DEMO

CMonkaSteerSDKDlg::CMonkaSteerSDKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonkaSteerSDKDlg::IDD, pParent), m_isTimerActive(true), m_pWheelInputCalibrateDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonkaSteerSDKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonkaSteerSDKDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(CALIBRATE_MSG, &CMonkaSteerSDKDlg::OnCalibrateInput)
	ON_BN_CLICKED(IDC_INIT, &CMonkaSteerSDKDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_CALIBRATE, &CMonkaSteerSDKDlg::OnBnClickedCalibrate)
	ON_BN_CLICKED(IDC_SHUTDOWN, &CMonkaSteerSDKDlg::OnBnClickedShutdown)
END_MESSAGE_MAP()

BOOL CMonkaSteerSDKDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	SetIcon(m_hIcon, true);
	SetIcon(m_hIcon, false);

	SetTimer(1, 1000 / 30, NULL );

	return true;
}

void CMonkaSteerSDKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMonkaSteerSDKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMonkaSteerSDKDlg::OnTimer(UINT_PTR nIDEvent)
{
	UNREFERENCED_PARAMETER(nIDEvent);

	int index_ = 0;
	float speedParam_[LOGI_MAX_CONTROLLERS] = {0.0f, 0.0f};
	float brakeParam_[LOGI_MAX_CONTROLLERS] = {0.0f, 0.0f};
	TCHAR sBuffer_[128];
	TCHAR* str_;
	TCHAR deviceConnected_[128][LOGI_MAX_CONTROLLERS];

	int counter_ = 0;

	if(!LogiUpdate()) return;

	if(!m_isTimerActive) return;

	LogiGenerateNonLinearValues(0, -40);
	LogiGenerateNonLinearValues(1, 80);

	for (index_ = 0; index_ < LOGI_MAX_CONTROLLERS; index_++)
	{
		if (LogiIsConnected(index_))
		{
			wchar_t deviceName[128];
			LogiGetFriendlyProductName(0, deviceName, 128);
			wsprintf(deviceConnected_[index_], deviceName);

			m_DIJoyState2Device[index_] = LogiGetState(index_);
			
			if (LogiIsManufacturerConnected(index_, LOGI_MANUFACTURER_LOGITECH) && LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_WHEEL))
			{
				LogiControllerPropertiesData propertiesData_;
				ZeroMemory(&propertiesData_, sizeof(propertiesData_));

				LogiGetCurrentControllerProperties(index_, propertiesData_);

				if (propertiesData_.combinePedals)
				{
					speedParam_[index_] = max(((-(float)(GetControlValue(index_, ACCELERATOR))) / 32767), 0);
					brakeParam_[index_] = max((((float)(GetControlValue(index_, BRAKE))) / 32767), 0);
				}
				else
				{
					speedParam_[index_] = ((-(float)(GetControlValue(index_, ACCELERATOR))) / 65535) + float(0.5);
					brakeParam_[index_] = ((-(float)(GetControlValue(index_, BRAKE))) / 65535) + float(0.5);
				}
			}
			else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_WHEEL) && LogiIsManufacturerConnected(index_, LOGI_MANUFACTURER_MICROSOFT))
			{
				wsprintf(deviceConnected_[index_], TEXT("Steering wheel, combined pedals"));
				speedParam_[index_] = max(((-(float)(GetControlValue(index_, ACCELERATOR))) / 32767), 0);
				brakeParam_[index_] = max((((float)(GetControlValue(index_, BRAKE))) / 32767), 0);
			}
			else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_WHEEL) && LogiIsManufacturerConnected(index_, LOGI_MANUFACTURER_OTHER) && m_DIJoyState2Device[index_] != NULL)
			{
				if (m_DIJoyState2Device[index_]->lRz == 32767
					&& m_DIJoyState2Device[index_]->lY == 0
					&& m_DIJoyState2Device[index_]->rglSlider[0] != 0
					&& m_DIJoyState2Device[index_]->lZ== 0)
				{
					wsprintf(deviceConnected_[index_],	TEXT("Steering wheel, combined pedals"));
					speedParam_[index_] = max(((-(float)(m_DIJoyState2Device[index_]->rglSlider[0])) / 32767), 0);
					brakeParam_[index_] = max((((float)(m_DIJoyState2Device[index_]->rglSlider[0])) / 32767), 0);
				}
				else if (m_DIJoyState2Device[index_]->lRz == 32767
					&& m_DIJoyState2Device[index_]->lY != 0
					&& m_DIJoyState2Device[index_]->rglSlider[0] != 0
					&& m_DIJoyState2Device[index_]->lZ== 0)
				{
					wsprintf(deviceConnected_[index_], TEXT("Steering wheel, separate pedals"));
					speedParam_[index_] = ((-(float)(m_DIJoyState2Device[index_]->lY)) / 65535) + float(0.5);
				}
			}
			else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_JOYSTICK))
			{
				speedParam_[index_] = max(((-(float)(m_DIJoyState2Device[index_]->rglSlider[0])) / 32767), 0);
				brakeParam_[index_] = max((((float)(m_DIJoyState2Device[index_]->rglSlider[0])) / 32767), 0);
			}

			else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_GAMEPAD))
			{
				speedParam_[index_] = max((((float)(m_DIJoyState2Device[index_]->lY)) / 32767), 0);
				brakeParam_[index_] = max(((-(float)(m_DIJoyState2Device[index_]->lY)) / 32767), 0);
			}
			else
			{
				wsprintf(deviceConnected_[index_], TEXT("Steering wheel"));
				speedParam_[index_] = 0;
				brakeParam_[index_] = 0;
			}

			if (index_ == 0)
			{
				wsprintf(sBuffer_, TEXT("%ld"), int(1000 * speedParam_[index_]));
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_SPEED), sBuffer_);

				wsprintf(sBuffer_, TEXT("%ld"), int(1000 * brakeParam_[index_]));
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_BRAKE), sBuffer_);

				::SetWindowText(::GetDlgItem(m_hWnd, IDC_DEVICE), deviceConnected_[index_]);

				wsprintf(sBuffer_, TEXT("%ld"), m_DIJoyState2Device[index_]->lX);
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_X_AXIS), sBuffer_);

				str_ = sBuffer_;
				for(counter_ = 0; counter_ < 128; counter_++)
				{
					if (m_DIJoyState2Device[index_]->rgbButtons[counter_] & 0x80)
					{
						str_ += wsprintf(str_, TEXT("%02d "), counter_);
					}
				}
				*str_ = 0;   // Terminate the string

				::SetWindowText(::GetDlgItem(m_hWnd, IDC_BUTTONS), sBuffer_);
			}

		}
		else
		{
			if (index_ == 0)
			{
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_DEVICE ), _T("No device connected"));
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_SPEED), _T("0"));
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_BRAKE), _T("0"));
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_X_AXIS ), _T("0"));
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_BUTTONS), _T(""));
			}
		}
	}
}

void CMonkaSteerSDKDlg::OnDestroy()
{
	KillTimer(1);
	LogiSteeringShutdown();
}

void CMonkaSteerSDKDlg::OnBnClickedInit()
{
	LogiSteeringInitializeWithWindow(true, m_hWnd);
}

void CMonkaSteerSDKDlg::OnBnClickedCalibrate()
{
	ShowInputCalibrate(0);
}

void CMonkaSteerSDKDlg::ShowInputCalibrate(int device)
{
	m_isTimerActive = false;
	m_calibrateDevice = device;
	LogiSteeringShutdown();

	m_pWheelInputCalibrateDlg = new CSteeringWheelInputCalibrateDlg(device, m_hWnd, FromHandle(m_hWnd));
	if (m_pWheelInputCalibrateDlg != NULL)
	{
		if (!m_pWheelInputCalibrateDlg->Create(IDD_INPUTCALIBRATE_DIALOG, this))
		{
			AfxMessageBox(_T("Error creating Dialog"));
		}

		m_pWheelInputCalibrateDlg->ShowWindow(SW_SHOW);
		EnableWindow(false);
	}
}

LRESULT CMonkaSteerSDKDlg::OnCalibrateInput(WPARAM wParam, LPARAM lParam)
{
	if (wParam == SAVE || wParam == CANCEL)
	{
		EnableWindow(true);
		LogiSteeringInitializeWithWindow(true, m_hWnd);
		m_isTimerActive = true;
	}
	if (wParam == SAVE)
	{
		m_controlMap[m_calibrateDevice] = m_tempMap[m_calibrateDevice];
		m_tempMap[m_calibrateDevice].clear();
	}
	if (wParam == CANCEL)
	{
		m_tempMap[m_calibrateDevice].clear();
	}
	if (wParam != SAVE && wParam != CANCEL)
	{
		m_tempMap[m_calibrateDevice][wParam] = lParam;
	}
	return 0;
}

void CMonkaSteerSDKDlg::OnBnClickedShutdown()
{
	LogiSteeringShutdown();
}

long CMonkaSteerSDKDlg::GetControlValue(int device, int control)
{
	if (control == WHEEL)
	{
		switch (m_controlMap[device][control])
		{
		case X_AXIS:
			return m_DIJoyState2Device[device]->lX;
		case Y_AXIS:
			return m_DIJoyState2Device[device]->lY;
		case Z_AXIS:
			return m_DIJoyState2Device[device]->lZ;
		case SLIDER_0:
			return m_DIJoyState2Device[device]->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState2Device[device]->rglSlider[1];
		case X_ROT:
			return m_DIJoyState2Device[device]->lRx;
		case Y_ROT:
			return m_DIJoyState2Device[device]->lRy;
		case Z_ROT:
			return m_DIJoyState2Device[device]->lRz;
		default:
			return m_DIJoyState2Device[device]->lX;
		}
	}
	if (control == ACCELERATOR)
	{
		switch (m_controlMap[device][control])
		{
		case X_AXIS:
			return m_DIJoyState2Device[device]->lX;
		case Y_AXIS:
			return m_DIJoyState2Device[device]->lY;
		case Z_AXIS:
			return m_DIJoyState2Device[device]->lZ;
		case SLIDER_0:
			return m_DIJoyState2Device[device]->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState2Device[device]->rglSlider[1];
		case X_ROT:
			return m_DIJoyState2Device[device]->lRx;
		case Y_ROT:
			return m_DIJoyState2Device[device]->lRy;
		case Z_ROT:
			return m_DIJoyState2Device[device]->lRz;
		default:
			return m_DIJoyState2Device[device]->lY;
		}
	}
	if (control == BRAKE)
	{
		switch (m_controlMap[device][control])
		{
		case X_AXIS:
			return m_DIJoyState2Device[device]->lX;
		case Y_AXIS:
			return m_DIJoyState2Device[device]->lY;
		case Z_AXIS:
			return m_DIJoyState2Device[device]->lZ;
		case SLIDER_0:
			return m_DIJoyState2Device[device]->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState2Device[device]->rglSlider[1];
		case X_ROT:
			return m_DIJoyState2Device[device]->lRx;
		case Y_ROT:
			return m_DIJoyState2Device[device]->lRy;
		case Z_ROT:
			return m_DIJoyState2Device[device]->lRz;
		default:
			return m_DIJoyState2Device[device]->lRz;
		}
	}
	if (control == CLUTCH)
	{
		switch (m_controlMap[device][control])
		{
		case X_AXIS:
			return m_DIJoyState2Device[device]->lX;
		case Y_AXIS:
			return m_DIJoyState2Device[device]->lY;
		case Z_AXIS:
			return m_DIJoyState2Device[device]->lZ;
		case SLIDER_0:
			return m_DIJoyState2Device[device]->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState2Device[device]->rglSlider[1];
		case X_ROT:
			return m_DIJoyState2Device[device]->lRx;
		case Y_ROT:
			return m_DIJoyState2Device[device]->lRy;
		case Z_ROT:
			return m_DIJoyState2Device[device]->lRz;
		default:
			return m_DIJoyState2Device[device]->rglSlider[0];
		}
	}

	return 0;
}
