//-----------------------------------------------------------------------------
//----- DlgTKSZ.cpp : Implementation of CDlgTKSZ
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgTKSZ.h"
#include "MyExcel.h"
#include "Utility.h"
#include "MyBaseUtils.h"
#include <time.h>
#include "DlgProress.h"
#include "BaseUtils.h"
#include "DlgInitSetting.h"
#include "CGlobalQuery.h"
#include "CGlobalRWriteFile.h"
extern CGlobalRWriteFile gRWriteFile;

extern CDlgProress *modelessDlg;
extern CGlobalQuery gQuery;
extern Adesk::Boolean startDlg();
extern Adesk::Boolean endDlg();
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgTKSZ, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgTKSZ, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &CDlgTKSZ::OnCbnSelchangeComboSize)
	ON_BN_CLICKED(IDOK, &CDlgTKSZ::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CDlgTKSZ::OnBnClickedButtonBrowser)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CDlgTKSZ::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CDlgTKSZ::OnBnClickedButtonSetting)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgTKSZ::CDlgTKSZ (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgTKSZ::IDD, pParent, hInstance) , m_strSize(_T(""))
, m_bBiaoti(FALSE)
, m_bFujia(FALSE)
, m_bDaohao(FALSE)
, m_strFileName(_T(""))
, m_strBmText(_T(""))
, m_strGZJ(_T(""))
, m_strProGrade(_T(""))
, m_strProStage(_T(""))
{
	/*m_bBiaoti = gGlobal.GetIniValue(_T("图框设置"), _T("标题"), 0);
	m_bDaohao =  (int)gGlobal.GetIniValue(_T("图框设置"), _T("倒号"), 0);
	m_bFujia = (int)gGlobal.GetIniValue(_T("图框设置"), _T("附加"), 0);*/
	m_strSize = gGlobal.GetIniValue(_T("图框设置"), _T("图框类型"));
	m_strFileName = gGlobal.GetIniValue(_T("图框设置"), _T("路径"));
	m_strBmText = gGlobal.GetIniValue(_T("图框设置"), _T("BM"));
	m_strDesigner = gGlobal.GetIniValue(_T("图框设置"), _T("设计师"));
	m_strProName = gGlobal.GetIniValue(_T("图框设置"), _T("产品名称"));
	m_strProGrade = gGlobal.GetIniValue(_T("图框设置"), _T("项目等级"));
	m_strProStage = gGlobal.GetIniValue(_T("图框设置"), _T("项目阶段"));
	m_strGZJ = gGlobal.GetIniValue(_T("图框设置"), _T("关重件"));
}

CString CDlgTKSZ::getFile()
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

//-----------------------------------------------------------------------------
void CDlgTKSZ::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_COMBO_SIZE, m_CombSize);
	DDX_CBString(pDX, IDC_COMBO_SIZE, m_strSize);
	//DDX_Check(pDX, IDC_CHECK_BT, m_bBiaoti);
	//DDX_Check(pDX, IDC_CHECK_FJ, m_bFujia);
	//DDX_Check(pDX, IDC_CHECK3, m_bDaohao);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_BMTEXT, m_strBmText);
	DDX_Text(pDX, IDC_EDIT_Designer, m_strDesigner);
	DDX_Text(pDX, IDC_EDIT_PRONAME, m_strProName);
	DDX_Text(pDX, IDC_EDIT_GZJ, m_strGZJ);
	DDX_Text(pDX, IDC_EDIT_PROGRADE, m_strProGrade);
	DDX_Text(pDX, IDC_EDIT_PROSTAGE, m_strProStage);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgTKSZ::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgTKSZ::OnCbnSelchangeComboSize()
{
	// TODO: Add your control notification handler code here
	int nCursel = m_CombSize.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombSize.SetCurSel(nCursel);
	m_CombSize.GetLBText(m_CombSize.GetCurSel(), m_strSize);
	gGlobal.SetIniValue(_T("图框设置"), _T("图框类型"), m_strSize);
	gGlobal.WriteIniFile();
}

