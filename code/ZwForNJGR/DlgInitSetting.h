//-----------------------------------------------------------------------------
//----- DlgInitSetting.h : Declaration of the CDlgInitSetting
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxcmn.h"
#include "EditList.h"
#include "GraphBlockViewWnd.h"
//-----------------------------------------------------------------------------
class CDlgInitSetting : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgInitSetting)

public:
	CDlgInitSetting (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgInitSetting();

	enum { IDD = IDD_DLGINITSETTING} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void initData();
	void setInfoToListCtrl(CString strType);
	CEditList m_ListCtrl;

	CGraphBlockViewWnd m_gsView;
	AcDbDatabase* m_pDb;
	void ClearDatabase();
	void InitDatabase();
	bool InitGraphList();
private:
	CString m_strType;
	CString m_strValue;
public:
	afx_msg void OnBnClickedOk();
	bool getInitData();
	CString getInfoToListCtrl(int param1);
	void ReWriteInitData();
	void setDataToIniFile(int nItem);
	afx_msg void OnBnClickedButtonHelp();
} ;
