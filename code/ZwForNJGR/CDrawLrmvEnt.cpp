#include "StdAfx.h"
#include "CDrawLrmvEnt.h"
#include "BaseUtils.h"
#include "DlgTKSZ.h"
#include "Utility.h"

#include "CGlobalQuery.h"
#include "CDrawFrame.h"
#include "CGlobalRWriteFile.h"

extern CGlobalQuery gQuery;
extern CGlobalRWriteFile gRWriteFile;
CDrawLrmvEnt::CDrawLrmvEnt(void)
{
	m_objIdArr.removeAll();
	m_bIsDrawMod = false;
	m_nCount = -1;
	m_bIsLastDraw = false;
}


CDrawLrmvEnt::~CDrawLrmvEnt(void)
{
	m_objIdArr.removeAll();
}

void CDrawLrmvEnt::setData(CSheetStoreData data)
{
	m_sheetData = data;
	m_FrameInfo = gRWriteFile.getFrameInfoByName(data.getLrmName());
	int nTemp = m_nCount;
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

	CDrawFrame dlg;
	dlg.setFrameIni(m_sheetData.getSheetName(), m_FrameInfo, m_nCount);
	dlg.draw();

	m_InsertPt = dlg.getInsertPt();
	m_basePt = dlg.getContentPt();
	m_bIsDrawHorLine = true;
}

void CDrawLrmvEnt::sortData()
{

}

bool CDrawLrmvEnt::draw()
{
	m_layerId = MySymble::CreateNewLayer(m_sheetData.getSheetName());
	vector<CTableData> tblVecdata;
	vector<CRFData> rfVecdata;
	vector<CGLData> glVecData;
	vector<CCommonData> comVecData;

	tblVecdata = m_sheetData.getTableData();
	/*if (tblVecdata.size() < 1)
	{
		return false;
	}
	else
	{
		CTableData tmpTable = tblVecdata.at(0);
		if (m_bIsDrawMod)
		{
			if (tmpTable.getCommonVec().size() != 7)
			{
				return false;
			}
			else
			{
				if (tmpTable.getCommonVec().size() != 9)
				{
					return false;
				}
			}
		}

	}*/
	rfVecdata = m_sheetData.getRfData();
	glVecData = m_sheetData.getGLData();
	comVecData = m_sheetData.getCommonData();
	//处理图框
	for (vector<CTableData>::iterator iter = tblVecdata.begin(); iter != tblVecdata.end(); ++iter)
	{
		CTableData tbData = *iter;
		drawTable(tbData);
	}
	for (vector<CRFData>::iterator iter = rfVecdata.begin(); iter != rfVecdata.end(); ++iter)
	{
		m_bIsDrawVerLine = true;
		CRFData rData = *iter;
		drawRfData(rData);
	}

	for (vector<CGLData>::iterator iter = glVecData.begin(); iter != glVecData.end(); ++iter)
	{
		m_bIsDrawVerLine = true;
		CGLData gData = *iter;
		drawGLData(gData);
	}
	//m_drawBaseData.setInsertPt(m_InsertPt);
	//for (vector<CCommonData>::iterator iter = comVecData.begin(); iter != comVecData.end(); ++iter)
	//{
	//	CCommonData comData = *iter;
	//	drawComData(comData);
	//}
	tblVecdata.clear();
	rfVecdata.clear();
	glVecData.clear();
	comVecData.clear();
	return true;
}

