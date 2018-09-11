#include "StdAfx.h"
#include "CLrmSheetInfo.h"
#include "BaseUtils.h"
#include "CGlobalQuery.h"
#include <algorithm>
#include "Utility.h"

extern CGlobalQuery gQuery;
extern bool CmpTest(const CCommonCell& p1,const CCommonCell& p2);
bool comp(const int &a, const int &b)
{
	return a < b;
}

CLrmSheetInfo::CLrmSheetInfo(void)
{
	m_Data.clear();
	m_MapName.clear();

}

CLrmSheetInfo::~CLrmSheetInfo(void)
{
	m_Data.clear();
	m_MapName.clear();
	
}

void CLrmSheetInfo::setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow)
{
	m_Data.clear();
	m_MapName.clear();
	
	m_Data.insert(m_Data.end(), vData.begin(), vData.end());
	vData.clear();

	m_MapName = tmpVec;
	m_strSheetName = strSheetName;
	
	m_singleData.setSheetName(strSheetName);
	if (tmpVec.size() > 0)
	{
		map<int, CNameCell>::iterator iter = tmpVec.begin();
		CString strLrm = iter->second.getLjqName();
		CString strModLrm = iter->second.getModLjqName();
		m_singleData.setModLrmName(strModLrm);
		m_singleData.setLrmName(strLrm);
	}
	m_nColNum = nCol;
	m_nRowNum = nRow;

	m_nItem = nItem;
}

bool CLrmSheetInfo::prompt()
{
	vector<CString> tmpVec;
	int nCount=1;
	CString strTemp;
	for (vector<vector<CString> >::iterator iter = m_Data.begin(); iter != m_Data.end(); iter++)
	{
		tmpVec.clear();
		tmpVec = *iter;
		strTemp.Format(_T("\n%d:"), nCount);
		for (vector<CString>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); Itr++)
		{
			strTemp += *Itr;
			strTemp += _T(",");
		}
		acutPrintf(strTemp);
		nCount++;
		tmpVec.clear();
	}
	return true;
}
CString CLrmSheetInfo::getSheetMod() const
{
	CString strPre,strFix, strNextFix, strEnd, strOut, strNum;

	int nFind = m_strSheetName.Find(_T("("));
	if (nFind>=0)
	{
		strPre = m_strSheetName.Left(nFind);
		return strPre;
	}
	else
	{
		return m_strSheetName;
	}
}
bool CLrmSheetInfo::doIt()
{
	//获取类型
	getType();
	if (m_bIsMod)
	{
		m_strModSheetName = getSheetMod();
	}

	m_bIsLrmv = false;
	if (m_nType == 1)
	{
		readSpecial();
	}
	else
	{
		return false;
	}

	return true;
}

CSheetStoreData CLrmSheetInfo::getData() const
{
	return m_singleData;
}

CSheetStoreData CLrmSheetInfo::getModData()
{
	if (m_bIsMod)
	{
		CString strMod = modSheetName(m_strSheetName);
		m_ModSingleData.setSheetName(strMod);
		m_ModSingleData.setLrmName(m_singleData.getModLrmName());
	}
	return m_ModSingleData;
}

bool CLrmSheetInfo::IsMod()
{
	return m_bIsMod;
}

CString CLrmSheetInfo::getType()
{
	m_strType = m_Data[0][1];
	if (m_strType.Find(_T("[模块]")) >= 0)
	{
		m_bIsMod = true;
		m_strType.Replace(_T("[模块]"), _T(""));
	}
	else
	{
		m_bIsMod = false;
	}
	
	if (m_strType.CompareNoCase(_T("普通")) == 0)
	{
		m_nType = 0;
	}
	else if (m_strType.CompareNoCase(_T("特殊")) == 0)
	{
		m_nType = 1;
	}
	else if (m_strType.CompareNoCase(_T("VPX")) == 0)
	{
		m_nType = 2;
	}
	else
	{
		m_nType = 3;
	}
	return m_strType;
}

