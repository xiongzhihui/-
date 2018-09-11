// IniFile.cpp: implementation of the CIniFile class.
// Written by: Adam Clauss
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"
//#include "fstream.h"
#include <fstream>
#include "MyTransFunc.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
//CIniFile m_IniTool;
//default constructor

CIniFile::CIniFile()
{
}

//constructor, can specify pathname here instead of using SetPath later
//default destructor
CIniFile::~CIniFile()
{

}

/////////////////////////////////////////////////////////////////////
// Public Functions
/////////////////////////////////////////////////////////////////////

//sets path of ini file to read and write from
void CIniFile::SetPath(CString newpath)
{
	path = newpath;
}

//reads ini file specified using CIniFile::SetPath()
//returns true if successful, false otherwise
bool CIniFile::ReadFile()
{
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(path,status))
		return 0;
	ifstream inifile;
	CString readinfo;
	inifile.open(path);
	int curkey = -1, curval = -1;
	if (inifile.fail())
	{
		error = _T("Unable to open ini file.");
		return 0;
	}
	CString keyname, valuename, value;
	CString temp;
	while (getline(inifile,readinfo))
	{
		if (readinfo != _T(""))
		{
			if (readinfo[0] == '[' && readinfo[readinfo.GetLength()-1] == ']') //if a section heading
			{
				keyname = readinfo;
				keyname.TrimLeft('[');
				keyname.TrimRight(']');
			}
			else //if a value
			{
				valuename = readinfo.Left(readinfo.Find(_T("=")));
				value = readinfo.Right(readinfo.GetLength()-valuename.GetLength()-1);
				SetValue(keyname,valuename,value);
			}
		}
	}
	inifile.close();
	return 1;
}

//writes data stored in class to ini file
void CIniFile::WriteFile()
{
	ofstream inifile;
	inifile.open(path);
	for (int keynum = 0; keynum <= names.GetUpperBound(); keynum++)
	{
		if (keys[keynum].names.GetSize() != 0)
		{
#ifdef UNICODE
			char tmp1[256];
			WideCharToMultiByte(CP_ACP,0,names[keynum],-1,tmp1,MAX_PATH,NULL,NULL); 
			inifile << '[' << tmp1 << ']' << endl;
#else
			inifile << '[' << names[keynum].GetBuffer(0) << ']' << endl;
#endif
			for (int valuenum = 0; valuenum <= keys[keynum].names.GetUpperBound(); valuenum++)
			{
#ifdef UNICODE
				char tmp2[256];
				WideCharToMultiByte(CP_ACP,0,keys[keynum].names[valuenum],-1,tmp2,MAX_PATH,NULL,NULL); 
				char tmp3[256];
				WideCharToMultiByte(CP_ACP,0,keys[keynum].values[valuenum],-1,tmp3,MAX_PATH,NULL,NULL); 
				inifile << tmp2 << "=" << tmp3;
#else
				inifile << keys[keynum].names[valuenum].GetBuffer(0) << _T("=") << keys[keynum].values[valuenum].GetBuffer(0);
#endif
				//inifile << keys[keynum].names[valuenum].GetBuffer(0) << "=" << keys[keynum].values[valuenum].GetBuffer(0);
				if (valuenum != keys[keynum].names.GetUpperBound())
					inifile << endl;
				else
					if (keynum < names.GetSize())
						inifile << endl;
			}
			if (keynum < names.GetSize())
				inifile << endl;
		}
	}
	inifile.close();
}

//void CIniFile::WriteFile()
//{
//	ofstream inifile;
//	inifile.open(path);
//	for (int keynum = 0; keynum <= names.GetUpperBound(); keynum++)
//	{
//		if (keys[keynum].names.GetSize() != 0)
//		{
//			inifile << '[' << names[keynum] << ']' << endl;
//			for (int valuenum = 0; valuenum <= keys[keynum].names.GetUpperBound(); valuenum++)
//			{
//				inifile << keys[keynum].names[valuenum] << "=" << keys[keynum].values[valuenum];
//				if (valuenum != keys[keynum].names.GetUpperBound())
//					inifile << endl;
//				else
//					if (keynum < names.GetSize())
//						inifile << endl;
//			}
//			if (keynum < names.GetSize())
//				inifile << endl;
//		}
//	}
//	inifile.close();
//}

