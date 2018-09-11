#include "StdAfx.h"
#include "CSheetInfo.h"
#include "BaseUtils.h"
#include "CGlobalQuery.h"
#include <algorithm>

extern CGlobalQuery gQuery;
extern bool CmpTest(const CCommonCell& p1,const CCommonCell& p2);

CSheetInfo::CSheetInfo(void)
{
	m_Data.clear();
	m_MapName.clear();
	m_MapInfo.clear();
	m_sheetGL.clear();
}

CSheetInfo::~CSheetInfo(void)
{
	m_Data.clear();
	m_MapName.clear();
	m_MapInfo.clear();
	m_sheetGL.clear();

}

void CSheetInfo::setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow)
{
	m_Data.clear();
	m_MapName.clear();
	
	m_Data.insert(m_Data.end(), vData.begin(), vData.end());
	vData.clear();

	for (map<int, CNameCell>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		m_MapName.insert(make_pair(iter->first, iter->second));
	}
	m_strSheetName = strSheetName;
	m_sheetGL.setSheetName(strSheetName);

	m_nColNum = nCol;
	m_nRowNum = nRow;

	m_nItem = nItem;
}

bool CSheetInfo::prompt()
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

bool CSheetInfo::doIt()
{
	//获取类型
	getType();
	//if (m_bIsMod)
	{
		m_strModSheetName = getSheetMod();
	}
	
	m_bIsLrmv = false;
	if (m_nType == 0)
	{
		readCommon();
	}
	else if (m_nType == 1)
	{
		readSpecial();
	}
	else
	{
		return false;
	}
	m_sheetData.setMapInfo(m_MapInfo);
	m_sheetData.setSheetStoreData(m_sheetGL);
	
	return true;
}

CSingleSheetData CSheetInfo::getdata() const
{
	return m_sheetData;
}

CString CSheetInfo::getType()
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

void CSheetInfo::readCommon()
{
	int nSize = m_MapName.size();
	int nItem, nNextItem;
	CString strItem,strTemp;
	CNameCell cell;
	CCommonCell commonCell;
	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
		vector<CCommonCell> comVec;
		comVec.clear();
		
		nItem = itr->first;
		cell = itr->second;
		//清理差分信号
		gQuery.clearDifferentialSignal();

		if (m_bIsMod)
		{
			m_strName = cell.getRealName();
		}
		else
		{
			m_strName = cell.getName();
		}
		m_NameCell = cell;
		
		if (++itr == m_MapName.end())//超出范围
		{
			comVec = readDuanziData(nItem, m_nRowNum);
		}
		else//没有超过范围
		{
			int nRow = itr->first;
			comVec = readDuanziData(nItem, nRow-1);
		}
		comVec.clear();
		gQuery.doDifferSignal();
	}
}

void CSheetInfo::readSpecial()
{
	int nSize = m_MapName.size();
	int nItem, nFirstItem, nNextItem;
	CString strItem,strTemp, strPreName, strTmp, strDuanzi;
	CCommonCell commonCell;
	CNameCell cell;
	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
		vector<CCommonCell> comVec;
		comVec.clear();
		//清理差分信号
		gQuery.clearDifferentialSignal();

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
			if (strTemp.CompareNoCase(_T("端子")) == 0)
			{
				comVec = readDuanziData(nItem, m_nRowNum);
			}
			else//其它情况
			{

			}
		}
		else
		{
			int nRow = itr->first;
			strTemp = m_Data[nItem][0];
			if (strTemp.CompareNoCase(_T("端子")) == 0)
			{
				comVec = readDuanziData(nItem, nRow-1);
			}
			else//其它情况
			{

			}
		}

		m_MapInfo.insert(make_pair(cell, comVec));
		gQuery.doDifferSignal();
	}
}


