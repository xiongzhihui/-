// DlgEditTexing.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgEditTexing.h"
#include "afxdialogex.h"


// CDlgEditTexing 对话框

IMPLEMENT_DYNAMIC(CDlgEditTexing, CAcUiDialog)

CDlgEditTexing::CDlgEditTexing(CString row, CString col, CString sheetname, int type, CString duanzi, CString xianhao,
CString quxiang, CString texing, AcDbObjectId idEdit, CWnd* pParent /*=NULL*/)
: CAcUiDialog(CDlgEditTexing::IDD, pParent)
{
	m_sRow = row;
	m_sCol = col;
	m_sDuanzi = duanzi;
	m_sXianhao = xianhao;
	m_sQxiang = quxiang;
	m_sTexing = texing;
	//记录原有特性
	m_sTexingYuanyou = texing;
	m_sSheetName = sheetname;
	m_sType = CBaseUtils::GetTexingType(type);
	m_idEdit = idEdit;
	m_bDelete = false;
	m_bAdd = false;
	m_bEdit = false;
	init();
}

CDlgEditTexing::~CDlgEditTexing()
{
}

void CDlgEditTexing::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COL, m_sCol);
	DDX_Text(pDX, IDC_EDIT_DUANZI, m_sDuanzi);
	DDX_Text(pDX, IDC_EDIT_QXIANG, m_sQxiang);
	DDX_Text(pDX, IDC_EDIT_ROW, m_sRow);
	DDX_Text(pDX, IDC_EDIT_SHEETNAME, m_sSheetName);
	DDX_Text(pDX, IDC_EDIT_TEXING, m_sTexing);
	DDX_Text(pDX, IDC_EDIT_XIANHAO, m_sXianhao);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_sType);
}


BEGIN_MESSAGE_MAP(CDlgEditTexing, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CDlgEditTexing::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEditTexing 消息处理程序


void CDlgEditTexing::editTx()
{
	//修改特性显示值
	AcDbObjectIdArray idArr = CBaseUtils::GetObjectIdArrayForRange(CBaseUtils::GetExtentsForEntity(m_idEdit));
	for (int i = 0; i < idArr.length(); i++)
	{
		CString sTag;
		MyBaseAppXdata::getXdata(ICD_XDATA_TEXINGTAG, sTag, idArr[i]);
		if (sTag != _T("是"))
		{
			continue;
		}
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es;
		es = acdbOpenAcDbEntity(pEnt, idArr[i], AcDb::kForWrite);
		if (es != eOk)
		{
			continue;
		}
		if (!pEnt->isKindOf(AcDbText::desc()))
		{
			pEnt->close();
			continue;
		}
		AcDbText *pText = static_cast<AcDbText*>(pEnt);
		pText->setTextString(m_sTexing);
		pText->close();
		MyBaseAppXdata::setXdata(ICD_XDATA_TEXING, m_sTexing, idArr[i]);
		break;
	}
}

void CDlgEditTexing::deleteTx()
{
	//修改特性显示值
	AcDbObjectIdArray idArr = CBaseUtils::GetObjectIdArrayForRange(CBaseUtils::GetExtentsForEntity(m_idEdit));
	for (int i = 0; i < idArr.length(); i++)
	{
		CString sTag;
		MyBaseAppXdata::getXdata(ICD_XDATA_TEXINGTAG, sTag, idArr[i]);
		if (sTag != _T("是"))
		{
			continue;
		}
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es;
		es = acdbOpenAcDbEntity(pEnt, idArr[i], AcDb::kForWrite);
		if (es != eOk)
		{
			continue;
		}
		if (!pEnt->isKindOf(AcDbText::desc()))
		{
			pEnt->close();
			continue;
		}
		AcDbText *pText = static_cast<AcDbText*>(pEnt);
		AcGePoint3d postion = pText->alignmentPoint();
		pText->erase();
		pText->close();
		MyBaseAppXdata::setXdata(ICD_XDATA_TEXING, m_sTexing, idArr[i]);
		//在外框中记录坐标
		MyBaseAppXdata::setXdata(ICD_XDATA_ZUOBIAO, postion, m_idEdit);
		break;
	}
}

void CDlgEditTexing::addTx()
{
	AcGePoint3d postion;
	MyBaseAppXdata::getXdata(ICD_XDATA_ZUOBIAO, postion, m_idEdit);
	AcDbObjectId textId=MyDrawEntity::DrawText(postion, m_sTexing, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
	MyBaseAppXdata::setXdata(ICD_XDATA_TEXINGTAG, _T("是"), textId);
	MyBaseAppXdata::setXdata(ICD_XDATA_TEXING, m_sTexing, textId);
}

void CDlgEditTexing::init()
{
	m_dFirstTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第一段长度"), 0);
	m_dSecondTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第二段长度"), 0);
	m_dThirdTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第三段长度"), 0);
	m_dTableLength = m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength;

	m_dRfSecondTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第二段长度"), 0);
	m_dRfThirdTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第三段长度"), 0);
}

void CDlgEditTexing::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//写入修改后信息 主要为端子信息的修改
	MyBaseAppXdata::setXdata(ICD_XDATA_TEXING, m_sTexing, m_idEdit);
	//写入edit标记 标记此图形已经修改
	MyBaseAppXdata::setXdata(ICD_XDATA_XIUGAI, _T("是"), m_idEdit);

	if (m_sTexing==_T("")&&m_sTexingYuanyou!=_T(""))
	{
		m_bDelete = true;
	}
	else if (m_sTexing != _T("") && m_sTexingYuanyou == _T(""))
	{
		m_bAdd = true;
	}
	else
	{
		m_bEdit = true;
	}
	if (m_bDelete)
	{
		deleteTx();
	}
	else if (m_bAdd)
	{
		addTx();
	}
	else
	{
		editTx();
	}
	CAcUiDialog::OnOK();
}
