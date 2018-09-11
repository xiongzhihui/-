#pragma once
#include "CCommonCell.h"
#include "WriteLog.h"
#include "SheetStoreData.h"
#include "CFrameInfo.h"
#include "SingleSheetData.h"
#include "CIgnoreErrorCell.h"
#include "CSingalNo.h"
#include "BaseUtils.h"


//全局查询函数
class CGlobalQuery
{
public:
	CGlobalQuery(void);
	~CGlobalQuery(void);

	void clear();
	//清理掉vector中数据
	void clearVec();
	//清空Error数据
	void clearErrorLog();

	void doCommonCell(CCommonCell& cell);
	void doMTCommonCell(CCommonCell& cell);
	//void doSzgMTCommonCell(CCommonCell cell);
	//void doMngMTCommonCell(CCommonCell cell);
	void doRFCommonCell(CCommonCell& cell);

	bool doIt();
	//通过类型找到各自对应的去向
	CString getQuxiang(CCommonCell& cell);

	//打印信息到输出文件夹中
	void addCellInfo(CCommonCell cell);
	bool writeCellInfoToFile();

	//////////////////////////////////////////////////////////////////////////
	//输出接线表
	//bool writeJxb();
	//输出接线连接关系表
	bool writeJxljgxb();
	//增加错误信息
	void addErrorString(CString strError);
	//写error log日志
	bool writeErrorLog();

	//获取LRMV数据
	void addLrmvSheetData(CSheetStoreData data);
	vector<CSheetStoreData> getLRMVSheetData();
	//获取lrmv mod数据
	void addLrmvModSheetData(CSheetStoreData data);
	vector<CSheetStoreData> getLRMVModSheetData();
	void addDigitalSheetData(CSheetStoreData data);
	//获取数字光数据
	vector<CSheetStoreData> getDigitalSheetData();
	//添加综合面板数据
	void addCommonSheetData(CString strSheet, CSingleSheetData data);
	map<CString, CSingleSheetData> getCommonData();
	//添加modname
	void addModNameItem(CString str);
	//添加name
	void addSheetNameItem(CString str);
	//添加块数据
	void addBlockName(CString str);

	//绘制
	bool draw();
	//普通图形交换特性
	bool exchangeTexingCommonData();

	//是否修改图框
	bool getEditFrameFlag();
	//将T属性数据加入vector中
	void addTDataToVec(CCommonCell comCell);
	//清理差分信号
	void clearDifferentialSignal();
	void adddifferSignal(CCommonCell comCell, bool bIsDefaultRF = false);
	void addPlusSignal(CString strItem, CCommonCell comCell);
	void addMinusSignal(CString strItem, CCommonCell comCell);
	void addPSignal(CString strItem, CCommonCell comCell);
	void addNSignal(CString strItem, CCommonCell comCell);
	void doDifferSignal();
	void TraversingDifferData(map<CString, CCommonCell> data1, map<CString, CCommonCell> data2); 
	void setFatalErrorFlag();

	void addExcelInfo(CString strSheetName, vector<vector<CString> > tmpVec);
	map<CString, vector<vector<CString> > > getExcelInfo() const;	
	
	map<CString, vector<pair<CCommonCell, CCommonCell> > > getMTOutPutData();
	CSingalNo getDpInfo() const;
	CSingalNo getRfInfo() const;
	//将MT数据存入map中map<cstring, vector<ccomcell> >
	map<CString, vector<pair<CCommonCell, CCommonCell> > > doMTInfoToMap();
	COMVEC getTInfo() const;
	vector<pair<CCommonCell, CCommonCell> > getRMTInfo() const;	
	//根据comcell得到错误信息
	CString getErrorString(CCommonCell comCell);
	void setErrorType(CString strErrorType);
	//添加空余数据到数据中
	void addFixNumAreaToStore(CString strType, int nStart, int nEnd);
	//设置预览的标记
	void setPreviewFlag(bool bIsFlag);
	bool getPreviewFlag();
	COMVEC getCellInfo();

private:
	bool doSData();

	bool doCData();
	bool doRData();
	bool doRDpData();
	
	//MT -R
	bool doRMTData();

	bool doRFData();
	bool doDPData();
	bool doMTData();
	bool doMData();

	bool doRMData();

	CString getSimpleResult(MDMap dData, CString strTexing, CString strTempQuxiang);
	CString getSpecialResult(MDMap dData, CCommonCell cell, CString strTexing, CString strTempQuxiang, int& nCount);

	//处理是R层级还是R+S层级，还是R+M层级
	bool doRLevel(CString& strTexing, CCommonCell& cell);
	//处理MT R层级
	bool doMTRLevel(CString& strTexing, CCommonCell& cell);
	//输出接线表
	//获取DP输出数据
	map<int, pair<CCommonCell, CCommonCell> > getDpOutPutData();
	//获取RF输出数据
	map<int, pair<CCommonCell, CCommonCell> > getRFOutPutData();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//绘制
	//绘制LRMV mod图形
	bool drawLrmvModData();
	//绘制LRMV图形
	bool drawLrmvData();
	void drawDigitalData();
	//绘制普通图形
	bool drawCommonData();

