#include "StdAfx.h"
#include "CLRMVSheet.h"
#include "MyExcel.h"
#include "CDrawEnt.h"
#include <algorithm>
#include "CLrmvTitleInfo.h"
#include "CGlobalQuery.h"
#include "Utility.h"

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
extern CGlobalQuery gQuery;

CLRMVSheet::CLRMVSheet(void)
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


CLRMVSheet::~CLRMVSheet(void)
{

}

void CLRMVSheet::setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow)
{
	m_Data.clear();
	m_MapName.clear();
	
	m_Data.insert(m_Data.end(), vData.begin(), vData.end());
	
	vData.clear();
	m_MapName = tmpVec;

	m_strSheetName = strSheetName;
	m_strModSheetName = modSheetName(strSheetName);

	m_nColNum = nCol;
	m_nRowNum = nRow;
	if (tmpVec.size() > 0)
	{
		map<int, CNameCell>::iterator iter = tmpVec.begin();
		CString strLrm = iter->second.getLjqName();
		CString strModLrm = iter->second.getModLjqName();
		m_ModSingleData.setLrmName(strModLrm);
		m_singleData.setLrmName(strLrm);
	}

	//m_singleData.setType(0);//LRMV类型
	m_singleData.setSheetName(strSheetName);


	//m_ModSingleData.setType(0);
	m_ModSingleData.setSheetName(m_strModSheetName);


}

bool CLRMVSheet::doIt()
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	for (int i=0; i<m_nRowNum; i++)
	{
		m_modData.push_back(tmpVec);
	}
	//获取座子信息
	getPlug();
	readVpxData();
	
	doMod();

	return true;
}


bool CLRMVSheet::prompt()
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
void CLRMVSheet::readVpxData()
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
		//清理差分信号
		gQuery.clearDifferentialSignal();

		for (int i=nCount; i<=nItem; i++)
		{
			for (int j=0; j<m_nColNum; j++)
			{
				m_modData[i][j] = m_Data[i][j];
			}
		}
		m_modData[0][1] = _T("LRMV-P");
		m_modData[1][1] = _T("I");
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
		gQuery.doDifferSignal();
	}
}

bool CLRMVSheet::writeExcel()
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

map<CNameCell, vector<CCommonCell> > CLRMVSheet::getMapInfo() const
{
	return m_MapInfo;
}

map<CNameCell, vector<CCommonCell> > CLRMVSheet::getMapModInfo() const
{
	return m_MapModInfo;
}
vector<vector<CString> > CLRMVSheet::getModExcelData() const
{
	return m_modData;
}

CSheetStoreData CLRMVSheet::getModData()
{
	return m_ModSingleData;
}

CString CLRMVSheet::getSheetName() const
{
	return m_strSheetName;
}

CSheetStoreData CLRMVSheet::getData()
{
	return m_singleData;
}

CString CLRMVSheet::modSheetName(CString strSheetName)
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

void CLRMVSheet::getPlug()
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

