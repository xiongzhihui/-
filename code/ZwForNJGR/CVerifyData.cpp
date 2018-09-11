#include "StdAfx.h"
#include "CVerifyData.h"
#include "Utility.h"
#include "MyExcel.h"
#include "BaseUtils.h"
#include "DlgProress.h"
#include "CGlobalQuery.h"

extern CDlgProress* modelessDlg;
extern Adesk::Boolean startDlg();
extern Adesk::Boolean endDlg();
extern CGlobalQuery gQuery;
CVerifyData::CVerifyData(void)
{
	m_CellInfo.clear();
	m_DifferInfo.clear();
}


CVerifyData::~CVerifyData(void)
{
	m_CellInfo.clear();
	m_DifferInfo.clear();
}

void CVerifyData::setData(COMVEC data)
{
	m_CellInfo.insert(m_CellInfo.end(), data.begin(), data.end());
}

bool CVerifyData::doIt()
{
	bool bRet = true;
	bRet = printToFile();
	if (!bRet)
	{
		return bRet;
	}
	bRet = writeExcelFile();

	printToTxt();
	return bRet;
}

bool CVerifyData::printToFile()
{
	CString strLog;
	CString strCol;
	CString strQuxiang;
	CString strFix = gGlobal.GetIniValue(_T("基础设置"), _T("层级分隔符"));
	CString strFile = CUtility::getFilePath() + _T("info.csv");
	CCommonCell cell;
	CStdioFile File;
	int i = 0;
	int nSize = m_CellInfo.size();
	int nFind = 0;
	if (nSize > 0)
	{
		if(!File.Open(strFile,CFile::modeReadWrite))     
		{     
			if (!File.Open(strFile, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
			{
				return false;
			} 
		}    
		strLog.Format(_T("编号,真实数据,特性,自身的位置,第一个去向, 第二个去向\n"));
		File.WriteString(strLog);

		/*startDlg();
		if (modelessDlg != NULL)
		{
		modelessDlg->SetWindowText(_T("生成中间校验数据"));
		}*/

		for (COMVEC::iterator iter = m_CellInfo.begin(); iter != m_CellInfo.end(); ++iter)
		{
			cell = *iter;

			//addCellToMap(cell);
			File.SeekToEnd();
			strCol.Format(_T("%c"), 64+ cell.getCol());
			CString strCount = cell.getCount();
			strCount.Replace(_T(","), _T("."));
			CString strTexing = cell.getChineseRealTexing();
			strTexing.Replace(_T(","), _T("."));

			strQuxiang = cell.getQuxiang();
			nFind = strQuxiang.Find(strFix);
			if (nFind >= 0)
			{
				CString strPre,strLast;
				strPre = strQuxiang.Left(nFind);
				strLast = strQuxiang.Right(strQuxiang.GetLength() - strFix.GetLength() - nFind);
				
				strLog.Format(_T("%s,%s,%s,%s,%s,%s\n"),
					strCount, 
					strTexing, 
					cell.getRealTexing(), 
					cell.getTempQuxiang(), 
					strPre, 
					strLast);
			}
			else
				strLog.Format(_T("%s,%s,%s,%s,%s\n"), 
				strCount, 
				strTexing, 
				cell.getRealTexing(), 
				cell.getTempQuxiang(), 
				cell.getQuxiang());
			File.WriteString(strLog);

			/*if (modelessDlg != NULL)
			{
			modelessDlg->setPos(i*90/nSize);
			Sleep(50);
			}
			i++;*/
		}

		File.Close();
		/*if (modelessDlg != NULL)
		{
		modelessDlg->setPos(100);
		Sleep(50);
		}
		endDlg();*/

		acutPrintf(_T("\n生成的文件位置为:%s"), strFile);

	}
	return true;
}

bool CVerifyData::writeExcelFile()
{
	bool bRet = true;
	CString strSheetName;
	map<CString, vector<vector<CString> > > tmpExcelInfo = gQuery.getExcelInfo();
	if (tmpExcelInfo.size() > 0)
	{
		//先复制原始的文件
		CString strPreFile = CBaseUtils::filePath();
		if (!gGlobal.IsFileExist(strPreFile))
		{
			CString strPrompt;
			strPrompt.Format(_T("原始文件%s,未找到"), strPreFile);
			return false;
		}
		//判断是否存在同名文件
		CString strFile = MyOperateFile::GetFileNameByPath(strPreFile);
		CString strCurFile = CUtility::getFilePath() + strFile + _T("_校验后") + getFromatFile(strPreFile);
		if (gGlobal.IsFileExist(strCurFile))
		{
			::DeleteFile(strCurFile);
		}
		::CopyFile(strPreFile, strCurFile, FALSE);

		//////////////////////////////////////////////////////////////////////////
		startDlg();
		if (modelessDlg != NULL)
		{
			modelessDlg->SetWindowText(_T("处理原始的EXCEL数据"));
		}

		//操作EXCEL文件
		CMyExcel myExcel;
		BOOL bOpen = myExcel.Open(strCurFile);
		if (!bOpen)
		{
			AfxMessageBox(_T("文件不能打开，请检查路径是否有效"));
			return false;
		}
		map<CString, vector<vector<CString> > > curMapInfo = myExcel.ReadAllData();
		cmpTwoExcelInfo(curMapInfo, tmpExcelInfo);

		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(100);
			Sleep(50);
		}
		endDlg();

		WriteCsvFile();
		myExcel.Clear();
		curMapInfo.clear();
		//acutPrintf(_T("\n校验后的文件位置为:%s"), strCurFile);
	}
	//编写初始文件
	WriteStartFile(tmpExcelInfo);
	tmpExcelInfo.clear();
	return bRet;
}

void CVerifyData::addCellToMap(CCommonCell cell)
{
	/*CString strSheetName;
	strSheetName = cell.getSheetName();
	COMVEC vec;
	vec.clear();
	vec.push_back(cell);

	pair<map<CString, COMVEC>::iterator, bool> ret = m_ExcelInfo.insert(make_pair(strSheetName, vec));
	if (!ret.second)
	{
	ret.first->second.push_back(cell);
	}*/
}

CString CVerifyData::getFromatFile(CString strPath)
{
	CString strtemp1, strTemp;
	strTemp = strPath;
	strtemp1 = strTemp.Right(strTemp.GetLength() - strTemp.ReverseFind('\\')-1);	
	int nFind = strtemp1.ReverseFind('.');
	if (nFind > 0 )
	{
		strTemp = strtemp1.Right(strtemp1.GetLength() - nFind);
	}
	return strTemp;
}

void CVerifyData::cmpTwoExcelInfo(map<CString, vector<vector<CString> > > curMapInfo,
									map<CString, vector<vector<CString> > > tmpExcelInfo)
{
	CString strSheetName;
	vector<vector<CString> > curVec;
	int j=1;
	CExcelDifferInfo diffInfo;
	CString strDescription;
	int nSize = curMapInfo.size();

	for (map<CString, vector<vector<CString> > >::iterator iter = curMapInfo.begin(); iter != curMapInfo.end(); ++iter)
	{
		strSheetName = iter->first;
		curVec = iter->second;
		map<CString, vector<vector<CString> > >::iterator IIR = tmpExcelInfo.find(strSheetName);
		if (IIR == tmpExcelInfo.end())
		{
			//说明没找到，这个sheet的信息没有被读取
			strSheetName = IIR->first;
			strDescription.Format(_T("sheet名称在第%d个丢失"), j);
			diffInfo.setSheetName(strSheetName);
			diffInfo.setDescription(strDescription);
			diffInfo.setNewValue(_T(""));
			diffInfo.setOldValue(_T(""));
			diffInfo.setRange(_T(""));
			CString strTemp = strSheetName;
			m_DifferInfo.insert(make_pair(strTemp, diffInfo));
		}
		else
		{
			CmpSheetContent(curVec, IIR->second, strSheetName);
			curVec.clear();
		}
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(j*90/nSize);
			Sleep(50);
		}
		j++;
	}
	curMapInfo.clear();
	tmpExcelInfo.clear();
}


