#pragma once
#include "CNameCell.h"
#include <map>
#include <vector>
#include "CCommonCell.h"
#include "CLrmvTitleInfo.h"
#include "SheetStoreData.h"
#include "WriteLog.h"
using namespace std;
//读取特殊LRMV数据

class CLRMVSheet
{
public:
	CLRMVSheet(void);
	~CLRMVSheet(void);

	void setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow);

	bool doIt();

	bool prompt();
	void readVpxData();

	bool writeExcel();
	map<CNameCell, vector<CCommonCell> > getMapInfo() const;
	map<CNameCell, vector<CCommonCell> > getMapModInfo() const;
	vector<vector<CString> > getModExcelData() const;
	CString getModSheetName();
	CString getSheetName() const;

	CSheetStoreData getData();
	CSheetStoreData getModData();
private:
	CString modSheetName(CString strSheetName);
	void	getPlug();
	vector<CCommonCell> readVpx(int nItem, int m_nRowNum, CNameCell cell);

	CString GetNameByIndex(int j, int nColNum);
	vector<CString> doEvenRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doOddRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doSingleRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doPowerRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doEmptyRelation(vector<CString> tmpCurVec, CNameCell cell);
	CCommonCell readEvenData(CString strDuanzi, CString strTexing, CString strQuxiang = _T(""));
	//处理数据
	void doLrmvTitle(vector<CLrmvTitleInfo> vec);
	//处理Item
	CCommonCell doTableItem(CString strItem);
	CCommonCell doRfItem(CString strItem);
	CCommonCell doGLItem(CString strItem);

	int GetGLType(int nSize);
	
	CString getRealSheetName();
	//处理模块数据
	void doModData(int nStart, int nEnd, CNameCell nameCell);
	//
	bool doMod();
private:
	CString m_strSheetName;//sheetName;
	CString m_strModSheetName;//插头sheetname
	CString m_strPlug;//插座
	CString m_strModPlug;//插头
	CString m_strName;
	vector<vector<CString> > m_Data;//原始插座数据
	vector<vector<CString> > m_modData;//插头数据
	map<int, CNameCell > m_MapName;
	map<CNameCell, vector<CCommonCell> > m_MapInfo;
	map<CNameCell, vector<CCommonCell> > m_MapModInfo;
	vector<CCommonCell> m_ComVec;
	map<CNameCell, vector<CCommonCell> > m_ComModVec;
	int m_nType;//lrmv数据类型
	int m_nColNum;
	int m_nRowNum;
	vector<CCommonCell> m_tmpVec;

	
	CSheetStoreData m_singleData;

	CSheetStoreData m_ModSingleData;
	CNameCell m_nameCell;
	CWriteLog m_log;
};
