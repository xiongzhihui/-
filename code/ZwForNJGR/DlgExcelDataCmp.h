//-----------------------------------------------------------------------------
//----- DlgExcelDataCmp.h : Declaration of the CDlgExcelDataCmp
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "DlgExcelFileCmp.h"
#include "afxcmn.h"
#include "EditList.h"
#include "resource.h"
#include "afxwin.h"

//-----------------------------------------------------------------------------
class CDlgExcelDataCmp : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgExcelDataCmp)

public:
	CDlgExcelDataCmp (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGEXCELDATACMP} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void setData(vector<CExcelDifferInfo> vecDif);
	bool Init();
	afx_msg void OnNMDblclkListCur(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListPre(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListCur(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	void getRowAndColByString(CString strValue, int& nRow, int& nCol);
	int getindex(int l);
	int titleToNumber(CString s);
	afx_msg void OnEnKillfocusEditOther();
	void setFile(CString strPreFile, CString strCurFile);
	afx_msg void OnNMClickListPre(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListCur(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListPre(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	vector<CExcelDifferInfo> m_VecInfo;
public:
	CListCtrl m_preListCtrl;
	CListCtrl m_curListCtrl;
	CEdit m_SubItemEdit;
	CString m_strItemtextEdit;

	int m_npreItem;
	int m_npreSubItem;
	int m_nType;
	int m_ncurItem;
	int m_ncurSubItem;
	BOOL m_bPreHit;
	BOOL m_bCurHit;
	CString m_strPreFile;
	CString m_strCurFile;

	CButton myBitmapButton1;
	CButton m_myBtn2;
} ;
