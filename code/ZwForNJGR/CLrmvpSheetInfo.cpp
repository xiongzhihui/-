#include "StdAfx.h"
#include "CLrmvpSheetInfo.h"
#include "MyExcel.h"
#include "CDrawEnt.h"
#include <algorithm>
#include "Utility.h"

extern bool  CmpTest(const CCommonCell& p1,const CCommonCell& p2);
CLrmvpSheetInfo::CLrmvpSheetInfo(void)
{
	m_Data.clear();
	m_modData.clear();
	m_MapName.clear();
}


CLrmvpSheetInfo::~CLrmvpSheetInfo(void)
{

}

void CLrmvpSheetInfo::setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow)
{
	m_Data.clear();
	m_MapName.clear();
	
	m_Data.insert(m_Data.end(), vData.begin(), vData.end());
	
	vData.clear();

	m_MapName = tmpVec;
	tmpVec.clear();

	m_strSheetName = strSheetName;
	//m_strModSheetName = modSheetName(strSheetName);
	m_strModSheetName = strSheetName;

	m_nColNum = nCol;
	m_nRowNum = nRow;
	m_nItem = nItem;
}

bool CLrmvpSheetInfo::doIt()
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	for (int i=0; i<m_nRowNum; i++)
	{
		m_modData.push_back(tmpVec);
	}
	int nXsize = m_Data.size();
	for (int i=0; i<nXsize; i++)
	{
		for (int j=0; j<m_nColNum; j++)
		{
			m_modData[i][j] = m_Data[i][j];
		}
	}
	//获取座子信息
	getPlug();
	readVpxData();
	//prompt();
	//writeExcel();
	return true;
}


bool CLrmvpSheetInfo::prompt()
{
	vector<CString> tmpVec;
	int nCount=1;
	CString strTemp;
	for (vector<vector<CString> >::iterator iter = m_modData.begin(); iter != m_modData.end(); iter++)
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
void CLrmvpSheetInfo::readVpxData()
{
	int nSize = m_MapName.size();
	int nItem, nNextItem;
	CString strItem,strTemp;
	CNameCell cell;
	CCommonCell commonCell;

	int nCount = 0;
	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
		vector<CCommonCell> comVec;
		comVec.clear();

		nItem = itr->first;
		cell = itr->second;
		m_strName = cell.getRealName();

		for (int i=nCount; i<=nItem; i++)
		{
			for (int j=0; j<m_nColNum; j++)
			{
				m_modData[i][j] = m_Data[i][j];
			}
		}
		m_modData[0][1] = _T("LRMV");
		m_modData[1][1] = _T("K");
		
		if (++itr == m_MapName.end())//超出范围
		{
			readVpx(nItem, m_nRowNum, cell);
		}
		else
		{
			int nRow = itr->first;
			readVpx(nItem, nRow-1, cell);
			if (m_Data[nItem-1][0].CompareNoCase(_T("模块用物资代码")) == 0)
			{
				nCount = nRow - 5;
			}
			else
			{
				nCount = nRow - 1;
			}
		}
	}
}

bool CLrmvpSheetInfo::writeExcel()
{
	CMyExcel excel;
	excel.Open();
	excel.AddSheet(m_strModSheetName);
	int i=1;
	for (vector<vector<CString> >::iterator iter = m_modData.begin(); iter != m_modData.end(); ++iter)
	{
		vector<CString> tmpVec = *iter;
		int j=1;
		for (vector<CString>::iterator ir = tmpVec.begin(); ir	!= tmpVec.end(); ++ir)
		{
			excel.SetItemText(i, j, *ir);
			j++;
		}
		tmpVec.clear();
		i++;
	}
	CString strApp = MyBaseUtils::GetAppPath();
	CString strFile = strApp + m_strModSheetName;
	excel.SaveAs(strFile);
	return true;
}

vector<vector<CString> > CLrmvpSheetInfo::getModData() const
{
	return m_modData;
}

