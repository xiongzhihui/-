#include "StdAfx.h"
#include "CDrawDigitalOptical.h"
#include "DlgTKSZ.h"
#include "Utility.h"
#include "BaseUtils.h"
#include <algorithm>
#include "CGlobalQuery.h"
#include "CGlobalRWriteFile.h"
#include "CDrawFrame.h"

extern CGlobalQuery gQuery;
extern CGlobalRWriteFile gRWriteFile;

CDrawDigitalOptical::CDrawDigitalOptical(void)
{
	m_objIdArr.removeAll();
	m_bIsDrawVerLine = true;
	m_bIsDrawHorLine = true;
	m_nFrameCount = -1;
}


CDrawDigitalOptical::~CDrawDigitalOptical(void)
{
	m_objIdArr.removeAll();
}

void CDrawDigitalOptical::setData(CSheetStoreData data)
{
	m_sheetData = data;
	//设置数据
	m_FrameInfo = gRWriteFile.getFrameInfoByName(data.getLrmName());
	
	int nTemp = m_nFrameCount;
	if (nTemp == -1)
	{
		nTemp = CBaseUtils::frameCount();
	}
	map<int, CFrameInfo> AllFrame = gRWriteFile.getAllFrameInfo();
	if (AllFrame.size() > 0)
	{
		map<int, CFrameInfo>::iterator iter = AllFrame.find(nTemp);
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

	m_FrameInfo.setFrameType(_T("A1"));//数字光模拟光都用A1的图框，中心线在中间
	m_FrameInfo.setSpaceVerGap(297);
	CDrawFrame dlg;
	dlg.setFrameIni(m_sheetData.getSheetName(), m_FrameInfo, m_nFrameCount);
	dlg.draw();

	m_InsertPt = dlg.getInsertPt();
	m_basePt = dlg.getContentPt();
}

void CDrawDigitalOptical::doIt()
{

}

bool  CmpGLData(const CGLData& p1,const CGLData& p2)
{
	/*return strcmp(p1.aa,p2.aa)>0?1:-1;//小于号表示：从小到大排序（小的在前，大的在后）大于号相反*/
	CString str1,str2, strPre1,strPre2;
	str1 = p1.number();
	str2 = p2.number();
	int nCount1,nCount2;
	nCount1 = MyParserString::GetPileLength(str1, strPre1);
	nCount2 = MyParserString::GetPileLength(str2, strPre2);

	if (str1.Compare(str2) > 0)
	{
		return false;
	}
	else if (strPre1.Compare(strPre2) == 0)
	{
		if (nCount1 > nCount2)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	return true;
}
bool CDrawDigitalOptical::draw()
{
	m_layerId = MySymble::CreateNewLayer(m_sheetData.getSheetName());
	
	vector<CGLData> glVecData;
	
	glVecData = m_sheetData.getGLData();
	//处理图框
	AcGePoint3d tempPt = m_basePt;
	//将GL光纤按照12 24 48分门别类
	std::sort(glVecData.begin(), glVecData.end(), CmpGLData);
	m_nCount = 0;
	m_nGlCount = 0;
	m_bIsUp = true;
	for (vector<CGLData>::iterator iter = glVecData.begin(); iter != glVecData.end(); ++iter)
	{
		CGLData gData = *iter;
		if (((gData.type() == 3) && (m_nCount >= 7)) || (m_nCount >= 8))
		{
			m_nCount = 0;
			m_basePt.y = m_InsertPt.y + 297 - m_FrameInfo.getTableHeight();
			
			m_basePt.x = tempPt.x;
			m_bIsUp = false;
		}
		if ((m_nGlCount >= 16) || ((gData.type() == 3)&&m_nGlCount >= 15))
		{
			CDrawFrame dlg;
			dlg.setFrameIni(m_sheetData.getSheetName(), m_FrameInfo, m_nFrameCount);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();

			m_nGlCount = 0;
			m_nCount = 0;
			tempPt = m_basePt;
			m_bIsDrawHorLine = true;
			m_bIsUp = true;
		}
		drawGLData(gData);
		if (m_nCount >= 8)
		{
			m_basePt.y = m_InsertPt.y + 297 - m_FrameInfo.getTableHeight();
			m_basePt.x = tempPt.x;
			m_bIsUp = false;
			m_nCount = 0;
		}
	}
	glVecData.clear();
	return true;
}

void CDrawDigitalOptical::setFrameCount(int nFrameCount)
{
	m_nFrameCount = nFrameCount;
}	

bool CDrawDigitalOptical::drawGLData(CGLData glData)
{
	vector<CCommonCell> tmpVec = glData.getCommonVec();
	int nType = glData.type();


	AcGePoint3d tmpPt, tempPt;	
	int nGlSize = tmpVec.size();
	if (m_bIsUp)
	{
		tmpPt.y = m_InsertPt.y + 297 + m_FrameInfo.getTableHeight();
		tmpPt.z = 0;
		tmpPt.x = m_basePt.x - m_FrameInfo.getLineLength();	
		m_glMinPt = tmpPt;
		m_glMinPt.y += m_FrameInfo.getTableHeight();
		m_glMinPt.y += 2;
		m_glMinPt.x += m_FrameInfo.getLineLength()/2;
		acutPolar(asDblArray(tmpPt), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));
	}
	else
	{
		tmpPt.y = m_InsertPt.y + 297 - m_FrameInfo.getTableHeight();
		tmpPt.z = 0;
		tmpPt.x = m_basePt.x - m_FrameInfo.getLineLength();
		m_glMinPt = tmpPt;
		m_glMinPt.x += m_FrameInfo.getLineLength()/2;
		m_glMinPt.y -= m_FrameInfo.getTableHeight();
		m_glMinPt.y -= 2;
		acutPolar(asDblArray(tmpPt), 3*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));	
	}
	
	
	CString strNumber = glData.number();

	if ((nType == 1) || (nType == 0))//1表示12光纤
	{
		if (m_bIsUp)
		{
			tmpPt.y += m_FrameInfo.getTableHeight();
		}
		else
		{
			tmpPt.y -= m_FrameInfo.getTableHeight();
		}
		drawGLEnt(tmpPt, strNumber, tmpVec);
		m_basePt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		m_nCount++;
		m_nGlCount++;
	}
	else if (nType == 2)//2表示24光纤
	{
		if (m_bIsUp)
		{
			tmpPt.y += m_FrameInfo.getTableHeight();
		}
		else
		{
			tmpPt.y -= m_FrameInfo.getTableHeight();
		}
		drawGLEnt(tmpPt, strNumber, tmpVec);
		m_basePt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		m_nCount++;
		m_nGlCount++;
	}
	else if (nType == 3)//48光纤，那么坐标点需要往右边移动两格
	{
		if (m_bIsUp)
		{
			tmpPt.y += m_FrameInfo.getTableHeight();
		}
		else
		{
			tmpPt.y -= m_FrameInfo.getTableHeight();
		}
		drawGLEnt(tmpPt, strNumber, tmpVec);
		m_basePt.x += 2*(m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace());
		m_nCount++;
		m_nCount++;
		m_nGlCount++;
		m_nGlCount++;
	}

	CString strText = glData.getNameCell().getName();
	CString strSheet = m_sheetData.getSheetName();
	strSheet = MyParserString::RemoveRightSymbol(_T('('), strSheet);
	strSheet += strText;
	strSheet.Replace(_T("[MT]"), _T(""));
	drawGlExts(strSheet);
	tmpVec.clear();

	return true;
}