vector<CCommonCell> CLRMVSheet::readVpx(int nItem, int nNum, CNameCell cell)
{
	vector<CCommonCell> comVec;
	comVec.clear();

	CString strItem,strTemp, strTmp, strDuanzi, strTexing, strTitle, strPreText;
	CCommonCell commonCell;
	commonCell.setNameCell(cell);
	int nCount, nIndex, nSize, nStart,nEnd, nType;
	nIndex = 0;
	nStart = nEnd = nType = 0;
	
	bool bIsAdded = false;
	vector<CString> tmpCurVec, tmpVec;
	vector<CLrmvTitleInfo> tempVec;
	CLrmvTitleInfo tempTitleInfo;
	m_nameCell = cell;
	for (int i=nItem+1;i<nNum; i++)
	{
		//既不是col.也不为空，那么就是具体的数字
		tmpCurVec = m_Data[i];
		nSize = tmpCurVec.size();
		strTitle = tmpCurVec.at(0);
		
		if (strTitle.CompareNoCase(_T("RF")) == 0)
		{
			//全是射频
			if (strPreText.IsEmpty())
			{
				if (nEnd >= nStart)
				{
					tempTitleInfo.setStartIndex(nStart);
					tempTitleInfo.setEndIndex(nEnd);
					tempTitleInfo.setType(nType);
					tempTitleInfo.setName(strTemp);
					tempVec.push_back(tempTitleInfo);
				}
			}
			nStart = i;
			nEnd = nStart;
			nType = 1;
			strTemp = strTitle;
		}
		else if (strTitle.Find(_T("MT")) >= 0)
		{
			//全是MT
			if (strPreText.IsEmpty())
			{
				if (nEnd >= nStart)
				{
					tempTitleInfo.setStartIndex(nStart);
					tempTitleInfo.setEndIndex(nEnd);
					tempTitleInfo.setType(nType);
					tempTitleInfo.setName(strTemp);
					tempVec.push_back(tempTitleInfo);
				}
			}
			nStart = i;
			nEnd = i;
			nType = 2;
			strTemp = strTitle;
		}
		else if (tmpCurVec.at(1).CompareNoCase(_T("Col.")) == 0)
		{
			//那么是P0这种连接器
			if (strPreText.IsEmpty())
			{
				if (nEnd > nStart)
				{
					tempTitleInfo.setStartIndex(nStart);
					tempTitleInfo.setEndIndex(nEnd);
					tempTitleInfo.setType(nType);
					tempTitleInfo.setName(strTemp);
					tempVec.push_back(tempTitleInfo);
				}
			}
			nStart = i+1;
			nType = 0;
			strTemp = strTitle;
		}
		else if (strTitle.IsEmpty())
		{
			//如果为空，就用上一次的strPreText;
			nEnd = i;
			strPreText = strTitle;
		}
		else
		{
			continue;
		}
	}
	//将最后一个数据添加到vector中存储
	if (nEnd >= nStart)
	{
		tempTitleInfo.setStartIndex(nStart);
		tempTitleInfo.setEndIndex(nEnd);
		tempTitleInfo.setType(nType);
		tempTitleInfo.setName(strTemp);
		tempVec.push_back(tempTitleInfo);
	}

	
	if (tempVec.size() > 0)
	{
		doLrmvTitle(tempVec);
	}
	tempVec.clear();
	return comVec;
}

CString CLRMVSheet::GetNameByIndex(int j, int nColNum)
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

vector<CString> CLRMVSheet::doEvenRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	tmpCurVec.erase(tmpCurVec.begin());
	vector<CString> tmpVec(m_nColNum-3, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[1];//GND
	tmpVec[2] = tmpCurVec[2];
	tmpVec[3] = tmpCurVec[3];
	tmpVec[4] = tmpCurVec[4];
	tmpVec[5] = tmpCurVec[6];
	tmpVec[6] = tmpCurVec[7];
	tmpVec[7] = tmpCurVec[9];

	pair<map<CNameCell, vector<CCommonCell> >::iterator, bool> bRt;
	COMVEC comVec;
	comVec.clear();
	CString strDuanzi, strMidName,strTexing, strQuxiang;
	int nIndex = 0;
	CCommonCell commonCell;

	//commonCell.setSheetName(m_strSheetName);
	commonCell.setType(12);
	strMidName = tmpCurVec[0];
	////第一个数据
	//strDuanzi = _T("A") + strMidName;
	//strTexing = tmpCurVec[9];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);
	////第二个数据
	//strDuanzi = _T("B") + strMidName;
	//strTexing = tmpCurVec[8];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);
	////第三个数据
	//strDuanzi = _T("C") + strMidName;
	//strTexing = tmpCurVec[7];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("B") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第四个数据
	//strDuanzi = _T("D") + strMidName;
	//strTexing = tmpCurVec[6];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("C") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第五个数据
	//strDuanzi = _T("E") + strMidName;
	//strTexing = tmpCurVec[5];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);
	////第六个数据
	//strDuanzi = _T("F") + strMidName;
	//strTexing = tmpCurVec[4];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);
	////第G个数据
	//strDuanzi = _T("G") + strMidName;
	//strTexing = tmpCurVec[3];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("E") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第H个数据
	//strDuanzi = _T("H") + strMidName;
	//strTexing = tmpCurVec[2];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("F") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第I个数据
	//strDuanzi = _T("I") + strMidName;
	//strTexing = tmpCurVec[1];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);

	//m_MapInfo.insert(make_pair(cell, m_ComModVec));

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	commonCell.setSheetName(m_strModSheetName);

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[7];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[6];
	strQuxiang = cell.getName() + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[5];
	strQuxiang = cell.getName() + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[4];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[3];
	strQuxiang = cell.getName() + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[2];
	strQuxiang = cell.getName() + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[1];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	
	comVec.push_back(commonCell);

	bRt = m_ComModVec.insert(make_pair(cell, comVec));
	if (!bRt.second)
	{
		bRt.first->second.insert(bRt.first->second.end(), comVec.begin(), comVec.end());
	}
	comVec.clear();
	//strTmp = m_strModSheetName + _T(":") + strDuanzi;
	return tmpVec;
}


