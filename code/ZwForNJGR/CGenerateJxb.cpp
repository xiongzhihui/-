#include "StdAfx.h"
#include "CGenerateJxb.h"
#include "DlgProress.h"
#include "Utility.h"
#include "CGlobalQuery.h"

extern CGlobalQuery gQuery;
extern CDlgProress* modelessDlg;
extern Adesk::Boolean startDlg();
extern Adesk::Boolean endDlg();

CGenerateJxb::CGenerateJxb(void)
{
	m_bIsJxb = true;
	m_dpOutInfo.clear();
	m_rfOutInfo.clear();
	m_mtOutPutMap.clear();
}


CGenerateJxb::~CGenerateJxb(void)
{
	m_bIsJxb = true;
	m_Excel.Clear();
	m_dpOutInfo.clear();
	m_rfOutInfo.clear();
	m_mtOutPutMap.clear();
}

void CGenerateJxb::setType(bool bIsJxb)
{
	m_bIsJxb = bIsJxb;
}

bool CGenerateJxb::JudgeIsEmpty()
{
	m_dpOutInfo = gQuery.getDpInfo().getOutPutData();
	m_rfOutInfo= gQuery.getRfInfo().getOutPutData();
	//获取光的信息
	m_mtOutPutMap = gQuery.getMTOutPutData();
	if ((m_dpOutInfo.size() == 0) && (m_rfOutInfo.size() == 0) && (m_mtOutPutMap.size() == 0))
	{
		acutPrintf(_T("\n没有低频和射频数据及光纤数据"));
		return false;
	}
	m_nSize = m_dpOutInfo.size() + m_rfOutInfo.size();
	return true;
}

bool CGenerateJxb::openExcel()
{
	CString strPre;
	BOOL bFlag = FALSE;
	if (m_bIsJxb)
	{
		strPre = MyBaseUtils::GetAppPath() + _T("support\\接线表.xls");
		m_strFile = CUtility::getFilePath() + _T("接线表.xls");
		bFlag = ::CopyFile(strPre, m_strFile, FALSE);
	}
	else
	{
		strPre = MyBaseUtils::GetAppPath() + _T("support\\电缆接线关系表.xls");
		m_strFile = CUtility::getFilePath() + _T("电缆接线关系表.xls");
		bFlag = ::CopyFile(strPre, m_strFile, FALSE);
	}
	if (!bFlag)
	{
		CString strPrompt;
		if (m_bIsJxb)
		{
			strPrompt.Format(_T("文件拷贝失败，请查看接线表是否占用"));
		}
		else
		{
			strPrompt.Format(_T("文件拷贝失败，请查看接线关系表是否占用"));
		}
		AfxMessageBox(strPrompt);
		return false;
	}
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		if (m_bIsJxb)
		{
			modelessDlg->SetWindowText(_T("生成接线表"));
		}
		else
		{
			modelessDlg->SetWindowText(_T("生成电缆接线关系表"));
		}
		modelessDlg->setPos(0);
	}
	if (!m_Excel.Open(m_strFile))
	{
		CString strPrompt;
		if (gGlobal.IsFileExist(m_strFile))
		{
			strPrompt.Format(_T("%s文件不可写"), strPre);
		}
		else
		{
			strPrompt.Format(_T("%s文件未找到,请查看配置文件是否完整"), strPre);
		}
		AfxMessageBox(strPrompt);
		modelessDlg->setPos(100);
		endDlg();
		return false;
	}

	m_Excel.OpenSheet(1);
	return true;
}

bool CGenerateJxb::writeJxb()
{
	bool bRet = true;
	int nRow = 7;
	setType(true);
	if (!JudgeIsEmpty())
	{
		return false;
	}
	if (!openExcel())
	{
		return false;
	}
	//处理接线表的格式
	doJxbExcel();

	CString strBmNo = gGlobal.GetIniValue(_T("图框设置"), _T("BM"));
	m_Excel.SetItemText(1, 2, strBmNo);
	m_Excel.SetItemText(45, 10, strBmNo);

	writeDpInfoToExcel(nRow);

	writeRfInfoToExcel(nRow);

	writeMTjxbToExcel(nRow);

	writeRMTJxbToExcel(nRow);

	writeTInfoToExcel(nRow);
	m_Excel.Save();
	m_Excel.Clear();

	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(100);
	}
	endDlg();
	ShellExecute(NULL,_T("open"), m_strFile, NULL, NULL, SW_SHOW);
	return bRet;
}