void CDrawDigitalOptical::drawGLEnt(AcGePoint3d tmpPt, CString strNumber, vector<CCommonCell> tmpVec)
{
	//绘制表头
	CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF5.dwg");
	AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

	AcDbBlockReference *pBlkRef =new AcDbBlockReference(tmpPt, objId) ; 
	pBlkRef->setScaleFactors(1.0);
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
	AcDbObjectId blkId = pBlkRef->objectId();	
	blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
	m_objIdArr.append(blkId);

	AcGePoint3d numPt, blkPt;
	blkPt = tmpPt;
	numPt = tmpPt;
	numPt.x += 8;
	AcDbObjectId textId = MyDrawEntity::DrawText(numPt, strNumber);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);


	if (tmpVec.size() > 24)
	{	
		//重置表格坐标点
		if (m_bIsUp)
		{
			m_dYPos = tmpPt.y + (24+1.5)*m_FrameInfo.getTableHeight();
		}
		else
		{
			m_dYPos = tmpPt.y - (24+1.5)*m_FrameInfo.getTableHeight();
		}
		if (m_bIsUp)
		{
			acutPolar(asDblArray(tmpPt), PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));
		}
		else
		{
			acutPolar(asDblArray(tmpPt), 7*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));
		}
		tmpPt.x += m_FrameInfo.getLineLength();

		vector<CCommonCell> tempVec;
		tempVec.insert(tempVec.end(), tmpVec.begin(), tmpVec.begin() + 24);
		drawGlContent(tmpPt, tempVec);

		tmpPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		
		//
		blkPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		AcDbBlockReference *pBlkRef2 =new AcDbBlockReference(blkPt, objId) ; 
		pBlkRef2->setScaleFactors(1.0);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef2, acdbHostApplicationServices()->workingDatabase());
		AcDbObjectId blkId2 = pBlkRef2->objectId();	
		blkId2 = MyEditEntity::openEntChangeLayer(blkId2, m_layerId);


		numPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		textId = MyDrawEntity::DrawText(numPt, strNumber);
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);

		tempVec.clear();
		tempVec.insert(tempVec.end(), tmpVec.begin()+24, tmpVec.end());

		if (m_bIsUp)
		{
			m_dYPos = tmpPt.y + (tempVec.size()+1.5)*m_FrameInfo.getTableHeight();

			m_glMaxPt = tmpPt;
			m_glMaxPt.y += (24+2.5)*m_FrameInfo.getTableHeight();
		}
		else
		{
			m_dYPos = tmpPt.y - (tempVec.size()+1.5)*m_FrameInfo.getTableHeight();
			m_glMaxPt = tmpPt;
			m_glMaxPt.y -= (24+2.5)*m_FrameInfo.getTableHeight();
			m_glMaxPt.y += 3;
		}
		drawGlContent(tmpPt, tempVec);
	}
	else
	{
		if (m_bIsUp)
		{
			m_dYPos = tmpPt.y + (tmpVec.size()+1.5)*m_FrameInfo.getTableHeight();
		}
		else
		{
			m_dYPos = tmpPt.y - (tmpVec.size()+1.5)*m_FrameInfo.getTableHeight();
		}
		if (m_bIsUp)
		{
			acutPolar(asDblArray(tmpPt), PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));
		}
		else
		{
			acutPolar(asDblArray(tmpPt), 7*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));
		}
		tmpPt.x += m_FrameInfo.getLineLength();
		drawGlContent(tmpPt, tmpVec);
		if (m_bIsUp)
		{
			m_glMaxPt = tmpPt;
			m_glMaxPt.y += (tmpVec.size()+2.5)*m_FrameInfo.getTableHeight();
		}
		else
		{
			m_glMaxPt = tmpPt;
			m_glMaxPt.y -= (tmpVec.size()+2.5)*m_FrameInfo.getTableHeight();
			m_glMaxPt.y += 3;
		}
	}
	tmpVec.clear();
}