//deletes all stored ini data
void CIniFile::Reset()
{
	keys.SetSize(0);
	names.SetSize(0);
}

//returns number of keys currently in the ini

/*************函数定义****************
函数名   : CIniFile::GetNumKeys
函数描述 : 返回键的数目
返回值   : int 
最后修改日期：2003-4-28 16:59:53
**************************************/
int CIniFile::GetNumKeys()
{
	return keys.GetSize();
}

void CIniFile::GetStrKeys(CStringArray& allKeys)	//返回所有[XXXX]节名的字符
{
	allKeys.RemoveAll();
	for(int i=0;i<names.GetSize();i++)
		allKeys.Add(names[i]);
}

int CIniFile::GetKeyValues(CString keyname,CStringArray& allValues)		//返回某一节[XXXX]下的所有key的名字
{
	allValues.RemoveAll();
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return -1;
	else
	{
		for(int i=0;i<keys[keynum].names.GetSize();i++)
			allValues.Add(keys[keynum].names[i]);

		return keys[keynum].names.GetSize();
	}
}
int Myatoi(CString inputStr)
{
	BOOL isNumeic=TRUE;
	TCHAR temp[256];
	int len=0;
	wsprintf(temp,_T("%s"),inputStr);
	len=_tcslen(temp);
	//1.判断是否纯数字 否的话返回-1
	for(int i=0;i<len;i++)
	{
		if (temp[i]<'0'||temp[i]>'9')
		{
			isNumeic=FALSE;
			break;
		}
	}

	//2.是就返回_ttoi
	if(!isNumeic)
		return -1;
	else
		return _ttoi(temp);
}

int CIniFile::GetKeyValues(CString keyname,CStringArray& allValues,int nGarbishNums)		//返回某一节[XXXX]下的所有key的名字
{
	allValues.RemoveAll();
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return -1;
	else
	{
		for(int i=0;i<keys[keynum].names.GetSize();i++)
		{
			CString strName = keys[keynum].names[i];
			int nTemp=Myatoi(strName);
			CString strTemp;
			strTemp.Format(_T("%d"),nTemp);

			BOOL boo= strTemp.CompareNoCase(strName)==0;
			if(!boo)
			{
				allValues.Add(strName);
				continue;
			}

			if(nTemp<=nGarbishNums && nTemp>=1)
				allValues.Add(strName);
		}

		return keys[keynum].names.GetSize();
	}
}

//returns number of values stored for specified key, or -1 if key found

/*************函数定义****************
函数名   : CIniFile::GetNumValues
函数描述 : 返回某一键的键值的数目
返回值   : int 
参数1    : CString keyname
最后修改日期：2003-4-28 17:00:25
**************************************/
int CIniFile::GetNumValues(CString keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return -1;
	else
		return keys[keynum].names.GetSize();
}

//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double

/*************函数定义****************
函数名   : CIniFile::GetValue
函数描述 : 得到字符串键值
返回值   : CString 
参数1    : CString keyname
参数2    : CString valuename
最后修改日期：2003-4-28 17:01:34
**************************************/
CString CIniFile::GetValue(CString keyname, CString valuename)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);
//CString strTemp;
//strTemp.Format(_T("keynum:%d"),keynum);
//AfxMessageBox(strTemp);
//strTemp.Format(_T("valuenum:%d"),valuenum);
//AfxMessageBox(strTemp);

	if (keynum == -1)
	{
		error = _T("Unable to locate specified key.");
		return _T("");
	}

	if (valuenum == -1)
	{
		error = _T("Unable to locate specified value.");
		return _T("");
	}
	//AfxMessageBox(keys[keynum].values[valuenum]);
	return keys[keynum].values[valuenum];
}
//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double
double CIniFile::GetValue(CString keyname, CString valuename,double)
{
	return MyTransFunc::StringToDouble(GetValue(keyname, valuename));
}

