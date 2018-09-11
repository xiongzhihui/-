#include "StdAfx.h"
#include "CDigitalOpticalSheet.h"
#include "CGlobalQuery.h"

extern bool CmpTest(const CCommonCell& p1,const CCommonCell& p2);

extern CGlobalQuery gQuery;

CDigitalOpticalSheet::CDigitalOpticalSheet(void)
{
	m_Data.clear();
	m_MapName.clear();
}


CDigitalOpticalSheet::~CDigitalOpticalSheet(void)
{

}

void CDigitalOpticalSheet::setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow, int nType)
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

	m_singleData.setType(0);//LRMV类型
	m_singleData.setSheetName(strSheetName);
	m_nType = nType;
}


CString CDigitalOpticalSheet::modSheetName(CString strSheetName)
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

bool CDigitalOpticalSheet::doIt()
{
	vector<CString> tmpVec(m_nColNum+2, _T(""));
	/*for (int i=0; i<m_nRowNum; i++)
	{
	m_modData.push_back(tmpVec);
	}*/
	//获取座子信息
	//getPlug();
	readCommon();
	return true;
}

void CDigitalOpticalSheet::readCommon()
{
	int nSize = m_MapName.size();
	int nItem, nNextItem;
	CString strItem,strTemp;
	CCommonCell commonCell;

	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
		vector<CCommonCell> comVec;
		comVec.clear();
		//清理差分信号
		gQuery.clearDifferentialSignal();

		nItem = itr->first;
		m_nameCell = itr->second;
	
		m_strName = m_nameCell.getRealName();
		m_glData.setNamCell(m_nameCell);

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

CSheetStoreData CDigitalOpticalSheet::getData()
{
	return m_singleData;
}

int CDigitalOpticalSheet::GetGLType(int nSize)
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

vector<CCommonCell> CDigitalOpticalSheet::readDuanziData(int nItem, int nNum)
{
	CString strDuanzi,strTemp,strTexing, strTmp, strTempQuxiang;
	CCommonCell commonCell;
	commonCell.setNameCell(m_nameCell);
	vector<CCommonCell> comVec;
	comVec.clear();
	int nType,nCount;
	int nMulriple = m_nColNum/2;

	int nIndex = 0;
	bool bIsAdded = false;
	for (int j=0; j<nMulriple; j++)
	{
		CString strMTName;//MT名称
		strTemp = m_Data[nItem][2*j];
		if (strTemp.CompareNoCase(_T("端子")) == 0)
		{
			for (int i=nItem+1; i<nNum; i++)
			{
				strTexing = m_Data[i][1+2*j];
				strDuanzi = m_Data[i][2*j];
				if ((strDuanzi.CompareNoCase(_T("母板连接器名称")) == 0) ||
					(strDuanzi.CompareNoCase(_T("模块连接器名称")) == 0) ||
					(strDuanzi.CompareNoCase(_T("母板用物资代码")) == 0) ||
					(strDuanzi.CompareNoCase(_T("模块用物资代码")) == 0))
				{
					continue;
				}
				CString strLog;
				CString strQuxiang ;
				//处理编号
				int nFind = strTexing.Find(_T("[No="));
				if (nFind >= 0)
				{
					int nTemp = strTexing.Find(_T("]"), nFind);
					strTemp = strTexing.Mid(nFind+4, nTemp - nFind - 4);
					commonCell.setCount(strTemp);
					//strMTName.Format(_T("MT%d"), j+1);暂时不添加MT端子的信息
				}
				strQuxiang = getRealSheetName() + m_strName + strMTName +  _T(":") + strDuanzi;
				commonCell.setTempQuxiang(strQuxiang);
				commonCell.setModQuxiang(strQuxiang);
				
				strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);

				commonCell.setTexing(strTexing);
				commonCell.setRealTexing(strLog);
				commonCell.setRow(i+1);
				commonCell.setCol(2+2*j);
				//commonCell.setExcelCol(2+2*j);
				//commonCell.setExcelRow(i+1);
				commonCell.setSheetName(m_strSheetName);
				commonCell.setName(m_strName);

				//例外情况在此处理
				commonCell.setDuanzi(strDuanzi);
				if (strDuanzi.IsEmpty() && strTexing.IsEmpty())
				{
					continue;
				}
				if (!commonCell.getCount().IsEmpty())
				{
					m_glData.setNumber(commonCell.getCount());
				}
				gQuery.doMTCommonCell(commonCell);
				//暂时数字光和模拟光不分开
				comVec.push_back(commonCell);
			}
			nType = GetGLType(comVec.size());
			//std::sort(comVec.begin(), comVec.end(), CmpTest);
			m_glData.setCommonCellVec(comVec);
			m_glData.setType(nType);
			m_singleData.addGlDataItem(m_glData);

			comVec.clear();
			m_glData.clear();
		}				
	}
	return comVec;
}

CString CDigitalOpticalSheet::getRealSheetName()
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