void CDrawDigitalOptical::drawGlContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec)
{
	CCommonCell comCell;
	bool bIsFirst = true;
	int nSize = tmpVec.size();
	int nIndex = 0;
	for (vector<CCommonCell>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		comCell = *iter;
		if (m_bIsUp)
		{
			if (bIsFirst)
			{
				tmpPt.y += m_FrameInfo.getTableHeight();
				bIsFirst = false;
			}
		}
		else
		{
			if (bIsFirst)
			{
				tmpPt.y -= 2*m_FrameInfo.getTableHeight();
				drawContent(tmpPt, _T("端子"), _T("特性"), _T("去向"), _T(""), false);
				tmpPt.y -= m_FrameInfo.getTableHeight();	
				bIsFirst = false;
			}
		}
		
		
		gQuery.getQuxiang(comCell);
		gQuery.addCellInfo(comCell);
		m_ignoreErrorArr.removeAll();
		if ((comCell.getChineseRealTexing().IsEmpty() || (comCell.getChineseRealTexing().CompareNoCase(_T("NA")) == 0) || (comCell.getQuxiang().IsEmpty())) && !gQuery.getPreviewFlag())
			drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getQuxiang(), _T("0"), false);
		else
			drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getQuxiang(), _T("0"));

		if (m_bIsUp)
		{
			tmpPt.y += m_FrameInfo.getTableHeight();
		}
		else
		{
			tmpPt.y -= m_FrameInfo.getTableHeight();
		}
		if (CUtility::IsIgnoreErrorFlag(comCell))
		{
			CUtility::EditignoreError(m_ignoreErrorArr);
		}
		if (m_bIsUp)
		{
			if (nIndex == nSize-1)
			{
				drawContent(tmpPt, _T("端子"), _T("特性"), _T("去向"), _T(""), false);
				tmpPt.y += m_FrameInfo.getTableHeight();
			}
		}
		nIndex++;
	}	
	tmpVec.clear();
}

