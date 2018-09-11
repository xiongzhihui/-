//-----------------------------------------------------------------------------
//----- DlgCreateTouzi.cpp : Implementation of CDlgCreateTouzi
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgCreateTouzi.h"
#include "Utility.h"

extern CString GetNextPathName(POSITION& pos, OPENFILENAME& ofn); 
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgCreateTouzi, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgCreateTouzi, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CDlgCreateTouzi::OnBnClickedButtonBrowser)
	ON_BN_CLICKED(IDOK, &CDlgCreateTouzi::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgCreateTouzi::CDlgCreateTouzi (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgCreateTouzi::IDD, pParent, hInstance) {
	m_strFileName = gGlobal.GetIniValue(_T("图框设置"), _T("座子文件路径"));
}

//-----------------------------------------------------------------------------
void CDlgCreateTouzi::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFileName);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgCreateTouzi::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgCreateTouzi::OnBnClickedButtonBrowser()
{
	struct resbuf *result = NULL;
	CString strFileFormat;
	CString strFilter;

	int nBufLen = 256;
	TCHAR* pBuf = new TCHAR[nBufLen];
	memset( pBuf, 0, nBufLen*sizeof(TCHAR) );

	OPENFILENAME ofn;
	memset( &ofn, 0 ,sizeof(ofn) );
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = adsw_acadMainWnd();
	ofn.lpstrFile = pBuf;
	ofn.nMaxFile = nBufLen;
	ofn.lpstrFilter = _T("选择文件(*.xls|*.xlsx)\0*.xls;*.xlsx\0");
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER|/*OFN_ALLOWMULTISELECT|*/OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NODEREFERENCELINKS;

	if ( ::GetOpenFileName( &ofn ) )
	{
		CString strTmp = ofn.lpstrFile;
		int nNewFileItemCount = 0;
		POSITION pos = (POSITION)ofn.lpstrFile;
		while ( pos != NULL )
		{
			m_strFileName = GetNextPathName( pos, ofn ); // 依次找到选择的多个文件名
		}
		delete []pBuf;
	}
	else
	{
		delete []pBuf;
		return;
	}

	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(m_strFileName);
}

void CDlgCreateTouzi::OnBnClickedOk()
{
	if (m_strFileName.IsEmpty())
	{
		AfxMessageBox(_T("路径不能为空"));
		return;
	}

	gGlobal.SetIniValue(_T("图框设置"), _T("座子文件路径"), m_strFileName);
	gGlobal.WriteIniFile();

	CAcUiDialog::OnOK();
}

BOOL CDlgCreateTouzi::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow(acedGetAcadDwgView());
	return TRUE; 
}

CString CDlgCreateTouzi::getFile()
{
	CString strTemp = CUtility::getFileNameByFullName(m_strFileName);
	strTemp.Insert(0, _T("ICD-"));
	CString strOutPath, strFile;
	TCHAR OutPath[MAX_PATH];
	GetTempPath(MAX_PATH, OutPath);
	strOutPath = OutPath;
	strFile = strOutPath + strTemp;
	::DeleteFile(strFile);
	::CopyFile(m_strFileName, strFile, FALSE);
	return strFile;
}
