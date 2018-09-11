#include "StdAfx.h"
#include "CLrmvSheetInfo.h"
#include "MyExcel.h"
#include "CDrawEnt.h"
#include <algorithm>


CLrmvSheetInfo::CLrmvSheetInfo(void)
{
	m_Data.clear();
	m_modData.clear();
	m_MapName.clear();
	m_MapInfo.clear();
	m_MapModInfo.clear();
	m_ComVec.clear();
	m_ComModVec.clear();
	m_tmpVec.clear();
}


CLrmvSheetInfo::~CLrmvSheetInfo(void)
{

}

void CLrmvSheetInfo::setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow)
{
	m_Data.clear();
	m_MapName.clear();
	
	m_Data.insert(m_Data.end(), vData.begin(), vData.end());
	vData.clear();
	m_MapName = tmpVec;
	tmpVec.clear();
	m_strSheetName = strSheetName;
	m_strModSheetName = modSheetName(strSheetName);

	m_nColNum = nCol;
	m_nRowNum = nRow;
}

bool CLrmvSheetInfo::doIt()
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	for (int i=0; i<m_nRowNum; i++)
	{
		m_modData.push_back(tmpVec);
	}
	//获取座子信息
	getPlug();
	readVpxData();
	return true;
}


bool CLrmvSheetInfo::prompt()
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
		tmpVec.clear();
		acutPrintf(strTemp);
		nCount++;
	}
	return true;
}
void CLrmvSheetInfo::readVpxData()
{
	int nSize = m_MapName.size();
	int nItem, nNextItem;
	CString strItem,strTemp;
	CNameCell cell;
	CCommonCell commonCell;

	
	int nCount = 0;
	
	
	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
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
		m_modData[0][1] = _T("LRMV-PT");
		m_modData[1][1] = _T("J");
		m_modData[nItem][1] = _T("I");
		m_modData[nItem][2] = _T("H");
		m_modData[nItem][3] = _T("G");
		m_modData[nItem][4] = _T("F");
		m_modData[nItem][5] = _T("E");
		m_modData[nItem][6] = _T("D");
		m_modData[nItem][7] = _T("C");
		m_modData[nItem][8] = _T("B");
		m_modData[nItem][9] = _T("A");
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

bool CLrmvSheetInfo::writeExcel()
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

map<CNameCell, vector<CCommonCell> > CLrmvSheetInfo::getMapInfo() const
{
	return m_MapInfo;
}

map<CNameCell, vector<CCommonCell> > CLrmvSheetInfo::getMapModInfo() const
{
	return m_MapModInfo;
}

vector<vector<CString> > CLrmvSheetInfo::getModData() const
{
	return m_modData;
}

CString CLrmvSheetInfo::getModSheetName() const
{
	return m_strModSheetName;
}

CString CLrmvSheetInfo::getSheetName() const
{
	return m_strSheetName;
}

CString CLrmvSheetInfo::modSheetName(CString strSheetName)
{
	CString strPre,strFix, strNextFix, strEnd, strOut, strNum;

	int nFind = m_strSheetName.Find(_T("("));
	if (nFind>=0)
	{
		strPre = m_strSheetName.Left(nFind);
		strEnd = m_strSheetName.Right(m_strSheetName.GetLength() - nFind);
		strEnd = strEnd.Mid(1, strEnd.GetLength() - 3);

		strEnd += _T("Z");

		strOut = strPre + _T("(") + strEnd + _T(")");
	}

	return strOut;
}

void CLrmvSheetInfo::getPlug()
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
//定义比较函数
bool  CmpTest(const CCommonCell& p1,const CCommonCell& p2)
{
	/*return strcmp(p1.aa,p2.aa)>0?1:-1;//小于号表示：从小到大排序（小的在前，大的在后）大于号相反*/
	CString str1, str2, strPreFix1, strPreFix2;
	str1 = p1.getDuanzi();
	str2 = p2.getDuanzi();
	int nLen1 = MyParserString::GetPileLength(str1, strPreFix1);
	int nLen2 = MyParserString::GetPileLength(str2, strPreFix2);
	if (strPreFix1.CompareNoCase(strPreFix2) == 0)
	{
		if (nLen1 > nLen2)
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
		if (strPreFix1.Compare(strPreFix2) > 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
vector<CCommonCell> CLrmvSheetInfo::readVpx(int nItem, int nNum, CNameCell cell)
{
	vector<CCommonCell> comVec;
	comVec.clear();

	CString strItem,strTemp, strTmp, strDuanzi, strTexing;
	CCommonCell commonCell;
	int nCount, nIndex, nSize;
	nIndex = 0;
	bool bIsAdded = false;
	vector<CString> tmpCurVec, tmpVec;
	for (int i=nItem+1;i<nNum; i++)
	{
		//既不是col.也不为空，那么就是具体的数字
		tmpCurVec = m_Data[i];
		nSize = tmpCurVec.size();
		m_nType = -1;
		if ((tmpCurVec[1].CompareNoCase(_T("GND")) == 0) &&
			(tmpCurVec[4].CompareNoCase(_T("GND")) == 0) &&
			(tmpCurVec[7].CompareNoCase(_T("GND")) == 0))
		{
			m_nType = 1;
		}
		else if ((tmpCurVec[2].CompareNoCase(_T("GND")) == 0) &&
			(tmpCurVec[5].CompareNoCase(_T("GND")) == 0))
		{
			m_nType = 2;
		}
		else if ((tmpCurVec[3].CompareNoCase(_T("GND")) == 0) &&
			(tmpCurVec[5].CompareNoCase(_T("GND")) == 0))
		{
			m_nType = 3;
		}
		else if ((tmpCurVec[4].CompareNoCase(_T("NA")) == 0))
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
			tmpVec = doSpecial(tmpCurVec, cell);
		}
		else
		{
			continue;
		}

		m_modData[i] = tmpVec;
		tmpCurVec.clear();
		tmpVec.clear();
	}
	//
	if (m_tmpVec.size() > 0)
	{
		std::sort(m_tmpVec.begin(), m_tmpVec.end(), CmpTest);
		
		m_ComModVec.insert(m_ComModVec.end(), m_tmpVec.begin(), m_tmpVec.end());
		m_ComVec.insert(m_ComVec.end(), m_tmpVec.begin(), m_tmpVec.end());
	}

	m_MapInfo.insert(make_pair(cell, m_ComModVec));
	m_MapModInfo.insert(make_pair(cell, m_ComVec));
	return comVec;
}

CString CLrmvSheetInfo::GetNameByIndex(int j, int nColNum)
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

vector<CString> CLrmvSheetInfo::doEvenRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[1];//GND
	tmpVec[2] = tmpCurVec[2];
	tmpVec[3] = tmpCurVec[3];
	tmpVec[4] = tmpCurVec[4];
	tmpVec[5] = tmpCurVec[4];
	tmpVec[6] = tmpCurVec[5];
	tmpVec[7] = tmpCurVec[6];
	tmpVec[8] = tmpCurVec[7];
	tmpVec[9] = tmpCurVec[7];

	CString strDuanzi, strMidName,strTexing, strQuxiang;
	int nIndex = 0;
	CCommonCell commonCell;
	
	commonCell.setSheetName(m_strSheetName);

	strMidName = tmpCurVec[0];
	//第一个数据
	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[9];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第二个数据
	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[8];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第三个数据
	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[7];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第四个数据
	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[6];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第五个数据
	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[5];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第六个数据
	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[4];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第G个数据
	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[3];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("E") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第H个数据
	strDuanzi = _T("H") + strMidName;
	strTexing = tmpVec[2];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("F") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第I个数据
	strDuanzi = _T("I") + strMidName;
	strTexing = tmpVec[1];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);

	//m_MapInfo.insert(make_pair(cell, m_ComModVec));

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	commonCell.setSheetName(m_strModSheetName);

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpCurVec[7];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpCurVec[6];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpCurVec[5];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpCurVec[4];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpCurVec[3];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpCurVec[2];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpCurVec[1];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	//strTmp = m_strModSheetName + _T(":") + strDuanzi;
	return tmpVec;
}


vector<CString> CLrmvSheetInfo::doOddRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[1];//GND
	tmpVec[2] = tmpCurVec[2];
	tmpVec[3] = tmpCurVec[2];
	tmpVec[4] = tmpCurVec[3];
	tmpVec[5] = tmpCurVec[4];
	tmpVec[6] = tmpCurVec[5];
	tmpVec[7] = tmpCurVec[5];
	tmpVec[8] = tmpCurVec[6];
	tmpVec[9] = tmpCurVec[7];

	CString strDuanzi, strMidName,strTexing, strQuxiang;
	int nIndex = 0;
	CCommonCell commonCell;
	commonCell.setSheetName(m_strSheetName);

	strMidName = tmpCurVec[0];
	//第一个数据
	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[9];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("A") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第二个数据
	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[8];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第三个数据
	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[7];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第四个数据
	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[6];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第五个数据
	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[5];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("D") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第六个数据
	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[4];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("E") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第G个数据
	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[3];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第H个数据
	strDuanzi = _T("H") + strMidName;
	strTexing = tmpVec[2];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第I个数据
	strDuanzi = _T("I") + strMidName;
	strTexing = tmpVec[1];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("G") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	commonCell.setSheetName(m_strModSheetName);

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpCurVec[7];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("A") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpCurVec[6];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpCurVec[5];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpCurVec[4];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("E") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpCurVec[3];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("F") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpCurVec[2];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpCurVec[1];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("I") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);
	return tmpVec;
}