bool CGenerateJxb::writeJxbgxb()
{
	bool bRet = true;
	int nRow = 2;
	setType(false);
	if (!JudgeIsEmpty())
	{
		return false;
	}
	if (!openExcel())
	{
		return false;
	}

	CString strBmNo = gGlobal.GetIniValue(_T("图框设置"), _T("BM"));

	writeDpInfoToExcel(nRow);

	writeRfInfoToExcel(nRow);

	writeMTJxgxbToExcel(nRow);

	writeTInfoToExcel(nRow);

	m_Excel.Save();
	m_Excel.Clear();

	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(100);
	}
	endDlg();
	ShellExecute(NULL,_T("open"), m_strFile, NULL, NULL, SW_SHOW);
	return bRet;
}

bool CGenerateJxb::writeDpInfoToExcel(int& nRow)
{
	bool bRet = true;
	CString strNo;
	for (map<int, pair<CCommonCell, CCommonCell> >::iterator iter = m_dpOutInfo.begin(); iter != m_dpOutInfo.end(); ++iter)
	{
		strNo.Format(_T("%d"), iter->first);
		if (m_bIsJxb)
		{
			nRow = skipRow(nRow);
			m_Excel.SetItemText(nRow, 2, strNo);
			m_Excel.SetItemText(nRow, 3+1, iter->second.first.getTempQuxiang());
			m_Excel.SetItemText(nRow, 4+2, iter->second.second.getTempQuxiang());
		}
		else
		{
			pair<CCommonCell, CCommonCell> comPair = iter->second;
			CCommonCell preComCell = comPair.first;
			CCommonCell curComCell = comPair.second;
			CString strPrintSymbol;

			m_Excel.SetItemText(nRow, 1, strNo);
			m_Excel.SetItemText(nRow, 4, preComCell.getNameCell().getWzNo());//物资代码
			m_Excel.SetItemText(nRow, 5, preComCell.getWeihao());//位号
			strPrintSymbol = strNo + _T("-") + preComCell.getTempQuxiang();
			m_Excel.SetItemText(nRow, 6, strPrintSymbol);//打印标识
			m_Excel.SetItemText(nRow, 7, preComCell.getDuanzi());//端子
			m_Excel.SetItemText(nRow, 8, preComCell.getRealTexing());//特性

			//另一端
			m_Excel.SetItemText(nRow, 10, curComCell.getNameCell().getWzNo());
			m_Excel.SetItemText(nRow, 5+6, curComCell.getWeihao());//位号
			strPrintSymbol = strNo + _T("-") + curComCell.getTempQuxiang();
			m_Excel.SetItemText(nRow, 6+6, strPrintSymbol);//打印标识
			m_Excel.SetItemText(nRow, 7+6, curComCell.getDuanzi());//端子
			m_Excel.SetItemText(nRow, 8+6, curComCell.getRealTexing());//特性
			m_Excel.SetItemText(nRow, 8+10, _T("低频"));
		}
		
		nRow++;
		if (modelessDlg != NULL)
		{
			if (m_bIsJxb)
			{
				modelessDlg->setPos(20+(nRow-8)* 20/m_nSize);
			}
			else
			{
				modelessDlg->setPos((nRow-2)* 40/m_nSize);
			}
		}
	}
	m_dpOutInfo.clear();
	return bRet;
}

bool CGenerateJxb::writeRfInfoToExcel(int& nRow)
{
	bool bRet = true;
	CString strNo;
	for (map<int, pair<CCommonCell, CCommonCell> >::iterator iter = m_rfOutInfo.begin(); iter != m_rfOutInfo.end(); ++iter)
	{
		strNo.Format(_T("W%d"), iter->first);
		if (m_bIsJxb)
		{
			nRow = skipRow(nRow);
			m_Excel.SetItemText(nRow, 2, strNo);
			m_Excel.SetItemText(nRow, 3+1, iter->second.first.getTempQuxiang());
			m_Excel.SetItemText(nRow, 4+2, iter->second.second.getTempQuxiang());
		}
		else
		{
			pair<CCommonCell, CCommonCell> comPair = iter->second;
			CCommonCell preComCell = comPair.first;
			CCommonCell curComCell = comPair.second;
			CString strPrintSymbol;
			m_Excel.SetItemText(nRow, 1, strNo);
			m_Excel.SetItemText(nRow, 4, preComCell.getNameCell().getWzNo());//物资代码
			m_Excel.SetItemText(nRow, 5, preComCell.getWeihao());//位号
			strPrintSymbol = strNo + _T("-") + preComCell.getTempQuxiang();
			m_Excel.SetItemText(nRow, 6, strPrintSymbol);//打印标识
			m_Excel.SetItemText(nRow, 7, preComCell.getDuanzi());//端子
			m_Excel.SetItemText(nRow, 8, preComCell.getRealTexing());//特性

			//另一端
			m_Excel.SetItemText(nRow, 10, curComCell.getNameCell().getWzNo());
			m_Excel.SetItemText(nRow, 5+6, curComCell.getWeihao());//位号
			strPrintSymbol = strNo + _T("-") + curComCell.getTempQuxiang();
			m_Excel.SetItemText(nRow, 6+6, strPrintSymbol);//打印标识
			m_Excel.SetItemText(nRow, 7+6, curComCell.getDuanzi());//端子
			m_Excel.SetItemText(nRow, 8+6, curComCell.getRealTexing());//特性
			m_Excel.SetItemText(nRow, 8+10, _T("射频"));
		}
		nRow++;
		if (modelessDlg != NULL)
		{
			if (m_bIsJxb)
			{
				modelessDlg->setPos(20 + (nRow-8)* 20/m_nSize);
			}
			else
			{
				modelessDlg->setPos((nRow-2)* 40/m_nSize);
			}
		}
	}
	m_rfOutInfo.clear();
	return bRet;
}

