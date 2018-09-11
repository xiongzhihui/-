//----- DlgEidtFrame.cpp : Implementation of CDlgEidtFrame
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgEidtFrame.h"
#include "CGlobalQuery.h"
#include "CDrawFrame.h"
#include "CDrawLrmvEnt.h"
#include "CGlobalRWriteFile.h"
#include "CDrawEnt.h"
#include "CDrawDigitalOptical.h"
extern CGlobalRWriteFile gRWriteFile;

extern CGlobalQuery gQuery;

static int Text_List_Type_Edit_Init( int col )
{
	if ( col == 0 )
		return CEditList::eLast;
	return CEditList::eEdit;
}
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgEidtFrame, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgEidtFrame, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &CDlgEidtFrame::OnCbnSelchangeComboSize)
	ON_BN_CLICKED(IDOK, &CDlgEidtFrame::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgEidtFrame::CDlgEidtFrame (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgEidtFrame::IDD, pParent, hInstance) , m_strSize(_T(""))
{
	m_pDb = NULL;
	ClearDatabase();
}

//-----------------------------------------------------------------------------
void CDlgEidtFrame::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_COMBO_SIZE, m_CombSize);
	DDX_CBString(pDX, IDC_COMBO_SIZE, m_strSize);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgEidtFrame::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgEidtFrame::OnCbnSelchangeComboSize()
{
	int nCursel = m_CombSize.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombSize.SetCurSel(nCursel);
	m_CombSize.GetLBText(m_CombSize.GetCurSel(), m_strSize);
	m_FrameInfo.setFrameType(m_strSize);
}

BOOL CDlgEidtFrame::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CenterWindow(acedGetAcadDwgView());
	// TODO:  Add extra initialization here

	m_ListCtrl.SetExtendedStyle( m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT/* | LVS_EX_CHECKBOXES*/);

	m_ListCtrl.InsertColumn( 0, _T("类型"));
	m_ListCtrl.InsertColumn( 1, _T("数据"));


	m_ListCtrl.SetColumnWidth( 0, 100);
	m_ListCtrl.SetColumnWidth( 1, 80);


	// set functionality of list according to column
	m_ListCtrl.SetColumnType ( (fGetType)Text_List_Type_Edit_Init);	

	initData();

	InitDatabase();
	InitGraphList();


	CString strTmp;
	CStringArray tmpArr;
	tmpArr.RemoveAll();
	CString strGanNature = gGlobal.GetIniValue(_T("图框设置"), _T("图框序列"));
	MyParserString::GetCommaFromString(strGanNature, tmpArr);

	for (int i=0; i<tmpArr.GetCount(); i++)
	{
		m_CombSize.AddString(tmpArr.GetAt(i));
	}
	
	int nCur = m_CombSize.FindString(-1, m_strSize);
	if (nCur < 0)
	{
		nCur = 0;
	}
	m_CombSize.SetCurSel(nCur);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgEidtFrame::initData()
{
	m_strValue = MyTransFunc::doubleToStr(m_FrameInfo.getHorSpace());
	setInfoToListCtrl(_T("表格横向间距(A)"));
	m_strValue =  MyTransFunc::doubleToStr(m_FrameInfo.getVerSpace());
	setInfoToListCtrl(_T("表格纵向间距(B)"));
	//m_strValue =  MyTransFunc::doubleToStr(m_FrameInfo.getLineLength());
	//setInfoToListCtrl(_T("文字线长"));
	//m_strValue =  MyTransFunc::doubleToStr(m_FrameInfo.getSpaceHorGap());
	//setInfoToListCtrl(_T("  备用横向长(C)"));
	m_strValue =  MyTransFunc::doubleToStr(m_FrameInfo.getLeftVerGap());
	setInfoToListCtrl(_T("  左上纵向长(C)"));
	m_strValue =  MyTransFunc::doubleToStr(m_FrameInfo.getLeftHorGap());
	setInfoToListCtrl(_T("  左上横向长(D)"));
	m_strValue =  MyTransFunc::doubleToStr(m_FrameInfo.getSpaceVerGap());
	setInfoToListCtrl(_T("  备用纵向长(E)"));
	m_strValue =  MyTransFunc::doubleToStr(m_FrameInfo.getGap());
	setInfoToListCtrl(_T("        间隙(F)"));
}
void CDlgEidtFrame::setInfoToListCtrl(CString strType)
{
	int nItem = m_ListCtrl.GetItemCount();
	nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
	m_ListCtrl.SetItemText(nItem, 0, strType);
	m_ListCtrl.SetItemText(nItem, 1, m_strValue);
}

void CDlgEidtFrame::ClearDatabase()
{
	m_gsView.freeTempEntities();
	if (m_pDb!=NULL)
	{
		delete m_pDb;
	}
}

void CDlgEidtFrame::InitDatabase()
{
	ClearDatabase();
	CAcModuleResourceOverride rs;
	m_pDb = new AcDbDatabase(false);
	CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\setting.dwg");
	m_pDb->readDwgFile(strFile);
}