vector<CString> CLrmvSheetInfo::doSingleRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[3];//GND
	tmpVec[2] = tmpCurVec[1];
	tmpVec[3] = tmpCurVec[2];
	tmpVec[4] = tmpCurVec[3];
	tmpVec[5] = tmpCurVec[4];
	tmpVec[6] = tmpCurVec[5];
	tmpVec[7] = tmpCurVec[6];
	tmpVec[8] = tmpCurVec[7];
	tmpVec[9] = tmpCurVec[5];

	CString strDuanzi, strMidName,strTexing, strQuxiang;
	int nIndex = 0;
	CCommonCell commonCell;
	commonCell.setSheetName(m_strSheetName);

	strMidName = tmpCurVec[0];
	//第一个数据
	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[9];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第二个数据
	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[8];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("A") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第三个数据
	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[7];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第四个数据
	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[6];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第五个数据
	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[5];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("D") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第六个数据
	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[4];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第G个数据
	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[3];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("F") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第H个数据
	strDuanzi = _T("H") + strMidName;
	strTexing = tmpVec[2];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("G") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第I个数据
	strDuanzi = _T("I") + strMidName;
	strTexing = tmpVec[1];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	commonCell.setSheetName(m_strModSheetName);

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpCurVec[7];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpCurVec[6];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpCurVec[5];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpCurVec[4];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("E") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpCurVec[3];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpCurVec[2];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("G") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpCurVec[1];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("H") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);
	return tmpVec;
}

