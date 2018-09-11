#include "StdAfx.h"
#include "DirTreeCtrl.h"
#include "SortStringArray.h"

CDirTreeCtrl::CDirTreeCtrl(void)
{
	m_strRoot = _T("");
	m_bFlag = false;
}

CDirTreeCtrl::~CDirTreeCtrl(void)
{
	m_ImageList.Detach();
	m_bFlag = false;
}

BEGIN_MESSAGE_MAP(CDirTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
END_MESSAGE_MAP()


BOOL CDirTreeCtrl::SetSelPath(LPCTSTR strPath)
{
	HTREEITEM hParent = TVI_ROOT;
	int iLen = lstrlen(strPath) + 2;
	TCHAR* pszPath = new TCHAR[iLen];
	TCHAR* pPath = pszPath;
	BOOL bRet = FALSE;
	if (!IsValidPath(strPath))
	{
		delete [] pszPath;
		return FALSE;
	}
	_tcscpy(pszPath, (TCHAR*)strPath);
	_tcsupr(pszPath);
	if (pszPath[_tcsclen(pszPath) - 1] != _T('\\'))
	{
		lstrcat(pszPath, _T("\\"));
	}
	int iLen2 = _tcsclen(pszPath);
	for (WORD i=0; i<iLen2; i++)
	{
		if (pszPath[i] == _T('\\'))
		{
			SetRedraw(FALSE);
			pszPath[i] = _T('\0');
			hParent = SearchSiblingItem(hParent, pPath);
		}
		else
		{
			UINT uState;
			uState = GetItemState(hParent, TVIS_EXPANDEDONCE);
			if (uState)
			{
				Expand(hParent, TVE_EXPAND);
				Expand(hParent, TVE_COLLAPSE | TVE_COLLAPSERESET);
				InsertItem(_T(""), hParent);
				Expand(hParent, TVE_EXPAND);
			}
			else
			{
				Expand(hParent, TVE_EXPAND);
			}	
			pPath += _tcsclen(pPath) + 1;
		}
	}
	delete [] pszPath;
	if (hParent)
	{
		SelectItem(hParent);
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	SetRedraw(TRUE);
	return bRet;
}

HTREEITEM CDirTreeCtrl::AddItem(HTREEITEM hParent, LPCTSTR strPath)
{
	SHFILEINFO shFinfo;
	int iIcon;
	int iIconSel;
	CString strTemp = strPath;

	if (hParent == TVI_ROOT)
	{
		strTemp = m_strParent + strPath;
	}
	if(strTemp.Right(1) != _T('\\'))
	{
		strTemp += _T("\\");
	}
	if(!SHGetFileInfo(strTemp,0,&shFinfo,sizeof(shFinfo),SHGFI_ICON|SHGFI_SMALLICON))
	{
		m_strError= _T("Error Getting systemfileinfo!");
		return NULL;

	}
	iIcon = shFinfo.iIcon;
	DestroyIcon(shFinfo.hIcon);

	if (!SHGetFileInfo(strTemp,0,&shFinfo,sizeof(shFinfo),SHGFI_ICON|SHGFI_OPENICON|SHGFI_SMALLICON))
	{
		m_strError = _T("Error Getting System FileInfo!");
		return NULL;
	}
	iIconSel = shFinfo.iIcon;
	DestroyIcon(shFinfo.hIcon);

	if (strTemp.Right(1) == _T('\\'))
	{
		strTemp.SetAt(strTemp.GetLength()-1, _T('\0'));
	}
	if (hParent == TVI_ROOT)
	{
		return InsertItem(strPath,iIcon,iIconSel,hParent);
	}
	CString strFile = GetSubPath(strTemp);
	if (!strFile.IsEmpty())
	{
		return InsertItem(GetSubPath(strTemp),iIcon,iIconSel,hParent);
	}
	
	return NULL;
}

LPCTSTR CDirTreeCtrl::GetSubPath(LPCTSTR strPath)
{
	static CString strTemp;
	int iPos;
	strTemp = strPath;
	if(strTemp.Right(1) == _T('\\'))
	{
		strTemp.SetAt(strTemp.GetLength()-1, _T('\0'));
	}
	iPos = strTemp.ReverseFind(_T('\\'));
	if (iPos != -1)
	{
		if (!readContent(strTemp))
		{
			strTemp = strTemp.Mid(iPos+1);
			return strTemp;
		}
		else
		{
			CString strTmp = strTemp.Right(3);
			if (strTmp.CompareNoCase(_T(".tj")) == 0)
			{
				int nCount = strTemp.GetLength() - iPos - 4;
				strTemp = strTemp.Mid(iPos+1, nCount);
			}
			else
			{
				strTemp = strTemp.Mid(iPos+1);
			}
		}
		
	}
	return (LPCTSTR)strTemp;
}

BOOL CDirTreeCtrl::DisplayDrives()
{
	DeleteAllItems();
	TCHAR szDrives[128];
	TCHAR * pDrive;
	if ( !GetLogicalDriveStrings(sizeof(szDrives),szDrives))
	{
		m_strError = _T("Error Getting Logical DriveStrings");
		return FALSE;
	}
	pDrive = szDrives;
	while (*pDrive)
	{
		HTREEITEM hPatent = AddItem(TVI_ROOT,pDrive);
		if(FindSubDir(pDrive))
		{
			InsertItem(_T(""),0,0,hPatent);
		}
		pDrive += _tcsclen(pDrive) +1;
	}

	return TRUE;
}

BOOL CDirTreeCtrl::FindSubDir(LPCTSTR strPath)
{
	CFileFind find;
	CString strTemp = strPath;
	BOOL bFind;
	if (strTemp[strTemp.GetLength() -1] == _T('\\'))
	{
		strTemp += _T("*.*");
	}
	else
	{
		strTemp += _T("\\*.*");
	}
	bFind = find.FindFile(strTemp);
	while(bFind)
	{
		bFind = find.FindNextFile();
		if (find.IsDirectory() && !find.IsDots())
		{
			return TRUE;
		}if (!find.IsDirectory() && m_bFile && !find.IsHidden())
		{
			return TRUE;
		}

	}
	return FALSE;

}    

BOOL CDirTreeCtrl::DisPlayTree(LPCTSTR strRoot, BOOL bFile)
{
	DWORD dwStyle = GetStyle();
	if (dwStyle & TVS_EDITLABELS)
	{
		ModifyStyle(TVS_EDITLABELS,0);
	}
	DeleteAllItems();
	if(!GetSysImgList())
	{
		return FALSE;
	}
	m_bFile = bFile;
	if (strRoot == NULL ||strRoot[0] == _T('\0'))
	{
		if (!DisplayDrives())
		{
			return FALSE;
		}
		m_strRoot = _T("");
	}
	else
	{
		m_strRoot = strRoot;	
		HTREEITEM hParent = AddItem(TVI_ROOT,m_strRoot);
		CString strTmp;
		strTmp = m_strParent + strRoot;
		DisplayPath(hParent,strTmp);
	}
	return TRUE;

}

BOOL CDirTreeCtrl::IsValidPath(LPCTSTR strPath)
{
	HTREEITEM hChild;
	CString strItem;
	CString strTempPath = strPath;
	BOOL bFound = FALSE;
	CFileFind find;
	hChild = GetChildItem(TVI_ROOT);
	strTempPath.MakeUpper();
	strTempPath.TrimRight(_T('\\'));
	while (hChild)
	{
		strItem = GetItemText(hChild);
		strItem.MakeUpper();
		if (strItem == strTempPath.Mid(0, strItem.GetLength()))
		{
			bFound = TRUE;
			break;
		}
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
	if (!bFound)
	{
		return FALSE;
	}
	strTempPath += _T("\\null");
	if (find.FindFile(strTempPath))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDirTreeCtrl::GetSysImgList()
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;
	if (GetImageList(TVSIL_NORMAL))
	{
		m_ImageList.Detach();
	}
	hImgList = (HIMAGELIST)SHGetFileInfo(_T("C:\\"),0,&shFinfo,sizeof(shFinfo),SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	if(!hImgList)
	{
		m_strError = _T("Cannot retrive the Handle of system ImageList!");
		return FALSE;
	}
	m_ImageList.m_hImageList = hImgList;
	SetImageList(&m_ImageList,TVSIL_NORMAL);
	return TRUE;

}

void CDirTreeCtrl::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
{
	CFileFind find;
	CString strPathFiles = strPath;
	BOOL bFind;
	CSortStringArray strDirArray;
	CSortStringArray strFileArray;

	if (strPathFiles.Right(1) != _T('\\'))
	{
		strPathFiles += _T("\\");
	}
	
	strPathFiles += _T("*.*");

	bFind = find.FindFile(strPathFiles);
	while(bFind)
	{
		bFind = find.FindNextFile();
		if (find.IsDirectory() && !find.IsDots())
		{
			strDirArray.Add(find.GetFilePath());
		}
		if (!find.IsDirectory() && m_bFile)
		{
			CString strTmp = find.GetFilePath();
			CString strTmp1 = strTmp.Right(3);
			if (strTmp1.CompareNoCase(_T(".tj")) == 0)
			{
				if (readContent(strTmp))
				{
					strFileArray.Add(find.GetFilePath());
				}
			}
		}    
	}
	strDirArray.Sort();
	SetRedraw(FALSE);
	CWaitCursor wait;
	for(int i = 0; i < strDirArray.GetSize(); i++)
	{
		CString strTmp = strDirArray.GetAt(i);
		HTREEITEM hItem = AddItem(hParent, strTmp);
		if (FindSubDir(strDirArray.GetAt(i)))
		{
			InsertItem(_T(""),0,0,hItem);
		}
	}
	if (m_bFile)
	{
		strFileArray.Sort();
		for(int i = 0; i< strFileArray.GetSize(); i++)
		{
			CString strTmp = strFileArray.GetAt(i);
			HTREEITEM hItem = AddItem(hParent, strTmp);
		}
	}
	SetRedraw(TRUE);
}

void CDirTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strPath;
	if (pNMTreeView->itemNew.state & TVIS_EXPANDED)
	{
		ExpandItem(pNMTreeView->itemNew.hItem, TVE_EXPAND);
	}
	else
	{
		HTREEITEM hChild = GetChildItem(pNMTreeView->itemNew.hItem);
		while(hChild)
		{
			DeleteItem(hChild);
			hChild = GetChildItem(pNMTreeView->itemNew.hItem);
		}
		InsertItem(_T(""), pNMTreeView->itemNew.hItem);
	}

	*pResult = 0;
}

void CDirTreeCtrl::ExpandItem(HTREEITEM hItem, UINT nCode)
{
	CString strPath;
	if (nCode == TVE_EXPAND)
	{
		HTREEITEM hChild = GetChildItem(hItem);
		while(hChild)
		{
			DeleteItem(hChild);
			hChild = GetChildItem(hItem);
		}
		strPath = GetFullPath(hItem);
		DisplayPath(hItem,strPath);
	}
}

CString CDirTreeCtrl::GetFullPath(HTREEITEM hItem)
{
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;
	strReturn = _T("");
	while(hParent)
	{
		strTemp = GetItemText(hParent);
		strTemp += _T("\\");
		strReturn = strTemp + strReturn;
		hParent = GetParentItem(hParent);
	}
	strReturn = m_strParent + strReturn;
	strReturn.TrimRight(_T('\\'));
	return strReturn;
}

CString CDirTreeCtrl::GetPath(HTREEITEM hItem)
{
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;
	strReturn = _T("");
	while(hParent)
	{	
		hParent = GetParentItem(hParent);
		strTemp = GetItemText(hParent);
		strTemp += _T("\\");
		strReturn = strTemp + strReturn;

	}
	strReturn.TrimLeft(_T('\\'));
	strReturn = m_strParent + strReturn;
	//strReturn.TrimRight(_T('\\'));
	return strReturn;
}

HTREEITEM CDirTreeCtrl::SearchSiblingItem(HTREEITEM hItem, LPCTSTR strText)
{
	HTREEITEM hFound = GetChildItem(hItem);
	CString strTemp;
	while (hFound)
	{
		strTemp = GetItemText(hFound);
		strTemp.MakeUpper();
		if (strTemp == strText)
		{
			return hFound;
		}
		hFound = GetNextItem(hFound, TVGN_NEXT);
	}
	return NULL;
}


void CDirTreeCtrl::SetParentPath(LPCTSTR strPath)
{
	m_strParent = strPath;
}


void CDirTreeCtrl::SetContent(LPCTSTR strContent)
{
	m_strContent = strContent;
}

bool CDirTreeCtrl::readContent(LPCTSTR strFileName)
{
	bool bIsFind = false;
	if (m_strContent.IsEmpty())
	{
		return true;
	}
	CStdioFile File;
	if (!File.Open(strFileName, CFile::modeReadWrite  | CFile::typeText)) 
	{
		return false;
	}
	CString buffer;
	while(File.ReadString(buffer))
	{
		if (buffer.Find(m_strContent) >=0)
		{
			bIsFind = true;
			break;
		}
	}
	File.Close();
	return bIsFind;
}

bool CDirTreeCtrl::DelFolder(CString strPath)
{
	int nLength = strPath.GetLength();
	TCHAR* NewPath = new TCHAR[nLength+2];
	_tcscpy(NewPath , strPath);
	NewPath[nLength] = _T('\0');
	NewPath[nLength+1] = _T('\0');
	SHFILEOPSTRUCT fileOp;
	ZeroMemory((void*)&fileOp, sizeof(SHFILEOPSTRUCT));
	fileOp.fFlags = FOF_NOCONFIRMATION;
	fileOp.hwnd = NULL;
	fileOp.lpszProgressTitle = NULL;
	fileOp.pFrom = NewPath;
	fileOp.pTo = NULL;
	fileOp.wFunc = FO_DELETE;
	bool bRet = (SHFileOperation(&fileOp) == 0);
	return bRet;
}

bool CDirTreeCtrl::RenameFolder(CString strFromPath, CString strToPath)
{
	int nLengthFrom = strFromPath.GetLength();
	TCHAR* NewPathFrom = new TCHAR[nLengthFrom+2];
	_tcscpy(NewPathFrom, strFromPath);
	NewPathFrom[nLengthFrom] = _T('\0');
	NewPathFrom[nLengthFrom+1] = _T('\0');
	SHFILEOPSTRUCT fileOp;
	ZeroMemory((void*)&fileOp, sizeof(SHFILEOPSTRUCT));
	fileOp.fFlags = FOF_NOCONFIRMATION;
	fileOp.hNameMappings = NULL;
	fileOp.hwnd = NULL;
	fileOp.lpszProgressTitle = NULL;
	fileOp.pFrom = NewPathFrom;
	fileOp.pTo = strToPath;
	fileOp.wFunc = FO_RENAME;
	bool bRet = (SHFileOperation(&fileOp) == 0);
	return bRet;
}
