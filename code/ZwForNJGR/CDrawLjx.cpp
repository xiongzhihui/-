#include "StdAfx.h"
#include "CDrawLjx.h"

CDrawLjx::CDrawLjx(void)
{
	m_pointMap.clear();
}

CDrawLjx::~CDrawLjx(void)
{
	m_pointMap.clear();
}

bool CDrawLjx::selEnt()
{
	ads_name ss;
	resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("INSERT"), 2, _T("ZW_TABLE"), -4,_T("and>"), RTNONE);
	int nRet = acedSSGet(NULL, NULL, NULL, filter, ss);	
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return false;
	}

	long sslen;
	acedSSLength(ss, &sslen);
	ads_name ename;
	AcDbObjectId objId;
	AcDbBlockReference* pBlkRef = NULL;
	AcDbExtents exts;
	AcGePoint3d pt;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ss, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		pBlkRef->getGeomExtents(exts);
		m_LayerId = pBlkRef->layerId();
		pBlkRef->close();

		pt.x = exts.maxPoint().x;
		pt.y = (exts.maxPoint().y + exts.minPoint().y)/2;
		pt.z = 0;

		m_pointMap.insert(make_pair(pt.y, pt));
	}

	acedSSFree(ss);
	return true;
}

bool CDrawLjx::doIt()
{
	if (!selEnt())
		return false;

	drawSpecialLine();
	return true;
}

bool CDrawLjx::drawSpecialLine()
{
	double dHorDist = gGlobal.GetIniValue(_T("配件"), _T("横向间距"), 0);
	double dVerDist = gGlobal.GetIniValue(_T("配件"), _T("纵向间距"), 0);
	double dSmallHorDist = gGlobal.GetIniValue(_T("配件"), _T("短线"), 0);

	AcGePoint3d startPt,endPt;
	AcDbObjectId lineId = AcDbObjectId::kNull;
	for (map<double, AcGePoint3d>::iterator iter = m_pointMap.begin(); iter != m_pointMap.end(); ++iter)
	{
		startPt = iter->second;
		endPt = startPt;
		endPt.x += dHorDist;
		lineId = MyDrawEntity::DrawLine(startPt, endPt, m_LayerId);		
	}

	if (m_pointMap.size() > 1)
	{
		startPt = m_pointMap.rbegin()->second;
		endPt = m_pointMap.begin()->second;
	}
	else
	{
		startPt = m_pointMap.begin()->second;
		endPt = startPt;
	}
	startPt.x += dHorDist;
	endPt.x += dHorDist;

	endPt.y -= dVerDist;
	lineId = MyDrawEntity::DrawLine(startPt, endPt, m_LayerId);

	//////////////////////////////////////////////////////////////////////////
	//绘制小横线
	AcGePoint3d tmpPt1,tmpPt2;
	tmpPt1 = tmpPt2 = endPt;
	tmpPt1.x -= dSmallHorDist/2;
	tmpPt2.x += dSmallHorDist/2;

	lineId = MyDrawEntity::DrawLine(tmpPt1, tmpPt2, m_LayerId);
	return true;
}
