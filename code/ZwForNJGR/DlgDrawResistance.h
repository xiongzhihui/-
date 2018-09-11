//-----------------------------------------------------------------------------
//----- DlgDrawResistance.h : Declaration of the CDlgDrawResistance
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgDrawResistance : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgDrawResistance)

public:
	CDlgDrawResistance (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGDRAWRESISTANCE} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	// -----------------------------------------------------------------------------
	virtual BOOL OnInitDialog(void);

	void readIniFile();
	void writeIniFile();
protected:
	// -----------------------------------------------------------------------------
	virtual void OnOK(void);
	AcGePoint3d selEnt();
	void drawQuxiangText(AcGePoint3d pt);
	void drawDzText(AcDbObjectId objId);
public:
	CString m_strQuxiang;
	CString m_strName;
	AcDbObjectId m_layerId;
} ;