BOOL CDlgTKSZ::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	CenterWindow(acedGetAcadDwgView());
	// TODO:  Add extra initialization here
	CString strTmp;
	CStringArray tmpArr;
	tmpArr.RemoveAll();
	CString strGanNature = gGlobal.GetIniValue(_T("图框设置"), _T("图框序列"));
	MyParserString::GetCommaFromString(strGanNature, tmpArr);

	for (int i=0; i<tmpArr.GetCount(); i++)
	{
		m_CombSize.AddString(tmpArr.GetAt(i));
	}
	m_strSize = gGlobal.GetIniValue(_T("图框设置"), _T("图框类型"));
	int nCur = m_CombSize.FindString(-1, m_strSize);
	if (nCur < 0)
	{
		nCur = 0;
	}
	m_CombSize.SetCurSel(nCur);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTKSZ::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	/*BeginEditorCommand();
	CString strFile = MyBaseUtils::GetAppPath() + _T("图框\\") + m_strSize + _T(".dwg");

	AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);
	AcGePoint3d basePt;
	basePt.set(0, 0,0 );
	ads_name ssname,ename;
	acedSSAdd(NULL, NULL, ssname);
	AcDbBlockReference *pBlkRef =new AcDbBlockReference(basePt, objId) ; 
	pBlkRef->setScaleFactors(1.0);
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
	AcDbObjectId layerId = MySymble::CreateNewLayer(_T("RQY_图框"), 7);
	AcDbObjectId blkId = pBlkRef->objectId();	
	MyEditEntity::openEntChangeLayer(blkId, layerId);

	acdbGetAdsName(ename, blkId);
	acedSSAdd(ename, ssname, ssname);

	int nRet = MyDrag::Draggen(_T("\n指定插入点:"), ssname, asDblArray(basePt), asDblArray(basePt), _T(""), -1);
	if (nRet == -1)
	{
		acedSSFree(ssname);
		CancelEditorCommand();
		return;
	}
	acedSSFree(ssname);
	CompleteEditorCommand();*/
	//BeginEditorCommand();
	getDlgData();

	if (m_strDesigner.IsEmpty())
	{
		AfxMessageBox(_T("设计者不能为空"));
		return;
	}
	int nBmLen = gGlobal.GetIniValue(_T("验证设置"), _T("BM长度"), 0);
	int nDesignerLen = gGlobal.GetIniValue(_T("验证设置"), _T("设计师长度"), 0);
	int nProductLen = gGlobal.GetIniValue(_T("验证设置"), _T("产品名称长度"), 0);
	if (nBmLen <= 0)
	{
		nBmLen = 100;
	}
	if (nDesignerLen <= 0)
	{
		nDesignerLen = 100;
	}
	if (nProductLen <= 0)
	{
		nProductLen = 100;
	}
	CString strPromptOut;
	if (getLen(m_strBmText) > nBmLen)
	{
		AfxMessageBox(_T("产品编号长度超过默认长度，请查看是否正确"));
		return;
	}
	if (getLen(m_strDesigner) > nDesignerLen)
	{
		AfxMessageBox(_T("设计师名称长度超过默认长度，请查看是否正确"));
		return;
	}
	if (getLen(m_strProName) > nProductLen)
	{
		AfxMessageBox(_T("产品名称长度超过默认长度，请查看是否正确"));
		return;
	}
	/*if (m_strProName.IsEmpty())
	{
		AfxMessageBox(_T("产品名称不能为空"));
		CancelEditorCommand();
		return;
	}*/

	if (m_strFileName.IsEmpty())
	{
		AfxMessageBox(_T("路径不能为空"));
		return;
	}
	saveIniFile();
	//将文件完整路径存入dwg文件中
	CBaseUtils::setFilePath(m_strFileName);
	//acedSetStatusBarProgressMeterPos(90);
	//MyBaseUtils::flushGraphics(acdbCurDwg());
	//CompleteEditorCommand();
	CAcUiDialog::OnOK();
}


void CDlgTKSZ::saveIniFile()
{
	//gGlobal.SetIniValue(_T("图框设置"), _T("标题"), m_bBiaoti);
	//gGlobal.SetIniValue(_T("图框设置"), _T("倒号"), m_bDaohao);
	//gGlobal.SetIniValue(_T("图框设置"), _T("附加"), m_bFujia);
	gGlobal.SetIniValue(_T("图框设置"), _T("图框类型"), m_strSize);
	gGlobal.SetIniValue(_T("图框设置"), _T("路径"), m_strFileName);
	gGlobal.SetIniValue(_T("图框设置"), _T("BM"), m_strBmText);
	gGlobal.SetIniValue(_T("图框设置"), _T("设计师"), m_strDesigner);
	gGlobal.SetIniValue(_T("图框设置"), _T("产品名称"), m_strProName);
	gGlobal.SetIniValue(_T("图框设置"), _T("项目等级"), m_strProGrade);
	gGlobal.SetIniValue(_T("图框设置"), _T("项目阶段"), m_strProStage);
	gGlobal.SetIniValue(_T("图框设置"), _T("关重件"), m_strGZJ);
	gGlobal.WriteIniFile();

	MyBaseUtils::SetVar(_T("USERS1"), m_strSize);
}