vector<CString> CLrmvSheetInfo::doPowerRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[1];//GND
	tmpVec[2] = tmpCurVec[1];
	tmpVec[3] = tmpCurVec[2];
	tmpVec[4] = tmpCurVec[3];
	tmpVec[5] = tmpCurVec[4];
	tmpVec[6] = tmpCurVec[5];
	tmpVec[7] = tmpCurVec[6];
	tmpVec[8] = tmpCurVec[7];
	tmpVec[9] = tmpCurVec[7];

	CString strDuanzi, strMidName,strTexing, strQuxiang;
	CCommonCell commonCell;
	commonCell.setSheetName(m_strSheetName);

	strMidName = tmpCurVec[0];
	//第一个数据
	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[9];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第二个数据
	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[8];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("A") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第三个数据
	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[7];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第四个数据
	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[6];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第五个数据
	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[5];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);
	//第六个数据
	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[4];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("E") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第G个数据
	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[3];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("F") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第H个数据
	strDuanzi = _T("H") + strMidName;
	strTexing = tmpVec[2];
	strQuxiang = m_strModPlug + m_strName + _T(":") + _T("G") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComVec.push_back(commonCell);
	//第I个数据
	strDuanzi = _T("I") + strMidName;
	strTexing = tmpVec[1];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComVec.push_back(commonCell);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	commonCell.setSheetName(m_strModSheetName);

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpCurVec[7];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpCurVec[6];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpCurVec[5];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("D") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpCurVec[4];
	commonCell = readEvenData(strDuanzi, strTexing);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpCurVec[3];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("F") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpCurVec[2];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("G") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpCurVec[1];
	strQuxiang = m_strPlug + m_strName + _T(":") + _T("H") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	m_ComModVec.push_back(commonCell);
	return tmpVec;
}