	//绘制图块
	bool drawTu();
	//将R+s数据存入R中
	void AddRSToRData();

	//调用ignoreerror对话框
	bool CallIgnoreDlg(CString strPrompt);
	//获取R层级Cell的去向
	CString getRLevelQuxiang(CCommonCell cell);
	//获取R层级MT Cell的去向
	CString getRMTLevelQuxiang(CCommonCell cell);
	//获取DP count数据
	CString getDpSpecialResult(CCommonCell& cell);
	
	//************************************
	// Method:    getDpCmpString
	// FullName:  CGlobalQuery::getDpCmpString
	// Access:    private 
	// Returns:   CString
	// Qualifier:传入cell，返回用于查找数据的字符串，返回是否带层级的标识
	// Parameter: CCommonCell cell
	// Parameter: bool & bIsLevel
	//************************************
	CString getDpCmpString(CCommonCell cell, CString& strPre, bool& bIsLevel);
	//************************************
	// Method:    FindDataFromRDpData
	// FullName:  CGlobalQuery::FindDataFromRDpData
	// Access:    private 
	// Returns:   CString
	// Qualifier:从DP数据找到去向数据，并设置相应的编号
	// Parameter: CCommonCell & cell
	// Parameter: CString strTmp
	//************************************
	CString FindDataFromRDpData(CCommonCell& cell, CString strTmp);
	//************************************
	// Method:    GetMDpError
	// FullName:  CGlobalQuery::GetMDpError
	// Access:    private 
	// Returns:   bool
	// Qualifier:处理M S DP的数据一一匹配验证
	// Parameter: vector<CCommonCell> mComVec
	// Parameter: vector<CCommonCell> sComVec
	//************************************
	bool GetMDpError(vector<CCommonCell> mComVec, vector<CCommonCell> sComVec);
	//************************************
	// Method:    doDpNumber
	// FullName:  CGlobalQuery::doDpNumber
	// Access:    private 
	// Returns:   void
	// Qualifier:获取手动设置的dp number并设置到数据中
	// Parameter: CCommonCell & cell
	//************************************
	void doDpNumber(CCommonCell& cell);
	CString getQuxiangByDpNum(CCommonCell& cell);
	CString getQuxiangByRfNum(CCommonCell& cell);
	void EraseFixedNum();
	void doRFNumber(CCommonCell& cell);

	//int getDpNumbyCount(int nCount);
	
private:
	MDMap m_RFData;
	MDMap m_DPData;
	map<CString, vector<pair<CCommonCell, CCommonCell> > > m_RDPRealData;//M数据在前面，S在后面
	MRMAP m_RRDpData;

	MDMap m_SData;
	MDMap m_MData;
	MDMap m_MTData;
	MDMap m_CData;//普通数据

	//MDMap m_SZGMTData;//数字光数据
	//MDMap m_MNGMTData;//模拟光数据
	//////////////////////////////////////////////////////////////////////////
	MRMAP m_RData;
	MRMAP m_RSData;
	MRMAP m_RMData;
	MRMAP m_RMTData;

	map<pair<int, CString>, CCommonCell> m_RRMTData;
	//用于光纤数据的接线表生成
	vector<pair<CCommonCell, CCommonCell> > m_RRMTJxData;
	//////////////////////////////////////////////////////////////////////////

	map<CString, CString> m_MMData;
	map<CString, CString> m_SSData;
	map<pair<int, CString>, CCommonCell> m_RRData;

	CWriteLog m_log;

	vector<CString> m_ErrorVec;//记录错误信息
	vector<CIgnoreErrorCell> m_IgnoreVec;//记录忽略错误信息
	
	//记录LRMV数据
	vector<CSheetStoreData> m_LrmvSheetData;
	//记录LRMV mod数据
	vector<CSheetStoreData> m_LrmvModSheetData;
	//记录数字光数据
	vector<CSheetStoreData> m_DigitalSheetData;

	//记录块名
	vector<CString> m_blkNameVec;
	vector<CString> m_nameVec;
	vector<CString> m_modNameVec;//全是模块的数据
	map<CString, CSingleSheetData> m_datainfo;
	vector<CCommonCell> m_ComvecError;
	//记录T属性数据
	vector<CCommonCell> m_TComVec;
	//记录差分+信号
	map<CString, CCommonCell> m_PlusSignal;
	map<CString, CCommonCell> m_MinusSignal;
	map<CString, CCommonCell> m_PSignal;
	map<CString, CCommonCell> m_NSignal;
	bool m_bSignalFlag;//记录差分信号的错误状态
	bool m_bFatalError;//致命错误
	int m_RDplevel;//记录DP中R层级
	
	CString m_strErrorType;//错误类型

	map<CString, COMVEC> m_dpNumInfo;//dp手动设置的数值
	map<CString, COMVEC> m_rfNumInfo;//RF手动设置的数值
	map<int, int> m_dpNumVec;
	map<int, int> m_rfNumVec;
	COMVEC m_InfoVec;//所有Commoncell信息
	map<CString, vector<vector<CString> > > m_ExcelInfo;

	CSingalNo m_DpInfo;
	CSingalNo m_RfInfo;
	bool m_bIsPreview;//是否预览
};

