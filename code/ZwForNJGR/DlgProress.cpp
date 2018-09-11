//-----------------------------------------------------------------------------
//----- DlgProress.cpp : Implementation of CDlgProress
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgProress.h"
#include "Utility.h"

extern Adesk::Boolean endDlg();
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgProress, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgProress, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_MESSAGE(WM_PROCESS, do_process)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgProress::CDlgProress (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgProress::IDD, pParent, hInstance) {


}

//-----------------------------------------------------------------------------
void CDlgProress::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgProress::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgProress::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow(acedGetAcadDwgView());
	m_ctrlProgress.SetRange(0, 100);
	//m_ctrlProgress.SetPos(0);
	m_ctrlProgress.SetStep(1);
	return TRUE;
}

LRESULT CDlgProress::do_process(WPARAM wParam,LPARAM lParam)  
{      
	//AfxMessageBox(_T("test"));  

	if(wParam >= 0 && wParam <= 100)
		m_ctrlProgress.SetPos(wParam);

	return 0;
}  

// ProcessDlg message handlers


BOOL CDlgProress::Create(CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CAcUiDialog::Create(CDlgProress::IDD, pParentWnd);
}

void CDlgProress::setPos(int nCur)
{
	m_ctrlProgress.SetPos(nCur);
	//CString strNum, strTmp;
	//strNum.Format(_T("%d"), nCur);
	//strTmp = _T("%") + strNum;
	//GetDlgItem(IDC_STATIC_OUT)->SetWindowText(strTmp);
}