bool CDrawLrmvEnt::drawTable(CTableData tblData)
{
	vector<vector<CCommonCell> > dataVec = tblData.getCommonVec();
	vector<CCommonCell> tmpVec;
	AcGePoint3d tmpPt, curPt;
	curPt = m_basePt;

	m_nameCell = tblData.getNameCell();
	int nSize = 0;
	bool bIsFirst = true;//是否第一次绘制，即要绘制表头
	for (vector<vector<CCommonCell> >::iterator iter = dataVec.begin(); iter != dataVec.end(); ++iter)
	{
		tmpVec.clear();
		tmpVec = *iter;
		if (bIsFirst)
		{
			nSize = tmpVec.size();
			CString strName = m_nameCell.getSpecailaName();
			if (m_nameCell.getDrawText())
			{
				AcDbObjectId textId = MyDrawEntity::DrawText(m_basePt, strName);
				MyEditEntity::openEntChangeLayer(textId, m_layerId);
				if (m_bIsDrawMod)
				{
					if (!m_nameCell.getModLjqName().IsEmpty())
					{
						m_basePt.y -= 3.5;
						textId = MyDrawEntity::DrawText(m_basePt, m_nameCell.getModLjqName());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
					if (!m_nameCell.getModWzNo().IsEmpty())
					{
						m_basePt.y -= 3.5;
						textId = MyDrawEntity::DrawText(m_basePt, m_nameCell.getModWzNo());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
				}
				else
				{
					if (!m_nameCell.getLjqName().IsEmpty())
					{
						m_basePt.y -= 3.5;
						textId = MyDrawEntity::DrawText(m_basePt, m_nameCell.getLjqName());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
					if (!m_nameCell.getWzNo().IsEmpty())
					{
						m_basePt.y -= 3.5;
						textId = MyDrawEntity::DrawText(m_basePt, m_nameCell.getWzNo());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
				}
				
			}
			tmpPt = m_basePt;	

			//if (nameCell.getStatus().CompareNoCase(_T("1")) == 0)//需要加虚线框
			//{
			//	if (nameCell.getDrawText())
			//	{
			//		m_basePt.y -= 2;
			//	}	
			//	m_bIsDrawXxk = true;
			//}
			//else	
			//{
			//	m_basePt.y -= 1.5;
			//	m_bIsDrawXxk = false;
			//}

			bIsFirst = false;
		}
		else
		{
			CString strName = m_nameCell.getSpecailaName();
			curPt.x = tmpPt.x;
			tmpPt = curPt;
			if (m_nameCell.getDrawText())
			{
				AcDbObjectId textId = MyDrawEntity::DrawText(tmpPt, strName);
				MyEditEntity::openEntChangeLayer(textId, m_layerId);
				if (m_bIsDrawMod)
				{
					if (!m_nameCell.getModLjqName().IsEmpty())
					{
						tmpPt.y -= 3.5;
						textId = MyDrawEntity::DrawText(tmpPt, m_nameCell.getModLjqName());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
					if (!m_nameCell.getModWzNo().IsEmpty())
					{
						tmpPt.y -= 3.5;
						textId = MyDrawEntity::DrawText(tmpPt, m_nameCell.getModWzNo());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
				}
				else
				{
					if (!m_nameCell.getLjqName().IsEmpty())
					{
						tmpPt.y -= 3.5;
						textId = MyDrawEntity::DrawText(tmpPt, m_nameCell.getLjqName());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
					if (!m_nameCell.getWzNo().IsEmpty())
					{
						tmpPt.y -= 3.5;
						textId = MyDrawEntity::DrawText(tmpPt, m_nameCell.getWzNo());
						MyEditEntity::openEntChangeLayer(textId, m_layerId);
					}
				}

			}
		}
		drawTableContent(tmpPt, tmpVec);
		tmpPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
	}
	if (nSize != 0)
	{
		m_basePt.y -= (nSize+1)*m_FrameInfo.getTableHeight() + m_FrameInfo.getVerSpace();
	}
	dataVec.clear();

	return true;
}

bool CDrawLrmvEnt::drawGLData(CGLData glData)
{
	//光纤在固定的位置绘制
	vector<CCommonCell> tmpVec = glData.getCommonVec();
	int nType = glData.type();
	AcGePoint3d tmpPt;
	tmpPt.y = m_InsertPt.y + m_FrameInfo.getSpaceVerGap() + 2*m_FrameInfo.getTableHeight();
	tmpPt.x = m_basePt.x - m_FrameInfo.getLineLength();
	tmpPt.z = 0;
	
	m_glMinPt = tmpPt;
	m_glMinPt.y += m_FrameInfo.getTableHeight();

	acutPolar(asDblArray(tmpPt), 3*PI/4,  m_FrameInfo.getLineLength()/3, asDblArray(tmpPt));

	CString strNumber = glData.number();
	if (nType == 0)//单光纤
	{
		if (tmpVec.size() == 1)
		{
			drawGL(tmpPt, tmpVec.at(0));
			m_basePt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		}
	}
	else if ((nType == 1))//1表示12光纤
	{
		drawGLEnt(tmpPt, strNumber, tmpVec);
		m_basePt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
	}
	else if (nType == 2)//2表示24光纤
	{
		drawGLEnt(tmpPt, strNumber, tmpVec);
		m_basePt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
	}
	else if (nType == 3)//48光纤，那么坐标点需要往右边移动两格
	{
		drawGLEnt(tmpPt, strNumber, tmpVec);
		m_basePt.x += 2*(m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace());
	}

	if (nType != 0)
	{
		CString strText = glData.getNameCell().getName();
		CString strSheet = m_sheetData.getSheetName();
		strSheet = MyParserString::RemoveRightSymbol(_T('('), strSheet);
		strSheet += strText;
		strSheet.Replace(_T("[MT]"), _T(""));
		drawGlExts(strSheet);
	}
	glData.clear();
	return true;
}

bool CDrawLrmvEnt::drawRfData(CRFData rfData)
{
	vector<CCommonCell> tmpVec = rfData.getCommonVec();
	CString strTexing;
	AcGePoint3d tmpPt = m_basePt;
	AcGePoint3d tempPt = tmpPt;
	int nIndex = 1;
	int nHx = 0; 
	int nZx = 0;
	int nMod = 9;
	if (m_bIsDrawMod)
	{
		nMod = 7;
	}
	m_strRealName = rfData.getNameCell().getRealName();
	for (vector<CCommonCell>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		if (nIndex > nMod)
		{
			m_bIsDrawVerLine = false;
		}
		else
		{
			m_bIsDrawVerLine = true;
		}
		CCommonCell comCell = *iter;
		gQuery.getQuxiang(comCell);
		gQuery.addCellInfo(comCell);

		strTexing = comCell.getChineseRealTexing();
		
		if ((strTexing.IsEmpty()) 
			|| (strTexing.CompareNoCase(_T("NC")) == 0) 
			|| (strTexing.CompareNoCase(_T("NA")) == 0)
			|| (comCell.getQuxiang().IsEmpty()))
		{
			drawRF(tmpPt, comCell, false);
		}
		else if (strTexing.Find(_T("[K]")) >= 0)
		{
			drawK(tmpPt, comCell);
		}
		else
		{
			drawRF(tmpPt, comCell, true);
		}
		nHx = nIndex%nMod;
		nZx = nIndex/nMod;
		if (nHx > 0)
		{
			tmpPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		}	
		else
		{
			tmpPt.x = m_basePt.x;
		}
		if ((nHx == 0) && (nZx > 0))//当刚好是最后一个时，图形需要往下偏移一格
		{
			tmpPt.y -= 17.5;
		}
		nIndex++;
	}
	m_basePt.y -= nZx* 17.5 + m_FrameInfo.getVerSpace();
	tmpVec.clear();
	return true;
}

void CDrawLrmvEnt::drawComData(CCommonData comData)
{
	return;
}


void CDrawLrmvEnt::drawTableContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec)
{
	CCommonCell comCell;
	bool bIsFirst = true;
	for (vector<CCommonCell>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		comCell = *iter;
		
		if (bIsFirst)
		{
			tmpPt.y -= m_FrameInfo.getTableHeight() + 2;
			drawContent(tmpPt, _T("端子"), _T("特性"), _T("去向"), _T(""), false);
			bIsFirst = false;
			tmpPt.y -= m_FrameInfo.getTableHeight();
			m_bIsDrawVerLine = true;
		}
		
		gQuery.getQuxiang(comCell);
		gQuery.addCellInfo(comCell);

		m_ignoreErrorArr.removeAll();
		CString strXianhao= comCell.getCount();
		if (m_bIsDrawMod)
		{
			if (comCell.getChineseRealTexing().IsEmpty() || (comCell.getChineseRealTexing().CompareNoCase(_T("NA")) == 0))
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getModQuxiang(), strXianhao, false);
			else
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getModQuxiang(), strXianhao);
		}
		else
		{
			if ((comCell.getChineseRealTexing().IsEmpty() || (comCell.getChineseRealTexing().CompareNoCase(_T("NA")) == 0) || (comCell.getQuxiang().IsEmpty())) && !gQuery.getPreviewFlag())
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getQuxiang(), strXianhao, false);
			else
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getQuxiang(), strXianhao);
		}
		tmpPt.y -= m_FrameInfo.getTableHeight();
		if (!m_bIsDrawMod)
		{
			if (CUtility::IsIgnoreErrorFlag(comCell))
			{
				CUtility::EditignoreError(m_ignoreErrorArr);
			}
		}
	}	
	tmpVec.clear();
}


void CDrawLrmvEnt::drawContent(AcGePoint3d tmpPt, CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao, bool bIsDrawLine)
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
		strTexing = MyParserString::RemoveRightSymbol(_T('<'), strTexing);
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
		}
	}	
	AcGePoint3d startPt,midPt, endPt;
	startPt.x = tmpPt.x;
	startPt.y = tmpPt.y + m_FrameInfo.getTableHeight()/2;

	midPt.x = startPt.x - m_FrameInfo.getLineLength();
	midPt.y = startPt.y;
	startPt.z = midPt.z = 0;
	if (bIsDrawLine)
	{
		
		AcDbObjectId lineId = MyDrawEntity::DrawLine(startPt, midPt, m_layerId);

		acutPolar(asDblArray(midPt), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(endPt));
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
	}
	else
	{
		acutPolar(asDblArray(midPt), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(endPt));
	}
	//绘制长线
	if (m_bIsDrawVerLine)
	{	
		pt1 = endPt;

		midPt.x = pt1.x;
		midPt.y = m_InsertPt.y + m_FrameInfo.getSpaceVerGap() + 5;

		pt2.x = pt1.x + 5;
		pt2.y = midPt.y - 5;
		AcDbObjectId plineId = AcDbObjectId::kNull;
		if (bIsDrawLine)
		{
			AcGePoint3dArray points;
			points.append(pt1);
			points.append(midPt);
			points.append(pt2);
			plineId = MyDrawEntity::DrawPlineByPoints(points, m_layerId);
			plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);
			m_bIsDrawVerLine = false;
		}
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
				if (!m_bIsLastDraw)
					MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
				MyDrawEntity::DrawText(pt2, _T("接上页"), 2.5);
			}
			else
			{
				plineId = MyDrawEntity::DrawPlineByTwoPoint(pt3, pt2, m_layerId);
				plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);

				pt3.y +=1;
				if (!m_bIsLastDraw)
					MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
			}
		}
		m_bIsDrawHorLine = false;
	}
}

