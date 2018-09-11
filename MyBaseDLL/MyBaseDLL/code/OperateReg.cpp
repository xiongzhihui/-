#include "StdAfx.h"
#include "OperateReg.h"


COperateReg::COperateReg(void)
{
}


COperateReg::~COperateReg(void)
{
}

/////////////////////////////////////////////////////////////////////////////
//功 能：读取注册表读字符串的值
//传 入：hKey-注册表的HKEY_CLASSES_ROOT、HKEY_CURRENT_CONFIG、HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE、HKEY_USERS
//lpszSec-路径 lpszDef-默认值 lpszKey-主键
//传 出：lpszValue
//返 回：-1打开路径失败 -2打开主键失败 成功返回字符串的长度
int COperateReg::GetRegister(HKEY hKey, 
	LPCTSTR lpszSec, 
	LPCTSTR lpszKey, 
	LPCTSTR lpszDef, 
	LPTSTR lpszValue)
{
	HKEY hInfor;
	DWORD dwType;
	DWORD dwLength;

	if(RegOpenKeyEx(hKey,
		lpszSec,
		0, 
		KEY_READ|KEY_EXECUTE,
		&hInfor) != ERROR_SUCCESS)
	{
		_tcscpy(lpszValue,lpszDef);
		return -1;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		NULL,
		NULL,
		&dwLength) != ERROR_SUCCESS)
	{
		_tcscpy(lpszValue,lpszDef);
		RegCloseKey(hInfor);
		return -2;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		&dwType,
		(LPBYTE)lpszValue,
		&dwLength) != ERROR_SUCCESS)
	{
		_tcscpy(lpszValue,lpszDef);
		RegCloseKey(hInfor);
		return -2;
	}
	RegCloseKey(hInfor);
	return (_tcslen(lpszValue)+1);
}
/////////////////////////////////////////////////////////////////////////////
//读注册表,读DWORD的值
DWORD COperateReg::GetRegister(HKEY hKey,LPCTSTR lpszSec,LPCTSTR lpszKey,DWORD DefValue)
{

	HKEY hInfor;
	DWORD dwType;
	DWORD dwLength;
	DWORD Value = DefValue;

	if(RegOpenKeyEx(hKey,
		lpszSec,
		0, 
		KEY_READ|KEY_EXECUTE,
		&hInfor) != ERROR_SUCCESS)
		return Value;

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		NULL,
		NULL,
		&dwLength) != ERROR_SUCCESS)
	{
		RegCloseKey(hInfor);
		return Value;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		&dwType,
		(LPBYTE)&Value,
		&dwLength) != ERROR_SUCCESS)
	{
		RegCloseKey(hInfor);
		return Value;
	}
	RegCloseKey(hInfor);
	return Value;
}

/////////////////////////////////////////////////////////////////////////////
//功 能：写入注册表,写字符串
//传 入：hKey-注册表的HKEY_CLASSES_ROOT、HKEY_CURRENT_CONFIG、HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE、HKEY_USERS
//lpszSec-路径 lpszKey-主键 lpszValue-值
//返 回：FALSE-失败 TRUE-成功
BOOL COperateReg::SetRegister(HKEY hKey,LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	HKEY hInfor;
	DWORD dwStatus;	
	int nLen = 0;

#ifdef UNICODE
	nLen = 2*(_tcslen(lpszValue) + 1);
#else
	nLen = _tcslen(lpszValue) + 1;
#endif
	if(RegCreateKeyEx(hKey,
		lpszSec, 0, NULL, 
		REG_OPTION_NON_VOLATILE, 
		KEY_WRITE, 
		NULL,
		&hInfor,&dwStatus) == ERROR_SUCCESS) 
	{		
		dwStatus = RegSetValueEx(hInfor,
			lpszKey,
			0,
			REG_SZ,
			(LPBYTE)lpszValue,
			nLen);
		RegCloseKey(hInfor);
		return dwStatus==ERROR_SUCCESS;
	}
	else
		return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//功 能：写入注册表,写DWORD
BOOL COperateReg::SetRegister(HKEY hKey,LPCTSTR lpszSec,LPCTSTR lpszKey,DWORD Value)
{
	HKEY hInfor;
	DWORD dwStatus;			
	if(RegCreateKeyEx(hKey,
		lpszSec, 0, 
		NULL, 
		REG_OPTION_NON_VOLATILE, 
		KEY_WRITE, NULL,
		&hInfor,
		&dwStatus)==ERROR_SUCCESS) 
	{
		dwStatus = RegSetValueEx(hInfor,
			lpszKey,0,
			REG_DWORD,
			(LPBYTE)&Value,
			sizeof(DWORD));
		RegCloseKey(hInfor);
		return dwStatus==ERROR_SUCCESS;
	}
	else
		return FALSE;
}

CString COperateReg::GetStringFromReg(CString strRootKey, LPCTSTR strKey)
{
	CString strResult;
	TCHAR ResStr[MAX_PATH];
	COperateReg Reg;
	if (Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, _T(""), ResStr) > 0)
	{
		strResult = ResStr;
	}
	return strResult;
}

int COperateReg::GetDwordFromReg(CString strRootKey, LPCTSTR strKey)
{
	int nRet = 0;
	CString strResult;
	DWORD dRes = 0;
	COperateReg Reg;
	nRet = Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, dRes);
	return nRet;
}