vector<CString> CLRMVSheet::doOddRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	tmpCurVec.erase(tmpCurVec.begin());
	vector<CString> tmpVec(m_nColNum-3, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[1];//GND
	tmpVec[2] = tmpCurVec[2];
	tmpVec[3] = tmpCurVec[4];
	tmpVec[4] = tmpCurVec[5];
	tmpVec[5] = tmpCurVec[6];
	tmpVec[6] = tmpCurVec[8];
	tmpVec[7] = tmpCurVec[9];

	pair<map<CNameCell, vector<CCommonCell> >::iterator, bool> bRt;
	COMVEC comVec;
	comVec.clear();
	CString strDuanzi, strMidName,strTexing, strQuxiang;
	int nIndex = 0;
	CCommonCell commonCell;
	commonCell.setType(12);
	commonCell.setSheetName(m_strSheetName);

	strMidName = tmpCurVec[0];
	commonCell.setSheetName(m_strModSheetName);

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[7];
	strQuxiang = cell.getName() + _T(":") + _T("A") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	comVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[6];
	strQuxiang = cell.getName() + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	comVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[5];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	comVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[4];
	strQuxiang = cell.getName() + _T(":") + _T("E") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	comVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[3];
	strQuxiang = cell.getName() + _T(":") + _T("F") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	comVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[2];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	comVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[1];
	strQuxiang = cell.getName() + _T(":") + _T("I") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	comVec.push_back(commonCell);

	bRt = m_ComModVec.insert(make_pair(cell, comVec));
	if (!bRt.second)
	{
		bRt.first->second.insert(bRt.first->second.end(), comVec.begin(), comVec.end());
	}
	comVec.clear();
	return tmpVec;
}

vector<CString> CLRMVSheet::doSingleRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	tmpCurVec.erase(tmpCurVec.begin());
	vector<CString> tmpVec(m_nColNum-3, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[2];//GND
	tmpVec[2] = tmpCurVec[3];
	tmpVec[3] = tmpCurVec[4];
	tmpVec[4] = tmpCurVec[5];
	tmpVec[5] = tmpCurVec[6];
	tmpVec[6] = tmpCurVec[7];
	tmpVec[7] = tmpCurVec[8];

	pair<map<CNameCell, vector<CCommonCell> >::iterator, bool> bRt;
	COMVEC comVec;
	comVec.clear();
	CString strDuanzi, strMidName,strTexing, strQuxiang;
	int nIndex = 0;
	CCommonCell commonCell;
	commonCell.setType(12);
	commonCell.setSheetName(m_strSheetName);

	strMidName = tmpCurVec[0];
	commonCell.setSheetName(m_strModSheetName);

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[7];
	strQuxiang = cell.getName() + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[6];
	strQuxiang = cell.getName() + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[5];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[4];
	strQuxiang = cell.getName() + _T(":") + _T("E") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[3];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[2];
	strQuxiang = cell.getName() + _T(":") + _T("G") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[1];
	strQuxiang = cell.getName() + _T(":") + _T("H") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	bRt = m_ComModVec.insert(make_pair(cell, comVec));
	if (!bRt.second)
	{
		bRt.first->second.insert(bRt.first->second.end(), comVec.begin(), comVec.end());
	}
	comVec.clear();
	return tmpVec;
}

