#include "StdAfx.h"
#include "CDrawDotted.h"
#include <algorithm>
#include "Utility.h"
#include "BaseUtils.h"
#include "DlgProress.h"
#include "CGlobalRWriteFile.h"

extern CDlgProress* modelessDlg;
extern Adesk::Boolean startDlg();
extern Adesk::Boolean endDlg();
extern CGlobalRWriteFile gRWriteFile;

CDotPoint::CDotPoint(void)
{
	
}

CDotPoint::~CDotPoint(void)
{

}

AcGePoint3d CDotPoint::minPt() const
{
	return m_minPoint;
}

AcGePoint3d CDotPoint::maxPt() const
{
	return m_maxPoint;
}

AcGePoint3d CDotPoint::LeftUpPoint() const
{
	AcGePoint3d upPt;
	upPt.x = m_minPoint.x;
	upPt.y = m_maxPoint.y;
	upPt.z = 0;
	return upPt;
}

AcGePoint3d CDotPoint::RightDownPoint() const
{
	AcGePoint3d downPt;
	downPt.x = m_maxPoint.x;
	downPt.y = m_minPoint.y;
	downPt.z = 0;
	return downPt;
}

void CDotPoint::setMinPt(AcGePoint3d pt)
{
	m_minPoint = pt;
}

void CDotPoint::setMaxPt(AcGePoint3d pt)
{
	m_maxPoint = pt;
}

void CDotPoint::setExtents(AcDbExtents exts)
{
	AcGePoint3d minPt = exts.minPoint();
	AcGePoint3d maxPt = exts.maxPoint();
	m_minPoint.x = exts.minPoint().x;
	m_minPoint.y = exts.maxPoint().y;
	m_minPoint.z = 0;

	m_maxPoint = exts.minPoint();
}