void CLrmSheetInfo::readSpecial()
{
	int nSize = m_MapName.size();
	int nItem, nFirstItem, nNextItem;
	CString strItem,strTemp, strPreName, strTmp, strDuanzi;
	CNameCell cell;
	CCommonCell commonCell;
	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
		//清理差分信号
		gQuery.clearDifferentialSignal();

		vector<CCommonCell> comVec;
		comVec.clear();

		nItem = itr->first;
		cell = itr->second;
		if (m_bIsMod)
		{
			m_strName = cell.getRealName();
		}
		else
		{
			m_strName = cell.getSpecailaName();
		}
		m_NameCell = cell;
		if (++itr == m_MapName.end())//超出范围
		{
			strTemp = m_Data[nItem][0];
			if (strTemp.CompareNoCase(_T("腔号")) == 0)
			{
				comVec = readLrmData(nItem, m_nRowNum);
			}
		}
		else
		{
			int nRow = itr->first;
			strTemp = m_Data[nItem][0];
			if (strTemp.CompareNoCase(_T("腔号")) == 0)
			{
				comVec = readLrmData(nItem, nRow-1);
			}
		}
		comVec.clear();
		gQuery.doDifferSignal();
	}
}

CString CLrmSheetInfo::GetNameByIndex(int j, int nColNum)
{
	CString strTmp;
	if (j > nColNum)
	{
		return strTmp;
	}
	else
	{
		int nSize = nColNum - j;
		strTmp.Format(_T("%c"), nSize+64);
		return strTmp;
	}
}
CString CLrmSheetInfo::getRealSheetName()
{
	CString strSheet;
	int nFind = m_strSheetName.Find(_T("("));
	if (nFind >=0)
	{
		strSheet = m_strSheetName.Left(nFind);
	}
	else
	{
		strSheet = m_strSheetName;
	}
	return strSheet;
}