vector<CString> CLRMVSheet::doPowerRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	tmpCurVec.erase(tmpCurVec.begin());
	vector<CString> tmpVec(m_nColNum-3, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[2];//GND
	tmpVec[2] = tmpCurVec[3];
	tmpVec[3] = tmpCurVec[4];
	tmpVec[4] = tmpCurVec[5];
	tmpVec[5] = tmpCurVec[6];
	tmpVec[6] = tmpCurVec[7];
	tmpVec[7] = tmpCurVec[8];

	pair<map<CNameCell, vector<CCommonCell> >::iterator, bool> bRt;
	COMVEC comVec;
	comVec.clear();
	CString strDuanzi, strMidName,strTexing, strQuxiang;
	CCommonCell commonCell;
	commonCell.setType(12);
	commonCell.setSheetName(m_strSheetName);

	strMidName = tmpCurVec[0];
	//第一个数据
	//strDuanzi = _T("A") + strMidName;
	//strTexing = tmpCurVec[9];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);
	////第二个数据
	//strDuanzi = _T("B") + strMidName;
	//strTexing = tmpCurVec[8];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("A") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第三个数据
	//strDuanzi = _T("C") + strMidName;
	//strTexing = tmpCurVec[7];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("B") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第四个数据
	//strDuanzi = _T("D") + strMidName;
	//strTexing = tmpCurVec[6];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("C") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第五个数据
	//strDuanzi = _T("E") + strMidName;
	//strTexing = tmpCurVec[5];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);
	////第六个数据
	//strDuanzi = _T("F") + strMidName;
	//strTexing = tmpCurVec[4];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("E") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第G个数据
	//strDuanzi = _T("G") + strMidName;
	//strTexing = tmpCurVec[3];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("F") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第H个数据
	//strDuanzi = _T("H") + strMidName;
	//strTexing = tmpCurVec[2];
	//strQuxiang = m_strModPlug + m_strName + _T(":") + _T("G") + strMidName;
	//commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	//m_ComVec.push_back(commonCell);
	////第I个数据
	//strDuanzi = _T("I") + strMidName;
	//strTexing = tmpCurVec[1];
	//commonCell = readEvenData(strDuanzi, strTexing);
	//m_ComVec.push_back(commonCell);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	strDuanzi = _T("A") + strMidName;
	strTexing = tmpVec[7];
	strQuxiang = cell.getName() + _T(":") + _T("B") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("B") + strMidName;
	strTexing = tmpVec[6];
	strQuxiang = cell.getName() + _T(":") + _T("C") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("C") + strMidName;
	strTexing = tmpVec[5];
	strQuxiang = cell.getName() + _T(":") + _T("D") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("D") + strMidName;
	strTexing = tmpVec[4];
	commonCell = readEvenData(strDuanzi, strTexing);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("E") + strMidName;
	strTexing = tmpVec[3];
	strQuxiang = cell.getName() + _T(":") + _T("F") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("F") + strMidName;
	strTexing = tmpVec[2];
	strQuxiang = cell.getName() + _T(":") + _T("G") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	strDuanzi = _T("G") + strMidName;
	strTexing = tmpVec[1];
	strQuxiang = cell.getName() + _T(":") + _T("H") + strMidName;
	commonCell = readEvenData(strDuanzi, strTexing, strQuxiang);
	
	
	comVec.push_back(commonCell);

	bRt = m_ComModVec.insert(make_pair(cell, comVec));
	if (!bRt.second)
	{
		bRt.first->second.insert(bRt.first->second.end(), comVec.begin(), comVec.end());
	}
	comVec.clear();
	return tmpVec;
}