bool CDlgEidtFrame::InitGraphList()
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

bool CDlgEidtFrame::sel()
{
	ads_name ename;
	AcGePoint3d pt;
	int nRet = acedEntSel(_T("\n请选择图框："), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return false;
	}
	acdbGetObjectId(m_BlkId, ename);
	AcDbBlockReference* pBlkRef = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, m_BlkId, AcDb::kForRead) != Acad::eOk)
	{
		AfxMessageBox(_T("没有选中图框实体"));
		return false;
	}
	pBlkRef->close();
	CString strBlockName;
	MyBaseUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
	CString strTemp = strBlockName.Left(1);
	if (strTemp.CompareNoCase(_T("A")) != 0)
	{
		AfxMessageBox(_T("没有选中图框实体"));
		return false;
	}

	//获取frame信息
	m_strSheetName = MyEditEntity::GetObjStrXdata(m_BlkId, ICD_SHEETNAME);
	if (m_strSheetName.IsEmpty())
	{
		return false;
	}
	CString strPromptOut;
	strPromptOut = m_strSheetName + _T(" : 该sheet不允许修改图幅");
	if (m_strSheetName.CompareNoCase(_T("(ICD_TU)")) == 0)
	{
		AfxMessageBox(strPromptOut);
		return false;
	}
	bool bFind = false;
	map<CString, CSingleSheetData> tmpData = gQuery.getCommonData();
	for (map<CString, CSingleSheetData>::iterator iter = tmpData.begin(); iter != tmpData.end(); ++iter)
	{
		CString strTemp = iter->first;
		if (strTemp.CompareNoCase(m_strSheetName) == 0)
		{
			AfxMessageBox(strPromptOut);
			bFind = true;
			break;
		}
	}
	if (bFind)
	{
		return false;
	}
	vector<CSheetStoreData> tmpVec;
	tmpVec = gQuery.getDigitalSheetData();
	CSheetStoreData sData;
	for (vector<CSheetStoreData>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		sData = *iter;
		if (sData.getSheetName().CompareNoCase(m_strSheetName) == 0)
		{
			bFind = true;
			AfxMessageBox(strPromptOut);
			break;
		}
		sData.clear();
	}
	tmpVec.clear();
	if (bFind)
	{
		return false;
	}
	m_FrameInfo.setLrmName(MyEditEntity::GetObjStrXdata(m_BlkId, ICD_LRM_NAME));
	m_strSize = MyEditEntity::GetObjStrXdata(m_BlkId, ICD_FRAMETYPE);
	m_FrameInfo.setFrameType(m_strSize);
	m_FrameInfo.setHorSpace(MyEditEntity::GetObjDoubleXdata(m_BlkId, ICD_HORSPACE));
	m_FrameInfo.setVerSpace(MyEditEntity::GetObjDoubleXdata(m_BlkId, ICD_VerSapce));
	//m_FrameInfo.setSpaceHorGap(MyEditEntity::GetObjDoubleXdata(m_BlkId, ICD_SPAREHORGAP));
	m_FrameInfo.setSpaceVerGap(MyEditEntity::GetObjDoubleXdata(m_BlkId, ICD_SPAREVERGAP));
	m_FrameInfo.setLeftVerGap(MyEditEntity::GetObjDoubleXdata(m_BlkId, ICD_LEFTVERGAP));
	m_FrameInfo.setLeftHorGap(MyEditEntity::GetObjDoubleXdata(m_BlkId, ICD_LEFTHORGAP));
	m_nCount = MyEditEntity::GetObjIntXdata(m_BlkId, ICD_COUNT);
	
	return true;
}