CString CLrmvpSheetInfo::getModSheetName() const
{
	return m_strModSheetName;
}

CString CLrmvpSheetInfo::getSheetName() const
{
	return m_strSheetName;
}

CString CLrmvpSheetInfo::modSheetName(CString strSheetName)
{
	CString strPre,strFix, strNextFix, strEnd, strOut, strNum;

	int nFind = m_strSheetName.Find(_T("("));
	if (nFind>=0)
	{
		strPre = m_strSheetName.Left(nFind);
		strEnd = m_strSheetName.Right(m_strSheetName.GetLength() - nFind);
		strEnd = strEnd.Mid(1, strEnd.GetLength() - 2);

		strEnd += _T("Z");

		strOut = strPre + _T("(") + strEnd + _T(")");
	}

	return strOut;
}

void CLrmvpSheetInfo::getPlug()
{
	CString strEnd;
	int nFind = m_strSheetName.Find(_T("("));
	if (nFind >=0)
	{
		strEnd = m_strSheetName.Right(m_strSheetName.GetLength() - nFind);
		strEnd = strEnd.Mid(1, strEnd.GetLength() - 2);
		m_strModPlug = strEnd;
		nFind = m_strModPlug.Find(_T("_"));
		
		if (nFind >= 0)
		{
			m_strModPlug = m_strModPlug.Left(nFind);
		}

		m_strPlug = m_strModPlug.Left(m_strModPlug.GetLength());
		m_strPlug += _T("-Z");
	}

}