void CDrawDigitalOptical::drawContent(AcGePoint3d tmpPt, CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao, bool bIsDrawLine)
{
	//m_bFirstDrawRf = true;
	//绘制线
	AcGePoint3d pt1,pt2,pt3,pt4,pt5,pt6,pt7;
	AcDbObjectId blkTblRecId = CUtility::getContentBlkRef();
	AcDbBlockReference* pblRef = new AcDbBlockReference;
	pblRef->setBlockTableRecord(blkTblRecId);
	pblRef->setPosition(tmpPt);
	pblRef->setLayer(m_layerId);
	MyBaseUtils::addToCurrentSpaceAndClose(pblRef);
	m_objIdArr.append(pblRef->objectId());
	m_ignoreErrorArr.append(pblRef->objectId());
	AcGePoint3d textPt1,textPt2,textPt3;
	textPt1.x = tmpPt.x + m_FrameInfo.getFristTableLength()/2;
	textPt1.y = tmpPt.y + m_FrameInfo.getTableHeight()/2;

	textPt2.x = tmpPt.x + m_FrameInfo.getFristTableLength() + m_FrameInfo.getSecondTableLength()/2;
	textPt2.y = tmpPt.y + m_FrameInfo.getTableHeight()/2;

	textPt3.x = tmpPt.x + m_FrameInfo.getFristTableLength() + m_FrameInfo.getSecondTableLength() + m_FrameInfo.getThirdTableLength()/2;
	textPt3.y = tmpPt.y + m_FrameInfo.getTableHeight()/2;

	textPt1.z = textPt2.z = textPt3.z = 0;

	AcDbObjectId textId = MyDrawEntity::DrawText(textPt1, strDuanzi, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
	double dTextLen = MyEditEntity::OpenObjAndGetLength(textId);
	if (dTextLen > m_FrameInfo.getFristTableLength())
	{
		double dScale = 3.0*m_FrameInfo.getFristTableLength()/(dTextLen+1);
		textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
	}
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);
	m_ignoreErrorArr.append(textId);
	if (!strTexing.IsEmpty())
	{
		strTexing = MyParserString::RemoveSymbol(_T("]"), strTexing);
		textId = MyDrawEntity::DrawText(textPt2, strTexing, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		double dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_FrameInfo.getSecondTableLength())
		{
			double dScale = 3.0*m_FrameInfo.getSecondTableLength()/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
		m_ignoreErrorArr.append(textId);
	}

	if (!strQuxiang.IsEmpty())
	{
		textId = MyDrawEntity::DrawText(textPt3, strQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		double dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_FrameInfo.getThirdTableLength())
		{
			double dScale = 3.0*m_FrameInfo.getThirdTableLength()/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
		m_ignoreErrorArr.append(textId);
	}

	if (bIsDrawLine)
	{
		AcGePoint3d startPt,midPt, endPt;
		startPt.x = tmpPt.x;
		startPt.y = tmpPt.y + m_FrameInfo.getTableHeight()/2;

		midPt.x = startPt.x - m_FrameInfo.getLineLength();
		midPt.y = startPt.y;
		startPt.z = midPt.z = 0;
		AcDbObjectId lineId = MyDrawEntity::DrawLine(startPt, midPt, m_layerId);
		if (m_bIsUp)
		{
			acutPolar(asDblArray(midPt), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(endPt));
		}
		else
		{
			acutPolar(asDblArray(midPt), 3*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(endPt));
		}
		
		lineId = MyDrawEntity::DrawLine(midPt, endPt, m_layerId);

		//绘制线号
		if (strXianhao.CompareNoCase(_T("0")) != 0 )
		{
			AcGePoint3d xianhaoPt;
			xianhaoPt.x = (midPt.x + startPt.x)/2;
			xianhaoPt.y = startPt.y;
			xianhaoPt.z = 0;

			textId = MyDrawEntity::DrawText(xianhaoPt, strXianhao, 3.0, AcDbObjectId::kNull, AcDb::kTextCenter);

			double dLen = MyEditEntity::OpenObjAndGetLength(textId);
			if (dLen > m_FrameInfo.getLineLength())
			{
				double dScale = 3.0*m_FrameInfo.getLineLength()/(dLen+1);
				textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
			}

			textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
			m_objIdArr.append(textId);
		}
		//绘制长线
		if (m_bIsDrawVerLine)
		{
			pt1 = endPt;
			pt1.y = m_dYPos;

			midPt.x = pt1.x;
			tmpPt.x -= m_FrameInfo.getLineLength();	
			tmpPt.y += m_FrameInfo.getTableHeight()/2;
			if (m_bIsUp)
			{
				midPt.y = m_InsertPt.y + m_FrameInfo.getSpaceVerGap() + 5;		
				pt2.y = midPt.y - 5;
				acutPolar(asDblArray(tmpPt), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));
			}
			else
			{
				midPt.y = m_InsertPt.y + m_FrameInfo.getSpaceVerGap() - 5;
				pt2.y = midPt.y + 5;
				acutPolar(asDblArray(tmpPt), 3*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));
			}
			pt1.y = tmpPt.y;

			pt2.x = pt1.x + 5;

			AcGePoint3dArray points;
			points.append(pt1);
			points.append(midPt);
			points.append(pt2);
			AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(points, m_layerId);
			plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);
			//绘制横着的线
			if (m_bIsDrawHorLine)
			{
				pt3.x = m_InsertPt.x - m_FrameInfo.getSpaceHorGap();
				pt3.y = pt2.y;
				pt3.z = 0;
				int nIndex = CBaseUtils::frameCount();
				if (nIndex > 2)
				{
					pt2.x -= 15;
					plineId = MyDrawEntity::DrawPlineByTwoPoint(pt3, pt2, m_layerId);
					plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);

					pt3.y +=1;
					pt2.y +=1;
					MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
					MyDrawEntity::DrawText(pt2, _T("接上页"), 2.5);
				}
				else
				{
					plineId = MyDrawEntity::DrawPlineByTwoPoint(pt3, pt2, m_layerId);
					plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);

					pt3.y +=1;
					MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
				}
			}
			m_bIsDrawHorLine = false;
		}
		m_bIsDrawVerLine = true;
	}
}