CDotPoint& CDotPoint::operator=(const CDotPoint& node)
{
	m_minPoint = node.minPt();
	m_maxPoint = node.maxPt();
	return *this;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CDrawDotted::CDrawDotted(void)
{
	m_data.clear();
	m_Info.clear();
	m_LayerVec.clear();
	m_AInfo.clear();
	m_ptVec.clear();
	m_MBFrame.clear();
}

CDrawDotted::~CDrawDotted(void)
{
	m_data.clear();
	m_Info.clear();
	m_LayerVec.clear();
	m_AInfo.clear();
	m_ptVec.clear();
	m_MBFrame.clear();
}

bool CDrawDotted::doIt()
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	ads_name ename;
	AcGePoint3d pt;
	int nRet = acedEntSel(_T("\n请选择一个实体..."), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return false;
	}
	acdbGetObjectId(objId, ename);

	CString strGroupName;

	AcDbObjectIdArray objIdArr = MyEditEntity::openObjAndGetGroupIds(objId);
	if (objIdArr.length() < 1)
	{
		return false;
	}
	AcDbObjectId groupId;
	groupId = objIdArr.at(0);
	AcDbGroup* pGroup = NULL;
	if (acdbOpenAcDbObject((AcDbObject*&)pGroup, groupId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	strGroupName = pGroup->name();
	pGroup->close();
	
	//ZW_DICT_ICD_DOT_
	/*int nLen = strGroupName.Find(_T("ZW_DICT_ICD_"));
	if (nLen < 0)
	{
	AfxMessageBox(_T("该实体不能做虚线框处理，没有虚线框信息"));
	return false;
	}

	strGroupName.Replace(_T("ZW_DICT_ICD_"), _T("ZW_DICT_ICD_DOT_"));*/
	strGroupName += _T("DOT");
	MyEditEntity::EraseEntByGroupName(strGroupName);

	m_strGroupName = strGroupName;

	AcDbObjectIdArray entIdArr;
	entIdArr.removeAll();
	entIdArr = MyEditEntity::openGroupIdsAndGetEntIds(objIdArr);
	getData(entIdArr);

	if (m_idArrs.length() >= 1)
	{
		MyDrawEntity::MakeGroup(m_idArrs, false, m_strGroupName);
	}
	return true;
}

bool CDrawDotted::getData(AcDbObjectIdArray objIdArr)
{
	AcDbObjectId entId = AcDbObjectId::kNull;
	AcDbEntity* pEnt = NULL;
	CString strBlockName;
	for (int i=0; i<objIdArr.length(); i++)
	{
		entId = objIdArr.at(i);
		if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
			AcDbExtents exts;
			pBlkRef->getGeomExtents(exts);
			pBlkRef->close();

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			
			AcGePoint3d minPt,maxPt;
			maxPt = exts.maxPoint();
			minPt = exts.minPoint();
			
			acutPolar(asDblArray(minPt), 5*PI/4, 1, asDblArray(minPt));
			acutPolar(asDblArray(maxPt), PI/4, 1, asDblArray(maxPt));
			MyBaseUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
			if (strBlockName.Find(_T("RF")) >= 0)
			{		
				maxPt.y += 4;
			}
			CDotPoint doPt;

			doPt.setMinPt(minPt);
			doPt.setMaxPt(maxPt);

			int nRow = 0;
			int nCol=0;
			int nCount = CUtility::getRowAndColByPt(minPt, nRow, nCol);

			vector<CDotPoint> dotVec;
			dotVec.push_back(doPt);
			pair<map<int, vector<CDotPoint> >::iterator, bool> bRet = m_Info.insert(make_pair(nCount, dotVec));
			if (!bRet.second)
			{
				bRet.first->second.push_back(doPt);
			}
			dotVec.clear();
			/*pair<map<double, vector<CDotPoint> >::iterator, bool> bol =  m_data.insert(make_pair(minPt.x, dotVec));
			if (!bol.second)
			{
				bol.first->second.push_back(doPt);
			}*/
		}
		else
		{
			pEnt->close();
		}
	}
	for (map<int, vector<CDotPoint> >::iterator iter = m_Info.begin(); iter != m_Info.end(); ++iter)
	{
		vector<CDotPoint> tmpVec;
		tmpVec.clear();
		tmpVec = iter->second;
		map<pair<double, int>, vector<CDotPoint> > tmpData;

		for (vector<CDotPoint>::iterator itr = tmpVec.begin(); itr != tmpVec.end(); ++itr)
		{
			CDotPoint tmpDot;
			tmpDot = *itr;
			vector<CDotPoint> dotVec;
			dotVec.push_back(tmpDot);
			pair<double, int> tmpPair;
			tmpPair.first = tmpDot.minPt().x;
			tmpPair.second = iter->first;

			pair<map<pair<double, int>, vector<CDotPoint> >::iterator, bool> bol =  tmpData.insert(make_pair(tmpPair, dotVec));
			if (!bol.second)
			{
				bol.first->second.push_back(tmpDot);
			}
			dotVec.clear();
		}
		m_data.push_back(tmpData);
		tmpData.clear();
		tmpVec.clear();
	}
	doData();

	return true;
}
bool ComPare(const CDotPoint& Pt1, const CDotPoint& Pt2)
{
	return Pt1.minPt().y > Pt2.minPt().y;
}
bool CDrawDotted::doData(int nType)
{
	vector<CDotPoint> tmpVec;
	CString strLineType = gGlobal.GetIniValue(_T("虚线框设置"), _T("内部虚线框线型"));
	AcGePoint3d preleftUpPt,preLeftDownPt,curLeftUpPt,curLeftDownPt, preRightDownPt, curRightDownPt;
	AcDbObjectId layerId = MySymble::CreateNewLayer(_T("虚线"), 1, FALSE, strLineType);
	m_idArrs.removeAll();
	AcDbObjectId objId = AcDbObjectId::kNull;

	double dLeftVerGap = CBaseUtils::leftVerGap();
	double dLeftHorGap = CBaseUtils::leftHorGap();
	bool bHasOnlyFrame = true;	//是否只有一个虚线框
	bool bIsLastFrame = true;//是否最后一个虚线框
  	int nSize = m_data.size();
	
	double dStartX,dEndX;
	int j=1;
	AcDbObjectId textId = AcDbObjectId::kNull;
	AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("0"));

	for (vector<map<pair<double, int>, vector<CDotPoint> > >::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
	{
		map<pair<double, int>, vector<CDotPoint> > tmpData;
		tmpData = *iter;	
		AcGePoint3dArray upPoints,downPoints, points;
		points.removeAll();
		upPoints.removeAll();
		downPoints.removeAll();	
		int i=1;
		
		bool bFirst = true;
		bool bDrawDouble = false;
		bool bIsCirculated = false;
		for (map<pair<double, int>, vector<CDotPoint> >::iterator iter = tmpData.begin();
			iter != tmpData.end(); ++iter)
		{
			tmpVec = iter->second;
			int nFrameCount = iter->first.second;
			getInsertPt(nFrameCount);
			dStartX = startXPos(nFrameCount);
			dEndX = endXPos(nFrameCount);

			sort(tmpVec.begin(), tmpVec.end(), ComPare);

			CDotPoint startDot,endDot;
			startDot = *tmpVec.begin();
			endDot = *tmpVec.rbegin();

			if (i>1)
			{
				preleftUpPt = curLeftUpPt;
				preLeftDownPt = curLeftDownPt;
				preRightDownPt = curRightDownPt;
			}
			curLeftUpPt = startDot.LeftUpPoint();
			curLeftDownPt = endDot.minPt();
			curRightDownPt = endDot.RightDownPoint();
			AcGePoint3d midPt;
			if (i==1)
			{
				if (tmpData.size() == 1)
				{
					AcGePoint3d pt1,pt2,pt3,pt4;
					pt1 = startDot.LeftUpPoint();
					pt2 = startDot.maxPt();
					pt3 = endDot.minPt();
					pt4 = endDot.RightDownPoint();
					if (j == 1)
					{			
						m_startPt = pt1;	
						pt1.x -= 16;
						pt3.x -= 16;
						if (nSize == 1)
						{
							points.append(pt1);
							points.append(pt2);
							points.append(pt4);
							points.append(pt3);
							points.append(pt1);
						}
						else 
						{
							pt2.x = dEndX;
							pt4.x = dEndX;
							points.append(pt2);
							points.append(pt1);
							points.append(pt3);
							points.append(pt4);
							textId = MyDrawEntity::DrawText(pt4, _T("接下页"), 3.0, textStyleId, AcDb::kTextRight);
							m_idArrs.append(textId);
						}
					}
					else
					{
						if (j > 1)
						{
							//pt1.x = dStartX;
							//pt3.x = dStartX;
							////在此处绘制接上页文字
							//textId = MyDrawEntity::DrawText(pt3, _T("接上页"), 3.0, textStyleId, AcDb::kTextLeft);
							//m_idArrs.append(textId);
							//points.append(pt1);
							//points.append(pt2);
							//points.append(pt4);
							//points.append(pt3);
							double dTemp = curLeftDownPt.x - dStartX;
							if (dTemp < dLeftHorGap - 25)
							{
								curLeftDownPt.x = dStartX;
								curLeftUpPt.x = dStartX;
							}
							else
							{
								bIsCirculated = true;
							}

							if (bIsCirculated)
							{	

							}
							else
							{
								textId = MyDrawEntity::DrawText(curLeftDownPt, _T("接上页"), 3.0, textStyleId, AcDb::kTextLeft, AcDb::kTextTop);
								m_idArrs.append(textId);
							}
						}
					}
					
					break;
				}
				else
				{
					curLeftUpPt.x -= 15;
					curLeftDownPt.x -= 15;
					m_startPt = curLeftUpPt;
					if (j > 1)
					{
						curLeftUpPt.x = dStartX;
						curLeftDownPt.x = dStartX;
						//在此处绘制接上页文字
						textId = MyDrawEntity::DrawText(curLeftDownPt, _T("接上页"), 3.0, textStyleId, AcDb::kTextLeft);
						m_idArrs.append(textId);
					}
					
					upPoints.append(curLeftUpPt);
					downPoints.append(curLeftDownPt);
				}
			}

			if (i > 1)
			{
				curLeftUpPt.x -= 15;
				curLeftDownPt.x -= 15;
				
				if (preleftUpPt.y > curLeftUpPt.y)
				{
					curLeftUpPt.y = preleftUpPt.y;
					upPoints.append(curLeftUpPt);
					
				}
				else
				{
					midPt.x = curLeftUpPt.x;
					midPt.y = preleftUpPt.y;
					midPt.z = 0;
					upPoints.append(midPt);
					upPoints.append(curLeftUpPt);
				}

				if (preLeftDownPt.y < curLeftDownPt.y )
				{
					if ((i == tmpData.size()) && (j==nSize))//只有最后一个才需要拐弯
					{
						midPt.x = preRightDownPt.x;
						midPt.y = curLeftDownPt.y;
						midPt.z  = 0;

						curLeftDownPt.y = preLeftDownPt.y;
						curLeftDownPt.x = preRightDownPt.x;
						downPoints.append(curLeftDownPt);
						downPoints.append(midPt);
					}
					else
					{
						curLeftDownPt.y = preLeftDownPt.y;
						downPoints.append(curLeftDownPt);
					}
				}
				else
				{
					midPt.x = curLeftDownPt.x;
					midPt.y = preLeftDownPt.y;
					midPt.z = 0;
					downPoints.append(midPt);
					downPoints.append(curLeftDownPt);
				}

				if (i == tmpData.size())
				{
					AcGePoint3d pt1,pt2,pt3,pt4;
					pt2 = startDot.maxPt();
					pt3 = endDot.minPt();
					pt4 = endDot.RightDownPoint();
					
					if ((j == 1) && (nSize == j))
					{
						if (preleftUpPt.y > pt2.y )
						{
							pt2.y = preleftUpPt.y;
						}
						upPoints.append(pt2);
						downPoints.append(pt4);
						downPoints.reverse();

						points.append(upPoints);
						points.append(downPoints);
						points.append(upPoints.at(0));
					}
					else if (j == 1)
					{
						pt2 = upPoints.last();
						pt4 = downPoints.last();
						if (nSize > 1)
						{
							pt2.x = dEndX;
							pt4.x = dEndX;
						}
						//在此处绘制接下一页文字
						textId = MyDrawEntity::DrawText(pt4, _T("接下页"), 3.0, textStyleId, AcDb::kTextRight);
						m_idArrs.append(textId);
						upPoints.append(pt2);
						downPoints.append(pt4);
						downPoints.reverse();

						points.append(downPoints);
						points.append(upPoints);
					}
					else if (j == nSize)
					{
						if (preleftUpPt.y > pt2.y )
						{
							pt2.y = preleftUpPt.y;
						}
						upPoints.append(pt2);
						downPoints.append(pt4);
						downPoints.reverse();

						points.append(upPoints);
						points.append(downPoints);
					}
					else
					{
						pt2 = upPoints.last();
						pt4 = downPoints.last();
						if (nSize > 1)
						{
							pt2.x = dEndX;
							pt4.x = dEndX;
						}
						//在此处绘制接下一页文字
						textId = MyDrawEntity::DrawText(pt4, _T("接下页"), 3.0, textStyleId, AcDb::kTextRight);
						m_idArrs.append(textId);
						bDrawDouble = true;
						upPoints.append(pt2);
						downPoints.append(pt4);
					}
				}
			}
			i++;
			tmpVec.clear();
		}	
		tmpData.clear();
		if (bDrawDouble)
		{
			objId = MyDrawEntity::DrawPlineByPoints(upPoints, layerId);

			m_idArrs.append(objId);
			objId = MyDrawEntity::DrawPlineByPoints(downPoints, layerId);

			m_idArrs.append(objId);
			upPoints.removeAll();
			downPoints.removeAll();
		}
		else
		{
			objId = MyDrawEntity::DrawPlineByPoints(points, layerId);

			m_idArrs.append(objId);
			points.removeAll();
		}
		
		j++;
		tmpData.clear();
	}


	return true;
}

bool CDrawDotted::doMBData()
{
	CString strType = gGlobal.GetIniValue(_T("虚线框设置"), _T("母版虚线框线型"));
	AcDbObjectId layerId = MySymble::CreateNewLayer(_T("母板虚线"), 1, FALSE, strType);
	m_idArrs.removeAll();
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("0"));
	AcGePoint3dArray points;
	
	for (vector<int>::iterator iter = m_MBFrame.begin(); iter != m_MBFrame.end(); ++iter)
	{
		int nFrameCount = *iter;
		getInsertPt(nFrameCount);
		
		if (!selEntForMb())
		{
			continue;
		}
		
		points.removeAll();
		points = getPointsArray();
		objId = MyDrawEntity::DrawPlineByPoints(points, layerId);
		//objId = MyEditEntity::openPlineChangeWidth(objId, 0.6);
		m_idArrs.append(objId);
		points.removeAll();
	}
	return true;
}

