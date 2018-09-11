#pragma once
#include "CNameCell.h"
#include <vector>
#include <map>
#include "CCommonCell.h"
#include "SheetStoreData.h"
#include "CGlobalQuery.h"
using namespace std;

//常量
const int ColQHCol = 0;//腔号所在列数

class CReadInfo
{
public:
	CReadInfo(void);
	~CReadInfo(void);

	void setData(vector<vector<CString> > vData,map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow);
	virtual bool doIt();
protected:
	CString getRealSheetName();
	int GetGLType(int nSize);
	CString getSheetMod() const;
	CString getType();
	CSheetStoreData getSheetData() const;
	vector<int> getColInt(int nStart, int nNum);
	//获取namecell的状态

	bool AddRange(int nRow, int nEndRow);
	//获取commoncell的状态
	void getCellStatus(CString strItem);
	CCommonCell addCell(CString strItem, CString strDuanzi, CString strTexing, int nRow, int nCol);
	//add单个comcomcell
	void addToDataCell(CCommonCell comCell);
public:
	CNameCell m_NameCell;
	CString m_strSheetName;
	CString m_strName;
	vector<vector<CString> > m_Data;
	map<int, CNameCell > m_MapName;
	CSheetStoreData m_sheetGL;
	//数据存储
	COMVEC RFComVec;
	COMVEC RFModComVec;
	COMVEC comVec;
	COMVEC comModVec;

	int m_nItem;
	int m_nColNum;//列数
	int m_nRowNum;//行数
	bool m_bIsMod;
	bool m_bIsRF;//是否射频
	bool m_bIsGl;//是否光纤
	bool m_bIsAddData;//是否加T,如果加T就不在母版上显示数据
};