bool CGenerateJxb::writeMTjxbToExcel(int& nRow)
{
	bool bRet = true;
	vector<pair<CCommonCell, CCommonCell> > tmpVec;
	tmpVec.clear();
	int nSize = m_mtOutPutMap.size();
	int j=0;
	CString strNo;
	for (map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator iter = m_mtOutPutMap.begin(); iter != m_mtOutPutMap.end(); ++iter)
	{
		strNo = iter->first;
		tmpVec = iter->second;
		int i=0;
		for (vector<pair<CCommonCell, CCommonCell> >::iterator iTr = tmpVec.begin(); iTr != tmpVec.end(); ++iTr)
		{
			nRow = skipRow(nRow);
			m_Excel.SetItemText(nRow, 2, strNo);
			m_Excel.SetItemText(nRow, 3+1, iTr->first.getTempQuxiang());
			m_Excel.SetItemText(nRow, 4+2, iTr->second.getTempQuxiang());
			nRow++;
			i++;
			if (modelessDlg != NULL)
			{
				modelessDlg->setPos(40 + i*j*20/(nSize*tmpVec.size()));
			}
		}
		j++;
		tmpVec.clear();
	}
	m_mtOutPutMap.clear();
	return bRet;
}

bool CGenerateJxb::writeRMTJxbToExcel(int& nRow)
{
	vector<pair<CCommonCell, CCommonCell> > RRMTData = gQuery.getRMTInfo();
	CCommonCell preComCell,curComCell;
	pair<CCommonCell, CCommonCell> jxPair;
	CString strNo;
	int nSize = RRMTData.size();
	int nCount = 0;
	for (vector<pair<CCommonCell, CCommonCell> >::iterator iter = RRMTData.begin(); iter != RRMTData.end(); ++iter)
	{
		jxPair = *iter;
		preComCell = jxPair.first;
		curComCell = jxPair.second;
		if (preComCell.getCount().CompareNoCase(curComCell.getCount()) == 0)
		{
			strNo.Format(_T("%s"), preComCell.getCount());
		}
		else
		{
			strNo.Format(_T("%s-%s"), preComCell.getCount(), curComCell.getCount());
		}
		nRow = skipRow(nRow);
		m_Excel.SetItemText(nRow, 2, strNo);
		m_Excel.SetItemText(nRow, 3+1, preComCell.getTempQuxiang());
		m_Excel.SetItemText(nRow, 4+2, curComCell.getTempQuxiang());
		nCount++;
		nRow++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(60 + 30*nCount/(nSize));
		}
	}
	RRMTData.clear();
	return true;
}

