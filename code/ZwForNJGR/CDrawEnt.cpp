#include "StdAfx.h"
#include "CDrawEnt.h"
#include "CNameCell.h"
#include "CCommonCell.h"
#include "DlgTKSZ.h"
#include "Utility.h"
#include "BaseUtils.h"
#include "CDrawDotted.h"
#include "CGlobalQuery.h"
#include "CDrawFrame.h"
#include "CGlobalRWriteFile.h"
#include <algorithm>
extern CGlobalQuery gQuery;
extern CGlobalRWriteFile gRWriteFile;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDrawEntity::CDrawEntity(void)
{
	m_objIdArr.removeAll();
	m_bIsDrawMod = false;
	m_dictGroup.clear();
	m_nCount = -1;
	m_bIsNewFrame = true;
	m_bIsHor = false;
}

CDrawEntity::~CDrawEntity(void)
{
	m_objIdArr.removeAll();
	m_dictGroup.clear();
}

void CDrawEntity::Init()
{
	//m_dTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格长度"), 0);
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
	
	//m_dFirstTableLength = m_dTableLength/10;
	//m_dSecondTableLength = m_dTableLength/2;
	m_dFirstTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第一段长度"), 0);
	m_dSecondTableLength =  gGlobal.GetIniValue(_T("基础设置"), _T("表格第二段长度"), 0);
	m_dThirdTableLength  = gGlobal.GetIniValue(_T("基础设置"), _T("表格第三段长度"), 0);
	m_dTableLength = m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength;

	m_dRfSecondTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第二段长度"), 0);
	m_dRfThirdTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第三段长度"), 0);
	//m_dGap = gGlobal.GetIniValue(_T("基础设置"), _T("间隙"), 0);

}



bool CDrawEntity::drawFrame()
{
	//m_FrameInfo = gRWriteFile.getFrameInfoByName(data.getLrmName());普通实体没有LRM
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
	//设置数据
	m_dHorSpace = m_FrameInfo.getHorSpace();//表格横向间距
	m_dVerSapce =  m_FrameInfo.getVerSpace();//表格纵向间距
	m_dSpareHorGap =  m_FrameInfo.getSpaceHorGap();// 备用横向长
	m_dSpareVerGap = m_FrameInfo.getSpaceVerGap();//备用纵向长
	m_dLeftHorGap =  m_FrameInfo.getLeftHorGap();//左上横向长
	m_dLeftVerGap =  m_FrameInfo.getLeftVerGap();//左上纵向长
	m_dGap =  m_FrameInfo.getGap();//间隙
	CString strFrameType = m_FrameInfo.getFrameType();
	double dTemp = 0.0;
	if (strFrameType.IsEmpty())
	{
		strFrameType = _T("A1");
	}
	CUtility::getIniFromFile(strFrameType, m_dFrameLength, m_dFrameHeight, dTemp);
	CDrawFrame dlg;
	dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
	dlg.setAddFlag(m_bIsNewFrame);
	dlg.draw();

	m_InsertPt = dlg.getInsertPt();
	m_basePt = dlg.getContentPt();	
	m_bIsDrawHorLine = true;
	m_bIsDrawVerLine = true;	
	if (m_nCount != -1)
	{
		m_nCount++;
	}
	return true;
}

void CDrawEntity::setNewFrameFlag(bool bIsAdded /*= true*/)
{
	m_bIsNewFrame = bIsAdded;
}

void CDrawEntity::setCount(int nCount)
{
	m_nCount = nCount;
}

bool CDrawEntity::draw(map<CNameCell, vector<CCommonCell> > mapInfo)
{

	CNameCell namecell;
	vector<CCommonCell> comVec;
	comVec.clear();
	//插入第一个图框	

	for (map<CNameCell, vector<CCommonCell> >::iterator iter = mapInfo.begin(); iter != mapInfo.end(); ++iter)
	{
		namecell = iter->first;
		comVec = iter->second;

		drawTable(namecell, comVec);
		m_strPreName = namecell.getSpecailaName();
		comVec.clear();
	}

	mapInfo.clear();
	//绘制虚线框
	for (map<CString, int>::iterator iter = m_dictGroup.begin(); iter != m_dictGroup.end(); ++iter)
	{
		CString strGroupName = iter->first;

		AcDbDictionary *pGroupDict;	
		AcDbGroup* pGroup = NULL;
		AcDbObjectId groupId;
		AcDbObjectId entId = AcDbObjectId::kNull;
		AcDbObjectIdArray objIdarrs;
		acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
		if (pGroupDict->getAt(strGroupName, (AcDbObject*&)pGroup, AcDb::kForRead) != Acad::eOk)
		{
			pGroupDict->close();
			continue;
		}
		else
		{
			pGroup->allEntityIds(objIdarrs);
			pGroup->close();
			pGroupDict->close();
		}
		strGroupName = strGroupName + _T("DOT");

		CDrawDotted dot;
		dot.setGroupName(strGroupName);
		dot.getData(objIdarrs);
		dot.makeGroup();
	}
	m_dictGroup.clear();
	//移动insertPt;
	CBaseUtils::setEndPt(m_basePt);
	CBaseUtils::setStartPt(m_InsertPt);
	return true;
}


