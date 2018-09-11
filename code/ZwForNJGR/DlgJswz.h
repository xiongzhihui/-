//-----------------------------------------------------------------------------
//----- DlgJswz.h : Declaration of the CDlgJswz
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxcmn.h"
#include "DirTreeCtrl.h"
#include "Resource.h"

//-----------------------------------------------------------------------------
class CDlgJswz : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgJswz)

public:
	CDlgJswz (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGJSWZ} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CDirTreeCtrl m_TreeCtrl;
	virtual BOOL OnInitDialog();
	HTREEITEM FindItem(HTREEITEM hItem);
	vector<CString> readStringFromFile(CString strFile);
private:
	CString m_strParentPath;
	CString m_strSearch;
	CString m_strFileName;
	CRichEditCtrl m_EditUpdate;
	double m_dJsTxtHeight;
	double m_dTxtHeight;
	double m_dTextGap;
	map<CString, int> m_DataMap;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateMergeTreeCheck(const HTREEITEM hParent, const BOOL flag);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuAdd(); // Ôö¼Ó
	afx_msg void OnMenuDel();//É¾³ý
	afx_msg void OnMenuEdit();//±à¼­	
	afx_msg void OnMenuAddPath();

	afx_msg void OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	int getEnterCode(int nFlag, CString strText);
	void displayText();
	afx_msg void OnJsSave();
	CString getNewFileName(CString strPath);
} ;