vector<CString> CLRMVSheet::doEmptyRelation(vector<CString> tmpCurVec, CNameCell cell)
{
	tmpCurVec.erase(tmpCurVec.begin());
	vector<CString> tmpVec(m_nColNum-3, _T(""));
	tmpVec[0] = tmpCurVec[0];//编号
	tmpVec[1] = tmpCurVec[2];//GND
	tmpVec[2] = tmpCurVec[3];
	tmpVec[3] = tmpCurVec[4];
	tmpVec[4] = tmpCurVec[5];
	tmpVec[5] = tmpCurVec[6];
	tmpVec[6] = tmpCurVec[7];
	tmpVec[7] = tmpCurVec[8];

	pair<map<CNameCell, vector<CCommonCell> >::iterator, bool> bRt;
	COMVEC comVec;
	comVec.clear();
	CString strDuanzi, strMidName,strTexing, strQuxiang;
	CCommonCell commonCell;
	strTexing = tmpCurVec[1];
	strMidName = tmpCurVec[0];
	//第一个数据	

	//for (int j=9; j>=2; j--)
	//{
	//	CString strbianma = GetNameByIndex(j, 10);	
	//	strDuanzi = strbianma + strMidName;
	//	commonCell.setSheetName(m_strSheetName);
	//	commonCell = readEvenData(strDuanzi, strTexing);
	//	m_ComVec.push_back(commonCell);
	//}
	commonCell.setType(12);
	commonCell.setSheetName(m_strSheetName);
	for (int j=7; j>=1; j--)
	{
		CString strbianma = GetNameByIndex(j, 8);	
		strDuanzi = strbianma + strMidName;
		commonCell.setSheetName(m_strModSheetName);
		commonCell = readEvenData(strDuanzi, strTexing);
		
		
		comVec.push_back(commonCell);
	}

	bRt = m_ComModVec.insert(make_pair(cell, comVec));
	if (!bRt.second)
	{
		bRt.first->second.insert(bRt.first->second.end(), comVec.begin(), comVec.end());
	}
	comVec.clear();
	return tmpVec;
}

