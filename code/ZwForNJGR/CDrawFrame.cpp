#include "StdAfx.h"
#include "CDrawFrame.h"
#include "BaseUtils.h"
#include "Utility.h"
#include "CGlobalRWriteFile.h"

extern CGlobalRWriteFile gRWriteFile;

CDrawFrame::CDrawFrame(void)
{
	m_TextStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("0"));
	m_bIsAdded = true;
}

CDrawFrame::~CDrawFrame(void)
{

}

bool CDrawFrame::draw()
{	
	m_InsertPt = getPoint();
	drawFrame(m_InsertPt);
	return true;
}

void CDrawFrame::setAddFlag(bool bIsAdded /*= true*/)
{
	m_bIsAdded = bIsAdded;
}

AcGePoint3d CDrawFrame::getInsertPt()
{
	return m_InsertPt;
}


AcGePoint3d CDrawFrame::getContentPt()
{
	AcGePoint3d pt;
	CString strFrameType = m_FrameIni.getFrameType();
	double m_dFrameLength,m_dFrameHeight, dTemp;
	if (strFrameType.IsEmpty())
	{
		strFrameType = _T("A1");
	}
	CUtility::getIniFromFile(strFrameType, m_dFrameLength, m_dFrameHeight, dTemp);
	
	acutPolar(asDblArray(m_InsertPt), PI, m_dFrameLength - m_FrameIni.getLeftHorGap(), asDblArray(pt));
	acutPolar(asDblArray(pt), PI/2, m_dFrameHeight - m_FrameIni.getLeftVerGap() - m_FrameIni.getTableHeight(), asDblArray(pt));
	return pt;
}


void CDrawFrame::setFrameIni(CString strSheetName, CFrameInfo frameIni, int nCount)
{
	m_FrameIni = frameIni;
	
	m_strSheetName = strSheetName;	
	if (nCount == -1)
	{
		m_nCount = CBaseUtils::frameCount();
		m_bIsAdded = true;
	}
	else
	{
		m_nCount = nCount;
		m_bIsAdded = false;
	}
	m_InsertPt = getPoint();
}

AcGePoint3d CDrawFrame::getPoint()
{
	int m_nHxCount = CBaseUtils::hxFrameCount();
	double m_dFrameVerSpace = CBaseUtils::verFrameSpace();
	double m_dFrameHorSpace = CBaseUtils::horFrameSpace();
	double m_dFrameLength = 0.0;
	double m_dFrameHeight = 0.0;
	double dTemp = 0.0;
	CUtility::getIniFromFile(_T("A0"), m_dFrameLength, m_dFrameHeight, dTemp);
	int nYCount = (m_nCount-1)/m_nHxCount;
	int nXCount = m_nCount%m_nHxCount;

	AcGePoint3d insertPt;
	insertPt.set(0, 0, 0);
	acutPolar(asDblArray(insertPt), 3*PI/2, (nYCount)*(m_dFrameVerSpace+m_dFrameHeight), asDblArray(insertPt));

	if (nXCount == 0)
	{
		nXCount = m_nHxCount;
	}
	acutPolar(asDblArray(insertPt), 0, (nXCount-1)*(m_dFrameHorSpace+m_dFrameLength), asDblArray(insertPt));

	return insertPt;
}

