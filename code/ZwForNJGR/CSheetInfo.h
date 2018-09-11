#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include "SingleSheetData.h"
#include "SheetStoreData.h"

class COutstring
{
public:
	COutstring(void);
	~COutstring(void);
	void setStringOut(CString strOut);
	void setBianma(int nBianma);
	void setQuxiang(CString strQuxiang);
	//void setCount(int nCount);
	void setTexing(CString strTexing);
	CString getStringOut() const;
	int getBianma() const;
	CString getQuxiang() const;
	//int getCout() const;
	CString getTexing() const;

	COutstring& operator=(const COutstring& node);

	CString getOutPutstring() const;
	
private:
	CString m_strOut;
	int m_nBianma;
	//int m_nCout;
	CString m_strTexing;
	CString m_strQuxiang;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CSheetInfo
{
public:
	CSheetInfo(void);
	~CSheetInfo(void);
	void setData(vector<vector<CString> > vData,map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow);
	
	bool prompt();
	bool doIt();

	CSingleSheetData getdata() const;
private:
	CString getType();
	void readCommon();
	void readSpecial();
	//void readVpxData();
	CString GetNameByIndex(int j, int nNum);

	CString getRealSheetName();
	//读取端子标准数据
	vector<CCommonCell> readDuanziData(int nItem, int nNum);

	CString modSheetName(CString strSheetName) const;
	int GetGLType(int nSize);
	CString getSheetMod() const;
private:
	CString m_strType;//sheet类型，普通，LRM，VPX
	CString m_strSheetName;//sheetName;
	vector<vector<CString> > m_Data;
	map<int, CNameCell > m_MapName;

	map<CNameCell, vector<CCommonCell> > m_MapInfo;
	CSingleSheetData m_sheetData;
	CSheetStoreData m_sheetGL;//gl光纤数据

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