vector<CCommonCell> CLrmSheetInfo::readLrmData(int nItem,int nNum)
{
	vector<CCommonCell> comVec;
	vector<CCommonCell> comModVec;
	comVec.clear();
	int nFirstItem, nNextItem;
	CString strItem,strTemp, strPreName, strTmp, strDuanzi, strTexing;
	CNameCell cell;
	CCommonCell commonCell;
	commonCell.setNameCell(m_NameCell);

	vector<CCommonCell> RFComVec;//记录RF
	vector<CCommonCell> RFModComVec;//记录modRF

	int nCount, nType, nIndex;
	nIndex = 0;
	bool bIsAdded = false;
	bool bIsRF = false;
	CTableData tempTblData;		
	tempTblData.clear();
	tempTblData.setNamCell(m_NameCell);
	CRFData tmpRfData;
	tmpRfData.clear();
	tmpRfData.setNamCell(m_NameCell);

	CTableData tempModTblData;		
	tempModTblData.clear();
	tempModTblData.setNamCell(m_NameCell);
	CRFData tmpModRfData;
	tmpModRfData.clear();
	tmpModRfData.setNamCell(m_NameCell);
	vector<int> vecInt = getColInt(nItem+1, nNum);

	int nStart = nItem+1;
	bool bIsAddData= true;//是否是带T的数据，是带T的数据那么该值为false，不添加到母版数据中
	CString strMidName;
	vector<CString> tmpVec;
	//普通数据
	for (vector<int>::iterator iter = vecInt.begin(); iter != vecInt.end(); ++iter)
	{	
		nCount = *iter;
		if (nCount == nItem+1)
		{
			continue;
		}
		else
		{
			for (int j=2; j<m_nColNum; j++)
			{
				comVec.clear();

				for (int i= nStart; i<nCount; i++)
				{
					strTmp = m_Data[i][0];//A或B或其它
					if ((strTmp.CompareNoCase(_T("母板连接器名称")) == 0) ||
						(strTmp.CompareNoCase(_T("模块连接器名称")) == 0) ||
						(strTmp.CompareNoCase(_T("母板用物资代码")) == 0) ||
						(strTmp.CompareNoCase(_T("模块用物资代码")) == 0))
					{
						break;
					}
					if (!strTmp.IsEmpty())//如果为空不修改原始数据
					{
						strPreName = strTmp;
					}

					strTemp = m_Data[i][1];


					if (strTemp.IsEmpty())//那么是射频B1或者B1或者B1：
					{
						strItem = m_Data[i][j];		
						if (strItem.IsEmpty())//
						{
							//如果为空不处理
						}
						else if (strItem.Find(_T('(')) > 0)
						{
							int ndex = CUtility::ParserStringComma(strItem, strTexing);
							if (strTexing.Find(ICD_PROPERTY_RF) >= 0)
							{
								bIsRF = true;
							}
							else
							{
								bIsRF = false;
							}
							int nFind = strTexing.Find(_T("[T]"));
							if (nFind >= 0)
							{
								bIsAddData = false;
							}
							else
							{
								bIsAddData = true;
							}
							strItem = strItem.Left(ndex);
							commonCell = doItem(strItem);

							commonCell.setTexing(strTexing);
							CString	strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
							commonCell.setRealTexing(strLog);
							if (bIsAddData)
							{
								if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
								{
									strTmp = m_strModSheetName + m_strName +  _T(":") + strItem;
									commonCell.setModQuxiang(strTmp);
								}
								else
								{
									commonCell.setModQuxiang(_T(""));
								}
							}
							else
							{
								commonCell.setModQuxiang(_T(""));
							}
							commonCell.setRow(i+1);
							commonCell.setCol(j+1);
							commonCell.setSheetName(m_strSheetName);
							if (bIsAddData)
							{
								gQuery.doCommonCell(commonCell);
								if (bIsRF)
								{
									RFComVec.push_back(commonCell);
									RFModComVec.push_back(commonCell);
								}
								else
								{
									comVec.push_back(commonCell);
									comModVec.push_back(commonCell);
								}
								commonCell.setCount(_T(""));
							}
							else
							{
								if (bIsRF)
								{
									RFModComVec.push_back(commonCell);
								}
								else
								{
									comModVec.push_back(commonCell);
								}
								gQuery.addTDataToVec(commonCell);
							}
						}
						else if ((strItem.Find(_T(":")) > 0) || (strItem.Find(_T("：")) > 0))
						{
							int ndex = CUtility::ParserStringComma(strItem, strTexing);
							if (strTexing.Find(ICD_PROPERTY_RF) >= 0)
							{
								bIsRF = true;
							}
							else
							{
								bIsRF = false;
							}
							int nFind = strTexing.Find(_T("[T]"));
							if (nFind >= 0)
							{
								bIsAddData = false;
							}
							else
							{
								bIsAddData = true;
							}
							strItem = strItem.Left(ndex);
							commonCell = doItem(strItem);

							commonCell.setTexing(strTexing);
							CString	strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
							commonCell.setRealTexing(strLog);
							if (bIsAddData)
							{
								if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
								{
									strTmp = m_strModSheetName + m_strName +  _T(":") + strItem;
									commonCell.setModQuxiang(strTmp);
								}
								else
								{
									commonCell.setModQuxiang(_T(""));
								}
							}
							else
							{
								commonCell.setModQuxiang(_T(""));
							}
							commonCell.setRow(i+1);
							commonCell.setCol(j+1);
							commonCell.setSheetName(m_strSheetName);
							if (bIsAddData)
							{
								gQuery.doCommonCell(commonCell);
								if (bIsRF)
								{
									RFComVec.push_back(commonCell);
									RFModComVec.push_back(commonCell);
								}
								else
								{
									comVec.push_back(commonCell);
									comModVec.push_back(commonCell);
								}
								commonCell.setCount(_T(""));
							}
							else
							{
								if (bIsRF)
								{
									RFModComVec.push_back(commonCell);
								}
								else
								{
									comModVec.push_back(commonCell);
								}
								gQuery.addTDataToVec(commonCell);
							}
						}
						else
						{
							//数据不处理
							continue;
						}
					}
					else if (strTemp.CompareNoCase(_T("Col.")) == 0)//特殊数据
					{
						tmpVec = m_Data[i];
					}
					else
					{	
						if (tmpVec.size() > 0)
						{
							strMidName = tmpVec[j];
						}
						
						strTexing = m_Data[i][j];
						if (strTexing.Find(ICD_PROPERTY_RF) >= 0)
						{
							bIsRF = true;
						}
						else
						{
							bIsRF = false;
						}
						commonCell.setName(m_strName);
						commonCell.setTexing(strTexing);
						strDuanzi = strPreName + strMidName + m_Data[i][1];
						commonCell.setDuanzi(strDuanzi);

						CString strLog;
						CString strQuxiang ;
						strQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strDuanzi;

						commonCell.setTempQuxiang(strQuxiang);
						commonCell.setModQuxiang(strQuxiang);
						strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
						commonCell.setRealTexing(strLog);
						int nFind = strTexing.Find(_T("[T]"));
						if (nFind >= 0)
						{
							bIsAddData = false;
						}
						else
						{
							bIsAddData = true;
						}
						//if (m_bIsMod)
						{
							if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
							{
								strTmp = m_strModSheetName + m_strName +  _T(":") + strDuanzi;
								commonCell.setModQuxiang(strTmp);
								m_bIsContinued = true;
							}
							else
							{
								commonCell.setModQuxiang(_T(""));
								m_bIsContinued = false;
							}
						}

						commonCell.setRow(i+1);
						commonCell.setCol(1+j);
						commonCell.setSheetName(m_strSheetName);
						/*gQuery.doCommonCell(commonCell);
						comVec.push_back(commonCell);
						comModVec.push_back(commonCell);*/
						if (bIsAddData)
						{
							gQuery.doCommonCell(commonCell);
							if (bIsRF)
							{
								RFComVec.push_back(commonCell);
								RFModComVec.push_back(commonCell);
							}
							else
							{
								comVec.push_back(commonCell);
								comModVec.push_back(commonCell);
							}
							commonCell.setCount(_T(""));
						}
						else
						{
							if (bIsRF)
							{
								RFModComVec.push_back(commonCell);
							}
							else
							{
								comModVec.push_back(commonCell);
							}
							gQuery.addTDataToVec(commonCell);
						}
					}
				}
				if (comVec.size() > 0)
				{
					tempTblData.add(comVec);
					tempModTblData.add(comModVec);
					comVec.clear();
					comModVec.clear();
				}
				else
				{
					comVec.clear();
					comModVec.clear();
				}
			}
			nStart = nCount;
		}
		m_singleData.addTableItem(tempTblData);
		if (m_bIsMod)
		{
			m_ModSingleData.addTableItem(tempModTblData);
		}
		tempTblData.clear();
		tempModTblData.clear();
	}
	tempTblData.clear();
	tempModTblData.clear();
	tempModTblData.setNamCell(m_NameCell);
	tempTblData.setNamCell(m_NameCell);
	for (int j=2; j<m_nColNum; j++)
	{
		if (vecInt.size() > 0)
		{
			for (int i=vecInt.at(vecInt.size()-1); i<nNum; i++)
			{
				strTmp = m_Data[i][0];//A或B或其它
				if ((strTmp.CompareNoCase(_T("母板连接器名称")) == 0) ||
					(strTmp.CompareNoCase(_T("模块连接器名称")) == 0) ||
					(strTmp.CompareNoCase(_T("母板用物资代码")) == 0) ||
					(strTmp.CompareNoCase(_T("模块用物资代码")) == 0))
				{
					break;
				}
				if (!strTmp.IsEmpty())//如果为空不修改原始数据
				{
					strPreName = strTmp;
				}

				strTemp = m_Data[i][1];


				if (strTemp.IsEmpty())//那么是射频B1或者B1或者B1：
				{
					strItem = m_Data[i][j];		
					if (strItem.IsEmpty())//
					{
						//如果为空不处理
					}
					else if (strItem.Find(_T('(')) > 0)
					{
						int ndex = CUtility::ParserStringComma(strItem, strTexing);
						if (strTexing.Find(ICD_PROPERTY_RF) >= 0)
						{
							bIsRF = true;
						}
						else
						{
							bIsRF = false;
						}
						if (strTexing.Find(_T("T")) >= 0)
						{
							bIsAddData = false;
						}
						else
						{
							bIsAddData = true;
						}
						strItem = strItem.Left(ndex);
						commonCell = doItem(strItem);

						commonCell.setTexing(strTexing);
						CString	strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
						commonCell.setRealTexing(strLog);
						if (bIsAddData)
						{
							if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
							{
								strTmp = m_strModSheetName + m_strName +  _T(":") + strItem;
								commonCell.setModQuxiang(strTmp);
							}
							else
							{
								commonCell.setModQuxiang(_T(""));
							}
						}
						else
						{
							commonCell.setModQuxiang(_T(""));
						}
						commonCell.setRow(i+1);
						commonCell.setCol(j+1);
						commonCell.setSheetName(m_strSheetName);
						if (bIsAddData)
						{
							gQuery.doCommonCell(commonCell);
							if (bIsRF)
							{
								RFComVec.push_back(commonCell);
								RFModComVec.push_back(commonCell);
							}
							else
							{
								comVec.push_back(commonCell);
								comModVec.push_back(commonCell);
							}
							commonCell.setCount(_T(""));
						}
						else
						{
							if (bIsRF)
							{
								RFModComVec.push_back(commonCell);
							}
							else
							{
								comModVec.push_back(commonCell);
							}
						}
					}
					else if ((strItem.Find(_T(":")) > 0) || (strItem.Find(_T("：")) > 0))
					{
						int ndex = CUtility::ParserStringComma(strItem, strTexing);
						if (strTexing.Find(ICD_PROPERTY_RF) >= 0)
						{
							bIsRF = true;
						}
						else
						{
							bIsRF = false;
						}
						int nFind = strTexing.Find(_T("[T]"));
						if (nFind >= 0)
						{
							bIsAddData = false;
						}
						else
						{
							bIsAddData = true;
						}
						strItem = strItem.Left(ndex);
						commonCell = doItem(strItem);
						commonCell.setTexing(strTexing);
						CString	strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
						commonCell.setRealTexing(strLog);
						if (bIsAddData)
						{
							if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
							{
								strTmp = m_strModSheetName + m_strName +  _T(":") + strItem;
								commonCell.setModQuxiang(strTmp);
							}
							else
							{
								commonCell.setModQuxiang(_T(""));
							}
						}
						else
						{
							commonCell.setModQuxiang(_T(""));
						}
						commonCell.setRow(i+1);
						commonCell.setCol(j+1);
						commonCell.setSheetName(m_strSheetName);
						if (bIsAddData)
						{
							gQuery.doCommonCell(commonCell);
							if (bIsRF)
							{
								RFComVec.push_back(commonCell);
								RFModComVec.push_back(commonCell);
							}
							else
							{
								comVec.push_back(commonCell);
								comModVec.push_back(commonCell);
							}
							commonCell.setCount(_T(""));
						}
						else
						{
							if (bIsRF)
							{
								RFModComVec.push_back(commonCell);
							}
							else
							{
								comModVec.push_back(commonCell);
							}
							gQuery.addTDataToVec(commonCell);
						}
					}
					else
					{
						//数据不处理
						continue;
					}
				}
				else if (strTemp.CompareNoCase(_T("Col.")) == 0)//特殊数据
				{
					tmpVec = m_Data[i];
				}
				else
				{
					strTexing = m_Data[i][j];
					if (tmpVec.size() > 0)
					{
						strMidName = tmpVec[j];
					}
					if (strTexing.Find(ICD_PROPERTY_RF) >= 0)
					{
						bIsRF = true;
					}
					else
					{
						bIsRF = false;
					}
					commonCell.setName(m_strName);
					commonCell.setTexing(strTexing);		
					strDuanzi = strPreName + strMidName + m_Data[i][1];
					commonCell.setDuanzi(strDuanzi);

					CString strLog;
					CString strQuxiang ;
					strQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strDuanzi;

					commonCell.setTempQuxiang(strQuxiang);
					commonCell.setModQuxiang(strQuxiang);
					strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
					commonCell.setRealTexing(strLog);
					int nFind = strTexing.Find(_T("[T]"));
					if (nFind >= 0)
					{
						bIsAddData = false;
					}
					else
					{
						bIsAddData = true;
					}
					//if (m_bIsMod)
					{
						if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
						{
							strTmp = m_strModSheetName + m_strName +  _T(":") + strDuanzi;
							commonCell.setModQuxiang(strTmp);
							m_bIsContinued = true;
						}
						else
						{
							commonCell.setModQuxiang(_T(""));
							m_bIsContinued = false;
						}
					}

					commonCell.setRow(i+1);
					commonCell.setCol(1+j);
					commonCell.setSheetName(m_strSheetName);
					if (bIsAddData)
					{
						gQuery.doCommonCell(commonCell);
						if (bIsRF)
						{
							RFComVec.push_back(commonCell);
							RFModComVec.push_back(commonCell);
						}
						else
						{
							comVec.push_back(commonCell);
							comModVec.push_back(commonCell);
						}
						commonCell.setCount(_T(""));
					}
					else
					{
						if (bIsRF)
						{
							RFModComVec.push_back(commonCell);
						}
						else
						{
							comModVec.push_back(commonCell);
						}
						gQuery.addTDataToVec(commonCell);
					}
				}
			}
			if (comVec.size() > 0)
			{
				tempTblData.add(comVec);
				tempModTblData.add(comModVec);
				comVec.clear();
				comModVec.clear();
			}
			else
			{
				comVec.clear();
				comModVec.clear();
			}

		}
	}
	m_singleData.addTableItem(tempTblData);
	if (m_bIsMod)
	{
		m_ModSingleData.addTableItem(tempModTblData);
	}
	if (RFComVec.size() > 0)
	{
		std::sort(RFComVec.begin(), RFComVec.end(), CmpTest);
		tmpRfData.setCommonCellVec(RFComVec);
		std::sort(RFModComVec.begin(), RFModComVec.end(), CmpTest);
		tmpModRfData.setCommonCellVec(RFModComVec);
		m_singleData.addRfDataItem(tmpRfData);
		if (m_bIsMod)
		{
			m_ModSingleData.addRfDataItem(tmpModRfData);
		}
	}
	return comVec;
}

