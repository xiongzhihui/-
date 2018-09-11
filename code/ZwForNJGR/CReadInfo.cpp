#include "StdAfx.h"
#include "CReadInfo.h"
#include <algorithm>

extern CGlobalQuery gQuery;

bool compItem(const int &a, const int &b)
{
	return a < b;
}

CReadInfo::CReadInfo(void)
{
	m_MapName.clear();
	m_Data.clear();
	m_sheetGL.clear();
	m_bIsRF = false;
	m_bIsAddData = false;
}


CReadInfo::~CReadInfo(void)
{
	m_MapName.clear();
	m_Data.clear();
	m_sheetGL.clear();
	m_bIsRF = false;
	m_bIsAddData = false;
}

void CReadInfo::setData(vector<vector<CString> > vData,map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow)
{
	m_Data.insert(m_Data.end(), vData.begin(), vData.end());
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



bool CReadInfo::doIt()
{
	return true;
}

CString CReadInfo::getRealSheetName()
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

int CReadInfo::GetGLType(int nSize)
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

CString CReadInfo::getSheetMod() const
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

CString CReadInfo::getType()
{
	CString m_strType = m_Data[0][1];
	if (m_strType.Find(_T("[模块]")) >= 0)
	{
		m_bIsMod = true;
		m_strType.Replace(_T("[模块]"), _T(""));
	}
	else
	{
		m_bIsMod = false;
	}
	return m_strType;
}

CSheetStoreData CReadInfo::getSheetData() const
{
	return m_sheetGL;
}


vector<int> CReadInfo::getColInt(int nStart, int nNum)
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
	std::sort(VecInt.begin(), VecInt.end(), compItem);
	return VecInt;
}

bool CReadInfo::AddRange(int nRow, int nEndRow)
{
	//Rangle内包含Col类型，Duanzi类型，Comma类型
	CString strTemp = m_Data[nRow][0];
	if ((strTemp.CompareNoCase(_T("母板连接器名称")) == 0) ||
			(strTemp.CompareNoCase(_T("模块连接器名称")) == 0) ||
			(strTemp.CompareNoCase(_T("母板用物资代码")) == 0) ||
			(strTemp.CompareNoCase(_T("模块用物资代码")) == 0))
	{
		nRow++;
	}
	else if (strTemp.CompareNoCase(_T("端子")) == 0)//如果是端子
	{

	}
	else if (strTemp.CompareNoCase(_T("腔号")) == 0)//如果是腔号
	{

	}
	else if (strTemp.IsEmpty())
	{

	}
	return true;
}

void CReadInfo::getCellStatus(CString strItem)
{
	int nFind;
	if (!m_bIsRF)//如果已经设置为RF，说明在外层名称处得到了RF的名称,不需要在cell中查找RF信息
	{
		nFind = strItem.Find(_T("[RF]"));
		if (nFind >= 0)
			m_bIsRF = true;
		else
			m_bIsRF = false;
	}
	

	nFind = strItem.Find(_T("[T]"));
	if (nFind >= 0)
		m_bIsAddData = true;
	else
		m_bIsAddData = false;
}

CCommonCell CReadInfo::addCell(CString strItem, CString strDuanzi, CString strTexing, int nRow, int nCol)
{
	CCommonCell commonCell;
	commonCell.setNameCell(m_NameCell);
	commonCell.setDuanzi(strDuanzi);
	commonCell.setTexing(strItem);
	commonCell.setRealTexing(strTexing);
	CString strQuxiang;
	if ((m_NameCell.getRealName().CompareNoCase(_T("其它")) == 0) ||
		(m_NameCell.getRealName().CompareNoCase(_T("其他")) == 0))
	{
		strQuxiang = strDuanzi + _T(":1");
	}
	else
	{
		strQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strDuanzi;
	}
	commonCell.setModQuxiang(strQuxiang);
	commonCell.setTempQuxiang(strQuxiang);
	commonCell.setRow(nRow);
	commonCell.setCol(nCol);
	//处理MT编号
	int nFind = strTexing.Find(_T("[No="));
	if (nFind >= 0)
	{
		int nTemp = strTexing.Find(_T("]"), nFind);
		CString strTemp = strTexing.Mid(nFind+4, nTemp-nFind - 4);
		commonCell.setCount(strTemp);
	}

	return commonCell;
}

void CReadInfo::addToDataCell(CCommonCell commonCell)
{
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
}