vector<CString> CLrmvSheetInfo::doEmptyRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[2];//GND
	tmpVec[2] = tmpCurVec[3];
	tmpVec[3] = tmpCurVec[4];
	tmpVec[4] = tmpCurVec[5];
	tmpVec[5] = tmpCurVec[6];
	tmpVec[6] = tmpCurVec[7];
	tmpVec[7] = tmpCurVec[8];

	CString strDuanzi, strMidName,strTexing, strQuxiang;
	CCommonCell commonCell;
	strTexing = tmpCurVec[1];
	strMidName = tmpCurVec[0];
	//第一个数据	

	for (int j=9; j>=2; j--)
	{
		CString strbianma = GetNameByIndex(j, 10);	
		strDuanzi = strbianma + strMidName;
		commonCell.setSheetName(m_strSheetName);
		commonCell = readEvenData(strDuanzi, strTexing);
		m_ComVec.push_back(commonCell);
	}
	
	for (int j=7; j>=2; j--)
	{
		CString strbianma = GetNameByIndex(j, 8);	
		strDuanzi = strbianma + strMidName;
		commonCell = readEvenData(strDuanzi, strTexing);
		commonCell.setSheetName(m_strModSheetName);
		m_ComModVec.push_back(commonCell);
	}
	return tmpVec;
}

CCommonCell CLrmvSheetInfo::readEvenData(CString strDuanzi, CString strTexing, CString strQuxiang)
{
	CCommonCell commonCell;
	commonCell.setDuanzi(strDuanzi);
	commonCell.setTexing(strTexing);
	commonCell.setQuxiang(strQuxiang);
	commonCell.setSheetName(m_strSheetName);
	return commonCell;
}

vector<CString> CLrmvSheetInfo::doSpecial(vector<CString> tmpCurVec, CNameCell cell)
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	tmpVec[0] = tmpCurVec[0];//编码
	tmpVec[3] = tmpCurVec[1];
	tmpVec[4] = tmpCurVec[2];
	tmpVec[5] = tmpCurVec[3];
	tmpVec[6] = tmpCurVec[4];
	tmpVec[7] = tmpCurVec[5];
	tmpVec[8] = tmpCurVec[6];
	tmpVec[9] = tmpCurVec[7];

	bool bRet = true;
	CString strItem, strTexing;
	CCommonCell commonCell;
	vector<CCommonCell> comVec;

	for (vector<CString>::iterator iter = tmpCurVec.begin(); iter != tmpCurVec.end(); ++iter)
	{
		strItem = *iter;
		if (strItem.IsEmpty())
		{
			continue;
		}
		if (strItem.Find(_T("[")) < 0)
		{
			continue;
		}
		if (strItem.Find(_T('(')) > 0)
		{
			strTexing = MyParserString::GetSymbolString(strItem, _T("("), _T(")"));
			int ndex = strItem.Find(_T('('));
			strItem = strItem.Left(ndex);
			
			commonCell.setDuanzi(strItem);

			CString strLog;

			strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);

			commonCell.setTexing(strTexing);
			commonCell.setRealTexing(strLog);
			commonCell.setSheetName(m_strSheetName);
			comVec.push_back(commonCell);
			m_tmpVec.push_back(commonCell);
			/*m_ComModVec.push_back(commonCell);
			m_ComVec.push_back(commonCell);*/
		}
	}
	tmpCurVec.clear();
	return tmpVec;
}
