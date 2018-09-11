#include "StdAfx.h"
#include "CDrawOutLine.h"
#include "BaseUtils.h"
#include <algorithm>
#include "Utility.h"

bool CMPPoint(const AcGePoint3d pt1, const AcGePoint3d pt2)
{
	return pt1.x < pt2.x;
}

CDrawOutLine::CDrawOutLine(void)
{
	m_MapName.clear();
	m_idArrs.removeAll();
	m_pointsMap.clear();
	m_nColor = 2;//初始设定为2
}


CDrawOutLine::~CDrawOutLine(void)
{
}

bool CDrawOutLine::getAllLayerName()
{
	CString strName;
	CString strLayerName;
	vector<CString> tmpVec;
	AcDbLayerTable* pTextTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()  ->getSymbolTable(pTextTbl, AcDb::kForRead); 
	AcDbLayerTableIterator* pTblIter = NULL;
	AcDbLayerTableRecord* pTextTblRec = NULL;
	pTextTbl->newIterator(pTblIter);
	for (; !pTblIter->done(); pTblIter->step())
	{
		const TCHAR* locName;
		pTblIter->getRecord(pTextTblRec, AcDb::kForRead);
		pTextTblRec->getName(locName);
		strName = locName;
		int nFind = strName.Find(_T("("));
		if (nFind > 0)//如果大于0,表示有母版信息
		{
			strLayerName = strName.Left(nFind);
			CString strTemp;
			int nLen = MyParserString::GetPileLength(strLayerName, strTemp);
			if (nLen == 0)
			{
				//如果是母版，就处理
				tmpVec.clear();
				tmpVec.push_back(strName);
				pair<map<CString, vector<CString> >::iterator, bool> bRet = m_MapName.insert(make_pair(strLayerName, tmpVec));
				if (!bRet.second)
				{
					bRet.first->second.push_back(strName);
				}
			}
		}
		else if (nFind == 0)
		{
			//是综合面板类图层
			tmpVec.clear();
			tmpVec.push_back(strName);
			pair<map<CString, vector<CString> >::iterator, bool> bRet = m_MapName.insert(make_pair(_T(""), tmpVec));
			if (!bRet.second)
			{
				bRet.first->second.push_back(strName);
			}
		}
		else
		{
			//不是需要的信息
		}
		pTextTblRec->close();
	}
	pTextTbl->close();
	delete pTblIter;
	return true;
}

bool CDrawOutLine::doIt()
{
	//获取图层信息
	getAllLayerName();
	int nValue;
	MyBaseUtils::GetVar(_T("USERI3"), &nValue);
	if (nValue == 1)
	{
	//删除原有的信息
		EraseAllGroup();
		MyBaseUtils::SetVar(_T("USERI3"), 0);
		return true;
	}

	//根据类别获取图框的角点
	vector<CString> tmpVec;
	CString strName;
	for (map<CString, vector<CString> >::iterator iter = m_MapName.begin(); iter != m_MapName.end(); ++iter)
	{
		strName = iter->first;
		tmpVec = iter->second;
		if (strName.IsEmpty())
		{
			for (vector<CString>::iterator itr = tmpVec.begin(); itr != tmpVec.end(); ++itr)
			{
				strName = *itr;
				doLayerNameInfo(strName);
			}
		}
		else
		{
			strName += _T("(*");
			doLayerNameInfo(strName);
		}
	}
	MyBaseUtils::SetVar(_T("USERI3"), 1);
	return true;
}

void CDrawOutLine::getLayerPoints(CString strName)
{
	ads_name ssname;
	resbuf* filter = acutBuildList(-4, _T("<and"), RTDXF0, _T("INSERT"), 2, _T("A*"), 8, strName, -4, _T("and>"), RTNONE);
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbEntity* pEnt = NULL;
	ads_name ename;
	AcGePoint3d pt;
	vector<AcGePoint3d> tmpVec;

	double dVer = CBaseUtils::verFrameSpace();
	double dHor = CBaseUtils::horFrameSpace();
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
			pt = pBlkRef->position();
			pBlkRef->close();

			pt.x += dHor/3;
			pt.y -= dVer/3;
			tmpVec.clear();
			tmpVec.push_back(pt);
			pair<map<double, vector<AcGePoint3d> >::iterator, bool> bRet = m_pointsMap.insert(make_pair(pt.y, tmpVec));
			if (!bRet.second)
			{
				bRet.first->second.push_back(pt);
			}
		}
		else
		{
			pEnt->close();
		}
	}

	acedSSFree(ssname);
}