void CDrawLrmvEnt::drawRF(AcGePoint3d tempPt, CCommonCell comCell, bool bIsRF)
{
	CString strDuanzi,strTexing,strQuxiang, strXianhao;
	strDuanzi = comCell.getDuanzi();
	strTexing = comCell.getChineseRealTexing();
	if (m_bIsDrawMod)
	{
		strQuxiang = comCell.getModQuxiang();
	}
	else
	{
		strQuxiang = comCell.getQuxiang();
	}

	//先绘制端子号
		/*if (m_bFirstDrawRf)
		{
		m_bFirstDrawRf = false;
		tempPt.y -= 5;
		}*/
	if (!comCell.getModName().IsEmpty())
	{
		strDuanzi = m_strRealName + _T(":") + strDuanzi;
	}
	AcDbObjectId textId = MyDrawEntity::DrawText(tempPt, strDuanzi);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);
	tempPt.y -= 1.5;
	if (bIsRF)//绘制RF3
	{
		//绘制RF1
		CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF1.dwg");

		AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

		AcDbBlockReference *pBlkRef =new AcDbBlockReference(tempPt, objId) ; 
		pBlkRef->setScaleFactors(1.0);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		AcDbObjectId blkId = pBlkRef->objectId();	
		blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
		m_objIdArr.append(blkId);

		//绘制RF3
		AcGePoint3d insertPt;
		insertPt.x = tempPt.x - m_FrameInfo.getLineLength()/2;
		insertPt.y = tempPt.y - 6;
		insertPt.z = 0;

		strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF3.dwg");
		objId = MyDrawEntity::GetBlkRef(strFile);
		AcDbBlockReference *pBlkRef1 =new AcDbBlockReference(insertPt, objId) ; 
		pBlkRef1->setScaleFactors(1.0);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef1, acdbHostApplicationServices()->workingDatabase());
		blkId = pBlkRef1->objectId();	
		blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
		//m_objIdArr.append(blkId);
		//绘制RF3上的文字
		int nCount = MyTransFunc::StringToInt(comCell.getCount());
		if (nCount > 0)
		{
			strXianhao.Format(_T("W%d"), nCount);
			AcGePoint3d xianhaoPt;
			xianhaoPt.x = tempPt.x - m_FrameInfo.getLineLength()/2;
			xianhaoPt.y = tempPt.y;
			xianhaoPt.z = 0;
			textId = MyDrawEntity::DrawText(xianhaoPt, strXianhao, 3.0, AcDbObjectId::kNull, AcDb::kTextCenter);
			textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
			m_objIdArr.append(textId);
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		//绘制特性文字
		AcGePoint3d textPt;
		textPt.x = tempPt.x + (12+42)/2.0;
		textPt.y = tempPt.y - (12+6)/2.0;
		strTexing = MyParserString::RemoveSymbol(_T("]"), strTexing);
		strTexing = MyParserString::RemoveRightSymbol(_T('<'), strTexing);
		double dLen = 0;
		if ((strTexing.CompareNoCase(_T("NC")) == 0) || (strTexing.IsEmpty()))
		{

		}
		else
		{
			textId = MyDrawEntity::DrawText(textPt, strTexing, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
			dLen = MyEditEntity::OpenObjAndGetLength(textId);
			if (dLen > m_FrameInfo.getRfSecondTableLength())
			{
				double dScale = 3.0*m_FrameInfo.getRfSecondTableLength()/(dLen+1);
				textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
			}
			textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
			m_objIdArr.append(textId);
		}


		if (!strQuxiang.IsEmpty())
		{
			textPt.x = tempPt.x + (42+60)/2;
			textPt.y = tempPt.y - (12+6)/2;
			textId = MyDrawEntity::DrawText(textPt, strQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
			dLen = MyEditEntity::OpenObjAndGetLength(textId);
			if (dLen > m_FrameInfo.getRfThirdTableLength())
			{
				double dScale = 3.0*m_FrameInfo.getRfThirdTableLength()/(dLen+1);
				textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
			}
			textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
			m_objIdArr.append(textId);
		}				

		//绘制斜线和竖着的线
		AcGePoint3d pt1,pt2,pt3;
		pt1.x = insertPt.x - 5;
		pt1.y = insertPt.y;
		pt1.z = insertPt.z;
		acutPolar(asDblArray(pt1), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(pt2));
		AcDbObjectId lineId = MyDrawEntity::DrawLine(pt1, pt2, m_layerId);
		m_objIdArr.append(lineId);
		//绘制长线
		if (m_bIsDrawVerLine)
		{
			pt1 = pt2;
			AcGePoint3d midPt;
			midPt.x = pt1.x;
			midPt.y = m_InsertPt.y + m_FrameInfo.getSpaceVerGap() + 5;

			pt2.x = pt1.x + 5;
			pt2.y = midPt.y - 5;

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
					if (!m_bIsLastDraw)
						MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
					MyDrawEntity::DrawText(pt2, _T("接上页"), 2.5);
				}
				else
				{
					plineId = MyDrawEntity::DrawPlineByTwoPoint(pt3, pt2, m_layerId);
					plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);

					pt3.y +=1;
					if (!m_bIsLastDraw)
						MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
				}
			}
			m_bIsDrawHorLine = false;
			m_bIsDrawVerLine = false;
		}
	}
	else
	{
		//绘制RF2
		CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF2.dwg");

		AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

		AcDbBlockReference *pBlkRef =new AcDbBlockReference(tempPt, objId) ; 
		pBlkRef->setScaleFactors(1.0);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		AcDbObjectId blkId = pBlkRef->objectId();	
		blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
		m_objIdArr.append(blkId);
	}
	tempPt.y -= 15.5;
}