CString CSheetInfo::GetNameByIndex(int j, int nColNum)
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
CString CSheetInfo::getRealSheetName()
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
vector<CCommonCell> CSheetInfo::readDuanziData(int nItem, int nNum)
{
	
	CString strDuanzi,strTemp,strTexing, strTmp;
	CNameCell cell = m_NameCell;
	CCommonCell commonCell;
	commonCell.setNameCell(cell);

	vector<CCommonCell> comVec;
	int nType,nCount;
	int nMulriple = m_nColNum/2;

	bool bIsAdded = false;
	bool bIsGl = false;
	
	CGLData glData;
	glData.setNamCell(cell);
	if (cell.getName().Find(_T("[MT]")) >= 0)
	{
		CString strTempName = cell.getName();
		strTempName.Replace(_T("[MT]"), _T(""));
		cell.setName(strTempName);
		glData.setNamCell(cell);
		bIsGl = true;
	}
	for (int j=0; j<nMulriple; j++)
	{
		strTemp = m_Data[nItem][2*j];
		comVec.clear();
		CString strMTName;
		if (strTemp.CompareNoCase(_T("端子")) == 0)
		{
			for (int i=nItem+1; i<nNum; i++)
			{
				strTexing = m_Data[i][1+2*j];
				strDuanzi = m_Data[i][2*j];
				if (strTexing.IsEmpty() && strDuanzi.IsEmpty())
				{
					continue;
				}
				strTemp = m_Data[i][0];
				if ((strTemp.CompareNoCase(_T("母板连接器名称")) == 0) ||
					(strTemp.CompareNoCase(_T("模块连接器名称")) == 0) ||
					(strTemp.CompareNoCase(_T("母板用物资代码")) == 0) ||
					(strTemp.CompareNoCase(_T("模块用物资代码")) == 0))
				{
					continue;
				}
				CString strLog;
				strLog.Format(_T("sheetName = %s,第%d行，第%d列"),  m_strSheetName, i+1, 2*j+2);
				CString strQuxiang ;
				//处理编号
				int nFind = strTexing.Find(_T("[No="));
				if (nFind >= 0)
				{
					int nTemp = strTexing.Find(_T("]"), nFind);
					strTemp = strTexing.Mid(nFind+4, nTemp-nFind - 4);
					commonCell.setCount(strTemp);
					//strMTName.Format(_T("MT%d"), j+1);暂时不添加MT端子的信息
				}
				if ((m_NameCell.getRealName().CompareNoCase(_T("其它")) == 0) ||
					(m_NameCell.getRealName().CompareNoCase(_T("其他")) == 0))
				{
					strQuxiang = strDuanzi + _T(":1");
				}
				else
				{
					strQuxiang = getRealSheetName() + m_NameCell.getRealName() + strMTName + _T(":") + strDuanzi;
				}
				commonCell.setTempQuxiang(strQuxiang);
				commonCell.setModQuxiang(strQuxiang);
				
				strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
				
				commonCell.setTexing(strTexing);
				commonCell.setRealTexing(strLog);
				
				commonCell.setRow(i+1);
				commonCell.setCol(2+2*j);
				commonCell.setSheetName(m_strSheetName);
				
				//例外情况在此处理
				commonCell.setName(m_strName);
				commonCell.setDuanzi(strDuanzi);

				//if (m_bIsMod)
				{
					if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
					{
						strTmp = m_strModSheetName + m_strName + strMTName +  _T(":") + strDuanzi;
						commonCell.setModQuxiang(strTmp);
					}
					else
					{
						commonCell.setModQuxiang(_T(""));
					}
				}
				if (bIsGl)
				{
					if (!commonCell.getCount().IsEmpty())
					{
						glData.setNumber(commonCell.getCount());
					}
					gQuery.doMTCommonCell(commonCell);
				}
				else
				{
					gQuery.doCommonCell(commonCell);
				}
				comVec.push_back(commonCell);
				commonCell.setCount(_T(""));
			}
			if (bIsGl)
			{
				nType = GetGLType(comVec.size());
				glData.setCommonCellVec(comVec);
				glData.setType(nType);
				m_sheetGL.addGlDataItem(glData);
				comVec.clear();
				glData.clear();
			}	
			else
			{
				cell.setItem(j+nItem);
				m_MapInfo.insert(make_pair(cell, comVec));
			}
		}	
	}
	return comVec;
}

CString CSheetInfo::modSheetName(CString strSheetName) const
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

int CSheetInfo::GetGLType(int nSize)
{
	int nType = 0;
	if (nSize == 1)
	{
		nType = 0;
	}
	else if ((nSize > 1) && (nSize <= 12))
	{
		nType = 1;
	}
	else if ((nSize > 12) && (nSize <= 24))
	{
		nType = 2;
	}
	else if ((nSize > 24) && (nSize <= 48))
	{
		nType = 3;
	}
	else
	{
		nType = 0;
	}
	return nType;
}
CString CSheetInfo::getSheetMod() const
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
COutstring::COutstring(void)
{

}

COutstring::~COutstring(void)
{

}

void COutstring::setStringOut(CString strOut)
{
	m_strOut = strOut;
}

void COutstring::setBianma(int nBianma)
{
	m_nBianma = nBianma;
}

void COutstring::setQuxiang(CString strQuxiang)
{
	m_strQuxiang = strQuxiang;
}

//void COutstring::setCount(int nCount)
//{
//	m_nCout = nCount;
//}

void COutstring::setTexing(CString strTexing)
{
	m_strTexing = strTexing;
}

CString COutstring::getStringOut() const
{	
	return m_strOut;
}

int COutstring::getBianma() const
{	
	return m_nBianma;
}

CString COutstring::getQuxiang() const
{
	return m_strQuxiang;
}

//int COutstring::getCout() const
//{
//	return m_nCout;
//}

CString COutstring::getTexing() const
{
	return m_strTexing;
}

CString COutstring::getOutPutstring() const
{
	CString strTmp = m_strTexing + _T("------------") + m_strOut;
	return strTmp;
}

COutstring& COutstring::operator=(const COutstring& node)
{
	m_strOut = node.getStringOut();
	m_nBianma = node.getBianma();
	//m_nCout = node.getCout();
	m_strTexing = node.getTexing();
	m_strQuxiang = node.getQuxiang();
	return *this;
}
