// Global.h: interface for the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__10D86CB2_8BCE_49FE_B56F_E3657D5A006E__INCLUDED_)
#define AFX_GLOBAL_H__10D86CB2_8BCE_49FE_B56F_E3657D5A006E__INCLUDED_

#include <afxtempl.h>
#pragma once

//#include "StdAfx.h"

#include "map"

#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class SERVERDLL_API CGlobal  
{
	CGlobal();
	
public:
	//	BOOL WriteDictionary();
	BOOL SetIniValue(LPCTSTR key,LPCTSTR Name,LPCTSTR value);
	BOOL SetIniValue(LPCTSTR key, LPCTSTR Name, double value);
	int GetIniValueNum(CString key);
	CString GetIniPath();
	CString SubString(CString string,CString Split,int index);
	void WriteIniFile();
	void SetIni(CString path);//设置INI路径并读取内容
	virtual ~CGlobal();
	static CGlobal& Get();
	
	//得到Cyz数据
	LPCTSTR GetIniValue(LPCTSTR Key,LPCTSTR name);
	double GetIniValue(LPCTSTR Key,LPCTSTR name,double);
	/////////////////////////////////////////////////////////////////////////////
	//设置Cyz数据

	int Myatoi(CString inputStr);
	BOOL Myatoi(CString inputStr,int& iresult);	

	BOOL IsFileExist(CString strFileName);	


};

#endif
#define gGlobal CGlobal::Get()



