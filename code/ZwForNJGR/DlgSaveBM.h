//-----------------------------------------------------------------------------
//----- DlgSaveBM.h : Declaration of the CDlgSaveBM
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgSaveBM : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgSaveBM)

public:
	CDlgSaveBM (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGSAVEBM} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
} ;