bool CDrawDotted::doTcData()
{
	vector<CDotPoint> tmpVec;
	AcGePoint3d preleftUpPt,preLeftDownPt,curLeftUpPt,curLeftDownPt, preRightDownPt, curRightDownPt;
	CString strType = gGlobal.GetIniValue(_T("虚线框设置"), _T("模块虚线框线型"));
	AcDbObjectId layerId = MySymble::CreateNewLayer(_T("图层虚线"), 1, FALSE, strType);
	m_idArrs.removeAll();
	AcDbObjectId objId = AcDbObjectId::kNull;
	bool bHasOnlyFrame = true;	//是否只有一个虚线框
	bool bIsLastFrame = true;//是否最后一个虚线框
	int nSize = m_data.size();
	if (nSize > 1)
	{
		bHasOnlyFrame = false;
	}

	int j=1;
	AcDbObjectId textId = AcDbObjectId::kNull;
	AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("0"));
	for (vector<map<pair<double, int>, vector<CDotPoint> > >::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
	{
		map<pair<double, int>, vector<CDotPoint> > tmpData;
		tmpData = *iter;	
		AcGePoint3dArray upPoints,downPoints, points;
		points.removeAll();
		upPoints.removeAll();
		downPoints.removeAll();	
		int i=1;

		bool bFirst = true;
		bool bDrawDouble = false;
		bool bIsCirculated = false;
		for (map<pair<double, int>, vector<CDotPoint> >::iterator iter = tmpData.begin();
			iter != tmpData.end(); ++iter)
		{
			tmpVec = iter->second;
			int nFrameCount = iter->first.second;
			getInsertPt(nFrameCount);
			sort(tmpVec.begin(), tmpVec.end(), ComPare);

			CDotPoint startDot,endDot;
			startDot = *tmpVec.begin();
			endDot = *tmpVec.rbegin();

			if (i>1)
			{
				preleftUpPt = curLeftUpPt;
				preLeftDownPt = curLeftDownPt;
				preRightDownPt = curRightDownPt;
			}
			curLeftUpPt = startDot.LeftUpPoint();
			curLeftDownPt = endDot.minPt();
			curRightDownPt = endDot.RightDownPoint();
			AcGePoint3d midPt;
			if (i==1)
			{
				if (tmpData.size() == 1)
				{
					AcGePoint3d pt1,pt2,pt3,pt4;
					pt1 = startDot.LeftUpPoint();
					pt2 = startDot.maxPt();
					pt3 = endDot.minPt();
					pt4 = endDot.RightDownPoint();
					double dDist = m_maxPt.y- curLeftUpPt.y;
					if (dDist - dLeftVerGap < GeTol)
					{

					}
					else
					{
						dDist = m_maxPt.y- curLeftUpPt.y;
						if (dDist > dLeftVerGap + 10)
						{
							pt1.y += 11;
							pt2.y += 11;
						}
						else
						{
							pt1.y += 4;
							pt2.y += 4;
						}
						
					}
					pt1.x -= 17;
					pt3.x -= 17;
					if (j == 1)
					{
						m_startPt = pt1;
						if (nSize == 1)
						{
							points.append(pt1);
							points.append(pt2);
							points.append(pt4);
							points.append(pt3);
							points.append(pt1);
						}
						else
						{
							points.append(pt2);
							points.append(pt1);
							points.append(pt3);
							points.append(pt4);
							m_idArrs.append(textId);
						}
					}
					else if (j > 1)
					{
						m_idArrs.append(textId);
						points.append(pt1);
						points.append(pt2);
						points.append(pt4);
						points.append(pt3);
					}
					break;	
				}
				else
				{
					curLeftUpPt.x -= 16;
					curLeftDownPt.x -= 16;
						
					if (bFirst)
					{
						double dDist = m_maxPt.y- curLeftUpPt.y;
						if (dDist - dLeftVerGap < GeTol)
						{

						}
						else
						{
							dDist = m_maxPt.y- curLeftUpPt.y;
							if (dDist > dLeftVerGap + 10)
							{
								curLeftUpPt.y += 11;
							}
							else
							{
								curLeftUpPt.y += 4;
							}
						}
						bFirst = false;
						if (j == 1)
						{
							m_startPt = curLeftUpPt;
						}
					}
						
					upPoints.append(curLeftUpPt);
					downPoints.append(curLeftDownPt);
				}
			}
			if (i > 1)
			{
				curLeftUpPt.x -= 16;
				curLeftDownPt.x -= 16;
				
				if (preleftUpPt.y > curLeftUpPt.y)
				{
					curLeftUpPt.y = preleftUpPt.y;
					upPoints.append(curLeftUpPt);
				}
				else
				{
					midPt.x = curLeftUpPt.x;
					midPt.y = preleftUpPt.y;
					midPt.z = 0;
					upPoints.append(midPt);
					upPoints.append(curLeftUpPt);
				}

				if ((preLeftDownPt.y < curLeftDownPt.y))
				{
					if ((i == tmpData.size()) && (j==nSize))
					{
						curLeftDownPt.x += 3;
						
						midPt.x = preRightDownPt.x;
						midPt.y = curLeftDownPt.y;
						midPt.z  = 0;

						curLeftDownPt.y = preLeftDownPt.y;
						curLeftDownPt.x = preRightDownPt.x;
						downPoints.append(curLeftDownPt);
						downPoints.append(midPt);
					}
					else
					{
						curLeftDownPt.y = preLeftDownPt.y;
						downPoints.append(curLeftDownPt);
					}
				}
				else
				{
					midPt.x = curLeftDownPt.x;
					midPt.y = preLeftDownPt.y;
					midPt.z = 0;
					downPoints.append(midPt);
					downPoints.append(curLeftDownPt);
				}

				if (i == tmpData.size())
				{
					AcGePoint3d pt1,pt2,pt3,pt4;
					pt2 = startDot.maxPt();
					pt3 = endDot.minPt();
					pt4 = endDot.RightDownPoint();
					
					if ((j == 1) && (nSize == j))
					{
						if (preleftUpPt.y > pt2.y )
						{
							pt2.y = preleftUpPt.y;
						}
						upPoints.append(pt2);
						downPoints.append(pt4);
					}
					else if (j == 1)
					{
						m_idArrs.append(textId);
						upPoints.append(pt2);
						downPoints.append(pt4);
					}
					else if (j == nSize)
					{
						if (preleftUpPt.y > pt2.y )
						{
							pt2.y = preleftUpPt.y;
						}
						upPoints.append(pt2);
						downPoints.append(pt4);
					}
					else
					{
	
						m_idArrs.append(textId);
						upPoints.append(pt2);
					}
				}
			}
			i++;
			tmpVec.clear();
		}
		tmpData.clear();
		AcDbObjectId PlineId = doPointsAndDrawPline(downPoints, upPoints);
		j++;
	}
	return true;
}

