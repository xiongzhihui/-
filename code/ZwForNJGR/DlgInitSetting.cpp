//-----------------------------------------------------------------------------
//----- DlgInitSetting.cpp : Implementation of CDlgInitSetting
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgInitSetting.h"
#include "Utility.h"
#include "BaseUtils.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgInitSetting, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgInitSetting, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgInitSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CDlgInitSetting::OnBnClickedButtonHelp)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgInitSetting::CDlgInitSetting (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgInitSetting::IDD, pParent, hInstance)
{
	m_pDb = NULL;
	ClearDatabase();
}

//-----------------------------------------------------------------------------
void CDlgInitSetting::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgInitSetting::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}
static int Text_List_Type_Init( int col )
{
	if ( col == 0 )
		return CEditList::eLast;
	//else if (col == 2)
	//{
	//	return CEditList::eCombo;
	//}
	////else if (col == 3)
	//{
	//	return CEditList::eCombo;
	//}
	return CEditList::eEdit;
}
BOOL CDlgInitSetting::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());
	// TODO:  Add extra initialization here

	m_ListCtrl.SetExtendedStyle( m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT/* | LVS_EX_CHECKBOXES*/);

	m_ListCtrl.InsertColumn( 0, _T("类型"));
	m_ListCtrl.InsertColumn( 1, _T("数据"));


	m_ListCtrl.SetColumnWidth( 0, 100);
	m_ListCtrl.SetColumnWidth( 1, 80);


	// set functionality of list according to column
	m_ListCtrl.SetColumnType ( (fGetType)Text_List_Type_Init);	

	initData();

	InitDatabase();
	InitGraphList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInitSetting::initData()
{
	//setInfoToListCtrl(_T("表格长度"));
	//setInfoToListCtrl(_T("表格高度"));
	setInfoToListCtrl(_T("表格横向间距(A)"));
	setInfoToListCtrl(_T("表格纵向间距(B)"));
	//setInfoToListCtrl(_T("文字线长"));
	//setInfoToListCtrl(_T("图框横向间距"));
	//setInfoToListCtrl(_T("图框纵向间距"));
	//setInfoToListCtrl(_T("  备用横向长(C)"));
	setInfoToListCtrl(_T("  左上纵向长(C)"));
	setInfoToListCtrl(_T("  左上横向长(D)"));
	setInfoToListCtrl(_T("  备用纵向长(E)"));
	//setInfoToListCtrl(_T("横向图框数"));
	setInfoToListCtrl(_T("        间隙(F)"));
}

void CDlgInitSetting::setInfoToListCtrl(CString strType)
{
	m_strType = strType;
	m_strType = m_strType.Left(m_strType.GetLength() - 3);
	m_strType.Trim();
	
	m_strValue = gGlobal.GetIniValue(_T("基础设置"), m_strType);
	int nItem = m_ListCtrl.GetItemCount();
	nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
	m_ListCtrl.SetItemText(nItem, 0, strType);
	m_ListCtrl.SetItemText(nItem, 1, m_strValue);
}

void CDlgInitSetting::ClearDatabase()
{
	m_gsView.freeTempEntities();
	if (m_pDb!=NULL)
	{
		delete m_pDb;
	}
}

void CDlgInitSetting::InitDatabase()
{
	ClearDatabase();
	CAcModuleResourceOverride rs;
	m_pDb = new AcDbDatabase(false);
	CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\setting.dwg");
	m_pDb->readDwgFile(strFile);
}

bool CDlgInitSetting::InitGraphList()
{
	if (!m_gsView.SubclassDlgItem(IDC_STATIC_BMP, this))
	{
		return FALSE;
	}

	m_gsView.EnableOrbin(false);
	m_gsView.EnablePan(true);
	m_gsView.EnableZoom(true);

	m_gsView.InitBlockView(ACDB_MODEL_SPACE, m_pDb);
	return true;
}

void CDlgInitSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!getInitData())
	{
		AfxMessageBox(_T("数据不能有空值"));
		return;
	}
	CString strValue = m_ListCtrl.GetItemText(5, 1);
	double dValue = MyTransFunc::StringToDouble(strValue);
	if (dValue < 20)
	{
		AfxMessageBox(_T("间隙不能小于20"));
		return;
	}

	ReWriteInitData();

	CUtility::createBlkRef();
	CAcUiDialog::OnOK();
}

CString CDlgInitSetting::getInfoToListCtrl(int i)
{
	m_strValue = m_ListCtrl.GetItemText(i, 1);
	return m_strValue;
}


void CDlgInitSetting::ReWriteInitData()
{
	//表格横向间距
	setDataToIniFile(0);
	CBaseUtils::setHorSpace(MyTransFunc::StringToDouble(m_strValue));
	//表格纵向间距
	setDataToIniFile(1);
	CBaseUtils::setVerSpace(MyTransFunc::StringToDouble(m_strValue));
	//左上纵向长
	setDataToIniFile(2);
	CBaseUtils::setLeftVerGap(MyTransFunc::StringToDouble(m_strValue));
	//左上横向长
	setDataToIniFile(3);
	CBaseUtils::setLeftHorGap(MyTransFunc::StringToDouble(m_strValue));
	//备用纵向长
	setDataToIniFile(4);
	CBaseUtils::setSpaceVerGap(MyTransFunc::StringToDouble(m_strValue));
	//间隙
	setDataToIniFile(5);
	/*setDataToIniFile(6);
	setDataToIniFile(7);
	setDataToIniFile(8);*/
	/*setDataToIniFile(9);
	setDataToIniFile(10);
	setDataToIniFile(11);*/
	gGlobal.WriteIniFile();
}

void CDlgInitSetting::setDataToIniFile(int nItem)
{
	m_strType = m_ListCtrl.GetItemText(nItem, 0);
	m_strType = m_strType.Left(m_strType.GetLength() - 3);
	m_strType.Trim();
	m_strValue = m_ListCtrl.GetItemText(nItem, 1);
	gGlobal.SetIniValue(_T("基础设置"), m_strType, m_strValue);
	/*if (nItem == 4)
	{
		double dVerSpace = MyTransFunc::StringToDouble(m_strValue);
		CBaseUtils::setVerFrameSpace(dVerSpace);
	}
	else if (nItem == 3)
	{
		double dHorSpace = MyTransFunc::StringToDouble(m_strValue);
		CBaseUtils::setHorFrameSpace(dHorSpace);
	}*/
}

bool CDlgInitSetting::getInitData()
{
	if (getInfoToListCtrl(0).IsEmpty() ||
		getInfoToListCtrl(1).IsEmpty() ||
		getInfoToListCtrl(2).IsEmpty() ||
		getInfoToListCtrl(3).IsEmpty() ||
		getInfoToListCtrl(4).IsEmpty() ||
		getInfoToListCtrl(5).IsEmpty() /*||
		getInfoToListCtrl(6).IsEmpty() ||
		getInfoToListCtrl(7).IsEmpty() ||
		getInfoToListCtrl(8).IsEmpty() ||
		getInfoToListCtrl(9).IsEmpty() ||
		getInfoToListCtrl(10).IsEmpty()||
		getInfoToListCtrl(11).IsEmpty()*/)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CDlgInitSetting::OnBnClickedButtonHelp()
{
	// TODO: Add your control notification handler code here
	CString strFile = MyBaseUtils::GetAppPath() + _T("support\\配置.jpg");
	ShellExecute(NULL,_T("open"), strFile, NULL, NULL, SW_SHOW); 
}

CDlgInitSetting::~CDlgInitSetting()
{
	ClearDatabase();
}
