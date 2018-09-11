//-----------------------------------------------------------------------------
//----- DlgEidtFrame.h : Declaration of the CDlgEidtFrame
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxwin.h"
#include "editlist.h"
#include "GraphBlockViewWnd.h"
#include "CFrameInfo.h"

//-----------------------------------------------------------------------------
class CDlgEidtFrame : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgEidtFrame)

public:
	CDlgEidtFrame (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGEIDTFRAME} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboSize();
	virtual BOOL OnInitDialog();
	void initData();
	void setInfoToListCtrl(CString strType);
	void ClearDatabase();
	void InitDatabase();
	bool InitGraphList();
	bool sel();
	afx_msg void OnBnClickedOk();
	bool getInitData();
	CString getInfoToListCtrl(int i);
	void ReWriteInitData();
	void EraseSheetEnt();
public:
	CComboBox m_CombSize;
	CString m_strSize;
	CString m_strValue;
	int m_nCount;
	CString m_strSheetName;
	CGraphBlockViewWnd m_gsView;
	AcDbDatabase* m_pDb;

	AcDbObjectId m_BlkId;//øÈ≤Œ’’id
	CEditList m_ListCtrl;
	CFrameInfo m_FrameInfo;
	
} ;
