//-----------------------------------------------------------------------------
//----- DlgCreateTouzi.h : Declaration of the CDlgCreateTouzi
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgCreateTouzi : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgCreateTouzi)

public:
	CDlgCreateTouzi (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGCREATETOUZI} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnBnClickedButtonBrowser();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();	
	CString getFile();
public:
	CString m_strFileName;
} ;