CCommonCell CLRMVSheet::readEvenData(CString strDuanzi, CString strTexing, CString strQuxiang)
{
	CCommonCell commonCell;
	commonCell.setNameCell(m_nameCell);
	strTexing = MyParserString::RemoveSymbol(_T("]"), strTexing);
	commonCell.setDuanzi(strDuanzi);
	commonCell.setTexing(strTexing);
	if ((!strTexing.IsEmpty())
		&&(strTexing.CompareNoCase(_T("NC")) != 0)
		&&(strTexing.CompareNoCase(_T("NA")) != 0)
		&&(strTexing.CompareNoCase(_T("GND")) != 0))
	{
		commonCell.setModQuxiang(strQuxiang);
	}
	commonCell.setQuxiang(strQuxiang);
	commonCell.setRealTexing(strTexing);
	commonCell.setSheetName(m_strSheetName);
	commonCell.setType(12);

	return commonCell;
}
void CLRMVSheet::doLrmvTitle(vector<CLrmvTitleInfo> vec)
{
	int nStart,nEnd, nType;
	nStart = nEnd = nType = 0;
	CString strTitle,strItem,strDuanzi,strTempQuxiang;
	CLrmvTitleInfo tempLrmv;
	vector<CString> tmpCurVec;

	CCommonCell comCell;
	comCell.setNameCell(m_nameCell);

	vector<CCommonCell> tempComVec;
	CNameCell nameCell;
	CTableData tempTblData;	

	for (vector<CLrmvTitleInfo>::iterator iter = vec.begin(); iter != vec.end(); iter++)
	{
		tempLrmv = *iter;
		nStart = tempLrmv.startIndex();
		nEnd = tempLrmv.endIndex();
		nType = tempLrmv.type();
		strTitle = tempLrmv.name();
		if (nType == 0)//普通数据
		{			
			tempTblData.clear();
			nameCell = m_nameCell;

			nameCell.setName(nameCell.getName() + strTitle);
			nameCell.setItem(nStart);
			tempTblData.setNamCell(nameCell);
			//输出到excel文件中
			m_modData[nStart-1][0] = strTitle;
			m_modData[nStart-1][1] = _T("Col.");
			m_modData[nStart-1][2] = _T("G");
			m_modData[nStart-1][3] = _T("F");
			m_modData[nStart-1][4] = _T("E");
			m_modData[nStart-1][5] = _T("D");
			m_modData[nStart-1][6] = _T("C");
			m_modData[nStart-1][7] = _T("B");
			m_modData[nStart-1][8] = _T("A");
			
			//处理mod数据
			if (m_nColNum == 11)
			{
				doModData(nStart, nEnd, nameCell);
			}
			int nIndex = 1;
			for (int j=2; j<m_nColNum; j++)
			{
				tempComVec.clear();
				for (int i=nStart; i<=nEnd; i++)
				{
					tmpCurVec = m_Data[i];
					CString strMidName = m_Data[nStart-1][j];	
					strDuanzi = strMidName + tmpCurVec.at(1);
					strItem = tmpCurVec.at(j);
					comCell = doTableItem(strItem);
					//comCell.setRealTexing(strItem);
					comCell.setCol(j+1);
					comCell.setRow(i+1);
					comCell.setSheetName(m_strSheetName);
					comCell.setDuanzi(strDuanzi);
					comCell.setName(m_strName);
					strTempQuxiang = getRealSheetName() + nameCell.getName() + _T(":") + strDuanzi;
					comCell.setTempQuxiang(strTempQuxiang);
					comCell.setModQuxiang(strTempQuxiang);
					gQuery.doCommonCell(comCell);

					tempComVec.push_back(comCell);
					tmpCurVec.clear();
					comCell.setCount(_T(""));
				}
				tempTblData.add(tempComVec);
				tempComVec.clear();
				nIndex++;
			}	
			m_singleData.addTableItem(tempTblData);
			tempTblData.clear();
		}
		else if (nType == 1)//RF数据
		{
			CRFData tmpRfData;
			tmpRfData.clear();
			tempComVec.clear();
			tmpRfData.setNamCell(m_nameCell);
			for (int i=nStart; i<=nEnd; i++)
			{
				tmpCurVec = m_Data[i];
 				for (int j=1; j<m_nColNum; j++)
				{
					strItem = tmpCurVec.at(j);
					if (!strItem.IsEmpty())
					{
						comCell = doRfItem(strItem);
						comCell.setCol(j+1);
						comCell.setRow(i+1);
						comCell.setSheetName(m_strSheetName);
						comCell.setName(m_strName);
						comCell.setType(2);
						gQuery.doRFCommonCell(comCell);

						tempComVec.push_back(comCell);
						comCell.setCount(_T(""));
					}
				}
				tmpCurVec.clear();
			}
			//为输出excel添加代码
			for (int i=nStart; i<=nEnd; i++)
			{
				m_modData[i] = m_Data[i];
			}
			std::sort(tempComVec.begin(), tempComVec.end(), CmpTest);
			tmpRfData.setCommonCellVec(tempComVec);
			m_singleData.addRfDataItem(tmpRfData);
			tmpRfData.clear();
			tempComVec.clear();
		}
		else if (nType == 2)//集束光纤
		{
			CGLData tmpGlData;
			tempComVec.clear();
			nameCell = m_nameCell;

			nameCell.setName(nameCell.getRealName() + strTitle);
			tmpGlData.setNamCell(nameCell);	
			m_strName = nameCell.getName();
			CString strCount;
			for (int j=1; j<m_nColNum; j++)
			{
				for (int i=nStart; i<=nEnd; i++)
				{
					tmpCurVec = m_Data[i];
				
					strItem = tmpCurVec.at(j);
					if (!strItem.IsEmpty())
					{
						comCell = doGLItem(strItem);
						comCell.setCol(j+1);
						comCell.setRow(i+1);
						comCell.setSheetName(m_strSheetName);
						comCell.setName(m_strName);
						if (!comCell.getCount().IsEmpty())
						{
							tmpGlData.setNumber(comCell.getCount());
							strCount = comCell.getCount();
						}
						if (!strCount.IsEmpty())
						{
							comCell.setCount(strCount);
						}
						comCell.setType(5);
						//gQuery.doMTCommonCell(comCell);
						if ((!comCell.getTexing().IsEmpty()) && (comCell.getRealTexing().IsEmpty()) && (comCell.getDuanzi().IsEmpty()))
						{
							CString strCol, strLog;
							strCol.Format(_T("%c"), 64+ comCell.getCol());
							strLog.Format(_T("%s,%d,%s,%s,%s,数据不符合规范"), comCell.getSheetName(),comCell.getRow(), strCol, comCell.getTexing(), comCell.getRealTexing());
							gQuery.addErrorString(strLog);
						}
						gQuery.doMTCommonCell(comCell);
						tempComVec.push_back(comCell);
					}
					tmpCurVec.clear();
				}
			}
			//为输出excel添加代码
			for (int i=nStart; i<=nEnd; i++)
			{
				m_modData[i] = m_Data[i];
 			}
			nType = GetGLType(tempComVec.size());
			//std::sort(tempComVec.begin(), tempComVec.end(), CmpTest);
			tmpGlData.setCommonCellVec(tempComVec);
			tmpGlData.setType(nType);
			m_singleData.addGlDataItem(tmpGlData);
			tmpGlData.clear();
			tempComVec.clear();
		}

		
	}
	vec.clear();
	//////////////////////////////////////////////////////////////////////////
	
}