CString GetNextPathName(POSITION& pos, OPENFILENAME& ofn) 
{
	BOOL bExplorer = ofn.Flags & OFN_EXPLORER;
	TCHAR chDelimiter;
	if (bExplorer)
		chDelimiter = '\0';
	else
		chDelimiter = ' ';

	LPTSTR lpsz = (LPTSTR)pos;
	if (lpsz == ofn.lpstrFile) // first time
	{
		if ((ofn.Flags & OFN_ALLOWMULTISELECT) == 0)
		{
			pos = NULL;
			return ofn.lpstrFile;
		}

		// find char pos after first Delimiter
		while(*lpsz != chDelimiter && *lpsz != '\0')
			lpsz = _tcsinc(lpsz);
		lpsz = _tcsinc(lpsz);

		// if single selection then return only selection
		if (*lpsz == 0)
		{
			pos = NULL;
			return ofn.lpstrFile;
		}
	}

	CString strPath = ofn.lpstrFile;
	if (!bExplorer)
	{
		LPTSTR lpszPath = ofn.lpstrFile;
		while(*lpszPath != chDelimiter)
			lpszPath = _tcsinc(lpszPath);
		strPath = strPath.Left(lpszPath - ofn.lpstrFile);
	}

	LPTSTR lpszFileName = lpsz;
	CString strFileName = lpsz;

	// find char pos at next Delimiter
	while(*lpsz != chDelimiter && *lpsz != '\0')
		lpsz = _tcsinc(lpsz);

	if (!bExplorer && *lpsz == '\0')
		pos = NULL;
	else
	{
		if (!bExplorer)
			strFileName = strFileName.Left(lpsz - lpszFileName);

		lpsz = _tcsinc(lpsz);
		if (*lpsz == '\0') // if double terminated then done
			pos = NULL;
		else
			pos = (POSITION)lpsz;
	}

	// only add '\\' if it is needed
	if (!strPath.IsEmpty())
	{
		// check for last back-slash or forward slash (handles DBCS)
		LPCTSTR lpsz = _tcsrchr(strPath, '\\');
		if (lpsz == NULL)
			lpsz = _tcsrchr(strPath, '/');
		// if it is also the last character, then we don't need an extra
		if (lpsz != NULL &&
			(lpsz - (LPCTSTR)strPath) == strPath.GetLength()-1)
		{
			ASSERT(*lpsz == '\\' || *lpsz == '/');
			return strPath + strFileName;
		}
	}

	return strPath + '\\' + strFileName;
}

