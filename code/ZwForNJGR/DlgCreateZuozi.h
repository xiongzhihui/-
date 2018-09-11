//-----------------------------------------------------------------------------
//----- DlgCreateZuozi.h : Declaration of the CDlgCreateZuozi
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgCreateZuozi : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgCreateZuozi)
public:
	CDlgCreateZuozi (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGCREATEZUOZI} ;

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
