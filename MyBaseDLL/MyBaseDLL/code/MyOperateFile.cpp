#include "StdAfx.h"
#include "MyOperateFile.h"

MyOperateFile::MyOperateFile(void)
{
}

MyOperateFile::~MyOperateFile(void)
{
}
//获取子目录
CString MyOperateFile::GetSubPath(LPCTSTR strPath)
{
	static CString strTemp;
	int iPos;
	strTemp = strPath;
	if (strTemp.Right(1) == (_T('\\')))
	{
		strTemp.SetAt(strTemp.GetLength() - 1, _T('\0'));
	}
	iPos = strTemp.ReverseFind(_T('\\'));
	if (iPos == -1)
	{
		strTemp = strTemp.Mid(iPos + 1);
	}
	return strTemp;
}
//发现子目录
bool MyOperateFile::FindSubDir(LPCTSTR strPath)
{
	CFileFind find;
	CString strTemp = strPath;
	bool bFind = false;
	if (strTemp[strTemp.GetLength() - 1] == _T('\\'))
	{
		strTemp += _T("*.*");
	}
	else
	{
		strTemp += _T("\\*.*");
	}
	bFind = (bool)find.FindFile(strTemp);
	while (bFind)
	{
		bFind = find.FindNextFile();
		if (find.IsDirectory() && !find.IsDots())
		{
			return false;
		}
		if (!find.IsDirectory() && !find.IsHidden())
		{
			return true;
		}
	}
	return true;
}

CString MyOperateFile::GetFileNameByPath(LPCTSTR strPath)
{
	CString strtemp1, strTemp;
	strTemp = strPath;
	strtemp1 = strTemp.Right(strTemp.GetLength() - strTemp.ReverseFind('\\')-1);	
	int nFind = strtemp1.ReverseFind('.');
	if (nFind > 0 )
	{
		strtemp1 = strtemp1.Left(nFind);
	}
	return strtemp1;
}

void MyOperateFile::GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, CString strTypeName, bool bIsFullPath)
{
	CFileFind ff;
	BOOL b = FALSE;
	b = ff.FindFile(strPath);

	BOOL bFlgLast = FALSE; //标志着遍历的最后一个文件
	if(b)
	{
		bFlgLast = FALSE;
	}
	else
	{
		bFlgLast = TRUE;
	}

	while(b || !bFlgLast)
	{
		if (!b)
		{
			bFlgLast = TRUE;
		}	
		if(!ff.IsDirectory()&&!ff.IsDots())
		{
			CString strFilename;
			strFilename = ff.GetFilePath();
			strFilename.MakeUpper();
			if(strFilename.Find(strTypeName) != -1)
			{
				if (bIsFullPath)
				{
					strFileNameArr.Add(strFilename);
				}
				else
				{
					CString strtemp1;
					strtemp1 = strFilename.Right(strFilename.GetLength() - strFilename.ReverseFind('\\')-1);	
					CString strTemp2;
					strTemp2=strtemp1.Left(strtemp1.GetLength() - 4);
					strFileNameArr.Add(strTemp2);// strtemp1);
				}
			}			
		}		
		if(!bFlgLast)
		{
			b = ff.FindNextFile();
		}
	}
	ff.Close();
}