void CDlgTKSZ::OnBnClickedButtonBrowser()
{
	// TODO: Add your control notification handler code here
	struct resbuf *result = NULL;
	CString strFileFormat;
	CString strFilter;
	
	/*strFilter.Format(_T("XLS;XLSX;*"));
	strFileFormat.Format(_T("*.xls|*.xlsx|*.*"));
	
	int status = acedGetFileNavDialog( _T("打开文件"), NULL, strFilter, _T("打开"), 0, &result);
	if (status == RTNORM)
	{
		m_strFileName = result->resval.rstring;
		acutRelRb( result );
	}
	else
	{
		acutRelRb( result );
		return;
	}*/

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


int CDlgTKSZ::getLen(CString strTemp)
{
	int nLen =0;
	for(int i=0;i<strTemp.GetLength();i++)
	{
		if (!isascii(strTemp.GetAt(i)))
		{
			nLen += 2;
		}
		else
		{
			nLen++;
		}
	}
	return nLen;
}

void CDlgTKSZ::getDlgData()
{
	GetDlgItem(IDC_EDIT_BMTEXT)->GetWindowText(m_strBmText);
	GetDlgItem(IDC_EDIT_Designer)->GetWindowText(m_strDesigner);
	GetDlgItem(IDC_EDIT_PRONAME)->GetWindowText(m_strProName);
	GetDlgItem(IDC_EDIT_PROGRADE)->GetWindowText(m_strProGrade);
	GetDlgItem(IDC_EDIT_PROSTAGE)->GetWindowText(m_strProStage);
	GetDlgItem(IDC_EDIT_GZJ)->GetWindowText(m_strGZJ);
}

void CDlgTKSZ::OnBnClickedButtonPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	getDlgData();
	int nBmLen = gGlobal.GetIniValue(_T("验证设置"), _T("BM长度"), 0);
	int nDesignerLen = gGlobal.GetIniValue(_T("验证设置"), _T("设计师长度"), 0);
	int nProductLen = gGlobal.GetIniValue(_T("验证设置"), _T("产品名称长度"), 0);
	if (nBmLen <= 0)
	{
		nBmLen = 100;
	}
	if (nDesignerLen <= 0)
	{
		nDesignerLen = 100;
	}
	if (nProductLen <= 0)
	{
		nProductLen = 100;
	}
	CString strPromptOut;
	if (getLen(m_strBmText) > nBmLen)
	{
		AfxMessageBox(_T("产品编号长度超过默认长度，请查看是否正确"));
		return;
	}
	if (getLen(m_strDesigner) > nDesignerLen)
	{
		AfxMessageBox(_T("设计师名称长度超过默认长度，请查看是否正确"));
		return;
	}
	if (getLen(m_strProName) > nProductLen)
	{
		AfxMessageBox(_T("产品名称长度超过默认长度，请查看是否正确"));
		return;
	}
	this->ShowWindow(FALSE);

	saveIniFile();
	//将文件完整路径存入dwg文件中
	CBaseUtils::setFilePath(m_strFileName);
	//将所有excel进程关闭
	/*startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("清理无关数据"));
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(5);
		Sleep(50);
	}*/
	CString strFile = CUtility::getLogPath() + _T("\\error.csv");
	if (gGlobal.IsFileExist(strFile))
	{
		::DeleteFile(strFile);
	}
	//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
	/*if (modelessDlg != NULL)
	{
		modelessDlg->setPos(30);
		Sleep(50);
	}*/
	//////////////////////////////////////////////////////////////////////////

	CMyExcel myExcel;
	BOOL bRet = myExcel.Open(getFile());
	if (!bRet)
	{
		AfxMessageBox(_T("文件不能打开，请检查路径是否有效"));
		this->ShowWindow(TRUE);
		return;
	}
	CAcUiDialog::OnCancel();

	gQuery.clearVec();
	/*if (modelessDlg != NULL)
	{
		modelessDlg->setPos(60);
		Sleep(50);
	}*/
	MyEditEntity::EraseAllEnt();
	/*if (modelessDlg != NULL)
	{
		modelessDlg->setPos(70);
		Sleep(50);
	}*/
	//////////////////////////////////////////////////////////////////////////
	//MyBaseUtils::SetVar(_T("USERI1"), 0);//记录count值
	CBaseUtils::setFrameCount(1);
	CBaseUtils::setRfCount(1);
	CBaseUtils::setDpCount(1);
	gQuery.setPreviewFlag(true);
	/*if (modelessDlg != NULL)
	{
		modelessDlg->setPos(90);
		Sleep(50);
	}*/
	CBaseUtils::setStartPt(AcGePoint3d(0, 0, 0));
	/*if (modelessDlg != NULL)
	{
		modelessDlg->setPos(100);
		Sleep(50);
	}
	endDlg();*/

	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("读取数据"));
	}

	int nCount = myExcel.GetSheetCount();
	for (int i=1; i<=nCount; i++)
	{
		bRet = myExcel.OpenSheet(i);
		myExcel.GetArea();
		myExcel.readSpecailCol();	
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(10+i*80/nCount);
			Sleep(50);
		}

	}

	//myExcel.verifyOther();

	endDlg();
	if (gGlobal.IsFileExist(strFile))
	{
		myExcel.Clear();
		//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
		ShellExecute(NULL,_T("open"),strFile,NULL,NULL,SW_SHOWNORMAL );
		return;
	}	
	myExcel.Clear();

	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("生成预览图形"));
	}

	CUtility::preView();
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(95);
		Sleep(50);
	}
	CUtility::UpdatePageNo();
	endDlg();

	//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));	
}


void CDlgTKSZ::OnBnClickedButtonSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	//BeginEditorCommand();
	this->ShowWindow(FALSE);
	CAcModuleResourceOverride rs;
	CDlgInitSetting init;
	init.DoModal();
	//CancelEditorCommand();
	this->ShowWindow(TRUE);
}