void CDrawLrmvEnt::drawK(AcGePoint3d tmpPt, CCommonCell comCell)
{
	CString strDuanzi,strTexing,strQuxiang, strXianhao;
	strDuanzi = comCell.getDuanzi();
	strTexing = comCell.getChineseRealTexing();
	if (m_bIsDrawMod)
	{
		strQuxiang = comCell.getModQuxiang();
	}
	else
	{
		strQuxiang = comCell.getQuxiang();
	}

	//strQuxiang = comCell.getQuxiang();
	//先绘制端子号
	//if (m_bFirstDrawRf)
	/*{
	m_bFirstDrawRf = false;
	tmpPt.y -= 5;
	}*/
	if (!comCell.getModName().IsEmpty())
	{
		strDuanzi = m_strRealName + _T(":") + strDuanzi;
	}
	AcDbObjectId textId = MyDrawEntity::DrawText(tmpPt, strDuanzi);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);
	tmpPt.y -= 1.5;

	//绘制RF1
	CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF1.dwg");

	AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

	AcDbBlockReference *pBlkRef =new AcDbBlockReference(tmpPt, objId) ; 
	pBlkRef->setScaleFactors(1.0);
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
	AcDbObjectId blkId = pBlkRef->objectId();	
	blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
	m_objIdArr.append(blkId);

	//绘制RF4
	AcGePoint3d insertPt;
	insertPt.x = tmpPt.x - m_FrameInfo.getLineLength()/2;
	insertPt.y = tmpPt.y - 6;
	insertPt.z = 0;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//绘制特性文字
	AcGePoint3d textPt;
	textPt.x = tmpPt.x + (12+42)/2.0;
	textPt.y = tmpPt.y - (12+6)/2.0;
	double dLen = 0;
	strTexing = MyParserString::RemoveSymbol(_T("]"), strTexing);
	strTexing = MyParserString::RemoveRightSymbol(_T('<'), strTexing);
	if ((strTexing.CompareNoCase(_T("NC")) == 0) || (strTexing.IsEmpty()))
	{

	}
	else
	{
		textId = MyDrawEntity::DrawText(textPt, strTexing, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_FrameInfo.getRfSecondTableLength())
		{
			double dScale = 3.0*m_FrameInfo.getRfSecondTableLength()/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
	}
	if (!strQuxiang.IsEmpty())
	{
		textPt.x = tmpPt.x + (42+60)/2;
		textPt.y = tmpPt.y - (12+6)/2;
		textId = MyDrawEntity::DrawText(textPt, strQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_FrameInfo.getRfThirdTableLength())
		{
			double dScale = 3.0*m_FrameInfo.getRfThirdTableLength()/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
	}				

	//绘制斜线和竖着的线
	AcGePoint3d pt1,pt2,pt3;
	pt1.x = insertPt.x - 5;
	pt1.y = insertPt.y;
	pt1.z = insertPt.z;
	acutPolar(asDblArray(pt1), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(pt2));
	AcDbObjectId lineId = MyDrawEntity::DrawLine(pt1, pt2, m_layerId);

	insertPt.x = pt1.x + 10;
	lineId = MyDrawEntity::DrawLine(pt1, insertPt, m_layerId);
	//绘制长线
	if (m_bIsDrawVerLine)
	{
		pt1 = pt2;
		AcGePoint3d midPt;
		midPt.x = pt1.x;
		midPt.y = m_InsertPt.y + m_FrameInfo.getSpaceVerGap() + 5;

		pt2.x = pt1.x + 5;
		pt2.y = midPt.y - 5;

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
				//MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
				//MyDrawEntity::DrawText(pt2, _T("接上页"), 2.5);
			}
			else
			{
				plineId = MyDrawEntity::DrawPlineByTwoPoint(pt3, pt2, m_layerId);
				plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);

				pt3.y +=1;
				//MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
			}
		}
		m_bIsDrawHorLine = false;
		m_bIsDrawVerLine = false;
	}

	tmpPt.y -= 15.5;
}