void CDrawDotted::drawlineByPt(AcGePoint3d leftUpPt, AcGePoint3d RightUpPt, AcDbObjectId layerId)
{
	AcDbObjectId entId = AcDbObjectId::kNull;
	if (abs(leftUpPt.y  - RightUpPt.y) < GeTol)
	{
		entId = MyDrawEntity::DrawLine(leftUpPt, RightUpPt, layerId);
		m_idArrs.append(entId);
	}
	else
	{
		AcGePoint3d midPt;
		midPt.x = RightUpPt.x;
		midPt.y = leftUpPt.y;
		midPt.z = 0;
		entId = MyDrawEntity::DrawLine(leftUpPt, midPt, layerId);
		m_idArrs.append(entId);

		entId = MyDrawEntity::DrawLine(midPt, RightUpPt, layerId);
		m_idArrs.append(entId);		
	}
}

void CDrawDotted::setGroupName(CString strGroupName)
{
	m_strGroupName = strGroupName;
}

void CDrawDotted::makeGroup()
{
	if (m_idArrs.length() >= 1)
	{
		AcDbDictionary *pGroupDict;	
		AcDbGroup* pGroup = NULL;
		AcDbObjectId groupId;
		AcDbObjectId entId = AcDbObjectId::kNull;
		acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
		if (pGroupDict->getAt(m_strGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
		{
			pGroup = new AcDbGroup;
			pGroupDict->setAt(m_strGroupName, pGroup, groupId);
			pGroupDict->close();
			for (int i = 0; i < m_idArrs.length(); i++) 
			{
				entId = m_idArrs.at(i);
				pGroup->append(entId);
			}
			pGroup->close();
		}
		else
		{
			for (int i = 0; i < m_idArrs.length(); i++) 
			{
				entId = m_idArrs.at(i);
				pGroup->append(entId);
			}
			pGroup->close();
			pGroupDict->close();
		}
	}
}

AcDbObjectIdArray CDrawDotted::getIdArr()
{
	return m_idArrs;
}

bool CDrawDotted::doLayer()
{
	startDlg();
	m_ptVec.clear();
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("生成模块虚线框"));
		modelessDlg->setPos(0);
	}
	if (!getAllLayer())
	{
		return false;
	}
	CString strLayerName;
	int nSize = m_LayerVec.size();
	int nCount = 0;
	for (vector<CString>::iterator iter = m_LayerVec.begin(); iter != m_LayerVec.end(); ++iter)
	{
		strLayerName = *iter;
		m_idArrs.removeAll();
		m_Info.clear();
		m_data.clear();
		CString strTemp;
		strTemp = strLayerName;
		strTemp.Replace(_T("("), _T(""));
		strTemp.Replace(_T(")"), _T(""));
		
		//如果模块文字中包含母版信息，删除掉
		CString strType = gGlobal.GetIniValue(_T("基础设置"), _T("母板"));
		CStringArray strArr;
		strArr.RemoveAll();
		MyParserString::GetCommaFromString(strType, strArr);

		for (int i=0; i<strArr.GetCount(); i++)
		{
			strType = strArr.GetAt(i);
			int nFind = strTemp.Find(strType);
			if (nFind >= 0)
			{
				strTemp.Replace(strType, _T(""));
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		MyEditEntity::EraseEntByGroupName(strTemp);

		resbuf* filter = acutBuildList(-4,_T("<or"), -4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("ZW_TABLE"), 8,  strLayerName, -4,_T("and>"), 
											-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF1"), 8,  strLayerName, -4,_T("and>"), 
											-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF2"), 8,  strLayerName, -4,_T("and>"), 
											-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF5"), 8,  strLayerName, -4,_T("and>"),-4,_T("or>"), RTNONE);
		//resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("ZW_TABLE"), 8,  strLayerName, -4,_T("and>"),RTNONE);
		ads_name ssname;
		int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
		acutRelRb(filter);
		if (nRet != RTNORM)
		{
			continue;
		}
		AcDbObjectId entId = AcDbObjectId::kNull;
		AcDbEntity* pEnt = NULL;
		long sslen;
		acedSSLength(ssname, &sslen);
		ads_name ename;
		for (int i=0; i<sslen; i++)
		{
			acedSSName(ssname, i, ename);
			acdbGetObjectId(entId, ename);
			if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForRead) != Acad::eOk)
			{
				continue;
			}
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
				AcDbExtents exts;
				pBlkRef->getGeomExtents(exts);
				pBlkRef->close();

				//////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////
				AcGePoint3d minPt,maxPt;
				maxPt = exts.maxPoint();
				minPt = exts.minPoint();

				CString strBlockName;
				MyBaseUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
				if (strBlockName.Find(_T("RF5")) >= 0)
				{
					minPt.x += 80;
					maxPt.x += 70;
				}
				else
				{
					acutPolar(asDblArray(minPt), 5*PI/4, 2, asDblArray(minPt));
					acutPolar(asDblArray(maxPt), PI/4, 2, asDblArray(maxPt));
				}
				//maxPt.y += 7;
				
				CDotPoint doPt;

				doPt.setMinPt(minPt);
				doPt.setMaxPt(maxPt);

				int nRow = 0;
				int nCol = 0;
				int nFrameCount = CUtility::getRowAndColByPt(minPt, nRow, nCol);

				vector<CDotPoint> dotVec;
				dotVec.push_back(doPt);
				pair<map<int, vector<CDotPoint> >::iterator, bool> bRet = m_Info.insert(make_pair(nFrameCount, dotVec));
				if (!bRet.second)
				{
					bRet.first->second.push_back(doPt);
				}
				dotVec.clear();
			}
			else
			{
				pEnt->close();
			}
		}
		acedSSFree(ssname);
		for (map<int, vector<CDotPoint> >::iterator iter = m_Info.begin(); iter != m_Info.end(); ++iter)
		{
			vector<CDotPoint> tmpVec;
			tmpVec.clear();
			tmpVec = iter->second;
			map<pair<double, int>, vector<CDotPoint> > tmpData;

			for (vector<CDotPoint>::iterator itr = tmpVec.begin(); itr != tmpVec.end(); ++itr)
			{
				CDotPoint tmpDot;
				tmpDot = *itr;
				vector<CDotPoint> dotVec;
				dotVec.push_back(tmpDot);
				pair<double, int> tmpPair;
				tmpPair.first = tmpDot.minPt().x;
				tmpPair.second = iter->first;

				pair<map<pair<double, int>, vector<CDotPoint> >::iterator, bool> bol =  tmpData.insert(make_pair(tmpPair, dotVec));
				if (!bol.second)
				{
					bol.first->second.push_back(tmpDot);
				}
				dotVec.clear();
			}
			m_data.push_back(tmpData);
			tmpData.clear();
			tmpVec.clear();
		}
		doTcData();
		m_startPt.y += 1;
		AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("3"));
		for (vector<AcGePoint3d>::iterator itr = m_ptVec.begin(); itr  != m_ptVec.end(); ++itr)
		{
			m_startPt = *itr;
			m_startPt.y += 1;
			AcDbObjectId textId = MyDrawEntity::DrawText(m_startPt, strTemp, 3.0, textStyleId);
			m_idArrs.append(textId);
		}
		m_ptVec.clear();
		MyDrawEntity::MakeGroup(m_idArrs, false, strTemp);
		m_idArrs.removeAll();
		nCount++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(nCount*99/nSize);
		}
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(100);
	}
	endDlg();
	return true;
}

