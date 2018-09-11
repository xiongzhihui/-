//-----------------------------------------------------------------------------
//----- DlgProress.h : Declaration of the CDlgProress
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "Resource.h"
#include "MyProgress.h"
#define WM_PROCESS (WM_USER + 200)

//-----------------------------------------------------------------------------
class CDlgProress : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgProress)

public:
	CDlgProress (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGPRORESS} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CMyProgress m_ctrlProgress;
	virtual BOOL OnInitDialog();

	afx_msg LRESULT do_process(WPARAM wParam,LPARAM lParam)  ;
	virtual BOOL Create(CWnd* pParentWnd = NULL);
	void setPos(int nCur);
} ;
