#pragma once
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class SERVERDLL_API COperateReg
{
public:
	COperateReg(void);
	~COperateReg(void);

	/////////////////////////////////////////////////////////////////////////////
	//功 能：读取注册表读字符串的值
	//传 入：hKey-注册表的HKEY_CLASSES_ROOT、HKEY_CURRENT_CONFIG、HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE、HKEY_USERS
	//lpszSec-路径 lpszDef-默认值 lpszKey-主键
	//传 出：lpszValue
	//返 回：-1打开路径失败 -2打开主键失败 成功返回字符串的长度
	static int GetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszDef, LPTSTR lpszValue);
	/////////////////////////////////////////////////////////////////////////////
	//读注册表,读DWORD的值
	static DWORD GetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, DWORD DefValue);
	/////////////////////////////////////////////////////////////////////////////
	//功 能：写入注册表,写字符串
	//传 入：hKey-注册表的HKEY_CLASSES_ROOT、HKEY_CURRENT_CONFIG、HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE、HKEY_USERS
	//lpszSec-路径 lpszKey-主键 lpszValue-值
	//返 回：FALSE-失败 TRUE-成功
	static BOOL SetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszValue);
	/////////////////////////////////////////////////////////////////////////////
	//功 能：写入注册表,写DWORD
	static BOOL SetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey,DWORD Value);

	static CString GetStringFromReg(CString strRootKey, LPCTSTR strKey);
	static int GetDwordFromReg(CString strRootKey, LPCTSTR strKey);
};