void CDrawLrmvEnt::drawGLEnt(AcGePoint3d tmpPt, CString strNumber, vector<CCommonCell> tmpVec)
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

	AcGePoint3d numPt,blkPt;
	blkPt = tmpPt;
	numPt = tmpPt;
	numPt.x += 8;
	AcDbObjectId textId = MyDrawEntity::DrawText(numPt, strNumber);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);

	tmpPt.x = m_basePt.x;
	
	if (tmpVec.size() > 24)
	{
		vector<CCommonCell> tempVec;
		tempVec.insert(tempVec.end(), tmpVec.begin(), tmpVec.begin() + 24);
		drawGlContent(tmpPt, tempVec, true);

		tmpPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		tempVec.clear();
		tempVec.insert(tempVec.end(), tmpVec.begin()+24, tmpVec.end());

		blkPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		AcDbBlockReference *pBlkRef2 =new AcDbBlockReference(blkPt, objId) ; 
		pBlkRef2->setScaleFactors(1.0);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef2, acdbHostApplicationServices()->workingDatabase());
		AcDbObjectId blkId2 = pBlkRef2->objectId();	
		blkId2 = MyEditEntity::openEntChangeLayer(blkId2, m_layerId);

		numPt.x += m_FrameInfo.getTableLength() + m_FrameInfo.getHorSpace();
		textId = MyDrawEntity::DrawText(numPt, strNumber);
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		drawGlContent(tmpPt, tempVec, false);
		m_glMaxPt = tmpPt;
		m_glMaxPt.y += (24+2.5)*m_FrameInfo.getTableHeight();

	}
	else
	{
		drawGlContent(tmpPt, tmpVec, true);
		m_glMaxPt = tmpPt;
		m_glMaxPt.y += (tmpVec.size()+2.5)*m_FrameInfo.getTableHeight();
	}
	tmpVec.clear();
}