void CDrawDigitalOptical::drawGlExts(CString strText)
{
	double dFirstTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第一段长度"), 0);
	double dSecondTableLength =  gGlobal.GetIniValue(_T("基础设置"), _T("表格第二段长度"), 0);
	double dThirdTableLength  = gGlobal.GetIniValue(_T("基础设置"), _T("表格第三段长度"), 0);
	double dTableLength = dFirstTableLength + dSecondTableLength + dThirdTableLength;
	m_glMaxPt.x += dTableLength;
	m_glMaxPt.x += 0.5;
	//AcDbObjectId layerId = MySymble::CreateNewLayer(_T("集束光纤虚线"), 1, FALSE, _T("ACAD_ISO10W100"));
	AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("3"));
	AcGePoint3dArray points;
	AcGePoint3d pt1,pt2,pt3,pt4;
	pt1 = m_glMinPt;
	pt2.x = m_glMinPt.x;
	pt2.y = m_glMaxPt.y;

	pt3 = m_glMaxPt;
	pt4.x = m_glMaxPt.x;
	pt4.y = m_glMinPt.y;

	pt2.z = pt4.z = 0;
	points.append(pt1);
	points.append(pt2);
	points.append(pt3);
	points.append(pt4);
	points.append(pt1);

	AcDbObjectId objId = MyDrawEntity::DrawPlineByPoints(points, m_layerId);
	objId = MyEditEntity::openEntChangeColor(objId, 1);
	objId = MyEditEntity::openEntChangeLineType(objId, _T("ACAD_ISO10W100"));
	objId = MyEditEntity::openPlineChangeWidth(objId, 0.3);
	
	if (m_bIsUp)
	{
		AcDbObjectId textId = MyDrawEntity::DrawText(pt2, strText, 3.0, textStyleId, AcDb::kTextLeft, AcDb::kTextBottom);
	}
	else
	{
		AcDbObjectId textId = MyDrawEntity::DrawText(pt1, strText, 3.0, textStyleId, AcDb::kTextLeft, AcDb::kTextBottom);
	}
}