bool CDrawDotted::doAlayer()
{
	CUtility::ZoomExtent();
	startDlg();
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("生成母板虚线框"));
		modelessDlg->setPos(0);
	}
	if (!getAllExtLayer())
	{
		return false;
	}
	CString strLayerName, strTmp, strText;
	int nSize = m_AInfo.size();
	int nCount = 0;
	for (map<CString, int>::iterator iter = m_AInfo.begin(); iter != m_AInfo.end(); ++iter)
	{
		strLayerName = iter->first;
		strText = strLayerName;
		strText = strText.Left(strText.GetLength()-1);
		m_idArrs.removeAll();
		m_Info.clear();
		m_data.clear();
		m_MBFrame.clear();
		m_ptVec.clear();
		MyEditEntity::EraseEntByGroupName(strText);
		strTmp = strLayerName + _T("*");
		/*resbuf* filter = acutBuildList(-4,_T("<or"), -4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("ZW_TABLE"), 8,  strTmp, -4,_T("and>"), 
			-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF1"), 8,  strTmp, -4,_T("and>"), 
			-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF2"), 8,  strTmp, -4,_T("and>"), -4,_T("or>"), RTNONE);*/
		resbuf* filter = acutBuildList(-4, _T("<and"), RTDXF0, _T("INSERT"), 2, _T("A*"), 8, strTmp, -4, _T("and>"), RTNONE);
		ads_name ssname;
		int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
		acutRelRb(filter);
		if (nRet != RTNORM)
		{
			continue;
		}
		AcDbObjectId entId = AcDbObjectId::kNull;
		AcDbEntity* pEnt = NULL;
		long sslen;
		acedSSLength(ssname, &sslen);
		ads_name ename;
		for (int i=0; i<sslen; i++)
		{
			acedSSName(ssname, i, ename);
			acdbGetObjectId(entId, ename);
			if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForRead) != Acad::eOk)
			{
				continue;
			}
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
				AcDbExtents exts;
				pBlkRef->getGeomExtents(exts);
				pBlkRef->close();

				int nFrameCount = MyEditEntity::GetObjIntXdata(entId, ICD_COUNT);
				m_MBFrame.push_back(nFrameCount);
			}
			else
			{
				pEnt->close();
			}
		}
		acedSSFree(ssname);
		
		doMBData();	
		AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("3"));

		for (vector<AcGePoint3d>::iterator itr = m_ptVec.begin(); itr  != m_ptVec.end(); ++itr)
		{
			m_startPt = *itr;
			m_startPt.y += 1;
			AcDbObjectId textId = MyDrawEntity::DrawText(m_startPt, strText, 3.0, textStyleId);
			m_idArrs.append(textId);
		}
		m_ptVec.clear();

		MyDrawEntity::MakeGroup(m_idArrs, false, strText);
		m_idArrs.removeAll();
		nCount++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(99*nCount/nSize);
		}
		
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(100);
	}
	endDlg();
	return true;
}