void CDrawLrmvEnt::drawGlContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec, bool bDrawTitle)
{
	CCommonCell comCell;
	bool bIsFirst = true;
	int nIndex = 0;
	int nSize = tmpVec.size();
	for (vector<CCommonCell>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		comCell = *iter;
		if (bIsFirst)
		{
			tmpPt.y += m_FrameInfo.getTableHeight() + 2;
			bIsFirst = false;
		}

		/*if (bDrawTitle)
		{*/

		/*}*/
		/*int nCount = comCell.getCount();
		CString strXianhao;
		strXianhao.Format(_T("%d"), nCount);*/

		gQuery.getQuxiang(comCell);
		gQuery.addCellInfo(comCell);

		if (m_bIsDrawMod)
		{
			if (comCell.getChineseRealTexing().IsEmpty() || (comCell.getChineseRealTexing().CompareNoCase(_T("NA")) == 0))
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getModQuxiang(), _T("0"), false);
			else
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getModQuxiang(), _T("0"));
		}
		else
		{
			if ((comCell.getChineseRealTexing().IsEmpty() || (comCell.getChineseRealTexing().CompareNoCase(_T("NA")) == 0) || (comCell.getQuxiang().IsEmpty())) && !gQuery.getPreviewFlag())
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getQuxiang(), _T("0"), false);
			else
				drawContent(tmpPt, comCell.getDuanzi(), comCell.getChineseRealTexing(), comCell.getQuxiang(), _T("0"));
		}
		tmpPt.y += m_FrameInfo.getTableHeight();

		if (nIndex == nSize-1)
		{
			drawContent(tmpPt, _T("端子"), _T("特性"), _T("去向"), _T(""), false);
			tmpPt.y += m_FrameInfo.getTableHeight();
		}
		nIndex++;
	}	
	tmpVec.clear();
}