bool CVerifyData::CmpSheetContent(VIVec tmpPreVec, VIVec tmpCurVec, CString strSheetName)
{
	CString strDescription, strRange, strNewValue,strOldValue;
	vector<CString> tmpPVec;
	vector<CString> tmpCVec;
	CExcelDifferInfo diffInfo;
	diffInfo.setSheetName(strSheetName);
	MIPair curPair, prePair;
	int key = 0;
	bool bIsAdded = true;
	bool bIsLosed = true;
	m_bIsError = false;
	int i=0;
	for (VIVec::iterator curIter = tmpCurVec.begin(); curIter != tmpCurVec.end(); ++curIter)
	{
		tmpCVec = *curIter;
		if (i > tmpPreVec.size()-1)
		{
			if (bIsAdded)
			{
				strDescription.Format(_T("增加第%d行到第%d行数据"), i+1, tmpCurVec.size()+1);
				diffInfo.setDescription(strDescription);
				diffInfo.setNewValue(_T(""));
				diffInfo.setOldValue(_T(""));
				diffInfo.setRange(_T(""));
				CString strTemp = strSheetName+strRange;
				m_DifferInfo.insert(make_pair(strTemp, diffInfo));
				bIsAdded = false;
				m_bIsError = true;
			}	
		}
		else
		{
			int j=0;
			for (VIVec::iterator preIter = tmpPreVec.begin(); preIter != tmpPreVec.end(); ++preIter)
			{
				tmpPVec = *preIter;
				if (i==j)
				{
					int CurIndex = 0;

					for (vector<CString>::iterator curIr = tmpCVec.begin(); curIr != tmpCVec.end(); ++curIr)
					{
						CString strTemp;
						if (CurIndex > tmpPVec.size() - 1)
						{
							for (int k=tmpPVec.size(); k <tmpCVec.size(); k++)
							{
								strTemp = tmpCVec[k];
								if (strTemp.IsEmpty())
								{
									continue;
								}
								else
								{
									//strRange.Format(_T("第%d行%d列"), i+1, k+1);
									strRange = CUtility::getExcelPos(i+1, k+1);
									strTemp.Replace(_T("\r\n"), _T(""));
									strTemp.Trim();
									diffInfo.setRange(strRange);
									diffInfo.setNewValue(strTemp);
									diffInfo.setOldValue(_T(""));
									diffInfo.setDescription(_T("数据增加"));
									CString strTemp = strSheetName+strRange;
									m_DifferInfo.insert(make_pair(strTemp, diffInfo));
								}
							}
						}
						else
						{
							int PreIndex = 0;
							for (vector<CString>::iterator PreIr = tmpPVec.begin(); PreIr != tmpPVec.end(); ++PreIr)
							{
								if (CurIndex == PreIndex)
								{
									strNewValue = tmpCVec[CurIndex];
									strOldValue = tmpPVec[CurIndex];
									if (strNewValue.Compare(strOldValue) != 0)
									{
										strNewValue.Replace(_T("\r\n"), _T(""));
										strOldValue.Replace(_T("\r\n"), _T(""));
										strNewValue.Trim();
										strOldValue.Trim();
										diffInfo.setNewValue(strNewValue);
										diffInfo.setOldValue(strOldValue);
										//strRange.Format(_T("第%d行%d列"), i+1, CurIndex+1);
										strRange = CUtility::getExcelPos(i+1, CurIndex+1);
										diffInfo.setRange(strRange);
										diffInfo.setDescription(_T("数据修改"));
										CString strTemp = strSheetName+strRange;
										m_DifferInfo.insert(make_pair(strTemp, diffInfo));
									}
								}
								else
								{
									if (PreIndex > tmpCVec.size()-1)//如果比它大
									{
										CString strTemp;

										for (int k=tmpCVec.size(); k <tmpPVec.size(); k++)
										{
											strTemp = tmpPVec[k];
											if (strTemp.IsEmpty())
											{
												continue;
											}
											else
											{
												strTemp.Replace(_T("\r\n"), _T(""));
												strTemp.Trim();
												CString strTitle = tmpPVec[0];
												if ((strTemp.CompareNoCase(_T("1")) == 0
													||(strTitle.CompareNoCase(_T("名称")) == 0)))
												{
													//不处理
												}
												else
												{
													//strRange.Format(_T("第%d行%d列"), i+1, k+1);
													strRange = CUtility::getExcelPos(i+1, k+1);
													diffInfo.setRange(strRange);
													diffInfo.setOldValue(strTemp);
													diffInfo.setNewValue(_T(""));
													diffInfo.setDescription(_T("数据丢失"));
													CString strTemp = strSheetName+strRange;
													m_DifferInfo.insert(make_pair(strTemp, diffInfo));
													m_bIsError = true;
												}
											}
										}
									}
								}
								PreIndex++;
							}
						}
						CurIndex++;

					}
				}
				else
				{
					if (j> tmpCurVec.size()-1)
					{
						if (bIsLosed)
						{
							diffInfo.setSheetName(strSheetName);
							strDescription.Format(_T("丢失第%d行到第%d行数据"), j+1, tmpPreVec.size()+1);
							diffInfo.setDescription(strDescription);
							diffInfo.setRange(_T(""));
							diffInfo.setNewValue(_T(""));
							diffInfo.setOldValue(_T(""));
							CString strTemp = strSheetName+strRange;
							m_DifferInfo.insert(make_pair(strTemp, diffInfo));
							bIsLosed = false;	
							m_bIsError = true;
						}
					}
				}
				j++;
				tmpPVec.clear();
			}
		}
		i++;
		tmpCVec.clear();
	}
	return true;
}

