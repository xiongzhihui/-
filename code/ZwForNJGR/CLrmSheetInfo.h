#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include "SingleSheetData.h"
#include "SheetStoreData.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//读取Lrm数据
class CLrmSheetInfo
{
public:
	CLrmSheetInfo(void);
	~CLrmSheetInfo(void);
	void setData(vector<vector<CString> > vData,map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow);
	
	bool prompt();
	CString getSheetMod() const;
	bool doIt();

	CSheetStoreData getData() const;
	CSheetStoreData getModData();
	bool IsMod();
private:
	CString getType();
	void readSpecial();
	CString GetNameByIndex(int j, int nNum);

	CString getRealSheetName();
	//读取LRM标准数据
	vector<CCommonCell> readLrmData(int nItem, int nNum);

	CCommonCell doTableItem(CString strItem);
	CCommonCell doRfItem(CString strItem);
	CCommonCell doItem(CString strItem);
	vector<int> getColInt(int nStart, int nNum);
	CString modSheetName(CString strSheetName) const;
private:
	CString m_strType;//sheet类型，普通，LRM，VPX
	CString m_strSheetName;//sheetName;
	vector<vector<CString> > m_Data;
	//vector<int> m_NameIndex;
	map<int, CNameCell > m_MapName;

	CSheetStoreData m_singleData;
	CSheetStoreData m_ModSingleData;

	int m_nItem;
	int m_nColNum;
	int m_nRowNum;
	int m_nType;//0为普通，1为特殊，2为VPX
	bool m_bIsMod;//是否模块
	bool m_bIsContinued;
	CString m_strModSheetName;
	CString m_strName;
	bool m_bIsSplit;
	bool m_bIsLrmv;
	CNameCell m_NameCell;
};