void CDrawLrmvEnt::drawGlExts(CString strText)
{
	double dFirstTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第一段长度"), 0);
	double dSecondTableLength =  gGlobal.GetIniValue(_T("基础设置"), _T("表格第二段长度"), 0);
	double dThirdTableLength  = gGlobal.GetIniValue(_T("基础设置"), _T("表格第三段长度"), 0);
	double dTableLength = dFirstTableLength + dSecondTableLength + dThirdTableLength;
	m_glMaxPt.x += dTableLength;
	m_glMaxPt.x += 0.5;
	m_glMinPt.y += 4;
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
	
	AcDbObjectId textId = MyDrawEntity::DrawText(pt2, strText, 3.0, textStyleId, AcDb::kTextLeft, AcDb::kTextBottom);
}

void CDrawLrmvEnt::setModFlag(bool bIsMod)
{
	m_bIsDrawMod = bIsMod;
}

void CDrawLrmvEnt::setCount(int nCount)
{
	m_nCount = nCount;
}

void CDrawLrmvEnt::setLastDrawFlag(bool bIsLast)
{
	m_bIsLastDraw = bIsLast;
}

void CDrawLrmvEnt::drawGL(AcGePoint3d tmpPt, CCommonCell comCell)
{
	CString strDuanzi,strTexing,strQuxiang, strXianhao;
	strDuanzi = comCell.getDuanzi();
	strTexing = comCell.getChineseRealTexing();

	gQuery.getQuxiang(comCell);
	
	if (m_bIsDrawMod)
	{
		strQuxiang = comCell.getModQuxiang();
	}
	else
	{
		strQuxiang = comCell.getQuxiang();
	}
	if (!comCell.getModName().IsEmpty())
	{
		strDuanzi = m_strRealName + _T(":") + strDuanzi;
	}
	AcDbObjectId textId = MyDrawEntity::DrawText(m_basePt, strDuanzi);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);

	//绘制RF1
	CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF1.dwg");

	AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

	AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_basePt, objId) ; 
	pBlkRef->setScaleFactors(1.0);
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
	AcDbObjectId blkId = pBlkRef->objectId();	
	blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
	m_objIdArr.append(blkId);

	//绘制RF4
	AcGePoint3d insertPt;
	insertPt.x = m_basePt.x - m_FrameInfo.getLineLength()/2;
	insertPt.y = m_basePt.y - 6;
	insertPt.z = 0;

	strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF4.dwg");
	objId = MyDrawEntity::GetBlkRef(strFile);
	AcDbBlockReference *pBlkRef1 =new AcDbBlockReference(insertPt, objId) ; 
	pBlkRef1->setScaleFactors(1.0);
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef1, acdbHostApplicationServices()->workingDatabase());
	blkId = pBlkRef1->objectId();	
	blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
	//m_objIdArr.append(blkId);
	//绘制RF3上的文字
	strXianhao = comCell.getCount();
	if (!strXianhao.IsEmpty())
	{
		AcGePoint3d xianhaoPt;
		xianhaoPt.x = m_basePt.x - m_FrameInfo.getLineLength()/2;
		xianhaoPt.y = m_basePt.y;
		xianhaoPt.z = 0;
		textId = MyDrawEntity::DrawText(xianhaoPt, strXianhao, 3.0, AcDbObjectId::kNull, AcDb::kTextCenter);
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//绘制特性文字
	AcGePoint3d textPt;
	textPt.x = m_basePt.x + (12+42)/2.0;
	textPt.y = m_basePt.y - (12+6)/2.0;
	double dLen = 0;
	strTexing = MyParserString::RemoveRightSymbol(_T('<'), strTexing);
	if ((strTexing.CompareNoCase(_T("NC")) == 0) || (strTexing.IsEmpty()))
	{

	}
	else
	{
		textId = MyDrawEntity::DrawText(textPt, strTexing, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_FrameInfo.getRfSecondTableLength())
		{
			double dScale = 3.0*m_FrameInfo.getRfSecondTableLength()/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
	}
	if (!strQuxiang.IsEmpty())
	{
		textPt.x = m_basePt.x + (42+60)/2;
		textPt.y = m_basePt.y - (12+6)/2;
		textId = MyDrawEntity::DrawText(textPt, strQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_FrameInfo.getRfThirdTableLength())
		{
			double dScale = 3.0*m_FrameInfo.getRfThirdTableLength()/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
	}				

	//绘制斜线和竖着的线
	AcGePoint3d pt1,pt2,pt3;
	pt1.x = insertPt.x - 5;
	pt1.y = insertPt.y;
	pt1.z = insertPt.z;
	acutPolar(asDblArray(pt1), 5*PI/4, m_FrameInfo.getLineLength()/3, asDblArray(pt2));
	AcDbObjectId lineId = MyDrawEntity::DrawLine(pt1, pt2, m_layerId);
	m_objIdArr.append(lineId);

	AcGePoint3d midPt;
	//绘制长线
	if (m_bIsDrawVerLine)
	{
		pt1 = pt2;

		midPt.x = pt1.x;
		midPt.y = m_InsertPt.y + m_FrameInfo.getSpaceVerGap() + 5;

		pt2.x = pt1.x + 5;
		pt2.y = midPt.y - 5;

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
				if (!m_bIsLastDraw)
					MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
				MyDrawEntity::DrawText(pt2, _T("接上页"), 2.5);
			}
			else
			{
				plineId = MyDrawEntity::DrawPlineByTwoPoint(pt3, pt2, m_layerId);
				plineId = MyEditEntity::openPlineChangeWidth(plineId, 0.4);

				pt3.y +=1;
				if (!m_bIsLastDraw)
					MyDrawEntity::DrawText(pt3, _T("接下页"), 2.5, AcDbObjectId::kNull, AcDb::kTextRight);
			}
		}
		m_bIsDrawHorLine = false;
		m_bIsDrawVerLine = false;
	}
	
	return;
}