bool CDrawDotted::getAllLayer()
{

	CString strName;
	AcDbLayerTable* pTextTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()  ->getSymbolTable(pTextTbl, AcDb::kForRead); 

	//CString strType = gGlobal.GetIniValue(_T("基础设置"), _T("母板"));
	//CStringArray strArr;
	//strArr.RemoveAll();
	//MyParserString::GetCommaFromString(strType, strArr);
	//CString strTmp;

	AcDbLayerTableIterator* pTblIter = NULL;
	AcDbLayerTableRecord* pTextTblRec = NULL;
	pTextTbl->newIterator(pTblIter);
	for (; !pTblIter->done(); pTblIter->step())
	{
		const TCHAR* locName;
		pTblIter->getRecord(pTextTblRec, AcDb::kForRead);
		pTextTblRec->getName(locName);
		strName = locName;
		//int nFind = strName.Find(_T("("));
		//if (nFind > 0)
		//{
		//	strTmp = strName.Left(nFind + 1);
		//	strTmp = strTmp.Left(nFind);
		//	for (int i=0; i<strArr.GetSize(); i++)
		//	{
		//		CString strTemp = strArr.GetAt(i);
		//		if (strTemp.CompareNoCase(strTmp) == 0)
		//		{
		//			//m_AInfo.insert(make_pair(strName, 1));
		//			m_LayerVec.push_back(strName);
		//			break;
		//		}
		//	}
		//}
		int nFind = strName.Find(_T("("));
		if (nFind > 0)
		{
			m_LayerVec.push_back(strName);
		}

		pTextTblRec->close();
	}
	pTextTbl->close();
	delete pTblIter;
	return true;
}

