//-----------------------------------------------------------------------------
//----- DlgDrawResistance.cpp : Implementation of CDlgDrawResistance
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgDrawResistance.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgDrawResistance, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgDrawResistance, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgDrawResistance::CDlgDrawResistance (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgDrawResistance::IDD, pParent, hInstance) 
{
	readIniFile();
}

//-----------------------------------------------------------------------------
void CDlgDrawResistance::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_QUXIANG, m_strQuxiang);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgDrawResistance::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

// -----------------------------------------------------------------------------
BOOL CDlgDrawResistance::OnInitDialog(void)
{
	BOOL retCode =CAcUiDialog::OnInitDialog () ;
	CenterWindow(acedGetAcadDwgView());

	return (retCode) ;
}

// -----------------------------------------------------------------------------
void CDlgDrawResistance::OnOK(void)
{
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
	GetDlgItem(IDC_EDIT_QUXIANG)->GetWindowText(m_strQuxiang);
	//////////////////////////////////////////////////////////////////////////
	CString strFile = MyBaseUtils::GetAppPath() + _T("配件\\dz.dwg");
	if (!gGlobal.IsFileExist(strFile))
	{
		AfxMessageBox(_T("没有电阻文件"));
		return;
	}

	//选择实体
	AcGePoint3d pt = selEnt();
	//绘制电阻
	AcDbObjectId objId = MyDrawEntity::InsertBlkRef(strFile, pt);
	objId = MyEditEntity::openEntChangeLayer(objId, m_layerId);
	//绘制电阻文字
	drawDzText(objId);

	//绘制去向文字
	drawQuxiangText(pt);

	writeIniFile();
	CAcUiDialog::OnOK () ;
}

void CDlgDrawResistance::writeIniFile()
{
	gGlobal.SetIniValue(_T("配件"), _T("电阻名称"), m_strName);
	gGlobal.SetIniValue(_T("配件"), _T("去向"), m_strQuxiang);
	gGlobal.WriteIniFile();
}

void CDlgDrawResistance::readIniFile()
{
	m_strName = gGlobal.GetIniValue(_T("配件"), _T("电阻名称"));
	m_strQuxiang = gGlobal.GetIniValue(_T("配件"), _T("去向"));
}

AcGePoint3d CDlgDrawResistance::selEnt()
{
	AcGePoint3d pt;
	BeginEditorCommand();

	ads_name ename;
	int nRet = acedEntSel(_T("\n请选择一个需要增加接地的图形:"), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return pt;
	}
	AcDbEntity* pEnt = NULL;
	AcDbObjectId objId = AcDbObjectId::kNull;
	acdbGetObjectId(objId, ename);
	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
	{
		return pt;
	}
	AcDbExtents exts;
	CString strBlockName;
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
		pBlkRef->getGeomExtents(exts);
		MyBaseUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
		m_layerId = pBlkRef->layerId();
		pBlkRef->close();
	}
	else
	{
		pEnt->close();
	}
	if (strBlockName.CompareNoCase(_T("ZW_TABLE")) == 0)
	{
		pt.x = exts.maxPoint().x;
		pt.y = (exts.maxPoint().y + exts.minPoint().y)/2;
		pt.z = 0;
	}

	CompleteEditorCommand();
	return pt;
}

void CDlgDrawResistance::drawQuxiangText(AcGePoint3d pt)
{
	double dThirdTableLength  = gGlobal.GetIniValue(_T("基础设置"), _T("表格第三段长度"), 0);
	pt.x -= dThirdTableLength/2;

	AcDbObjectId textId = MyDrawEntity::DrawText(pt, m_strQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
}

void CDlgDrawResistance::drawDzText(AcDbObjectId objId)
{
	
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		return;
	}	
	AcDbExtents exts;

	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
		pBlkRef->getGeomExtents(exts);
		pBlkRef->close();
	}
	else
	{
		pEnt->close();
	}
	
	AcGePoint3d pt;
	pt.x = exts.maxPoint().x;
	pt.y = (exts.maxPoint().y + exts.minPoint().y)/2;
	pt.z = 0;

	AcDbObjectId textId = MyDrawEntity::DrawText(pt, m_strName, 3.0, AcDbObjectId::kNull, AcDb::kTextLeft, AcDb::kTextVertMid);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
}
