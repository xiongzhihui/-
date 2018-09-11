//-----------------------------------------------------------------------------
//----- DlgSaveBM.cpp : Implementation of CDlgSaveBM
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgSaveBM.h"
#include <shlwapi.h>
#include "OperateReg.h"

static int CALLBACK _SHBrowseForFolderCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )  
{  
	switch(uMsg)  
	{  
	case BFFM_INITIALIZED:      
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);   //传递默认打开路径 break;  
	case BFFM_SELCHANGED:    //选择路径变化，BFFM_SELCHANGED  
		{  
			char curr[MAX_PATH];     
			SHGetPathFromIDList((LPCITEMIDLIST)lParam,(LPWSTR)(LPCTSTR)curr);     
			::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)&curr[0]);    
		}  
		break;  
	default:  
		break;  
	}   
	return 0;  
}  
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgSaveBM, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgSaveBM, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSaveBM::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CDlgSaveBM::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgSaveBM::CDlgSaveBM (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgSaveBM::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CDlgSaveBM::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgSaveBM::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgSaveBM::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString strCurrentPath;
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(strCurrentPath);
	TCHAR Mycom[_MAX_PATH];  
	TCHAR szPath[MAX_PATH] ;
	_tcscpy_s(szPath,strCurrentPath);//自己设置的文件夹路径
	BROWSEINFO Myfold;  
	Myfold.hwndOwner= adsw_acadMainWnd();// sds_getmainhwnd();
	Myfold.pidlRoot=NULL;  
	Myfold.pszDisplayName=Mycom;  
	Myfold.lpszTitle = _T("BM保存位置");  
	Myfold.ulFlags=BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE ;  
	Myfold.lpfn=_SHBrowseForFolderCallbackProc;  
	Myfold.lParam=LPARAM(szPath);;  
	Myfold.iImage=NULL;  

	Mycom[0]=_T('\0');  
	SHGetPathFromIDList(SHBrowseForFolder(&Myfold),Mycom);  

	CString strDeFaultPath;
	strDeFaultPath.Format(_T("%s"),Mycom);
	if (strDeFaultPath.IsEmpty())
	{
		CString strOutPath;
		strOutPath = gGlobal.GetIniValue(_T("基础设置"), _T("默认路径"));
		if (strOutPath.IsEmpty())
		{
			strOutPath = MyBaseUtils::GetAppPath() + _T("BM输出目录\\");;
		}
		strDeFaultPath = strOutPath;
	}
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(strDeFaultPath);

}

BOOL CDlgSaveBM::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strCurrentPath;
	strCurrentPath = gGlobal.GetIniValue(_T("基础设置"), _T("默认路径"));
	if (strCurrentPath.IsEmpty())
	{
		strCurrentPath = MyBaseUtils::GetAppPath() + _T("BM输出目录\\");
	}
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(strCurrentPath);

	CenterWindow(acedGetAcadDwgView());
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSaveBM::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strCurrentPath;
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(strCurrentPath);
	if (strCurrentPath.GetLength() <= 0)
	{
		AfxMessageBox(_T("路径不能为空"));
		return;
	}
	strCurrentPath.TrimRight(_T("\\"));
	strCurrentPath += _T("\\");
	
	BOOL bRet = TRUE;
	if (!PathIsDirectory(strCurrentPath))  
	{  
		bRet = ::CreateDirectory(strCurrentPath, 0); 
		if (!bRet)
		{
			AfxMessageBox(_T("该路径无法创建，请查看路径是否正确及是否有权限创建"));
			return;
		}
	}  
	gGlobal.SetIniValue(_T("基础设置"), _T("默认路径"), strCurrentPath);
	gGlobal.WriteIniFile();

	//写Excel注册表
	BOOL bIsOk=TRUE;
	COperateReg reg;
	//处理excel2010
	CString tmpStr= _T("Software\\Microsoft\\Office\\14.0\\Excel\\Security\\Trusted Locations\\Location7");
	bIsOk = reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("AllowSubFolders"), 1);
	bIsOk = reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("PATH"), strCurrentPath);

	//处理Excel2013
	tmpStr= _T("Software\\Microsoft\\Office\\15.0\\Excel\\Security\\Trusted Locations\\Location7");
	bIsOk = reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("AllowSubFolders"), 1);
	bIsOk = reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("PATH"), strCurrentPath);
	//处理Excel2007
	tmpStr= _T("Software\\Microsoft\\Office\\12.0\\Excel\\Security\\Trusted Locations\\Location7");
	bIsOk = reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("AllowSubFolders"), 1);
	bIsOk = reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("PATH"), strCurrentPath);
	CAcUiDialog::OnOK();
}