vector<CCommonCell> CLrmvpSheetInfo::readVpx(int nItem, int nNum, CNameCell cell)
{
	vector<CCommonCell> comVec;
	comVec.clear();

	CString strItem,strTemp, strTmp, strDuanzi, strTexing;
	CCommonCell commonCell;
	int nCount, nIndex, nSize;
	nIndex = 0;
	bool bIsAdded = false;
	vector<CString> tmpCurVec, tmpVec;
	CString strTemp2,strTemp3,strTemp4,strTemp5,strTemp6,strTemp8;
	CString strName;//腔号
	int nFind= 0;	

	for (int i=nItem+1;i<nNum; i++)	
	{
		//既不是col.也不为空，那么就是具体的数字
		tmpCurVec = m_Data[i];

		strName = tmpCurVec[0];
		if (!strName.IsEmpty())
		{
			nFind = strName.Find(_T("RF"));
			if (nFind >= 0)
			{
				//是射频
				m_bIsCheck = false;
			}
			else
			{
				nFind = strName.Find(_T("MT"));
				if (nFind >= 0)
				{
					//是MT
					m_bIsCheck = false;
				}
				else
				{
					m_bIsCheck = true;
				}
			}
		}
		
		nSize = tmpCurVec.size();
		m_nType = -1;
		strTemp2 = tmpCurVec[2];
		strTemp5 = tmpCurVec[5];
		strTemp8 = tmpCurVec[8];
		strTemp3 = tmpCurVec[3];
		strTemp6 = tmpCurVec[6];
		strTemp4 = tmpCurVec[4];

		strTemp2 = MyParserString::RemoveSymbol(_T("]"), strTemp2);
		strTemp3 = MyParserString::RemoveSymbol(_T("]"), strTemp3);
		strTemp4 = MyParserString::RemoveSymbol(_T("]"), strTemp4);
		strTemp5 = MyParserString::RemoveSymbol(_T("]"), strTemp5);
		strTemp6 = MyParserString::RemoveSymbol(_T("]"), strTemp6);
		strTemp8 = MyParserString::RemoveSymbol(_T("]"), strTemp8);
		if ((strTemp2.CompareNoCase(_T("GND")) == 0) &&
			(strTemp5.CompareNoCase(_T("GND")) == 0) &&
			(strTemp8.CompareNoCase(_T("GND")) == 0))
		{
			m_nType = 1;
		}
		else if ((strTemp3.CompareNoCase(_T("GND")) == 0) &&
			(strTemp6.CompareNoCase(_T("GND")) == 0))
		{
			m_nType = 2;
		}
		else if ((strTemp4.CompareNoCase(_T("GND")) == 0) &&
			(strTemp6.CompareNoCase(_T("GND")) == 0))
		{
			m_nType = 3;
		}
		else if ((strTemp5.CompareNoCase(_T("NA")) == 0))
		{
			m_nType = 4;
		}
		else
		{
			m_nType = 5;
		}
		//if (tmpCurVec[nSize-1].CompareNoCase(_T("GND")) == 0)
		//{
		//	if (tmpCurVec[nSize-2].CompareNoCase(_T("GND")) == 0)
		//	{
		//		m_nType = 1;
		//	}
		//	else
		//	{
		//		m_nType = 3;
		//	}
		//}
		//else 
		//{
		//	if (tmpCurVec[nSize-3].CompareNoCase(_T("GND")) == 0)
		//	{
		//		if (tmpCurVec[nSize-4].CompareNoCase(_T("GND")) == 0)
		//		{
		//			m_nType = 2;
		//		}
		//	}
		//	else
		//	{
		//		if (tmpCurVec[nSize-1].CompareNoCase(_T("NA")) == 0)
		//		{
		//			m_nType = 5;
		//		}
		//		else if (tmpCurVec[nSize - 5].CompareNoCase(_T("NA")) == 0)
		//		{
		//			m_nType = 4;
		//		}
		//		else
		//		{
		//			//如果不是以上5种情况
		//			//if (!doSpecial(tmpCurVec, cell))
		//			{
		//				continue;
		//			}
		//		}
		//	}
		//}

		if (m_nType == 1)
		{
			tmpVec = doEvenRelation(tmpCurVec, cell);
		}
		else if (m_nType == 2)
		{
			tmpVec = doOddRelation(tmpCurVec, cell);
		}
		else if (m_nType == 3)
		{
			tmpVec = doSingleRelation(tmpCurVec, cell);
		}
		else if (m_nType == 4)
		{
			tmpVec = doPowerRelation(tmpCurVec, cell);
		}
		else if (m_nType == 5)
		{
			//tmpVec = doEmptyRelation(tmpCurVec, cell);
			tmpVec = doSpecial(tmpCurVec, cell, i);
		}
		else
		{
			continue;
		}

		m_modData[i] = tmpVec;

		tmpVec.clear();
		tmpCurVec.clear();
	}
	//
	return comVec;
}

CString CLrmvpSheetInfo::GetNameByIndex(int j, int nColNum)
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

vector<CString> CLrmvpSheetInfo::doEvenRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];
	tmpVec[0+1] = tmpCurVec[0+1];//编号
	tmpVec[1+1] = tmpCurVec[1+1];//GND
	tmpVec[2+1] = tmpCurVec[2+1];
	tmpVec[3+1] = tmpCurVec[3+1];
	tmpVec[4+1] = tmpCurVec[4+1];
	tmpVec[5+1] = tmpCurVec[4+1];
	tmpVec[6+1] = tmpCurVec[5+1];
	tmpVec[7+1] = tmpCurVec[6+1];
	tmpVec[8+1] = tmpCurVec[7+1];
	tmpVec[9+1] = tmpCurVec[7+1];
	return tmpVec;
}


vector<CString> CLrmvpSheetInfo::doOddRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];
	tmpVec[0+1] = tmpCurVec[0+1];//编号
	tmpVec[1+1] = tmpCurVec[1+1];//GND
	tmpVec[2+1] = tmpCurVec[2+1];
	tmpVec[3+1] = tmpCurVec[2+1];
	tmpVec[4+1] = tmpCurVec[3+1];
	tmpVec[5+1] = tmpCurVec[4+1];
	tmpVec[6+1] = tmpCurVec[5+1];
	tmpVec[7+1] = tmpCurVec[5+1];
	tmpVec[8+1] = tmpCurVec[6+1];
	tmpVec[9+1] = tmpCurVec[7+1];

	return tmpVec;
}