bool CDrawDotted::getAllExtLayer()
{
	CString strName, strTmp;
	AcDbLayerTable* pTextTbl = NULL;
	acdbHostApplicationServices()->workingDatabase() ->getSymbolTable(pTextTbl, AcDb::kForRead); 

	CString strType = gGlobal.GetIniValue(_T("基础设置"), _T("母板"));
	CStringArray strArr;
	strArr.RemoveAll();
	MyParserString::GetCommaFromString(strType, strArr);

	
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
		if (nFind > 0)
		{
			strName = strName.Left(nFind + 1);
			strTmp = strName.Left(nFind);
			for (int i=0; i<strArr.GetSize(); i++)
			{
				CString strTemp = strArr.GetAt(i);
				if (strTemp.CompareNoCase(strTmp) == 0)
				{
					m_AInfo.insert(make_pair(strName, 1));
					break;
				}
			}
		}

		pTextTblRec->close();
	}
	pTextTbl->close();
	delete pTblIter;
	return true;
}


AcGePoint3d CDrawDotted::getInsertPt(int nCount)
{
	map<int, CFrameInfo> AllFrame = gRWriteFile.getAllFrameInfo();
	if (AllFrame.size() > 0)
	{
		map<int, CFrameInfo>::iterator iter = AllFrame.find(nCount);
		if (iter == AllFrame.end())
		{
			//没有数据，不用处理
		}
		else
		{
			//找到数据重新获取
			m_FrameInfo = iter->second;
		}
	}
	else
	{
		gRWriteFile.saveAllFrameInfo();
		map<int, CFrameInfo> AllFrame = gRWriteFile.getAllFrameInfo();
		map<int, CFrameInfo>::iterator iter = AllFrame.find(nCount);
		if (iter == AllFrame.end())
		{
			//没有数据，不用处理
		}
		else
		{
			//找到数据重新获取
			m_FrameInfo = iter->second;
		}
	}
	double m_dFrameHorSpace, m_dFrameVerSpace;
	m_InsertPt.set(0, 0, 0);
	m_dFrameHorSpace = CBaseUtils::horFrameSpace();
	m_dFrameVerSpace = CBaseUtils::verFrameSpace();
	int m_nHxCount = CBaseUtils::hxFrameCount();
	CString strFrameType = m_FrameInfo.getFrameType();
	double dTemp = 0.0;
	if (strFrameType.IsEmpty())
	{
		strFrameType = _T("A1");
	}
	CUtility::getIniFromFile(strFrameType, m_dFrameLength, m_dFrameHeight, dTemp);

	dLeftVerGap = m_FrameInfo.getLeftVerGap();
	dLeftHorGap = m_FrameInfo.getLeftHorGap();
	dSpaceHorGap = m_FrameInfo.getSpaceHorGap();
	dFrameHorGap = m_FrameInfo.getHorSpace();
	dSpaceVerGap = m_FrameInfo.getSpaceVerGap();
	if (abs(dSpaceVerGap - 297) < GeTol)
	{
		dSpaceVerGap = CBaseUtils::spaceVerGap();
	}
	double dFrameLength = 0.0;
	double dFrameHeight = 0.0;
	CUtility::getIniFromFile(_T("A0"), dFrameLength, dFrameHeight, dTemp);
	int nYCount = (nCount-1)/m_nHxCount;
	int nXCount = nCount%m_nHxCount;

	AcGePoint3d insertPt;
	insertPt.set(0, 0, 0);
	acutPolar(asDblArray(insertPt), 3*PI/2, (nYCount)*(m_dFrameVerSpace+dFrameHeight), asDblArray(insertPt));

	if (nXCount == 0)
	{
		nXCount = m_nHxCount;
	}
	acutPolar(asDblArray(insertPt), 0, (nXCount-1)*(m_dFrameHorSpace+dFrameLength), asDblArray(insertPt));

	m_InsertPt = insertPt;
	
	m_minPt = m_maxPt = insertPt;
	m_minPt.x = insertPt.x - m_dFrameLength;
	m_maxPt.y = insertPt.y + m_dFrameHeight;
	return m_InsertPt;
}

double CDrawDotted::startXPos(int nCount)
{
	double dXPos = m_InsertPt.x - m_dFrameLength + 25;
	return dXPos;
}

double CDrawDotted::endXPos(int nCount)
{
	double dXPos = m_InsertPt.x - 10;
	return dXPos;
}

double CDrawDotted::startYPos(int nCount)
{
	double dXPos = m_InsertPt.y;
	return dXPos;
}

double CDrawDotted::endYPos(int nCount)
{
	double dXPos = m_InsertPt.y;
	return dXPos;
}

AcGePoint3d CDrawDotted::GetLeftUpPt()
{
	AcGePoint3d tmpPt = m_InsertPt;
	tmpPt.x = m_minPt.x;
	tmpPt.y = m_maxPt.y;
	return tmpPt;
}

AcGePoint3d CDrawDotted::GetLeftDownPt()
{	
	AcGePoint3d tmpPt = m_InsertPt;
	tmpPt.x = m_minPt.x;
	tmpPt.y = m_minPt.y;
	return tmpPt;
}

AcGePoint3d CDrawDotted::GetRightUpPt()
{
	AcGePoint3d tmpPt = m_InsertPt;
	tmpPt = m_maxPt;
	return tmpPt;
}

AcGePoint3d CDrawDotted::GetRightDownPt()
{
	AcGePoint3d tmpPt = m_InsertPt;
	tmpPt = m_maxPt;
	tmpPt.y = m_minPt.y;
	return tmpPt;
}

