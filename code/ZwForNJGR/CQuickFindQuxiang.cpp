#include "StdAfx.h"
#include "CQuickFindQuxiang.h"
#include "BaseUtils.h"
#include "CGlobalQuery.h"
#include "DlgListError.h"
extern CGlobalQuery gQuery;

CQuickFindQuxiang::CQuickFindQuxiang(void)
{
}


CQuickFindQuxiang::~CQuickFindQuxiang(void)
{
}

bool CQuickFindQuxiang::sel()
{
	bool bRet = false;
	AcGePoint3d pt;
	ads_name ename;
	int nRet = acedEntSel(_T("\n请选择特性数据："), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return false;
	}
	AcDbObjectId objId = AcDbObjectId::kNull;
	acdbGetObjectId(objId, ename);
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}

	if (pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText = AcDbText::cast(pEnt);
		m_strSheetName = pText->layer();
		m_strContent = pText->textString();
		pText->close();
	}
	else
	{
		AfxMessageBox(_T("没有选择去向数据"));
		pEnt->close();
		return false;
	}
	return true;
}

bool CQuickFindQuxiang::doIt()
{
	if (!CBaseUtils::jxbFlag())
	{
		AfxMessageBox(_T("先通过电气特性连接表命令生成CAD图形"));
		return false;
	}
	else
	{
		if (!sel())
			return false;
		if (!AnalysisData())
			return false;
		if (!showDialog())
			return false;
#ifdef DEBUG
		print();
#endif
	}
	return true;
}

bool CQuickFindQuxiang::AnalysisData()
{
	COMVEC allVec = gQuery.getCellInfo();
	if (allVec.size() < 1)
	{
		return false;
	}
	CString strTexing;
	CCommonCell cell;
	for (COMVEC::iterator iter = allVec.begin(); iter != allVec.end(); ++iter)
	{
		cell = *iter;
		strTexing = cell.getRealTexing();
		if (strTexing.CompareNoCase(m_strContent) == 0)
		{
			m_VecInfo.push_back(cell);
			addDlgInfo(cell);
		}
	}
	if (m_VecInfo.size() < 1)
	{
		AfxMessageBox(_T("没有找到特性数据，请确认是否选择的是特性数据"));
		return false;
	}

	return true;
}

void CQuickFindQuxiang::print()
{
	CCommonCell cell;
	for (COMVEC::iterator iter = m_VecInfo.begin(); iter != m_VecInfo.end(); ++iter)
	{
		cell = *iter;
		acutPrintf(_T("\n特性：%s,原始信息：%s,sheet名：%s-行：%d列：%d"), cell.getRealTexing(), cell.getTexing(), cell.getSheetName(), cell.getRow(), cell.getCol());
	}
}

bool CQuickFindQuxiang::showDialog()
{
	if (m_dlgInfo.size() < 1)
	{
		return false;
	}
	CAcModuleResourceOverride rs;
	CDlgListError dlg;
	CString strPrompt = _T("查找到的所有同名特性数据");
	dlg.setErrorData(strPrompt, m_dlgInfo);
	dlg.setShowFlag(false);
	if (dlg.DoModal() != IDOK)
	{
		return false;
	}

	m_dlgInfo.clear();
	return true;
}

void CQuickFindQuxiang::addDlgInfo(CCommonCell curComCell)
{
	CString strType = _T("查找到的所有同名特性数据");
	CIgnoreErrorCell ignoreCell;
	ignoreCell.set(curComCell.getSheetName(), MyTransFunc::intToStr(curComCell.getRow()), MyTransFunc::intToStr(curComCell.getCol()), curComCell.getTexing(), curComCell.getRealTexing(), strType);
	m_dlgInfo.push_back(ignoreCell);
}
