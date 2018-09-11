//-----------------------------------------------------------------------------
//----- DlgExcelFileCmp.h : Declaration of the CDlgExcelFileCmp
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

class CExcelDifferInfo;
typedef vector<pair<CString, vector<vector<CString> > > > MIMap;
typedef pair<CString, vector<vector<CString> > > MIPair;
typedef vector<vector<CString> > VIVec;
//-----------------------------------------------------------------------------
class CDlgExcelFileCmp : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgExcelFileCmp)

public:
	CDlgExcelFileCmp (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGEXCELFILECMP} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowserold();
	afx_msg void OnBnClickedButtonBrowsernew();
	afx_msg void OnBnClickedOk();
	CString GetNextPathName(POSITION& pos, OPENFILENAME& ofn); 

	bool CmpSheetName(MIMap preMapInfo, MIMap curMapInfo);
	bool CmpSheetContent(VIVec preMapInfo, VIVec curMapInfo);
	bool WriteLogFile();
	bool WriteCsvFile();


private:
	vector<CExcelDifferInfo> m_DifferInfo;
	CString m_strFileName;
public:
	CString m_strPreFile;
	CString m_strCurFile;
	afx_msg void OnBnClickedButtonSwap();
	bool m_bIsError;
} ;

class CExcelDifferInfo
{
public:
	CExcelDifferInfo(void);
	~CExcelDifferInfo();

	void setSheetName(CString strSheetName);
	void setRange(CString strRange);
	void setOldValue(CString strOldValue);
	void setNewValue(CString strNewValue);
	void setDescription(CString strDescription);

	CString getSheetName() const;
	CString getRange() const;
	CString getOldValue() const;
	CString getNewValue() const;
	CString getDescription() const;
	
	CExcelDifferInfo& operator=(const CExcelDifferInfo& node);
	CString getOutPutString() const;
	CString getCsvOutString() const;
private:
	CString m_strSheetName;
	CString m_strRange;
	CString m_strOldValue;
	CString m_strNewValue;
	CString m_strDescription;
};
