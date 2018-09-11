//-----------------------------------------------------------------------------
//----- DlgExcelFileCmp.cpp : Implementation of CDlgExcelFileCmp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgExcelFileCmp.h"
#include "MyExcel.h"
#include "Utility.h"
#include "DlgExcelDataCmp.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgExcelFileCmp, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgExcelFileCmp, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEROLD, &CDlgExcelFileCmp::OnBnClickedButtonBrowserold)
	ON_BN_CLICKED(IDC_BUTTON_BROWSERNEW, &CDlgExcelFileCmp::OnBnClickedButtonBrowsernew)
	ON_BN_CLICKED(IDOK, &CDlgExcelFileCmp::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SWAP, &CDlgExcelFileCmp::OnBnClickedButtonSwap)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgExcelFileCmp::CDlgExcelFileCmp (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgExcelFileCmp::IDD, pParent, hInstance) 
{
	m_DifferInfo.clear();
	m_strPreFile = gGlobal.GetIniValue(_T("文件比对"), _T("开始文件"));
	m_strCurFile = gGlobal.GetIniValue(_T("文件比对"), _T("结束文件"));
}

//-----------------------------------------------------------------------------
void CDlgExcelFileCmp::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_FILEPATHOLD, m_strPreFile);
	DDX_Text(pDX, IDC_EDIT_FILEPATHNEW, m_strCurFile);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgExcelFileCmp::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}


void CDlgExcelFileCmp::OnBnClickedButtonBrowserold()
{
	// TODO: 在此添加控件通知处理程序代码
	int nBufLen = 256;
	TCHAR* pBuf = new TCHAR[nBufLen];
	memset( pBuf, 0, nBufLen*sizeof(TCHAR) );

	OPENFILENAME ofn;
	memset( &ofn, 0 ,sizeof(ofn) );
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = adsw_acadMainWnd();
	ofn.lpstrFile = pBuf;
	ofn.nMaxFile = nBufLen;
	ofn.lpstrFilter = _T("选择文件(*.xls|*.xlsx)\0*.xls;*.xlsx\0");
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER|/*OFN_ALLOWMULTISELECT|*/OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NODEREFERENCELINKS;

	CString strFile;
	if ( ::GetOpenFileName( &ofn ) )
	{
		CString strTmp = ofn.lpstrFile;
		int nNewFileItemCount = 0;
		POSITION pos = (POSITION)ofn.lpstrFile;
		while ( pos != NULL )
		{
			strFile = GetNextPathName( pos, ofn ); // 依次找到选择的多个文件名
		}
		delete []pBuf;
	}
	else
	{
		delete []pBuf;
		return;
	}

	GetDlgItem(IDC_EDIT_FILEPATHOLD)->SetWindowText(strFile);
}


void CDlgExcelFileCmp::OnBnClickedButtonBrowsernew()
{
	// TODO: 在此添加控件通知处理程序代码
	int nBufLen = 256;
	TCHAR* pBuf = new TCHAR[nBufLen];
	memset( pBuf, 0, nBufLen*sizeof(TCHAR) );

	OPENFILENAME ofn;
	memset( &ofn, 0 ,sizeof(ofn) );
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = adsw_acadMainWnd();
	ofn.lpstrFile = pBuf;
	ofn.nMaxFile = nBufLen;
	ofn.lpstrFilter = _T("选择文件(*.xls|*.xlsx)\0*.xls;*.xlsx\0");
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER|/*OFN_ALLOWMULTISELECT|*/OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NODEREFERENCELINKS;

	CString strFile;
	if ( ::GetOpenFileName( &ofn ) )
	{
		CString strTmp = ofn.lpstrFile;
		int nNewFileItemCount = 0;
		POSITION pos = (POSITION)ofn.lpstrFile;
		while ( pos != NULL )
		{
			strFile = GetNextPathName( pos, ofn ); // 依次找到选择的多个文件名
		}
		delete []pBuf;
	}
	else
	{
		delete []pBuf;
		return;
	}

	GetDlgItem(IDC_EDIT_FILEPATHNEW)->SetWindowText(strFile);
}