void CDlgEidtFrame::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!getInitData())
	{
		AfxMessageBox(_T("数据不能有空值"));
		return;
	}
	CString strValue = m_ListCtrl.GetItemText(5, 1);
	double dValue = MyTransFunc::StringToDouble(strValue);
	ReWriteInitData();

	if (!gQuery.getEditFrameFlag())
	{
		return;
	}
	//删除所有sheet名称
	EraseSheetEnt();

	//获取该sheet数据，重新绘制该图形
	CSheetStoreData sData;
	vector<CSheetStoreData> tmpVec;
	tmpVec = gQuery.getLRMVModSheetData();
	bool bFind = false;
	for (vector<CSheetStoreData>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		sData = *iter;
		if (sData.getSheetName().CompareNoCase(m_strSheetName) == 0)
		{
			CDrawLrmvEnt drawEnt;
			drawEnt.setModFlag(true);
			drawEnt.setCount(m_nCount);
			drawEnt.setData(sData);
			drawEnt.draw();
			bFind = true;
			sData.clear();
			break;
		}
		sData.clear();
	}
	if (bFind)
	{
		CAcUiDialog::OnOK();
		return;
	}
	tmpVec.clear();
	tmpVec = gQuery.getLRMVSheetData();
	for (vector<CSheetStoreData>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		sData = *iter;
		if (sData.getSheetName().CompareNoCase(m_strSheetName) == 0)
		{
			CDrawLrmvEnt drawEnt;
			drawEnt.setCount(m_nCount);
			drawEnt.setData(sData);
			drawEnt.draw();
			bFind = true;

			sData.clear();
			break;
		}
		sData.clear();
	}
	if (bFind)
	{
		CAcUiDialog::OnOK();
		return;
	}
	//绘制普通数据
	/*map<CString, CSingleSheetData> tmpData = gQuery.getCommonData();
	vector<CGLData> glVecData;
	for (map<CString, CSingleSheetData>::iterator iter = tmpData.begin(); iter != tmpData.end(); ++iter)
	{
		CString strTemp = iter->first;
		if (strTemp.CompareNoCase(m_strSheetName) == 0)
		{
			CSingleSheetData singleData = iter->second;
			CDrawEntity drawEnt;

			drawEnt.Init();
			drawEnt.setSheetName(singleData.getSheetName());
			if (singleData.getMapInfo().size() > 0)
			{
				drawEnt.setCount(m_nCount);
				drawEnt.setNewFrameFlag(false);
				drawEnt.drawFrame();	
				drawEnt.draw(singleData.getMapInfo());
			}
			glVecData = singleData.getSheetStoreData().getGLData();
			if (glVecData.size() > 0)
			{
				CDrawDigitalOptical drawDg;
				drawDg.setFrameCount(m_nCount);
				drawDg.setData(singleData.getSheetStoreData());
				drawDg.draw();	
			}
			bFind = true;
			break;
		}
	}*/
	CAcUiDialog::OnOK();
}	


bool CDlgEidtFrame::getInitData()
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

CString CDlgEidtFrame::getInfoToListCtrl(int i)
{
	m_strValue = m_ListCtrl.GetItemText(i, 1);
	return m_strValue;
}

void CDlgEidtFrame::ReWriteInitData()
{
	m_strValue = m_ListCtrl.GetItemText(0, 1);
	m_FrameInfo.setHorSpace(MyTransFunc::StringToDouble(m_strValue));
	m_strValue = m_ListCtrl.GetItemText(1, 1);
	m_FrameInfo.setVerSpace(MyTransFunc::StringToDouble(m_strValue));
	m_strValue = m_ListCtrl.GetItemText(2, 1);
	m_FrameInfo.setLeftVerGap(MyTransFunc::StringToDouble(m_strValue));
	m_strValue = m_ListCtrl.GetItemText(3, 1);
	m_FrameInfo.setLeftHorGap(MyTransFunc::StringToDouble(m_strValue));
	m_strValue = m_ListCtrl.GetItemText(4, 1);
	m_FrameInfo.setSpaceVerGap(MyTransFunc::StringToDouble(m_strValue));
	m_strValue = m_ListCtrl.GetItemText(5, 1);
	m_FrameInfo.setGap(MyTransFunc::StringToDouble(m_strValue));
	//m_strValue = m_ListCtrl.GetItemText(6, 1);
	//m_strValue = m_ListCtrl.GetItemText(7, 1);

	MyEditEntity::OpenObjAppendStrToXdata(m_BlkId, ICD_LRM_NAME, m_FrameInfo.getLrmName());
	MyEditEntity::OpenObjAppendStrToXdata(m_BlkId, ICD_SHEETNAME, m_strSheetName);

	MyEditEntity::OpenObjAppendStrToXdata(m_BlkId, ICD_FRAMETYPE, m_FrameInfo.getFrameType());
	MyEditEntity::OpenObjAppendDoubleToXdata(m_BlkId, ICD_HORSPACE, m_FrameInfo.getHorSpace());
	//MyEditEntity::OpenObjAppendDoubleToXdata(m_BlkId, ICD_LINELENGTH, m_FrameInfo.getLineLength());
	MyEditEntity::OpenObjAppendDoubleToXdata(m_BlkId, ICD_VerSapce, m_FrameInfo.getVerSpace());
	//MyEditEntity::OpenObjAppendDoubleToXdata(m_BlkId, ICD_SPAREHORGAP, m_FrameInfo.getSpaceHorGap());
	MyEditEntity::OpenObjAppendDoubleToXdata(m_BlkId, ICD_SPAREVERGAP, m_FrameInfo.getSpaceVerGap());
	MyEditEntity::OpenObjAppendDoubleToXdata(m_BlkId, ICD_LEFTVERGAP, m_FrameInfo.getLeftVerGap());
	MyEditEntity::OpenObjAppendDoubleToXdata(m_BlkId, ICD_LEFTHORGAP, m_FrameInfo.getLeftHorGap());							
	gRWriteFile.saveAllFrameInfo();
}

void CDlgEidtFrame::EraseSheetEnt()
{
	resbuf* filter = acutBuildList(-4,_T("<and"), 8, m_strSheetName, -4,_T("and>"), RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen < 1)
	{
		return;
	}

	MyEditEntity::EraseSSName(ssname);
	
	acedSSFree(ssname);
}
