#pragma once
#include "CCommonCell.h"
#include <list>
using namespace  std;

//处理线号相关，包括DP和RF
class CSingalNo
{
public:
	CSingalNo(void);
	~CSingalNo(void);
	void clear();
	//设置类型
	void setType(CString strType);
	//存数据
	void AddData(CCommonCell& cell);
	//添加空余数据到数据中
	void addFixNumAreaToStore(int nStart, int nEnd);
	//提取去向数据
	CString getQuxiang(CCommonCell& cell);
	//删除多余的数据
	void EraseFixedInfo();	
	//生成接线表和接线关系表需要使用的数据
	MPMAP getOutPutData() const;

	//校验dp数据
	bool doDPData();

private:
	//在有固定编号的数据集里查找固定编号对应的去向
	CString getQuxiangByNum(CCommonCell& cell);
	//设置固定编号的数据
	void doNumber(CCommonCell& cell);
	bool doSpecailData(COMVEC tmpVec, int nCount);
	bool doRDpData();
	CString doMSNumAndQuxiang(COMVEC tmpVec, int nCount, CCommonCell& cell);
	CString doRMSNumAndQuxiang(COMVEC tmpVec, int nCount, CCommonCell& cell);
	map<int, COMVEC> doRLevelData(COMVEC tmpVec);

	bool FindMultiData(pair<MDMap::iterator, bool> ret, CCommonCell cell);
	map<int,COMVEC> doFixDataToMap(COMVEC tmpVec);

	//处理内部的数据
	CString doTwoData(COMVEC tmpVec, CCommonCell cell);
	//处理内部数据
	CString getFixNumFromFixMap(map<int, COMVEC> mapInf, int nCount, CCommonCell cell);
	void addDataToSpecailMData(int nCount, COMPAIR tmpPair);
	void doRMSAndSetCount(CCommonCell& cell);
private:
	list<CString> m_ListInfo;
	map<int, int> m_NumVec;
	MDMap m_NumInfo;
	MDMap m_Data;
	MDMap m_RData;
	MPMAP m_tmpRMSInfo;//记录临时的R的MS数据
	MPMAP m_dpoutPutMap;

	MVMAP m_RDPRealData;//M数据在前面，S在后面
	MRMAP m_RRDpData;

	CString m_strType;//类型，RF或DP
	CString m_strTypeEqual;//类型等号
};

