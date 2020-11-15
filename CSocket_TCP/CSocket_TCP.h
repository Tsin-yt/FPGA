
// CSocket_TCP.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCSocket_TCPApp:
// See CSocket_TCP.cpp for the implementation of this class
//

class CCSocket_TCPApp : public CWinApp
{
public:
	CCSocket_TCPApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCSocket_TCPApp theApp;