void CDrawOutLine::doLayerNameInfo(CString strName)
{
	m_pointsMap.clear();
	getLayerPoints(strName);

	if (m_pointsMap.size() > 0)
	{	
		int nFind = strName.Find(_T("*"));
		if (nFind > 0)
		{
			strName = strName.Left(nFind);
		}

		m_idArrs.removeAll();
		double dVer = CBaseUtils::verFrameSpace();
		double dHor = CBaseUtils::horFrameSpace();
		double dFrameLength,dFrameHeight, dTemp;	
		AcGePoint3d startPt, endPt;

		//移动insertPt;
		CUtility::getIniFromFile(_T("A0"), dFrameLength, dFrameHeight, dTemp);
		vector<AcGePoint3d> tmpVec;
		AcGePoint3d tmpPt;
		for (map<double, vector<AcGePoint3d> >::iterator iter = m_pointsMap.begin(); iter != m_pointsMap.end(); ++iter)
		{
			tmpVec = iter->second;
			if (tmpVec.size() == 1)//如果只有一个
			{
				endPt = tmpVec.at(0);
				tmpPt = endPt;
			}
			else
			{
				std::sort(tmpVec.begin(), tmpVec.end(), CMPPoint);
				endPt = *tmpVec.rbegin();
				tmpPt = *tmpVec.begin();
			}
			
			startPt.x = tmpPt.x - dFrameLength - 2*dHor/3;
			startPt.y = tmpPt.y + dFrameHeight + 2*dVer/3;
			drawPlineByTwoPoints(startPt, endPt);
		}
		m_nColor++;

		MyDrawEntity::MakeGroup(m_idArrs, true, strName);
	}
}

void CDrawOutLine::drawPlineByTwoPoints(AcGePoint3d startPt, AcGePoint3d endPt)
{
	AcDbObjectId layerId = MySymble::CreateNewLayer(_T("外框"), 7);
	CString strLineType = gGlobal.GetIniValue(_T("虚线框设置"),_T("外部虚线框线型"));

	AcDbObjectId objId = AcDbObjectId::kNull;
	AcGePoint3d tmpPt1,tmpPt2;
	tmpPt1.x = startPt.x;
	tmpPt1.y = endPt.y;
	tmpPt1.z = tmpPt2.z = 0;

	tmpPt2.x = endPt.x;
	tmpPt2.y = startPt.y;

	AcDbPolyline* pLine = new AcDbPolyline;
	pLine->setDatabaseDefaults();
	pLine->addVertexAt(0, tmpPt1.convert2d(AcGePlane::kXYPlane));
	pLine->addVertexAt(1, startPt.convert2d(AcGePlane::kXYPlane));
	pLine->addVertexAt(2, tmpPt2.convert2d(AcGePlane::kXYPlane));
	pLine->addVertexAt(3, endPt.convert2d(AcGePlane::kXYPlane));
	pLine->setClosed(true);
	pLine->setLayer(layerId);
	pLine->setColorIndex(m_nColor);
	if (!strLineType.IsEmpty())
	{
		pLine->setLinetype(strLineType);
	}

	MyBaseUtils::addToCurrentSpaceAndClose(pLine);
	objId = pLine->objectId();

	m_idArrs.append(objId);
}

void CDrawOutLine::EraseAllGroup()
{
	vector<CString> tmpVec;
	CString strName;
	for (map<CString, vector<CString> >::iterator iter = m_MapName.begin(); iter != m_MapName.end(); ++iter)
	{
		strName = iter->first;
		tmpVec = iter->second;
		if (strName.IsEmpty())
		{
			for (vector<CString>::iterator itr = tmpVec.begin(); itr != tmpVec.end(); ++itr)
			{
				strName = *itr;
				MyEditEntity::EraseEntByGroupName(strName);
			}
		}
		else
		{
			strName += _T("(");
			MyEditEntity::EraseEntByGroupName(strName);
		}
	}
}
