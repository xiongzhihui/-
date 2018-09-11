#pragma once
#include "CReadInfo.h"
#include "CGlobalQuery.h"

class CReadLrmInfo: public CReadInfo
{
public:
	CReadLrmInfo(void);
	~CReadLrmInfo(void);

	bool doIt();
	//具体数据读取
	bool readDuanzi(int nItem, int nNum);
	bool doCommaItem(CString strItem, int nRow, int nCol);
	void doColItem(CString strItem, int nRow, int nCol);
private:
	bool m_bIsMod;//是否模块
	CSheetStoreData m_ModSingleData;

	//记录RF和普通数据
	COMVEC RFComVec;
	COMVEC RFModComVec;
	COMVEC comVec;
	COMVEC comModVec;
	vector<CString> tmpVec;
	CString strPreName;
	CString strMidName;

};