bool CDrawFrame::drawFrame(AcGePoint3d basePt)
{
	CString strFile = MyBaseUtils::GetAppPath() + _T("图框\\") + m_FrameIni.getFrameType() + _T(".dwg");

	AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

	AcDbBlockReference *pBlkRef =new AcDbBlockReference(basePt, objId) ; 
	pBlkRef->setScaleFactors(1.0);
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
	AcDbObjectId layerId = MySymble::CreateNewLayer(m_strSheetName, 7);
	acdbHostApplicationServices()->workingDatabase()->setClayer(layerId);
	AcDbObjectId blkId = pBlkRef->objectId();	
	blkId = MyEditEntity::openEntChangeLayer(blkId, layerId);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	MyEditEntity::OpenObjAppendIntToXdata(blkId, ICD_COUNT, m_nCount);
	MyEditEntity::OpenObjAppendStrToXdata(blkId, ICD_LRM_NAME, m_FrameIni.getLrmName());
	MyEditEntity::OpenObjAppendStrToXdata(blkId, ICD_SHEETNAME, m_strSheetName);

	MyEditEntity::OpenObjAppendStrToXdata(blkId, ICD_FRAMETYPE, m_FrameIni.getFrameType());
	MyEditEntity::OpenObjAppendDoubleToXdata(blkId, ICD_HORSPACE, m_FrameIni.getHorSpace());
	MyEditEntity::OpenObjAppendDoubleToXdata(blkId, ICD_LINELENGTH, m_FrameIni.getLineLength());
	MyEditEntity::OpenObjAppendDoubleToXdata(blkId, ICD_VerSapce, m_FrameIni.getVerSpace());
	//MyEditEntity::OpenObjAppendDoubleToXdata(blkId, ICD_SPAREHORGAP, m_FrameIni.getSpaceHorGap());
	MyEditEntity::OpenObjAppendDoubleToXdata(blkId, ICD_SPAREVERGAP, m_FrameIni.getSpaceVerGap());
	MyEditEntity::OpenObjAppendDoubleToXdata(blkId, ICD_LEFTVERGAP, m_FrameIni.getLeftVerGap());
	MyEditEntity::OpenObjAppendDoubleToXdata(blkId, ICD_LEFTHORGAP, m_FrameIni.getLeftHorGap());											
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	drawBmText(basePt);
	drawCountText(basePt);

	drawDesigner(basePt);
	drawProName(basePt);

	drawVerifier(basePt);
	drawFirstEmpty(basePt);
	drawStandard(basePt);
	drawSecondEmpty(basePt);
	drawApproval(basePt);
	drawGzj(basePt);
	drawProGrade(basePt);
	drawProStage(basePt);
	return true;
}

bool CDrawFrame::drawBmText(AcGePoint3d basePt)
{
	CString strFrameType = m_FrameIni.getFrameType();
	double dFrameLength,dFrameHeight,dTemp;
	if (strFrameType.IsEmpty())
	{
		strFrameType = _T("A1");
	}
	CUtility::getIniFromFile(strFrameType, dFrameLength, dFrameHeight, dTemp);
	//绘制上面的bm文字
	AcGePoint3d pt, tmpPt;
	double dTextSize = 0;
	double dX = 0.0;
	double dY = 0.0;
	CUtility::getIniFromFile(_T("BMUP"), dX, dY, dTextSize);
	pt.x = basePt.x - dFrameLength + dX;//距左上角点横向的位置
	pt.y = basePt.y + dFrameHeight - dY;
	pt.z = 0.0;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	AcDbObjectId textId = MyDrawEntity::DrawText(pt, m_frameTitle.getBmText(), dTextSize, m_TextStyleId, AcDb::kTextMid);
	textId = MyEditEntity::openEntChangeRotation(textId, PI);	
	//绘制下面的bm文字
	pt = drawBasicText(_T("BMDOWN"), dTextSize);
	textId = MyDrawEntity::DrawText(pt, m_frameTitle.getBmText(), dTextSize, m_TextStyleId, AcDb::kTextMid);

	return true;
}

bool CDrawFrame::drawCountText(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	double dTextSize = 0;
	pt = drawBasicText(_T("单张"), dTextSize);
	
	AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("2"));
	
	CString strCount;
	strCount.Format(_T("%d"), m_nCount);
	MyDrawEntity::DrawText(pt, strCount, dTextSize, textStyleId, AcDb::kTextMid);
	//MyBaseUtils::SetVar(_T("USERI1"), nCount);
	if (m_bIsAdded)
	{
		m_nCount++;
		CBaseUtils::setFrameCount(m_nCount);
	}

	return true;
}


