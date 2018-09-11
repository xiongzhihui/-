#pragma once
#include "SheetStoreData.h"
#include "CCommonCell.h"
#include "CNameCell.h"
#include "GLData.h"
#include <algorithm>
using namespace std;
//数字光数据读取
class CDigitalOpticalSheet
{
public:
	CDigitalOpticalSheet(void);
	~CDigitalOpticalSheet(void);
	void setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow, int nType = 0);
	
	CString modSheetName(CString strSheetName);
	bool doIt();

	void readCommon();
	CSheetStoreData getData();

private:
	int GetGLType(int nSize);
	vector<CCommonCell> readDuanziData(int nItem, int param2);
	CString getRealSheetName();
private:
	CSheetStoreData m_singleData;

	CString m_strSheetName;//sheetName;
	CString m_strModSheetName;//插头sheetname
	CString m_strPlug;//插座
	CString m_strModPlug;//插头
	CString m_strName;
	vector<vector<CString> > m_Data;//原始插座数据
	int m_nType;//0表示数字光，1表示模拟光
	int m_nColNum;
	int m_nRowNum;
	vector<CCommonCell> m_tmpVec;
	map<int, CNameCell > m_MapName;
	CGLData m_glData;
	CNameCell m_nameCell;

};

