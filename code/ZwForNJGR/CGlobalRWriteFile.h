#pragma once
#include "CFrameInfo.h"
//全局读写文件

class CGlobalRWriteFile
{
public:
	CGlobalRWriteFile(void);
	~CGlobalRWriteFile(void);

	void clear();

	//读取图框的配置文件
	bool readFrameIniFile();
	//编辑图框中的配置文件
	bool EditFrameIniFile();
	//增加图框中的配置文件
	bool addFrameIniFile(CString strName, CFrameInfo frameInfo);
	//删除图框中的配置文件
	bool delFrameIniByName(CString strName);
	//从配置文件中得到数据，如果没有就返回默认值
	CFrameInfo getFrameInfoByName(CString strLrmName);

	map<int, CFrameInfo> getAllFrameInfo();
	void saveAllFrameInfo();

private:
	//配置文件
	map<CString, CFrameInfo> m_FrameIni;
	//第一个参数为sheet名称，第二个参数为具体的图框参数
	map<int, CFrameInfo> m_AllFrame;
};

