#pragma once

#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class SERVERDLL_API MyOperateFile
{
public:
	MyOperateFile(void);
	~MyOperateFile(void);
	//获取子目录
	static CString GetSubPath(LPCTSTR strPath);
	//发现子目录
	static bool FindSubDir(LPCTSTR strPath);
	//获取文件名
	static CString GetFileNameByPath(LPCTSTR strPath);
	//
	static void GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, 
		CString strTypeName, bool bIsFullPath = false);

};