CCommonCell CLRMVSheet::doTableItem(CString strTexing)
{
	CCommonCell cell;
	cell.setNameCell(m_nameCell);
	cell.setTexing(strTexing);
	CString strTemp = MyParserString::RemoveSymbol(_T("]"), strTexing);
	cell.setRealTexing(strTemp);
	return cell;
}


CCommonCell CLRMVSheet::doRfItem(CString strItem)
{
	CCommonCell cell;
	cell.setNameCell(m_nameCell);
	CString strTexing, tmpQuxiang;
	cell.setTexing(strItem);

	int ndex = CUtility::ParserStringComma(strItem, strTexing);

	strItem = strItem.Left(ndex);
	cell.setDuanzi(strItem);
	tmpQuxiang = getRealSheetName() + m_strName + _T(":") + strItem;
	cell.setTempQuxiang(tmpQuxiang);
	cell.setModQuxiang(tmpQuxiang);
	strTexing = MyParserString::RemoveSymbol(_T("]"), strTexing);
	cell.setRealTexing(strTexing);

	
	return cell;
}

CCommonCell CLRMVSheet::doGLItem(CString strItem)
{
	CCommonCell cell;
	cell.setNameCell(m_nameCell);

	CString strTexing, tmpQuxiang;
	cell.setTexing(strItem);
	
	int ndex = CUtility::ParserStringComma(strItem, strTexing);
	strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_GL);
	
	strItem = strItem.Left(ndex);
	cell.setDuanzi(strItem);
	CString strTempQuxiang = getRealSheetName() + m_strName + _T(":") + strItem;
	cell.setTempQuxiang(strTempQuxiang);
	
	//处理编号
	int nFind = strTexing.Find(_T("[No="));
	if (nFind >= 0)
	{
		int nTemp = strTexing.Find(_T("]"), nFind);
		CString strTemp = strTexing.Mid(nFind+4, nTemp-4-nFind);
		cell.setCount(strTemp);
	}
	strTexing = MyParserString::RemoveSymbol(_T("]"), strTexing);
	//LRMV中去向直接确定
	if ((!strTexing.IsEmpty())
		&& (strTexing.CompareNoCase(_T("NC")) !=0)
		&& (strTexing.CompareNoCase(_T("NA")) != 0)
		&& (strTexing.CompareNoCase(_T("GND")) != 0))
	{
		tmpQuxiang = getRealSheetName() + m_strName + _T(":") + strItem;
		cell.setModQuxiang(tmpQuxiang);
		cell.setTempQuxiang(tmpQuxiang);
	}
	
	cell.setRealTexing(strTexing);
	return cell;
}

int CLRMVSheet::GetGLType(int nSize)
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

CString CLRMVSheet::getRealSheetName()
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

