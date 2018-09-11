//-----------------------------------------------------------------------------
//----- DlgListError.h : Declaration of the CDlgListError
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "editlist.h"
#include "CCommonCell.h"
#include "CIgnoreErrorCell.h"
#include "Resource.h"

//-----------------------------------------------------------------------------
class CDlgListError : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgListError)

public:
	CDlgListError (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGLISTERROR} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void setErrorData(CString strTitle, vector<CIgnoreErrorCell> errorVec);
	void setInfoToListCtrl(CIgnoreErrorCell comCell);
	//Ë«»÷±à¼­
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

	void setShowFlag(bool bIsShowFlag);

public:
	CString m_strTitle;
	CListCtrl m_ListCtrl;
	vector<CIgnoreErrorCell> m_ErrorVec;
	CRect m_rect;
	afx_msg void OnClose();
	virtual void OnCancel();
	bool m_bFirst;
	bool m_bIsShowFlag;//ÊÇ·ñÏÔÊ¾°´Å¥
} ;