void CDrawFrame::drawProName(AcGePoint3d basePt)
{
	//绘制上面的产品名称文字
	AcGePoint3d pt;
	CString strText;
	strText = _T("电路图");
	double dTextSize = 0;
	pt = drawBasicText(_T("产品名称"), dTextSize);
	
	strText =  m_frameTitle.getProName();
	strText += _T("\n");
	strText += _T("电路图");

	MyDrawEntity::DrawMText(pt, strText, dTextSize, m_TextStyleId, AcDbMText::kMiddleCenter);
}

void CDrawFrame::drawDesigner(AcGePoint3d basePt)
{
	//绘制上面的产品名称文字
	AcGePoint3d pt;
	CString strText;
	double dTextSize = 0;
	pt = drawBasicText(_T("设计"), dTextSize);
	time_t timer; 
	time(&timer); 
	tm* t_tm = localtime(&timer);  

	CString strTime;
	strTime.Format(_T(" %d-%02d-%02d"), t_tm->tm_year+1900, t_tm->tm_mon + 1, t_tm->tm_mday);
	strText = m_frameTitle.getDesigner() + strTime;

	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, dTextSize, m_TextStyleId, AcDb::kTextMid);
}

void CDrawFrame::drawVerifier(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getVerifier();
	double dTextSize = 0;
	pt = drawBasicText(_T("审核"), dTextSize);
	
	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, dTextSize, m_TextStyleId, AcDb::kTextMid);
}

void CDrawFrame::drawFirstEmpty(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getFirstEmpty();
	pt.x = basePt.x - (172+130)/2;
	pt.y = basePt.y + (25+30)/2;
	
	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, 5.0, m_TextStyleId, AcDb::kTextMid);
}

void CDrawFrame::drawStandard(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getStandard();
	double dTextSize = 0;
	pt = drawBasicText(_T("标准"), dTextSize);
	
	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, dTextSize, m_TextStyleId, AcDb::kTextMid);
}

void CDrawFrame::drawSecondEmpty(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getSencondEmpty();
	pt.x = basePt.x - (172+130)/2;
	pt.y = basePt.y + (15+20)/2;
	
	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, 5.0, m_TextStyleId, AcDb::kTextMid);
}

void CDrawFrame::drawApproval(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getApproval();
	double dTextSize = 0;
	pt = drawBasicText(_T("批准"), dTextSize);
	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, dTextSize, m_TextStyleId, AcDb::kTextMid);
}


void CDrawFrame::drawGzj(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getGZJ();
	double dTextSize = 0;
	pt = drawBasicText(_T("关重件"), dTextSize);
	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, dTextSize, m_TextStyleId, AcDb::kTextMid);
}

void CDrawFrame::drawProGrade(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getProGrade();
	double dTextSize = 0;
	pt = drawBasicText(_T("项目等级"), dTextSize);

	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, dTextSize, m_TextStyleId, AcDb::kTextMid);
}

void CDrawFrame::drawProStage(AcGePoint3d basePt)
{
	AcGePoint3d pt;
	CString strText;
	strText = m_frameTitle.getProStage();
	double dTextSize = 0;
	pt = drawBasicText(_T("项目阶段"), dTextSize);

	AcDbObjectId textId = MyDrawEntity::DrawText(pt, strText, dTextSize, m_TextStyleId, AcDb::kTextMid);
}

AcGePoint3d CDrawFrame::drawBasicText(CString strDescription, double& dSize)
{
	AcGePoint3d pt;
	double dX = 0.0;
	double dY = 0.0;
	CUtility::getIniFromFile(strDescription, dX, dY, dSize);
	pt.x = m_InsertPt.x- dX;
	pt.y = m_InsertPt.y + dY;
	pt.z = 0;
	return pt;
}