AcGePoint3d CDrawEntity::getContentPt(AcGePoint3d insertPt)
{
	AcGePoint3d pt;
	acutPolar(asDblArray(insertPt), PI, m_dFrameLength - m_dLeftHorGap, asDblArray(pt));
	acutPolar(asDblArray(pt), PI/2, m_dFrameHeight - m_dLeftVerGap - m_dTableHeight, asDblArray(pt));
	return pt;
}

bool CDrawEntity::drawTable(CNameCell nameCell, vector<CCommonCell> comVec)
{
	m_layerId = MySymble::CreateNewLayer(m_strSheetName);
	m_bIsDrawXxk = false;
	m_objIdArr.removeAll();
	CString strName = nameCell.getSpecailaName();
	m_strRealName = nameCell.getRealName();
	m_dOffsetDist = -m_dTableHeight;
	double dTemp = 0.0;
	if ((strName.CompareNoCase(_T("其它")) != 0) || (strName.CompareNoCase(_T("其他")) != 0))
	{
		double dDist = 9+7;
		for (vector<CCommonCell>::iterator itr = comVec.begin(); itr != comVec.end(); ++itr)
		{	
			CCommonCell comCell = *itr;
			CString strTemp = comCell.getTexing();
			int ntmp = strTemp.Find(ICD_PROPERTY_RF);
			if (ntmp >= 0)
			{
				dDist += 17;
			}
			ntmp = strTemp.Find(ICD_PROPERTY_GL);
			if (ntmp >=0)
			{
				dDist += 17;
			}
			else
			{
				dDist += 7;
			}
		}
		if (m_strPreName.CompareNoCase(strName) == 0)
		{
			m_bIsHor = true;
		}
		else
		{
			m_bIsHor = false;
		}
		doTablePoint(dDist);
		if (nameCell.getDrawText())
		{
			AcDbObjectId textId = MyDrawEntity::DrawText(m_basePt, strName);
			MyEditEntity::openEntChangeLayer(textId, m_layerId);
			if (m_bIsDrawMod)
			{
				if (!nameCell.getModLjqName().IsEmpty())
				{
					m_basePt.y -= 3.5;
					m_dOffsetDist -= 3.5;
					dTemp -= 3.5;
					textId = MyDrawEntity::DrawText(m_basePt, nameCell.getModLjqName());
					MyEditEntity::openEntChangeLayer(textId, m_layerId);
				}
				if (!nameCell.getModWzNo().IsEmpty())
				{
					m_basePt.y -= 3.5;
					m_dOffsetDist -= 3.5;
					dTemp -= 3.5;
					textId = MyDrawEntity::DrawText(m_basePt, nameCell.getModWzNo());
					MyEditEntity::openEntChangeLayer(textId, m_layerId);
				}
			}
			else
			{
				if (!nameCell.getLjqName().IsEmpty())
				{
					m_basePt.y -= 3.5;
					m_dOffsetDist -= 3.5;
					dTemp -= 3.5;
					textId = MyDrawEntity::DrawText(m_basePt, nameCell.getLjqName());
					MyEditEntity::openEntChangeLayer(textId, m_layerId);
				}
				if (!nameCell.getWzNo().IsEmpty())
				{
					m_basePt.y -= 3.5;
					m_dOffsetDist -= 3.5;
					dTemp -= 3.5;
					textId = MyDrawEntity::DrawText(m_basePt, nameCell.getWzNo());
					MyEditEntity::openEntChangeLayer(textId, m_layerId);
				}
			}
			
		}
	}
	
	if (nameCell.getStatus().CompareNoCase(_T("1")) == 0)//需要加虚线框
	{
		if (nameCell.getDrawText())
		{
			m_basePt.y -= 2;
			m_dOffsetDist -= 2;
		}	
		dTemp -= 2;
		m_bIsDrawXxk = true;
	}
	else	
	{
		m_basePt.y -= 1.5;
		m_dOffsetDist -= 1.5;
		dTemp -= 1.5;
		m_bIsDrawXxk = false;
	}
		//先绘制端子 特性 去向
	CString strDuanzi,strTexing,strQuxiang,strXianhao;
	bool bIsTitle = true;
	m_bFirstDrawRf = true;

	for (vector<CCommonCell>::iterator itr = comVec.begin(); itr != comVec.end(); ++itr)
	{	
		CCommonCell comCell = *itr;
		gQuery.getQuxiang(comCell);
		gQuery.addCellInfo(comCell);

		strTexing = comCell.getTexing();

		int nRow, nCol,nType;
		nRow = comCell.getRow();
		nCol = comCell.getCol();
		nType= comCell.getType();

		if (strTexing.Find(ICD_PROPERTY_RF) >= 0)//有射频
		{
			if (nameCell.getStatus().CompareNoCase(_T("1")) == 0)//需要加虚线框
			{
				m_basePt.y -= 1.5;
			}
			else	
			{
				m_basePt.y -= 2;
			}
			CString strTemp = strTexing;
			int nIndex = CUtility::ParserStringComma(strTemp, strTexing);
			if (((comCell.getChineseRealTexing().IsEmpty()) 
				|| (comCell.getChineseRealTexing().CompareNoCase(_T("NC")) == 0)
				|| (comCell.getChineseRealTexing().CompareNoCase(_T("NA")) == 0)
				|| (comCell.getQuxiang().IsEmpty())) 
				&& comCell.getRFFlag())//射频不连接
			{
				doRFPoint();
				drawRF(comCell, false);
			}
			else if (strTexing.Find(_T("[K]")) >= 0)//图形不显示在cad中
			{
				doRFPoint();
				drawK(comCell);
			}
			else//普通的射频
			{
				doRFPoint();
				drawRF(comCell, true);
			}
		}
		else if (strTexing.Find(ICD_PROPERTY_GL) >= 0)//光缆端子
		{
			doRFPoint();
			drawGL(comCell);
		}
		else
		{
			if (m_bIsDrawVerLine && !bIsTitle)
			{
				if (!nameCell.getDrawText())
				{
					m_basePt.y += m_dOffsetDist;
					m_basePt.y -= 7;
				}
			}
			else if (m_bIsDrawVerLine && bIsTitle)
			{
				if (!nameCell.getDrawText())
				{
				    m_basePt.y += dTemp;
				}
			}

			if (bIsTitle)
			{
				getTablePt(nameCell.getDrawText());
				drawContent(_T("端子"), _T("特性"), _T("去向"), _T(""), false);
				bIsTitle = false;
			}
			getTablePt();
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
			
			strXianhao = comCell.getCount();
			
			m_ignoreErrorArr.removeAll();
			if ((strTexing.IsEmpty() || (strTexing.CompareNoCase(_T("NA")) == 0) || (strQuxiang.IsEmpty())) && !gQuery.getPreviewFlag())
				drawContent(strDuanzi, strTexing, strQuxiang, strXianhao, false,nRow,nCol,nType);
			else
				drawContent(strDuanzi, strTexing, strQuxiang, strXianhao,true,nRow,nCol,nType);
			if (!m_bIsDrawMod)
			{
				if (CUtility::IsIgnoreErrorFlag(comCell))
				{
					CUtility::EditignoreError(m_ignoreErrorArr);
				}
			}
		}
		
	}
	//处理坐标点
	doGapPoint();	

	if (m_bIsDrawXxk)
	{
		if (m_objIdArr.length() > 0)
		{
			CString strGroupName;
			CString strTmp;
			strTmp = m_strSheetName + nameCell.getName();
			strGroupName = strTmp + strGroupName;

			//MyEditEntity::EraseEntByGroupName(strGroupName);
			AcDbDictionary *pGroupDict;	
			AcDbGroup* pGroup = NULL;
			AcDbObjectId groupId;
			AcDbObjectId entId = AcDbObjectId::kNull;
			Acad::ErrorStatus es;
			es = acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
			
			if (pGroupDict->getAt(strGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
			{
				pGroup = new AcDbGroup;
				es = pGroupDict->setAt(strGroupName, pGroup, groupId);
				pGroupDict->close();
				pGroup->setSelectable(false);
				for (int i = 0; i < m_objIdArr.length(); i++) 
				{
					entId = m_objIdArr.at(i);
					pGroup->append(entId);
				}
				pGroup->close();
			}
			else
			{
				for (int i = 0; i < m_objIdArr.length(); i++) 
				{
					entId = m_objIdArr.at(i);
					pGroup->append(entId);
				}
				pGroup->close();
				pGroupDict->close();
				groupId = pGroup->objectId();
			}
			m_dictGroup.insert(make_pair(strGroupName, 1));
		}
	}
	comVec.clear();

	return true;
}

void CDrawEntity::setSheetName(CString strSheetName)
{
	m_strSheetName = strSheetName;
	m_bIsMuban = false;
	//CString strType = gGlobal.GetIniValue(_T("基础设置"), _T("母板"));
	//CStringArray strArr;
	//strArr.RemoveAll();
	//MyParserString::GetCommaFromString(strType, strArr);
	//CString strName = m_strSheetName;
	//CString strTmp;
	//int nFind = strName.Find(_T("("));
	//if (nFind > 0)
	//{
	//	m_bIsMuban = true;
	//	/*strName = strName.Left(nFind + 1);
	//	strTmp = strName.Left(nFind);
	//	for (int i=0; i<strArr.GetSize(); i++)
	//	{
	//		CString strTemp = strArr.GetAt(i);
	//		if (strTemp.CompareNoCase(strTmp) == 0)
	//		{
	//			m_bIsMuban = true;
	//			break;
	//		}
	//	}*/
	//}

}

void CDrawEntity::doTablePoint(double dDist)
{
	if (IsStartPt())
	{
		//m_basePt.y -= m_dTableHeight;
	}
	else
	{
		if (m_bIsMuban)
		{
			m_basePt.x += m_dTableLength + m_dHorSpace;
			m_basePt.y = m_InsertPt.y + m_dFrameHeight - m_dLeftVerGap - m_dTableHeight;
			m_bIsDrawVerLine = true;
			if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
			{
				//需要插入一个新的图框
				CDrawFrame dlg;
				dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
				dlg.setAddFlag(m_bIsNewFrame);
				dlg.draw();

				m_InsertPt = dlg.getInsertPt();
				m_basePt = dlg.getContentPt();	
				m_bIsDrawHorLine = true;
				m_bIsDrawVerLine = true;	
			}
		}
		else
		{

			if ((m_basePt.y - m_InsertPt.y - dDist < m_dSpareVerGap + m_dTableHeight + m_dGap)
				||m_bIsHor)//不能向下
			{
				//acutPolar(asDblArray(m_basePt), 0, m_dHorSpace + m_dTableLength, asDblArray(m_basePt));
				m_basePt.x += m_dTableLength + m_dHorSpace;
				m_basePt.y = m_InsertPt.y + m_dFrameHeight - m_dLeftVerGap - m_dTableHeight;
				m_bIsDrawVerLine = true;
				if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
				{
					//需要插入一个新的图框
					//////////////////////////////////////////////////////////////////////////
					CDrawFrame dlg;
					dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
					dlg.setAddFlag(m_bIsNewFrame);
					dlg.draw();

					m_InsertPt = dlg.getInsertPt();
					m_basePt = dlg.getContentPt();	
					m_bIsDrawHorLine = true;
					m_bIsDrawVerLine = true;	
				}
				/*else
				{
				acutPolar(asDblArray(m_basePt), 0, m_dHorSpace + m_dTableLength, asDblArray(m_basePt));
				m_basePt.y = m_InsertPt.y + m_dFrameHeight - 30;
				}*/
			}
			else
			{
				if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
				{
					//需要插入一个新的图框
					//////////////////////////////////////////////////////////////////////////
					CDrawFrame dlg;
					dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
					dlg.setAddFlag(m_bIsNewFrame);
					dlg.draw();

					m_InsertPt = dlg.getInsertPt();
					m_basePt = dlg.getContentPt();	
					m_bIsDrawHorLine = true;
					m_bIsDrawVerLine = true;	
				}
				else
				{
					m_basePt.y -= m_dTableHeight;
				}
			}
		}
		
	}
	
}

void CDrawEntity::getTablePt(bool bIsDrawText)
{
	if (m_basePt.y - m_InsertPt.y < m_dSpareVerGap + m_dTableHeight + m_dGap)//不能向下
	{
		//acutPolar(asDblArray(m_basePt), 0, m_dHorSpace + m_dTableLength, asDblArray(m_basePt));
		m_basePt.x += m_dTableLength + m_dHorSpace;
		m_basePt.y = m_InsertPt.y + m_dFrameHeight - m_dLeftVerGap - m_dTableHeight + m_dOffsetDist;
		m_bIsDrawVerLine = true;
		if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
		{
			//需要插入一个新的图框
			//////////////////////////////////////////////////////////////////////////
			CDrawFrame dlg;
			dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;	
			if (!bIsDrawText)
			{
				m_basePt.y += m_dOffsetDist;
			}
			m_bIsNewFrame = true;
		}
		/*else
		{
		acutPolar(asDblArray(m_basePt), 0, m_dHorSpace + m_dTableLength, asDblArray(m_basePt));
		m_basePt.y = m_InsertPt.y + m_dFrameHeight - 30;
		}*/
	}
	else
	{
		if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
		{
			//需要插入一个新的图框
			//////////////////////////////////////////////////////////////////////////
			CDrawFrame dlg;
			dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;	
			if (!bIsDrawText)
			{
				m_basePt.y += m_dOffsetDist;
			}
			m_bIsNewFrame = true;
		}
		else
		{
			if (!bIsDrawText)
			{
				m_basePt.y -= m_dTableHeight;
			}
			else
			{
				m_basePt.y -= m_dTableHeight;
			}
		}
	}
}

void CDrawEntity::doRFPoint()
{
	if (abs(m_basePt.y - m_InsertPt.y) < m_dSpareVerGap + 17 + m_dGap)//不能向下
	{
		//acutPolar(asDblArray(m_basePt), 0, m_dHorSpace + m_dTableLength, asDblArray(m_basePt));
		m_basePt.x += m_dTableLength + m_dHorSpace;
		m_basePt.y = m_InsertPt.y + m_dFrameHeight - m_dLeftVerGap - m_dTableHeight;
		m_bIsDrawVerLine = true;
		if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
		{
			//需要插入一个新的图框
			//////////////////////////////////////////////////////////////////////////
			CDrawFrame dlg;
			dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;	
		}
	}
	else
	{
		//m_basePt.y -= 15.5;
		if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
		{
			//需要插入一个新的图框
			//////////////////////////////////////////////////////////////////////////
			CDrawFrame dlg;
			dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;	
		}
	}
}

void CDrawEntity::doGapPoint()
{
	if (abs(m_basePt.y - m_InsertPt.y) < m_dSpareVerGap + m_dTableHeight + m_dGap)//不能向下
	{
		m_bIsDrawVerLine = true;
		if (m_InsertPt.x - m_basePt.x  < m_dSpareHorGap + m_dTableLength)//不能向右
		{
			//需要插入一个新的图框
			//////////////////////////////////////////////////////////////////////////
			CDrawFrame dlg;
			dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;	
		}
		else
		{
			//acutPolar(asDblArray(m_basePt), 0, m_dHorSpace + m_dTableLength, asDblArray(m_basePt));
			//m_basePt.y = m_InsertPt.y + m_dFrameHeight - m_dLeftVerGap - m_dTableHeight;
		}
	}
	else
	{
		if (m_InsertPt.x - m_basePt.x < m_dSpareHorGap + m_dTableLength)//不能向右
		{
			//需要插入一个新的图框
			CDrawFrame dlg;
			dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;	
		}
		else
		{
			m_basePt.y -= m_dVerSapce;
		}
	}
}

//void CDrawEntity::drawCommon(CString strDuanzi, CString strTexing, CString strQuxiang, bool bDrawLine = true)
//{
//	
//	
//	drawContent(strDuanzi, strTexing, strQuxiang, bDrawLine);
//
//}

void CDrawEntity::drawContent(CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao, bool bIsDrawLine,int nRow,int nCol,int nType)
{
	m_bFirstDrawRf = true;
	//绘制线  xzh线框等信息做成块 存入端子信息
	AcGePoint3d pt1,pt2,pt3,pt4,pt5,pt6,pt7;
	AcDbObjectId blkTblRecId = CUtility::getContentBlkRef();
	AcDbBlockReference* pblRef = new AcDbBlockReference;
	pblRef->setBlockTableRecord(blkTblRecId);
	pblRef->setPosition(m_basePt);
	pblRef->setLayer(m_layerId);
	MyBaseUtils::addToCurrentSpaceAndClose(pblRef);

	//xzh线框块中写入信息
	MyBaseAppXdata::setXdata(ICD_XDATA_TYPE, nType, pblRef->objectId());
	MyBaseAppXdata::setXdata(ICD_XDATA_SHEET, m_strSheetName, pblRef->objectId());
	MyBaseAppXdata::setXdata(ICD_XDATA_ROW, nRow, pblRef->objectId());
	MyBaseAppXdata::setXdata(ICD_XDATA_COL, nCol, pblRef->objectId());
	MyBaseAppXdata::setXdata(ICD_XDATA_DUANZI, strDuanzi, pblRef->objectId());
	MyBaseAppXdata::setXdata(ICD_XDATA_TEXING, strTexing, pblRef->objectId());
	MyBaseAppXdata::setXdata(ICD_XDATA_QUXIANG, strQuxiang, pblRef->objectId());
	MyBaseAppXdata::setXdata(ICD_XDATA_XIANHAO, strXianhao, pblRef->objectId());

	m_objIdArr.append(pblRef->objectId());
	m_ignoreErrorArr.append(pblRef->objectId());
	/*acutPolar(asDblArray(m_basePt), 0, m_dTableLength, asDblArray(pt1));
	MyDrawEntity::DrawLine(m_basePt, pt1, m_layerId);
	acutPolar(asDblArray(m_basePt), PI/2, m_dTableHeight, asDblArray(pt2));
	MyDrawEntity::DrawLine(m_basePt, pt2, m_layerId);
	acutPolar(asDblArray(pt2), 0, m_dTableLength, asDblArray(pt3));
	MyDrawEntity::DrawLine(pt2, pt3, m_layerId);
	MyDrawEntity::DrawLine(pt1, pt3, m_layerId);

	acutPolar(asDblArray(m_basePt), 0, m_dFirstTableLength, asDblArray(pt5));
	acutPolar(asDblArray(pt2), 0,m_dFirstTableLength, asDblArray(pt4));
	MyDrawEntity::DrawLine(pt4, pt5, m_layerId);

	acutPolar(asDblArray(pt4), 0, m_dSecondTableLength, asDblArray(pt6));
	acutPolar(asDblArray(pt5), 0, m_dSecondTableLength, asDblArray(pt7));
	MyDrawEntity::DrawLine(pt6, pt7, m_layerId);*/
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	AcGePoint3d textPt1,textPt2,textPt3;
	textPt1.x = m_basePt.x + m_dFirstTableLength/2;
	textPt1.y = m_basePt.y + m_dTableHeight/2;

	textPt2.x = m_basePt.x + m_dFirstTableLength + m_dSecondTableLength/2;
	textPt2.y = m_basePt.y + m_dTableHeight/2;

	textPt3.x = m_basePt.x + m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength/2;
	textPt3.y = m_basePt.y + m_dTableHeight/2;

	textPt1.z = textPt2.z = textPt3.z = 0;

	AcDbObjectId textId = MyDrawEntity::DrawText(textPt1, strDuanzi, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
	double dTextLen = MyEditEntity::OpenObjAndGetLength(textId);
	if (dTextLen > m_dFirstTableLength)
	{
		double dScale = 3.0*m_dFirstTableLength/(dTextLen+1);
		textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
	}
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);
	m_ignoreErrorArr.append(textId);
	if (!strTexing.IsEmpty())
	{
		strTexing = MyParserString::RemoveRightSymbol(_T('<'), strTexing);
		textId = MyDrawEntity::DrawText(textPt2, strTexing, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		//xzh 特性text中写入特性标记
		MyBaseAppXdata::setXdata(ICD_XDATA_TEXINGTAG, _T("是"),textId);
		MyBaseAppXdata::setXdata(ICD_XDATA_TEXING, strTexing, textId);

		double dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_dSecondTableLength)
		{
			double dScale = 3.0*m_dSecondTableLength/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
		m_ignoreErrorArr.append(textId);
	}

	if (!strQuxiang.IsEmpty())
	{
		textId = MyDrawEntity::DrawText(textPt3, strQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
		//xzh 去向text中写入去向标记
		MyBaseAppXdata::setXdata(ICD_XDATA_QUXIANGTAG, _T("是"), textId);
		MyBaseAppXdata::setXdata(ICD_XDATA_QUXIANG, strQuxiang, textId);

		double dLen = MyEditEntity::OpenObjAndGetLength(textId);
		if (dLen > m_dThirdTableLength)
		{
			double dScale = 3.0*m_dThirdTableLength/(dLen+1);
			textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
		}
		textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
		m_objIdArr.append(textId);
		m_ignoreErrorArr.append(textId);
	}

	if (bIsDrawLine)
	{
		AcGePoint3d startPt,midPt, endPt;
		startPt.x = m_basePt.x;
		startPt.y = m_basePt.y + m_dTableHeight/2;

		midPt.x = startPt.x - m_dLineLength;
		midPt.y = startPt.y;
		startPt.z = midPt.z = 0;
		AcDbObjectId lineId = MyDrawEntity::DrawLine(startPt, midPt, m_layerId);

		acutPolar(asDblArray(midPt), 5*PI/4, m_dLineLength/3, asDblArray(endPt));
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
				double dScale = 3.0*m_dLineLength/(dLen+1);
				textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
			}
			textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
			m_objIdArr.append(textId);
		}
		//绘制长线
		if (m_bIsDrawVerLine)
		{
			pt1 = endPt;

			midPt.x = pt1.x;
			midPt.y = m_InsertPt.y + m_dSpareVerGap + 5;
			
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
				pt3.x = m_InsertPt.x - m_dSpareHorGap;
				pt3.y = pt2.y;
				pt3.z = 0;
				int nIndex = getCount();
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
		m_bIsDrawVerLine = false;
	}
}

void CDrawEntity::drawRF(CCommonCell comCell, bool bisRF /*= true*/)
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
	if (m_bFirstDrawRf)
	{
		m_bFirstDrawRf = false;
		m_basePt.y -= 5;
	}
	if (!comCell.getModName().IsEmpty())
	{
		strDuanzi = m_strRealName + _T(":") + strDuanzi;
	}
	AcDbObjectId textId = MyDrawEntity::DrawText(m_basePt, strDuanzi);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);
	m_basePt.y -= 1.5;
	if (bisRF)//绘制RF3
	{
		//绘制RF1
		CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF1.dwg");

		AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

		AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_basePt, objId) ; 
		pBlkRef->setScaleFactors(1.0);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		AcDbObjectId blkId = pBlkRef->objectId();	
		blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
		m_objIdArr.append(blkId);

		//绘制RF3
		AcGePoint3d insertPt;
		insertPt.x = m_basePt.x - m_dLineLength/2;
		insertPt.y = m_basePt.y - 6;
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
			xianhaoPt.x = m_basePt.x - m_dLineLength/2;
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
		strTexing = MyParserString::RemoveRightSymbol(_T('<'), strTexing);
		double dLen = 0;
		if ((strTexing.CompareNoCase(_T("NC")) == 0) || (strTexing.IsEmpty()))
		{

		}
		else
		{
			textId = MyDrawEntity::DrawText(textPt, strTexing, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
			dLen = MyEditEntity::OpenObjAndGetLength(textId);
			if (dLen > m_dRfSecondTableLength)
			{
				double dScale = 3.0*m_dRfSecondTableLength/(dLen+1);
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
			if (dLen > m_dRfThirdTableLength)
			{
				double dScale = 3.0*m_dRfThirdTableLength/(dLen+1);
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
		acutPolar(asDblArray(pt1), 5*PI/4, m_dLineLength/3, asDblArray(pt2));
		AcDbObjectId lineId = MyDrawEntity::DrawLine(pt1, pt2, m_layerId);
		m_objIdArr.append(lineId);
		//绘制长线
		if (m_bIsDrawVerLine)
		{
			pt1 = pt2;
			AcGePoint3d midPt;
			midPt.x = pt1.x;
			midPt.y = m_InsertPt.y + m_dSpareVerGap + 5;

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
				pt3.x = m_InsertPt.x - m_dSpareHorGap;
				pt3.y = pt2.y;
				pt3.z = 0;
				int nIndex = getCount();
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
		m_bIsDrawVerLine = false;
	}
	else
	{
		//绘制RF2
		CString strFile = MyBaseUtils::GetAppPath() + _T("DWG\\RF\\RF2.dwg");

		AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);

		AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_basePt, objId) ; 
		pBlkRef->setScaleFactors(1.0);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		AcDbObjectId blkId = pBlkRef->objectId();	
		blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
		m_objIdArr.append(blkId);
	}
	m_basePt.y -= 15.5;
}

void CDrawEntity::setModFlag(bool bDrawModFlag)
{
	m_bIsDrawMod = bDrawModFlag;
}

void CDrawEntity::drawTu(vector<CString> tuVec)
{
	CDrawFrame dlg;
	dlg.setFrameIni(_T("(ICD_TU)"), m_FrameInfo, m_nCount);
	dlg.setAddFlag(m_bIsNewFrame);
	dlg.draw();

	double dTemp;

	CUtility::getIniFromFile(m_FrameInfo.getFrameType(), m_dFrameLength, m_dFrameHeight, dTemp);

	m_InsertPt = dlg.getInsertPt();
	m_basePt = dlg.getContentPt();	
	m_bIsDrawHorLine = true;
	m_bIsDrawVerLine = true;	
	AcDbObjectId objId = AcDbObjectId::kNull;
	std::sort(tuVec.begin(),tuVec.end());                    
	//tuVec.erase(unique(tuVec.begin(), tuVec.end()), tuVec.end()); 
	for (vector<CString>::iterator iter = tuVec.begin(); iter != tuVec.end(); ++iter)
	{
		CString strFile = MyBaseUtils::GetAppPath() + _T("TU\\") + *iter + _T(".dwg");
		objId = drawblkRef(strFile);
		if (objId.isNull())
		{
			return;
		}
		
		moveToPt(objId);
	}
	tuVec.clear();
	m_bIsNewFrame = true;
}

int CDrawEntity::getCount()
{
	int nCount = 0;
	//MyBaseUtils::GetVar(_T("USERI1"), &nCount);//记录count值
	//nCount++;
	nCount = CBaseUtils::frameCount();
	return nCount;
}

AcGePoint3d CDrawEntity::getInsertPt()
{
	int nCount = getCount();
	int nYCount = nCount/m_nHxCount;
	int nXCount = nCount%m_nHxCount;
	//移动insertPt;
	if ((nXCount == 1) && (nYCount > 0))
	{
		acutPolar(asDblArray(m_InsertPt), 3*PI/2, m_dFrameVerSpace+m_dFrameHeight, asDblArray(m_InsertPt));
		acutPolar(asDblArray(m_InsertPt), PI, (m_nHxCount-1)*(m_dFrameHorSpace+m_dFrameLength), asDblArray(m_InsertPt));
	}
	else
	{
		acutPolar(asDblArray(m_InsertPt), 0, m_dFrameHorSpace+m_dFrameLength, asDblArray(m_InsertPt));
	}
	return m_InsertPt;
}

bool CDrawEntity::drawGL(CCommonCell comCell)
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
	if (m_bFirstDrawRf)
	{
		m_bFirstDrawRf = false;
		m_basePt.y -= 5;
	}
	if (!comCell.getModName().IsEmpty())
	{
		strDuanzi = m_strRealName + _T(":") + strDuanzi;
	}
	AcDbObjectId textId = MyDrawEntity::DrawText(m_basePt, strDuanzi);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);
	m_basePt.y -= 1.5;
	
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
	insertPt.x = m_basePt.x - m_dLineLength/2;
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
		xianhaoPt.x = m_basePt.x - m_dLineLength/2;
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
		if (dLen > m_dRfSecondTableLength)
		{
			double dScale = 3.0*m_dRfSecondTableLength/(dLen+1);
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
		if (dLen > m_dRfThirdTableLength)
		{
			double dScale = 3.0*m_dRfThirdTableLength/(dLen+1);
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
	acutPolar(asDblArray(pt1), 5*PI/4, m_dLineLength/3, asDblArray(pt2));
	AcDbObjectId lineId = MyDrawEntity::DrawLine(pt1, pt2, m_layerId);

	//绘制长线
	if (m_bIsDrawVerLine)
	{
		pt1 = pt2;
		AcGePoint3d midPt;
		midPt.x = pt1.x;
		midPt.y = m_InsertPt.y + m_dSpareVerGap + 5;

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
			pt3.x = m_InsertPt.x - m_dSpareHorGap;
			pt3.y = pt2.y;
			pt3.z = 0;
			int nIndex = getCount();
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
	m_bIsDrawVerLine = false;

	m_basePt.y -= 15.5;
	return true;
}

void CDrawEntity::drawK(CCommonCell comCell)
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
	if (m_bFirstDrawRf)
	{
		m_bFirstDrawRf = false;
		m_basePt.y -= 5;
	}
	if (!comCell.getModName().IsEmpty())
	{
		strDuanzi = m_strRealName + _T(":") + strDuanzi;
	}
	AcDbObjectId textId = MyDrawEntity::DrawText(m_basePt, strDuanzi);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
	m_objIdArr.append(textId);
	m_basePt.y -= 1.5;

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
	insertPt.x = m_basePt.x - m_dLineLength/2;
	insertPt.y = m_basePt.y - 6;
	insertPt.z = 0;

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
		if (dLen > m_dRfSecondTableLength)
		{
			double dScale = 3.0*m_dRfSecondTableLength/(dLen+1);
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
		if (dLen > m_dRfThirdTableLength)
		{
			double dScale = 3.0*m_dRfThirdTableLength/(dLen+1);
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
	acutPolar(asDblArray(pt1), 5*PI/4, m_dLineLength/3, asDblArray(pt2));
	AcDbObjectId lineId = MyDrawEntity::DrawLine(pt1, pt2, m_layerId);
	
	insertPt.x = pt1.x + 10;
	lineId = MyDrawEntity::DrawLine(pt1, insertPt, m_layerId);
	//绘制长线
	if (m_bIsDrawVerLine)
	{
		pt1 = pt2;
		AcGePoint3d midPt;
		midPt.x = pt1.x;
		midPt.y = m_InsertPt.y + m_dSpareVerGap + 5;

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
			pt3.x = m_InsertPt.x - m_dSpareHorGap;
			pt3.y = pt2.y;
			pt3.z = 0;
			int nIndex = getCount();
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
	m_bIsDrawVerLine = false;

	m_basePt.y -= 15.5;
}

bool CDrawEntity::IsStartPt()
{
	double dTmp = m_dFrameHeight - m_dLeftVerGap - m_dTableHeight;
	bool bRet = abs(m_basePt.y - m_InsertPt.y  - dTmp) < GeTol;
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
bool CDrawEntity::getBlkPt(double dLen, double dHeight)
{
	bool bIsXiangxia = false;

	if (m_dBlkLen < dLen)
	{
		m_dBlkLen = dLen;
	}
	if (m_basePt.y - m_InsertPt.y < m_dSpareVerGap + dHeight)//不能向下
	{
		m_basePt.x += m_dBlkLen + m_dHorSpace;
		m_basePt.y = m_InsertPt.y + m_dFrameHeight - m_dLeftVerGap;
		
		if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + dLen)//不能向右
		{
			////需要插入一个新的图框
			CDrawFrame dlg;
			dlg.setFrameIni(_T("(ICD_TU)"), m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_dBlkLen = dLen;
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;	

		}
		else
		{
			m_dBlkLen = dLen;
			bIsXiangxia = false;
		}
		
	}
	else
	{
		if (m_InsertPt.x  - m_basePt.x < m_dSpareHorGap + m_dBlkLen)//不能向右
		{
			//需要插入一个新的图框
			CDrawFrame dlg;
			dlg.setFrameIni(m_strSheetName, m_FrameInfo, m_nCount);
			dlg.setAddFlag(m_bIsNewFrame);
			dlg.draw();

			m_InsertPt = dlg.getInsertPt();
			m_basePt = dlg.getContentPt();	
			m_dBlkLen = dLen;
			m_bIsDrawHorLine = true;
			m_bIsDrawVerLine = true;
		}
		else
		{
			//m_dBlkHeight = 0;
		}
		bIsXiangxia = true;
		
	}
	return bIsXiangxia;
}

AcDbObjectId CDrawEntity::drawblkRef(CString strFile)
{
	AcDbObjectId blkRefId = AcDbObjectId::kNull;
	blkRefId = MyDrawEntity::InsertBlkRef(strFile, m_basePt);
	return blkRefId;
}

void CDrawEntity::moveToPt(AcDbObjectId objId)
{
	AcDbExtents exts; 
	AcDbBlockReference* pRef = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pRef, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pRef->close();
		return;
	}
	pRef->geomExtentsBestFit(exts);
	double dWidth = exts.maxPoint().x - exts.minPoint().x;
	double dHeight = exts.maxPoint().y - exts.minPoint().y;
	//m_basePt.y = m_basePt.y - dHeight;
	if (getBlkPt(dWidth, dHeight))
	{

	}
	if (dWidth > m_dBlkLen)
	{
		m_dBlkLen = dWidth;
	}
	if (dHeight > m_dBlkHeight)
	{
		m_dBlkHeight = dHeight;
	}
	m_basePt.y -= dHeight;
	m_basePt.y -= m_dVerSapce;	
	pRef->setPosition(m_basePt);
	pRef->close();

}

AcGePoint3d CDrawEntity::getblkStartPt(int nCount)
{
	double dFrameLength,dFrameHeight, m_dFrameHorSpace, m_dFrameVerSpace;
	AcGePoint3d m_InsertPt;
	m_InsertPt.set(0, 0, 0);
	m_dFrameHorSpace = CBaseUtils::horFrameSpace();
	m_dFrameVerSpace = CBaseUtils::verFrameSpace();
	int m_nHxCount = CBaseUtils::hxFrameCount();
	CString strFrameType = m_FrameInfo.getFrameType();
	double dTemp = 0.0;
	CUtility::getIniFromFile(_T("A0"), dFrameLength, dFrameHeight, dTemp);

	int nYCount = nCount/m_nHxCount;
	int nXCount = nCount%m_nHxCount;
	//移动insertPt;
	if ((nXCount == 0) && (nYCount > 0))
	{
		m_InsertPt.x += (m_nHxCount-1)*(m_dFrameHorSpace+dFrameLength);
		m_InsertPt.y -= (nYCount-1)*(m_dFrameVerSpace+dFrameHeight);
	}
	else
	{
		m_InsertPt.x += (nXCount-1)*(m_dFrameHorSpace+dFrameLength);
		m_InsertPt.y -= nYCount*(m_dFrameVerSpace+dFrameHeight);
	}

	return m_InsertPt;
}
