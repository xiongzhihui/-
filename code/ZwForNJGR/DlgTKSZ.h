//-----------------------------------------------------------------------------
//----- DlgTKSZ.h : Declaration of the CDlgTKSZ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxwin.h"
#include "Resource.h"

//-----------------------------------------------------------------------------
class CDlgTKSZ : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgTKSZ)

public:
	CDlgTKSZ (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGTKSZ} ;

	CString getFile();
protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboSize();
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedButtonPreview();
	afx_msg void OnBnClickedButtonSetting();
	afx_msg void OnBnClickedOk();

	void saveIniFile();
	afx_msg void OnBnClickedButtonBrowser();
	int  getLen(CString strTemp);
	void getDlgData();
public:
	CComboBox m_CombSize;
	CString m_strSize;
	BOOL m_bBiaoti;
	BOOL m_bFujia;
	BOOL m_bDaohao;
	CString m_strFileName;
	CString m_strBmText;
	CString m_strDesigner;
	CString m_strProName;	
	CString m_strGZJ;//¹ØÖØ¼þ
	CString m_strProGrade;
	CString m_strProStage;
} ;