CCommonCell CLrmSheetInfo::doTableItem(CString strTexing)
{
	CCommonCell cell;
	cell.setNameCell(m_NameCell);
	cell.setTexing(strTexing);
	CString strTemp = MyParserString::RemoveSymbol(_T("]"), strTexing);
	cell.setRealTexing(strTemp);
	return cell;
}

CCommonCell CLrmSheetInfo::doRfItem(CString strItem)
{
	CCommonCell cell;
	cell.setNameCell(m_NameCell);
	CString strTexing, tmpQuxiang;
	cell.setTexing(strItem);
	int ndex = CUtility::ParserStringComma(strItem, strTexing);
	//strTexing.Replace(ICD_PROPERTY_RF, _T(""));

	strItem = strItem.Left(ndex);
	cell.setDuanzi(strItem);
	tmpQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strItem;
	cell.setTempQuxiang(tmpQuxiang);
	cell.setModQuxiang(tmpQuxiang);
	strTexing = MyParserString::RemoveSymbol(_T("]"), strTexing);
	cell.setRealTexing(strTexing);


	return cell;
}

CCommonCell CLrmSheetInfo::doItem(CString strItem)
{
	CCommonCell commonCell;
	commonCell.setNameCell(m_NameCell);
	CString strLog;
	CString strQuxiang ;
	strQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strItem;
	commonCell.setDuanzi(strItem);
	commonCell.setName(m_strName);
	commonCell.setTempQuxiang(strQuxiang);
	commonCell.setModQuxiang(strQuxiang);
	return commonCell;
}

