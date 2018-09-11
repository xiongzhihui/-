#if !defined(AFX_EDITLIST_H)
#define AFX_EDITLIST_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditList.h : header file
//
typedef int (* fGetType) (int);
#ifdef ARX
bool acedLinetypeDialog(AcDbObjectId,bool,TCHAR * &,AcDbObjectId &);
bool acedLineWeightDialog(AcDb::LineWeight,bool,AcDb::LineWeight &);
#else
bool zcedLinetypeDialog(AcDbObjectId,bool,TCHAR * &,AcDbObjectId &);
bool zcedLineWeightDialog(AcDb::LineWeight,bool,AcDb::LineWeight &);
#endif
/////////////////////////////////////////////////////////////////////////////
// CEditList window
#include <map>
#include <vector>
using namespace std;
class CEditList : public CListCtrl
{
	// Construction
public:
	CEditList();

// Attributes
public:
	enum eType{
		eEdit,
		eCombo,
		eComboColor,
		eComboLineType,
		eComboLineWeight,
		eLast	
	};

	fGetType m_fGetType;

	CStringList m_strList;
	map<int, vector<CString> > m_Map;

// Operations
public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColumnType(fGetType func) { m_fGetType = func;}
	virtual ~CEditList();	
	afx_msg void SetItemColor(DWORD iItem, COLORREF color);
	afx_msg void SetAllItemColor(DWORD iItem,COLORREF TextBkColor);	

	// Generated message map functions
protected:

	CComboBox * ComboItem( int nItem,  int nSubItem);
	CEdit * EditItem( int nItem, int nSubItem);
	//{{AFX_MSG(CEditList)
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

	CMap<DWORD, DWORD&, COLORREF, COLORREF&> MapItemColor;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};
#endif 