//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CIniFile::SetValue(CString keyname, CString valuename, CString value, bool create)
{
	int keynum = FindKey(keyname), valuenum = 0;
	//find key
	if (keynum == -1) //if key doesn't exist
	{
		if (!create) //and user does not want to create it,
			return 0; //stop entering this key
		names.SetSize(names.GetSize()+1);
		keys.SetSize(keys.GetSize()+1);
		keynum = names.GetSize()-1;
		names[keynum] = keyname;
	}

	//find value
	valuenum = FindValue(keynum,valuename);
	if (valuenum == -1)
	{
		if (!create)
			return 0;
		keys[keynum].names.SetSize(keys[keynum].names.GetSize()+1);
		keys[keynum].values.SetSize(keys[keynum].names.GetSize()+1);
		valuenum = keys[keynum].names.GetSize()-1;
		keys[keynum].names[valuenum] = valuename;
	}
	keys[keynum].values[valuenum] = value;
	return 1;
}

//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CIniFile::SetValue(CString keyname, CString valuename, double value, bool create)
{
	CString temp;
	temp.Format(_T("%.2f"),value);
	return SetValue(keyname, valuename, temp, create);
}

//deletes specified value
//returns true if value existed and deleted, false otherwise
bool CIniFile::DeleteValue(CString keyname, CString valuename)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);
	if (keynum == -1 || valuenum == -1)
		return 0;

	keys[keynum].names.RemoveAt(valuenum);
	keys[keynum].values.RemoveAt(valuenum);
	return 1;
}

//deletes specified key and all values contained within
//returns true if key existed and deleted, false otherwise

/*************函数定义****************
函数名   : CIniFile::DeleteKey
函数描述 : 删除键值
返回值   : bool 
参数1    : CString keyname
最后修改日期：2003-4-28 16:58:17
**************************************/
bool CIniFile::DeleteKey(CString keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return 0;
	keys.RemoveAt(keynum);
	names.RemoveAt(keynum);
	return 1;
}

/////////////////////////////////////////////////////////////////////
// Private Functions
/////////////////////////////////////////////////////////////////////

//returns index of specified key, or -1 if not found
int CIniFile::FindKey(CString keyname)
{
	int keynum = 0;
	while ( keynum < keys.GetSize() && names[keynum] != keyname)
		keynum++;
	if (keynum == keys.GetSize())
		return -1;
	return keynum;
}

//returns index of specified value, in the specified key, or -1 if not found
int CIniFile::FindValue(int keynum, CString valuename)
{
	if (keynum == -1)
		return -1;
	int valuenum = 0;
	while (valuenum < keys[keynum].names.GetSize() && keys[keynum].names[valuenum] != valuename)
		valuenum++;
	if (valuenum == keys[keynum].names.GetSize())
		return -1;
	return valuenum;
}

//overloaded from original getline to take CString
istream & CIniFile:: getline(istream & is, CString & str)
{
    char buf[2048];
    is.getline(buf,2048);
    str = buf;
    return is;
}


/*************函数定义****************
函数名   : CIniFile::GetItemFromString
函数描述 : 从字符串中分离子串
返回值   : CString 
参数1    : CString string
参数2    : CString Split
参数3    : int index 从0开始
最后修改日期：2003-5-15 11:04:43
**************************************/
CString CIniFile::SubString(CString string, CString Split, int index)
{
	int Num=string.Find(Split);
	if(Num==-1) return _T("");
	if(index==0)
		return string.Left(Num);
	int i=1;
	CString retStr=_T("");
	int nCount = 0;
#ifdef UNICODE
	nCount = 0;
#else
	nCount = 1;
#endif
	while(i<index)
	{
		Num=string.Find(Split,Num+1+nCount);
		if(Num==-1) return _T("");
		i++;
	}
	int Num2=string.Find(Split,Num+1+nCount);
	if(Num2==-1){
		retStr=string.Mid(Num+1+nCount);
	}
	else
	{
		retStr=string.Mid(Num+1+nCount,Num2-Num-1-nCount);
		
	}
	return retStr;
}
