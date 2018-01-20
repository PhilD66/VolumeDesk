
// VolumeDesk.h : main header file for the VolumeDesk application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CVolumeDeskApp:
// See VolumeDesk.cpp for the implementation of this class
//

class CVolumeDeskApp : public CWinAppEx
{
public:
	CVolumeDeskApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVolumeDeskApp theApp;