bool CGenerateJxb::writeMTJxgxbToExcel(int& nRow)
{
	map<CString, vector<pair<CCommonCell, CCommonCell> > > mapInfo = gQuery.doMTInfoToMap();

	int nSize = mapInfo.size();
	pair<CCommonCell, CCommonCell> jxPair;
	vector<pair<CCommonCell, CCommonCell> > tmpPairVec;
	CCommonCell preComCell;
	CCommonCell curComCell;
	CString strNo;
	int nStartRow,nEndRow;
	int nCount = 1;

	for (map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator iter = mapInfo.begin(); iter != mapInfo.end(); ++iter)
	{
		tmpPairVec = iter->second;
		bool bFirst = true;
		nStartRow = nRow;
		nEndRow = nRow +tmpPairVec.size();	
		m_Excel.MergeCells(nRow, 1, nEndRow, 1);

		for (vector<pair<CCommonCell, CCommonCell> >::iterator Itr = tmpPairVec.begin(); Itr != tmpPairVec.end(); ++Itr)
		{
			jxPair = *Itr;
			preComCell = jxPair.first;
			curComCell = jxPair.second;
			if (bFirst)
			{
				strNo.Format(_T("%s"), preComCell.getCount());
				m_Excel.SetItemText(nRow, 1, strNo);
				bFirst = false;
			}
			m_Excel.SetItemText(nRow, 4, preComCell.getNameCell().getWzNo());
			m_Excel.SetItemText(nRow, 5, preComCell.getRealSheetName() + preComCell.getWeihao());//位号
			m_Excel.SetItemText(nRow, 6, preComCell.getTempQuxiang());//打印标识
			m_Excel.SetItemText(nRow, 7, preComCell.getDuanzi());//端子
			m_Excel.SetItemText(nRow, 8, preComCell.getRealTexing());//特性

			//另一端
			m_Excel.SetItemText(nRow, 10, curComCell.getNameCell().getWzNo());
			m_Excel.SetItemText(nRow, 5+6, curComCell.getRealSheetName() + curComCell.getWeihao());//位号
			m_Excel.SetItemText(nRow, 6+6, curComCell.getTempQuxiang());//打印标识
			m_Excel.SetItemText(nRow, 7+6, curComCell.getDuanzi());//端子
			m_Excel.SetItemText(nRow, 8+6, curComCell.getRealTexing());//特性
			m_Excel.SetItemText(nRow, 8+10, _T("集束光纤"));
			nRow++;
		}

		tmpPairVec.clear();

		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(40 + nCount* 50/nSize);
		}
		nCount++;
	}
	mapInfo.clear();
	return true;
}

bool CGenerateJxb::writeTInfoToExcel(int& nRow)
{
	COMVEC TInfo = gQuery.getTInfo();
	CCommonCell preComCell;
	for (vector<CCommonCell>::iterator iter = TInfo.begin(); iter != TInfo.end(); ++iter)
	{
		preComCell = *iter;
		if (m_bIsJxb)
		{
			nRow = skipRow(nRow);
			m_Excel.SetItemText(nRow, 3+1,preComCell.getTempQuxiang());
		}
		else
		{
			m_Excel.SetItemText(nRow, 5, preComCell.getRealSheetName() + preComCell.getWeihao());//位号
			m_Excel.SetItemText(nRow, 6, preComCell.getTempQuxiang());//打印标识
			m_Excel.SetItemText(nRow, 7, preComCell.getDuanzi());//端子
			m_Excel.SetItemText(nRow, 8, preComCell.getRealTexing());//特性
		}
		nRow++;
	}
	
	TInfo.clear();
	return true;
}

int CGenerateJxb::skipRow(int nRow)
{
	if (nRow == 45)
	{
		nRow = 49; 
	}

	return nRow;
}

bool CGenerateJxb::insertRow(int nRow)
{
	//大于等于48，并余数为0
	int nMod = nRow/48;
	int nYushu = nRow%48;

	int nFlagRow = 51;
	for (int j=0; j<nMod; j++)
	{
		for (int i=0; i<48; i++)
		{
			m_Excel.InsertRow(_T("A40"), _T("L40"), nFlagRow);
			if (modelessDlg != NULL)
			{
				modelessDlg->setPos(j*20/nMod + (i*20)/(48*nMod));
			}
		}
	}

	//处理第51行数据
	if (nRow > 48)
	{
		m_Excel.MergeCells(nFlagRow, 2, nFlagRow+1, 3);
		m_Excel.MergeCells(nFlagRow, 4, nFlagRow+1, 5);
		m_Excel.MergeCells(nFlagRow, 6, nFlagRow+1, 7);
	}
	return true;
}

void CGenerateJxb::doJxbExcel()
{
	int nDpCount = m_dpOutInfo.size();
	int nRfCount = m_rfOutInfo.size();
	int nSize = 0;
	vector<pair<CCommonCell, CCommonCell> > tmpVec;
	for (map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator iter = m_mtOutPutMap.begin(); iter != m_mtOutPutMap.end(); ++iter)
	{
		tmpVec = iter->second;
		nSize += tmpVec.size();
		tmpVec.clear();
	}
	vector<pair<CCommonCell, CCommonCell> > RRMTData = gQuery.getRMTInfo();
	COMVEC TInfo = gQuery.getTInfo();
	nSize += RRMTData.size();
	nSize += nDpCount;
	nSize += nRfCount;
	nSize += TInfo.size();

	insertRow(nSize);

	
}