void CDlgExcelFileCmp::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPreFile,strCurFile;
	GetDlgItem(IDC_EDIT_FILEPATHOLD)->GetWindowText(strPreFile);
	GetDlgItem(IDC_EDIT_FILEPATHNEW)->GetWindowText(strCurFile);
	if (strPreFile.IsEmpty() || strCurFile.IsEmpty())
	{
		AfxMessageBox(_T("文件名不能为空"));
		return;
	}
	gGlobal.SetIniValue(_T("文件比对"), _T("开始文件"), strPreFile);
	gGlobal.SetIniValue(_T("文件比对"), _T("结束文件"), strCurFile);
	gGlobal.WriteIniFile();
	CAcUiDialog::OnOK();

	VIVec preMapInfo;
	VIVec curMapInfo;

	CMyExcel excel;
	CString strOut;
	CString strPreSheetName,strCurSheetName;
	if (!excel.Open(strPreFile))
	{
		strOut.Format(_T("文件%s无法打开,请查找原因"), strPreFile);
		AfxMessageBox(strOut);
		return;
	}
	else
	{
		preMapInfo = excel.ReadActiveSheetData(strPreSheetName);
		excel.Clear();
	}
	

	if (!excel.Open(strCurFile))
	{
		strOut.Format(_T("文件%s无法打开,请查找原因"), strCurFile);
		AfxMessageBox(strOut);
		return;
	}
	else
	{
		curMapInfo = excel.ReadActiveSheetData(strCurSheetName);
		excel.Clear();
	}
	if (strPreSheetName.Compare(strCurSheetName) != 0)
	{
		CString strTemp;
		strTemp.Format(_T("%s的当前sheet名：%s与%s的当前sheet名：%s不一致，请查找文图"), strPreFile, strPreSheetName, strCurFile, strCurSheetName);
		AfxMessageBox(strTemp);
		return;
	}

	CmpSheetContent(preMapInfo, curMapInfo);
	if (m_bIsError)
	{
		WriteCsvFile();

		AfxMessageBox(_T("两个Excel数据并不是一一匹配的，请参照log查看数据"));
		return;
	}

	if (m_DifferInfo.size() < 1)
	{
		AfxMessageBox(_T("两个文件内容没有区别"));
		return;
	}
	//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CAcModuleResourceOverride rs;
	CDlgExcelDataCmp dlg;
	dlg.setData(m_DifferInfo);
	dlg.setFile(strPreFile, strCurFile);
	dlg.DoModal();
}


CString CDlgExcelFileCmp::GetNextPathName(POSITION& pos, OPENFILENAME& ofn) 
{
	BOOL bExplorer = ofn.Flags & OFN_EXPLORER;
	TCHAR chDelimiter;
	if (bExplorer)
		chDelimiter = '\0';
	else
		chDelimiter = ' ';

	LPTSTR lpsz = (LPTSTR)pos;
	if (lpsz == ofn.lpstrFile) // first time
	{
		if ((ofn.Flags & OFN_ALLOWMULTISELECT) == 0)
		{
			pos = NULL;
			return ofn.lpstrFile;
		}

		// find char pos after first Delimiter
		while(*lpsz != chDelimiter && *lpsz != '\0')
			lpsz = _tcsinc(lpsz);
		lpsz = _tcsinc(lpsz);

		// if single selection then return only selection
		if (*lpsz == 0)
		{
			pos = NULL;
			return ofn.lpstrFile;
		}
	}

	CString strPath = ofn.lpstrFile;
	if (!bExplorer)
	{
		LPTSTR lpszPath = ofn.lpstrFile;
		while(*lpszPath != chDelimiter)
			lpszPath = _tcsinc(lpszPath);
		strPath = strPath.Left(lpszPath - ofn.lpstrFile);
	}

	LPTSTR lpszFileName = lpsz;
	CString strFileName = lpsz;

	// find char pos at next Delimiter
	while(*lpsz != chDelimiter && *lpsz != '\0')
		lpsz = _tcsinc(lpsz);

	if (!bExplorer && *lpsz == '\0')
		pos = NULL;
	else
	{
		if (!bExplorer)
			strFileName = strFileName.Left(lpsz - lpszFileName);

		lpsz = _tcsinc(lpsz);
		if (*lpsz == '\0') // if double terminated then done
			pos = NULL;
		else
			pos = (POSITION)lpsz;
	}

	// only add '\\' if it is needed
	if (!strPath.IsEmpty())
	{
		// check for last back-slash or forward slash (handles DBCS)
		LPCTSTR lpsz = _tcsrchr(strPath, '\\');
		if (lpsz == NULL)
			lpsz = _tcsrchr(strPath, '/');
		// if it is also the last character, then we don't need an extra
		if (lpsz != NULL &&
			(lpsz - (LPCTSTR)strPath) == strPath.GetLength()-1)
		{
			ASSERT(*lpsz == '\\' || *lpsz == '/');
			return strPath + strFileName;
		}
	}

	return strPath + '\\' + strFileName;
}


