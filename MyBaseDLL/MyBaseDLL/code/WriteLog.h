#pragma once

#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
//写log日志
class SERVERDLL_API CWriteLog
{
public:
	CWriteLog(void);
	CWriteLog(LPCTSTR strFile);

	virtual ~CWriteLog(void);

	//设置文件路径
	void setPath(LPCTSTR strFile);
	//写数据
	bool writeLog(LPCTSTR strLog, bool bIsAddTime = false);
	//写数据
	bool writeCsvLog(LPCTSTR strLog, bool bIsAddTime = false);
	//
private:
	CString m_strFilePath;
};