bool CVerifyData::WriteCsvFile()
{
	if (m_DifferInfo.size() >= 1)
	{
		CString strFile = CUtility::getFilePath() + _T("校验结果.csv");
		if (gGlobal.IsFileExist(strFile))
		{
			::DeleteFile(strFile);
		}
		CStdioFile File;
		if(!File.Open(strFile,CFile::modeReadWrite))     
		{     
			if (!File.Open(strFile, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
			{
				return false;
			} 
		} 
		CString strLog;
		File.SeekToEnd();
		strLog = _T("Sheet,Range,Old Value,New Value,Description\n");
		File.WriteString(strLog);
		for (map<CString, CExcelDifferInfo>::iterator iter = m_DifferInfo.begin(); iter != m_DifferInfo.end(); ++iter)
		{
			CExcelDifferInfo dif;
			dif	= iter->second;
			strLog = dif.getCsvOutString();
			File.SeekToEnd();
			File.WriteString(strLog);
		}
		File.Close();
		ShellExecute(NULL,_T("open"),strFile,NULL,NULL,SW_SHOWNORMAL);
	}
	else
	{
		AfxMessageBox(_T("校验成功"));
	}
	return true;
}

bool CVerifyData::WriteStartFile(map<CString, vector<vector<CString> > > tmpExcelInfo)
{
	CString strSheetName;
	VIVec tmpVec;
	if (tmpExcelInfo.size() > 0)
	{
		CString strFile = CUtility::getFilePath() + _T("初始文件.csv");
		if (gGlobal.IsFileExist(strFile))
		{
			::DeleteFile(strFile);
		}
		CStdioFile File;
		if(!File.Open(strFile,CFile::modeReadWrite))     
		{     
			if (!File.Open(strFile, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
			{
				return false;
			} 
		} 
		CString strLog;
		

		for (MIMap::iterator iter = tmpExcelInfo.begin(); iter != tmpExcelInfo.end(); ++iter)
		{
			strSheetName = iter->first;
			tmpVec = iter->second;
			strLog = strSheetName + _T("\n");
			File.SeekToEnd();
			File.WriteString(strLog);
			vector<CString> CellVec;

			for (VIVec::iterator ITr = tmpVec.begin(); ITr != tmpVec.end(); ++ITr)
			{
				CellVec = *ITr;	
				strLog = _T("");
				for (vector<CString>::iterator IIIIR = CellVec.begin(); IIIIR != CellVec.end(); ++IIIIR)
				{
					strLog += *IIIIR;
					strLog += _T(",");
				}
				strLog += _T("\n");
				File.SeekToEnd();
				File.WriteString(strLog);

				CellVec.clear();
			}
			tmpVec.clear();
		}
		File.Close();
	}
	tmpExcelInfo.clear();

	return true;
}

bool CVerifyData::printToTxt()
{
	CString strLog;
	CString strFileName = MyOperateFile::GetFileNameByPath(CBaseUtils::filePath());
	CString strFile = CUtility::getFilePath() + strFileName + _T(".txt");
	CCommonCell cell;
	CStdioFile File;
	if (gGlobal.IsFileExist(strFile))
	{
		::DeleteFile(strFile);
	}
	int nSize = m_CellInfo.size();
	if (nSize > 0)
	{
		if(!File.Open(strFile,CFile::modeReadWrite))     
		{     
			if (!File.Open(strFile, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
			{
				return false;
			} 
		}    
		File.WriteString(strLog);

		for (COMVEC::iterator iter = m_CellInfo.begin(); iter != m_CellInfo.end(); ++iter)
		{
			cell = *iter;

			File.SeekToEnd();
			strLog.Format(_T("%s,%s,%s,%d,%d\n"), cell.getTexing(), cell.getRealTexing(), cell.getSheetName(), cell.getRow(), cell.getCol());
			
			File.WriteString(strLog);
		}

		File.Close();
		//acutPrintf(_T("\n生成的文件位置为:%s"), strFile);
	}
	return true;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CExcelDifferInfo::CExcelDifferInfo(void)
{
	m_strRange = _T("");
	m_strNewValue = _T("");
	m_strOldValue = _T("");
}

CExcelDifferInfo::~CExcelDifferInfo()
{

}

void CExcelDifferInfo::setSheetName(CString strSheetName)
{
	m_strSheetName = strSheetName;
}

void CExcelDifferInfo::setRange(CString strRange)
{
	m_strRange = strRange;
}

void CExcelDifferInfo::setOldValue(CString strOldValue)
{
	m_strOldValue = strOldValue;
}

void CExcelDifferInfo::setNewValue(CString strNewValue)
{
	m_strNewValue = strNewValue;
}

void CExcelDifferInfo::setDescription(CString strDescription)
{
	m_strDescription = strDescription;
}

CString CExcelDifferInfo::getSheetName() const
{
	return m_strSheetName;
}

CString CExcelDifferInfo::getRange() const
{
	return m_strRange;
}

CString CExcelDifferInfo::getOldValue() const
{
	return m_strOldValue;
}

CString CExcelDifferInfo::getNewValue() const
{
	return m_strNewValue;
}

CString CExcelDifferInfo::getDescription() const
{
	return m_strDescription;
}

CString CExcelDifferInfo::getOutPutString() const
{
	CString strOut;
	strOut.Format(_T("%s      %s      %s      %s      %s\n"), m_strSheetName, m_strRange, m_strOldValue, m_strNewValue, m_strDescription);
	return strOut;
}

CString CExcelDifferInfo::getCsvOutString() const
{
	CString strOut;
	strOut.Format(_T("%s,%s,%s,%s,%s\n"), m_strSheetName, m_strRange, m_strOldValue, m_strNewValue, m_strDescription);
	return strOut;
}

CExcelDifferInfo& CExcelDifferInfo::operator=(const CExcelDifferInfo& node)
{
	m_strSheetName = node.getSheetName();
	m_strRange = node.getRange();
	m_strOldValue = node.getOldValue();
	m_strNewValue = node.getNewValue();
	m_strDescription = node.getDescription();
	return *this;
}