void CLRMVSheet::doModData(int nStart, int nEnd, CNameCell nameCell)
{
	vector<CString> tmpCurVec;
	vector<CString> tmpVec;
	int nSize;
	for (int i=nStart; i<=nEnd; i++)
	{
		tmpCurVec = m_Data[i];
		nSize = tmpCurVec.size();
		if (nSize == 11)//如果大小刚好为11就表示数据能吻合或者后续判断只有类型为LRMV的才能处理mod
		{
			CString strTemp1 = MyParserString::RemoveSymbol(_T("]"), tmpCurVec[nSize-1]);
			CString strTemp2 = MyParserString::RemoveSymbol(_T("]"), tmpCurVec[nSize-2]);
			CString strTemp3 = MyParserString::RemoveSymbol(_T("]"), tmpCurVec[nSize-3]);
			CString strTemp4 = MyParserString::RemoveSymbol(_T("]"), tmpCurVec[nSize-4]);
			CString strTemp5 = MyParserString::RemoveSymbol(_T("]"), tmpCurVec[nSize-5]);
			if (strTemp1.CompareNoCase(_T("GND")) == 0)
			{
				if (strTemp2.CompareNoCase(_T("GND")) == 0)
				{
					m_nType = 1;
				}
				else
				{
					m_nType = 3;
				}
			}
			else 
			{
				if (strTemp3.CompareNoCase(_T("GND")) == 0)
				{
					if (strTemp4.CompareNoCase(_T("GND")) == 0)
					{
						m_nType = 2;
					}
				}
				else
				{
					if (strTemp1.CompareNoCase(_T("NA")) == 0)
					{
						m_nType = 5;
					}
					else if (strTemp5.CompareNoCase(_T("NA")) == 0)
					{
						m_nType = 4;
					}
					else
					{
						return;
					}
				}
			}

			if (m_nType == 1)
			{
				tmpVec = doEvenRelation(tmpCurVec, nameCell);
			}
			else if (m_nType == 2)
			{
				tmpVec = doOddRelation(tmpCurVec, nameCell);
			}
			else if (m_nType == 3)
			{
				tmpVec = doSingleRelation(tmpCurVec, nameCell);
			}
			else if (m_nType == 4)
			{
				tmpVec = doPowerRelation(tmpCurVec, nameCell);
			}
			else
			{
				tmpVec = doEmptyRelation(tmpCurVec, nameCell);
			}
			for (int j=0; j<tmpVec.size(); j++)
			{
				m_modData[i][j+1] = tmpVec[j];
			}
		}
		tmpCurVec.clear();
		tmpVec.clear();
	}
}

bool CLRMVSheet::doMod()
{
	//处理mod
	CTableData tempTblData;
	COMVEC tempComVec;
	CCommonCell comCell;
	if (m_ComModVec.size() > 0)
	{
		CNameCell nameCell;	
		COMVEC tmpVec;
		for (map<CNameCell, COMVEC >::iterator iter = m_ComModVec.begin(); iter != m_ComModVec.end(); ++iter)
		{
			tmpVec = iter->second;
			nameCell = iter->first;
			int nSize = tmpVec.size();
			int mNum = nSize/7;
			tempTblData.clear();

			for (int j=0; j<7; j++)
			{
				tempComVec.clear();
				for (int i=0; i<mNum; i++)
				{
					comCell = tmpVec.at(j+7*i);
					tempComVec.push_back(comCell);
				}
				tempTblData.setNamCell(nameCell);
				tempTblData.add(tempComVec);
			}
			m_ModSingleData.addTableItem(tempTblData);
		}
	}
	else
	{
		m_ModSingleData = m_singleData;
		m_ModSingleData.setSheetName(m_strModSheetName);
		return true;
	}
	CRFData rfData;
	vector<CRFData> tmpRfVec = m_singleData.getRfData();
	for (vector<CRFData>::iterator iter = tmpRfVec.begin(); iter != tmpRfVec.end(); ++iter)
	{
		rfData = *iter;
		m_ModSingleData.addRfDataItem(*iter);
	}
	tmpRfVec.clear();

	CGLData glData;
	vector<CGLData> tmpGlData = m_singleData.getGLData();
	for (vector<CGLData>::iterator iter = tmpGlData.begin(); iter != tmpGlData.end(); ++iter)
	{
		glData = *iter;
		m_ModSingleData.addGlDataItem(*iter);
	}
	tmpGlData.clear();
	return true;
}

CString CLRMVSheet::getModSheetName()
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
	strTemp.Format(_T("%s%d"), strFix, nCount);
	return strTemp;
}