AcGePoint3dArray CDrawDotted::getPointsArray()
{
	AcGePoint3dArray points;
	points.removeAll();
	AcGePoint3d pt1,pt2,pt3,pt4;
	double dGap = 2.5;
	pt1 = GetLeftUpPt();
	pt1.y += dGap;//预留向上2.5,为文字位置留空间
	m_ptVec.push_back(pt1);
	pt2 = GetLeftDownPt();
	pt3 = GetRightDownPt();
	pt4 = GetRightUpPt();
	pt4.y += dGap;//预留向上2.5,为文字位置留空间
	points.append(pt1);
	points.append(pt2);
	points.append(pt3);
	points.append(pt4);
	points.append(pt1);
	return points;
}

bool CDrawDotted::selEntForMb(int nType)
{
	ads_name ssname;
	resbuf* rb = acutBuildList(-4,_T("<or"), -4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("ZW_TABLE"), -4,_T("and>"), 
		-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF1"), -4,_T("and>"), 
		-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF2"), -4,_T("and>"), 
		-4,_T("<and"),RTDXF0,_T("INSERT"),  2, _T("RF5"), -4,_T("and>"), -4,_T("or>"), RTNONE);
	int nRet = acedSSGet(_T("C"), asDblArray(m_minPt), asDblArray(m_maxPt), rb, ssname);
	if (nRet != RTNORM)
	{
		acutRelRb(rb);
		return false;
	}
	
	map<double, int> tmpX;
	map<double, int> tmpY;
	AcDbObjectId entId = AcDbObjectId::kNull;
	AcDbEntity* pEnt = NULL;
	long sslen;
	acedSSLength(ssname, &sslen);
	ads_name ename;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(entId, ename);
		if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
			AcDbExtents exts;
			pBlkRef->getGeomExtents(exts);
			pBlkRef->close();

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////	
			AcGePoint3d minPt,maxPt;
			maxPt = exts.maxPoint();
			minPt = exts.minPoint();

			CString strBlockName;
			MyBaseUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
			if (strBlockName.Find(_T("RF5")) >= 0)
			{
				tmpY.insert(make_pair(minPt.y, 1));
			}
			else
			{
				acutPolar(asDblArray(minPt), 5*PI/4, 2, asDblArray(minPt));
				acutPolar(asDblArray(maxPt), PI/4, 2, asDblArray(maxPt));
				tmpX.insert(make_pair(minPt.x, 1));
				tmpX.insert(make_pair(maxPt.x, 1));
				tmpY.insert(make_pair(minPt.y, 1));
				tmpY.insert(make_pair(maxPt.y, 1));
			}
			
		}
		else
		{
			pEnt->close();
		}
	}
	acedSSFree(ssname);
	if (nType == 2)
	{
		m_minPt.x = tmpX.begin()->first-17;
		m_minPt.y = tmpY.begin()->first-5;
		m_maxPt.x = tmpX.rbegin()->first+5;
		
		double dTempMaxPt = m_maxPt.y;
		double dEndYPos = tmpY.rbegin()->first;
		if ((dTempMaxPt - dEndYPos) < CBaseUtils::leftVerGap() + 9)//为什么是这个数据，如果小于这个数据，那么母版的位置必须向下，不然就跟图框相交
		{
			m_maxPt.y = dEndYPos + 10;
		}
		else
		{
			m_maxPt.y = dEndYPos + 14.2;
		}
	}
								  
	acutRelRb(rb);
	return true;
}

AcDbObjectId CDrawDotted::doPointsAndDrawPline(AcGePoint3dArray downPoints, AcGePoint3dArray upPoints)
{
	AcGePoint3d tmpPt;
	downPoints = doPointsToTwoPoint(downPoints, false);
	upPoints = doPointsToTwoPoint(upPoints);
	
	AcGePoint3d downLastPt, upLastPt, pt1, pt2;
	CString strType = gGlobal.GetIniValue(_T("虚线框设置"), _T("模块虚线框线型"));
	AcDbObjectId layerId = MySymble::CreateNewLayer(_T("图层虚线"), 1, FALSE, strType);
	
	downLastPt = downPoints.last();
	upLastPt = upPoints.last();
	pt1 = downPoints.first();
	pt2 = upPoints.first();
	downPoints.removeAll();
	upPoints.removeAll();


	AcGePoint3dArray points;
	points.removeAll();
	points.append(pt1);
	points.append(pt2);
	points.append(upLastPt);
	points.append(downLastPt);
	points.append(pt1);

	AcDbObjectId objId = MyDrawEntity::DrawPlineByPoints(points, layerId);
	m_ptVec.push_back(upPoints.first());
	return objId;
}

AcGePoint3dArray CDrawDotted::doPointsToTwoPoint(AcGePoint3dArray downPoints, bool bIsUp)
{
	AcGePoint3d leftdownPt;
	AcGePoint3d rightDownPt;
	AcGePoint3d tmpPt;
	if (downPoints.length() > 1)
	{
		leftdownPt = downPoints.first();
		rightDownPt = downPoints.last();
	}

	for (int i=1; i<downPoints.length()-1; i++)
	{
		tmpPt = downPoints.at(i);
		if (bIsUp)
		{
			if (tmpPt.y > leftdownPt.y)
			{
				leftdownPt.y = tmpPt.y;
			}
			if (tmpPt.y > rightDownPt.y)
			{
				rightDownPt.y = tmpPt.y;
			}
		}
		else
		{
			if (tmpPt.y < leftdownPt.y)
			{
				leftdownPt.y = tmpPt.y;
			}
			if (tmpPt.y < rightDownPt.y)
			{
				rightDownPt.y = tmpPt.y;
			}
		}
		
	}

	downPoints.removeAll();
	downPoints.append(leftdownPt);
	downPoints.append(rightDownPt);
	return downPoints;
}
