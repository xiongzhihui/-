#pragma once
#include "CReadInfo.h"
#include "SingleSheetData.h"

class CReadCommonInfo:public CReadInfo
{
public:
	CReadCommonInfo(void);
	~CReadCommonInfo(void);
	//初始数据设置
	bool doIt();
	//具体数据读取
	bool readDuanzi(int nItem, int nNum);

	CSingleSheetData getdata() const;
private:
	CSingleSheetData m_sheetData;
	map<CNameCell, vector<CCommonCell> > m_MapInfo;
};

