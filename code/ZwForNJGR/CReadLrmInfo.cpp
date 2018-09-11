#include "StdAfx.h"
#include "CReadLrmInfo.h"
#include "CGlobalQuery.h"
#include "Utility.h"
extern CGlobalQuery gQuery;

CReadLrmInfo::CReadLrmInfo(void)
{
	m_MapName.clear();
	m_Data.clear();
	m_ModSingleData.clear();

	RFComVec.clear();
	RFModComVec.clear();
	comVec.clear();
	comModVec.clear();
}

CReadLrmInfo::~CReadLrmInfo(void)
{
	m_MapName.clear();
	m_Data.clear();
	m_ModSingleData.clear();

	RFComVec.clear();
	RFModComVec.clear();
	comVec.clear();
	comModVec.clear();
}

bool CReadLrmInfo::doIt()
{
	getType();
	int nItem = 1;
	CString strTemp;
	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
		//清理差分信号
		gQuery.clearDifferentialSignal();

		nItem = itr->first;
		m_NameCell = itr->second;
		m_strName = m_NameCell.getRealName();
		
		if (++itr == m_MapName.end())//超出范围
		{
			strTemp = m_Data[nItem][ColQHCol];
			if (strTemp.CompareNoCase(_T("腔号")) == 0)
			{
				readDuanzi(nItem, m_nRowNum);
			}
		}
		else
		{
			int nRow = itr->first;
			strTemp = m_Data[nItem][ColQHCol];
			if (strTemp.CompareNoCase(_T("腔号")) == 0)
			{
				readDuanzi(nItem, nRow-1);
			}
		}
		gQuery.doDifferSignal();
	}
	return true;
}

bool CReadLrmInfo::doCommaItem(CString strItem, int nRow, int nCol)
{
	CString strTexing,strTmp,strQuxiang,strLog;
	CCommonCell commonCell;
	commonCell.setNameCell(m_NameCell);

	int ndex = CUtility::ParserStringComma(strItem, strTexing);
	if (strTexing.Find(_T("[RF]")) >= 0)
	{
		m_bIsRF = true;
	}
	else
	{
		m_bIsRF = false;
	}
	int nFind = strTexing.Find(_T("[T]"));
	if (nFind >= 0)
	{
		m_bIsAddData = false;
	}
	else
	{
		m_bIsAddData = true;
	}
	strItem = strItem.Left(ndex);
	strQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strItem;
	commonCell.setDuanzi(strItem);
	commonCell.setName(m_strName);
	commonCell.setTempQuxiang(strQuxiang);
	commonCell.setTexing(strItem);
	strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
	commonCell.setRealTexing(strLog);
	if (m_bIsAddData)
	{
		if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
		{
			strTmp = getSheetMod() + m_strName +  _T(":") + strItem;
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
	commonCell.setRow(nRow+1);
	commonCell.setCol(nCol+1);
	commonCell.setSheetName(m_strSheetName);
	if (m_bIsAddData)
	{
		gQuery.doCommonCell(commonCell);
		if (m_bIsRF)
		{
			RFComVec.push_back(commonCell);
			RFModComVec.push_back(commonCell);
		}
		else
		{
			comVec.push_back(commonCell);
			comModVec.push_back(commonCell);
		}
	}
	else
	{
		if (m_bIsRF)
		{
			RFModComVec.push_back(commonCell);
		}
		else
		{
			comModVec.push_back(commonCell);
		}
		gQuery.addTDataToVec(commonCell);
	}
	return true;
}

	
void CReadLrmInfo::doColItem(CString strItem, int nRow, int nCol)
{
	CString strTexing,strQuxiang,strLog,strDuanzi;
	strTexing = m_Data[nRow-1][nCol-1];
	getCellStatus(strTexing);

	CCommonCell commonCell;
	commonCell.setNameCell(m_NameCell);
	commonCell.setName(m_strName);
	commonCell.setTexing(strTexing);
	strDuanzi = strPreName + strMidName + m_Data[nRow-1][1];
	commonCell.setDuanzi(strDuanzi);
	strQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strDuanzi;
	commonCell.setTempQuxiang(strQuxiang);
	commonCell.setModQuxiang(strQuxiang);
	strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);
	commonCell.setRealTexing(strLog);	
	commonCell.setRow(nRow);
	commonCell.setCol(nCol);
	commonCell.setSheetName(m_strSheetName);

}

bool CReadLrmInfo::readDuanzi(int nItem, int nNum)
{
//先获取最小间隔
	vector<int> vecInt = getColInt(nItem+1, nNum);
	//普通数据
	int nCount,nStart;
	nStart = nItem+1;
	CString strTmp,strTemp,strItem;

	tmpVec.clear();
	for (vector<int>::iterator iter = vecInt.begin(); iter != vecInt.end(); ++iter)
	{	
		nCount = *iter;
		if (nCount == nItem+1)
		{
			continue;
		}
		else
		{
			for (int j=1; j<m_nColNum; j++)
			{
				for (int i= nStart; i<nCount; i++)
				{
					strTmp = m_Data[i][0];//A或B或其它
					if ((strTmp.CompareNoCase(_T("母板连接器名称")) == 0) ||
						(strTmp.CompareNoCase(_T("模块连接器名称")) == 0) ||
						(strTmp.CompareNoCase(_T("母板用物资代码")) == 0) ||
						(strTmp.CompareNoCase(_T("模块用物资代码")) == 0))
					{
						continue;
					}
					strPreName = m_Data[nStart][0];

					strTemp = m_Data[nStart][1];//要么为Col,要么是带冒号的数据
					if (strTemp.CompareNoCase(_T("Col.")) == 0)//特殊数据
					{
						tmpVec = m_Data[nStart];
						if (i+1 <= nCount)
						{
							if (j+1 <= m_nColNum)
							{
								strItem = m_Data[i+1][j+1];
								strMidName = m_Data[nStart][j+1];
								doColItem(strItem, i+1, j+1);//如果为Col.的数据
							}
						}
					}
					else
					{
						strItem = m_Data[i][j];	
						doCommaItem(strItem, i, j);
					}
				}
			}
		}	
		nStart = nCount;
	}
	return true;
}
