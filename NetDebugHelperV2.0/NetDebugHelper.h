// NetDebugHelper.h : main header file for the NETDEBUGHELPER application
//

#if !defined(AFX_NETDEBUGHELPER_H__6E233136_89E5_4206_BDBB_80965854E800__INCLUDED_)
#define AFX_NETDEBUGHELPER_H__6E233136_89E5_4206_BDBB_80965854E800__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetDebugHelperApp:
// See NetDebugHelper.cpp for the implementation of this class
//

class CNetDebugHelperApp : public CWinApp
{
public:
	CNetDebugHelperApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetDebugHelperApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNetDebugHelperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETDEBUGHELPER_H__6E233136_89E5_4206_BDBB_80965854E800__INCLUDED_)
