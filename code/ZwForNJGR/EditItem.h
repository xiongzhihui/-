#if !defined(AFX_EDITITEM_H)
#define AFX_EDITITEM_H

#if _MSC_VER > 1000
#pragma once
#endif 
/////////////////////////////////////////////////////////////////////////////
// CEditItem window

class CEditItem : public CEdit
{
// Construction
public:
	CEditItem(int nItem, int nSubItem, CString &sContent);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditItem();

	// Generated message map functions
protected:
	int		m_nItem;
	int		m_nSubItem;
	CString m_sContent;
	BOOL	m_bVK_ESCAPE;
	//{{AFX_MSG(CEditItem)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
#endif 