bool CDlgExcelFileCmp::CmpSheetName(MIMap preMapInfo, MIMap curMapInfo)
{
	CExcelDifferInfo diffInfo;

	MIMap preInfo,curInfo;
	CString strOldValue,strNewValue, strSheetName, strDescription;
	int i=1;
	int nCurSize = curMapInfo.size();
	int nPreSize = preMapInfo.size();
	MIPair curPair, prePair;
	bool bIsAdded = true;
	for (MIMap::iterator iR = curMapInfo.begin(); iR != curMapInfo.end(); ++iR)
	{
		strDescription.Format(_T("sheet名称在第%d个不同"), i);

		curPair = *iR;
		strSheetName = curPair.first;
		strNewValue = strSheetName;
		
		if (i>nPreSize)
		{
			strDescription.Format(_T("sheet名称在第%d个增加"), i+1);
			diffInfo.setSheetName(strSheetName);
			diffInfo.setDescription(strDescription);
			diffInfo.setNewValue(_T(""));
			diffInfo.setOldValue(_T(""));
			m_DifferInfo.push_back(diffInfo);
		}
		else
		{
			int j=1;
			for (MIMap::iterator iter = preMapInfo.begin(); iter != preMapInfo.end(); ++iter)
			{
				prePair = *iter;
				strOldValue = prePair.first;	
				if (i == j)
				{
					if (strNewValue.Compare(strOldValue) != 0)
					{
						strNewValue.Replace(_T("\r\n"), _T(""));
						strOldValue.Replace(_T("\r\n"), _T(""));
						strNewValue.Trim();
						strOldValue.Trim();
						diffInfo.setSheetName(strSheetName);
						diffInfo.setNewValue(strNewValue);
						diffInfo.setOldValue(strOldValue);
						strDescription.Format(_T("sheet名称在第%d个不同"), i+1);
						diffInfo.setDescription(strDescription);
						diffInfo.setRange(_T(""));
						m_DifferInfo.push_back(diffInfo);
						continue;
					}
					else
					{
						preInfo.push_back(prePair);
						curInfo.push_back(curPair);
					}
				}
				if (j > nCurSize)
				{
					if (bIsAdded)
					{
						strSheetName = prePair.first;
						strDescription.Format(_T("sheet名称在第%d个丢失"), j);
						diffInfo.setSheetName(strSheetName);
						diffInfo.setDescription(strDescription);
						diffInfo.setNewValue(_T(""));
						diffInfo.setOldValue(_T(""));
						diffInfo.setRange(_T(""));
						m_DifferInfo.push_back(diffInfo);
					}
				}
				j++;
			}
			bIsAdded = false;
		}
		i++;
	}


	//CmpSheetContent(preInfo, curInfo);
	return true;	
}

bool CDlgExcelFileCmp::CmpSheetContent(VIVec tmpPreVec, VIVec tmpCurVec)
{
	CString strSheetName,strDescription, strRange, strNewValue,strOldValue;
	vector<CString> tmpPVec;
	vector<CString> tmpCVec;
	CExcelDifferInfo diffInfo;
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
				m_DifferInfo.push_back(diffInfo);
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
									m_DifferInfo.push_back(diffInfo);
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
										m_DifferInfo.push_back(diffInfo);
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
												//strRange.Format(_T("第%d行%d列"), i+1, k+1);
												strRange = CUtility::getExcelPos(i+1, k+1);
												diffInfo.setRange(strRange);
												diffInfo.setOldValue(strTemp);
												diffInfo.setNewValue(_T(""));
												diffInfo.setDescription(_T("数据丢失"));
												m_DifferInfo.push_back(diffInfo);
												m_bIsError = true;
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
							m_DifferInfo.push_back(diffInfo);
							bIsLosed = false;	
							m_bIsError = true;
						}
					}
				}
				j++;
			}
		}
		i++;
	}
	return true;
}

bool CDlgExcelFileCmp::WriteLogFile()
{
	if (m_DifferInfo.size() >= 1)
	{
		CString strFile = CUtility::getLogPath() + _T("\\excel文件比较结果.log");
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
		strLog = _T("Sheet      Range      Old Value      New Value      Description\n");
		File.WriteString(strLog);
		for (vector<CExcelDifferInfo>::iterator iter = m_DifferInfo.begin(); iter != m_DifferInfo.end(); ++iter)
		{
			CExcelDifferInfo dif;
			dif	= *iter;
			strLog = dif.getOutPutString();
			File.SeekToEnd();
			File.WriteString(strLog);
		}
		File.Close();
		ShellExecute(NULL,_T("open"),strFile,NULL,NULL,SW_SHOWNORMAL);
	}
	return true;
}

bool CDlgExcelFileCmp::WriteCsvFile()
{
	if (m_DifferInfo.size() >= 1)
	{
		CString strFile = CUtility::getLogPath() + _T("\\excel文件比较结果.csv");
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
		for (vector<CExcelDifferInfo>::iterator iter = m_DifferInfo.begin(); iter != m_DifferInfo.end(); ++iter)
		{
			CExcelDifferInfo dif;
			dif	= *iter;
			strLog = dif.getCsvOutString();
			File.SeekToEnd();
			File.WriteString(strLog);
		}
		File.Close();
		ShellExecute(NULL,_T("open"),strFile,NULL,NULL,SW_SHOWNORMAL);
	}
	return true;
}


void CDlgExcelFileCmp::OnBnClickedButtonSwap()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPreFile,strCurFile;
	GetDlgItem(IDC_EDIT_FILEPATHOLD)->GetWindowText(strPreFile);
	GetDlgItem(IDC_EDIT_FILEPATHNEW)->GetWindowText(strCurFile);

	GetDlgItem(IDC_EDIT_FILEPATHOLD)->SetWindowText(strCurFile);
	GetDlgItem(IDC_EDIT_FILEPATHNEW)->SetWindowText(strPreFile);
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