vector<int> CLrmSheetInfo::getColInt(int nStart, int nNum)
{
	vector<int> tmpVec,VecInt;
	CString strTemp,strPreTemp;
	//先将A腔B腔分隔开
	int nItem;
	for (int i= nStart; i<nNum; i++)
	{
		strTemp = m_Data[i][0];
		if ((strPreTemp.CompareNoCase(strTemp) != 0) 
			&& (!strTemp.IsEmpty()))
		{
			nItem = i;
			tmpVec.push_back(nItem);
			strPreTemp = strTemp;
		}
		if (!strTemp.IsEmpty())
		{
			strPreTemp = strTemp;
		}
		else
		{
			m_Data[i][0] = strPreTemp;
		}
	}

	int nCount;
	int nSize = tmpVec.size();
	bool bIsStart = true;
	for (vector<int>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		nCount = *iter;
		if (nCount == nStart)
		{
			continue;
		}
		else
		{
			for (int i= nStart; i<nCount; i++)
			{
				strTemp = m_Data[i][1];
				if (strTemp.CompareNoCase(_T("Col.")) == 0)
				{
					VecInt.push_back(i);
				}
			}
			nStart = nCount;
		}
	}
	if (nSize > 1)
	{
		for (int i=tmpVec.at(nSize-1); i<nNum; i++)
		{
			strTemp = m_Data[i][1];
			if (strTemp.CompareNoCase(_T("Col.")) == 0)
			{
				VecInt.push_back(i);
			}
		}
	}
	
	VecInt.insert(VecInt.end(), tmpVec.begin(), tmpVec.end());
	std::sort(VecInt.begin(), VecInt.end(), comp);
	tmpVec.clear();
	return VecInt;
}
CString CLrmSheetInfo::modSheetName(CString strSheetName) const
{
	CString strPre,strFix, strNextFix, strEnd, strOut, strNum;

	int nFind = m_strSheetName.Find(_T("("));
	if (nFind>=0)
	{
		strPre = m_strSheetName.Left(nFind);
		strEnd = m_strSheetName.Right(m_strSheetName.GetLength() - nFind);
		strEnd = strEnd.Mid(1, strEnd.GetLength() - 2);
	}
	else
	{
		return m_strSheetName;
	}
	int nLen = MyParserString::GetPileLength(strPre, strFix);
	nLen++;
	int nCount = MyParserString::GetPileLength(strEnd, strNextFix);

	CString strTemp;
	strTemp.Format(_T("%s%d(%s%d)"), strFix, nCount, strNextFix, nLen);
	return strTemp;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