vector<CString> CLrmvpSheetInfo::doSingleRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];
	tmpVec[0+1] = tmpCurVec[0+1];//编号
	tmpVec[1+1] = tmpCurVec[3+1];//GND
	tmpVec[2+1] = tmpCurVec[1+1];
	tmpVec[3+1] = tmpCurVec[2+1];
	tmpVec[4+1] = tmpCurVec[3+1];
	tmpVec[5+1] = tmpCurVec[4+1];
	tmpVec[6+1] = tmpCurVec[5+1];
	tmpVec[7+1] = tmpCurVec[6+1];
	tmpVec[8+1] = tmpCurVec[7+1];
	tmpVec[9+1] = tmpCurVec[5+1];

	return tmpVec;
}

vector<CString> CLrmvpSheetInfo::doPowerRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[0+1] = tmpCurVec[0+1];//GND
	tmpVec[1+1] = tmpCurVec[1+1];
	tmpVec[2+1] = tmpCurVec[1+1];
	tmpVec[3+1] = tmpCurVec[2+1];
	tmpVec[4+1] = tmpCurVec[3+1];
	tmpVec[5+1] = tmpCurVec[4+1];
	tmpVec[6+1] = tmpCurVec[5+1];
	tmpVec[7+1] = tmpCurVec[6+1];
	tmpVec[8+1] = tmpCurVec[7+1];
	tmpVec[9+1] = tmpCurVec[7+1];

	return tmpVec;
}

vector<CString> CLrmvpSheetInfo::doEmptyRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];
	tmpVec[0+1] = tmpCurVec[0];//编号
	tmpVec[1+1] = tmpCurVec[2];//GND
	tmpVec[2+1] = tmpCurVec[3];
	tmpVec[3+1] = tmpCurVec[4];
	tmpVec[4+1] = tmpCurVec[5];
	tmpVec[5+1] = tmpCurVec[6];
	tmpVec[6+1] = tmpCurVec[7];
	tmpVec[7+1] = tmpCurVec[8];

	return tmpVec;
}

CCommonCell CLrmvpSheetInfo::readEvenData(CString strDuanzi, CString strTexing, CString strQuxiang)
{
	CCommonCell commonCell;
	commonCell.setDuanzi(strDuanzi);
	commonCell.setTexing(strTexing);
	commonCell.setQuxiang(strQuxiang);
	commonCell.setSheetName(m_strSheetName);
	return commonCell;
}

vector<CString> CLrmvpSheetInfo::doSpecial(vector<CString> tmpCurVec, CNameCell cell, int nRow)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	CString strItem = tmpCurVec[1];
	for (int i=0; i<m_nColNum; i++)
	{
		tmpVec[i] = tmpCurVec[i];
	}
	if (strItem.CompareNoCase(_T("Col.")) == 0)
	{
		tmpVec[1+1] = _T("I");
		tmpVec[2+1] = _T("H");
		tmpVec[3+1] = _T("G");
		tmpVec[4+1] = _T("F");
		tmpVec[5+1] = _T("E");
		tmpVec[6+1] = _T("D");
		tmpVec[7+1] = _T("C");
		tmpVec[8+1] = _T("B");
		tmpVec[9+1] = _T("A");
	}
	else
	{
		if (m_bIsCheck)
		{
			CWriteLog log;
			CString strLog;
			log.setPath(CUtility::getLogPath());
			strLog.Format(_T("sheet名称：%s,第%d行数据与现有规则不匹配"), m_strSheetName, nRow+1);
			log.writeCsvLog(strLog);
		}
	}
	
	return tmpVec;
}
