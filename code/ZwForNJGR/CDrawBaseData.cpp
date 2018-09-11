#include "StdAfx.h"
#include "CDrawBaseData.h"
#include "BaseUtils.h"


CDrawBaseData::CDrawBaseData(void)
{
}


CDrawBaseData::~CDrawBaseData(void)
{
}

void CDrawBaseData::init()
{
	m_dTableHeight = gGlobal.GetIniValue(_T("基础设置"), _T("表格高度"), 0);

	m_dHorSpace = gGlobal.GetIniValue(_T("基础设置"), _T("表格横向间距"), 0);
	m_dVerSapce = gGlobal.GetIniValue(_T("基础设置"), _T("表格纵向间距"), 0);
	m_dLineLength = gGlobal.GetIniValue(_T("基础设置"), _T("文字线长"), 0);
	m_dFrameHorSpace = gGlobal.GetIniValue(_T("基础设置"), _T("图框横向间距"), 0);
	m_dFrameVerSpace = gGlobal.GetIniValue(_T("基础设置"), _T("图框纵向间距"), 0);
	m_dSpareHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用横向长"), 0);
	m_dSpareVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用纵向长"), 0);

	m_dLeftHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上横向长"), 0);
	m_dLeftVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上纵向长"), 0);
	m_nHxCount = (int)gGlobal.GetIniValue(_T("基础设置"), _T("横向图框数"), 0);
	CString strFrameType = gGlobal.GetIniValue(_T("图框设置"), _T("图框类型"));
	if (strFrameType.CompareNoCase(_T("A0")) == 0)
	{
		m_dFrameLength = 1189;
		m_dFrameHeight = 841;
	}
	else if (strFrameType.CompareNoCase(_T("A1")) == 0)
	{
		m_dFrameLength = 841;
		m_dFrameHeight = 594;
	}
	else if (strFrameType.CompareNoCase(_T("A2")) == 0)
	{
		m_dFrameLength = 594;
		m_dFrameHeight = 420;
	}
	else if (strFrameType.CompareNoCase(_T("A3")) == 0)
	{
		m_dFrameLength = 420;
		m_dFrameHeight = 297;
	}
	else if (strFrameType.CompareNoCase(_T("A4")) == 0)
	{
		m_dFrameLength = 297;
		m_dFrameHeight = 210;
	}
	else
	{
		m_dFrameLength = 841;
		m_dFrameHeight = 594;
	}
	m_dFirstTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第一段长度"), 0);
	m_dSecondTableLength =  gGlobal.GetIniValue(_T("基础设置"), _T("表格第二段长度"), 0);
	m_dThirdTableLength  = gGlobal.GetIniValue(_T("基础设置"), _T("表格第三段长度"), 0);
	m_dTableLength = m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength;

	m_dRfSecondTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第二段长度"), 0);
	m_dRfThirdTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第三段长度"), 0);
	m_dGap = gGlobal.GetIniValue(_T("基础设置"), _T("间隙"), 0);

	//将数据记录在图纸中
	CBaseUtils::setFrameType(strFrameType);
	CBaseUtils::setHxFrameCount(m_nHxCount);
	CBaseUtils::setVerFrameSpace(m_dFrameVerSpace);
	CBaseUtils::setHorFrameSpace(m_dFrameHorSpace);
	CBaseUtils::setFrameHeight(m_dFrameHeight);
	CBaseUtils::setFrameLength(m_dFrameLength);
	CBaseUtils::setLeftVerGap(m_dLeftVerGap);
	CBaseUtils::setLeftHorGap(m_dLeftHorGap);
	CBaseUtils::setSpaceHorGap(m_dSpareHorGap);
	CBaseUtils::setSpaceVerGap(m_dSpareVerGap);
	CBaseUtils::setHorSpace(m_dHorSpace);
	CBaseUtils::setVerSpace(m_dVerSapce);
}

double CDrawBaseData::frameLength()
{
	return m_dFrameLength;
}

double CDrawBaseData::frameHeight()
{
	return m_dFrameHeight;
}

double CDrawBaseData::frameHorSpace()
{
	return m_dFrameHorSpace;
}

double CDrawBaseData::frameVerSpace()
{
	return m_dFrameVerSpace;
}

double CDrawBaseData::tableLength()
{
	return m_dTableLength;
}

double CDrawBaseData::tableHeight()
{
	return m_dTableHeight;
}

double CDrawBaseData::horSpace()
{
	return m_dHorSpace;
}

double CDrawBaseData::verSpace()
{
	return m_dVerSapce;
}

double CDrawBaseData::lineLength()
{
	return m_dLineLength;
}

double CDrawBaseData::spaceHorGap()
{
	return m_dSpareHorGap;
}

double CDrawBaseData::spaceVerGap()
{
	return m_dSpareVerGap;
}

int CDrawBaseData::HxFrameCount()
{
	return m_nHxCount;
}

double CDrawBaseData::firstTableLength()
{
	return m_dFirstTableLength;
}

double CDrawBaseData::secondTableLength()
{
	return m_dSecondTableLength;
}

double CDrawBaseData::thirdTableLength()
{
	return m_dThirdTableLength;
}

double CDrawBaseData::rfSecondTableLength()
{
	return m_dRfSecondTableLength;
}

double CDrawBaseData::rfThirdTableLength()
{
	return m_dRfThirdTableLength;
}

AcGePoint3d CDrawBaseData::getInsertPt()
{
	AcGePoint3d insertPt;
	insertPt =  CBaseUtils::startPt();
	return insertPt;
}


AcGePoint3d CDrawBaseData::getContentPt(AcGePoint3d insertPt)
{
	AcGePoint3d pt;
	acutPolar(asDblArray(insertPt), PI, m_dFrameLength - m_dLeftHorGap, asDblArray(pt));
	acutPolar(asDblArray(pt), PI/2, m_dFrameHeight - m_dLeftVerGap - m_dTableHeight, asDblArray(pt));
	return pt;
}

void CDrawBaseData::setInsertPt(AcGePoint3d insertPt)
{
	int nCount = CBaseUtils::frameCount();
	insertPt = CBaseUtils::startPt();
	int nYCount = nCount/m_nHxCount;
	int nXCount = nCount%m_nHxCount;
	//移动insertPt;
	if ((nXCount == 1) && (nYCount > 0))
	{
		acutPolar(asDblArray(insertPt), 3*PI/2, m_dFrameVerSpace+m_dFrameHeight, asDblArray(insertPt));
		acutPolar(asDblArray(insertPt), PI, (m_nHxCount-1)*(m_dFrameHorSpace+m_dFrameLength), asDblArray(insertPt));
	}
	else
	{
		acutPolar(asDblArray(insertPt), 0, (m_dFrameHorSpace+m_dFrameLength), asDblArray(insertPt));
	}
	CBaseUtils::setStartPt(insertPt);

}
