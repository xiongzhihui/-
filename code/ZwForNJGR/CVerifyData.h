#pragma once
#include "CCommonCell.h"
//////////////////////////////////////////////////////////////////////////
//校验数据
typedef map<CString, vector<vector<CString> > > MIMap;
typedef pair<CString, vector<vector<CString> > > MIPair;
typedef vector<vector<CString> > VIVec;
class CExcelDifferInfo;

class CVerifyData
{
public:
	CVerifyData(void);
	~CVerifyData(void);

	void setData(vector<CCommonCell> data);

	bool doIt();
private:
	bool printToFile();
	bool printToTxt();
	bool writeExcelFile();
	void addCellToMap(CCommonCell cell);

	CString getFromatFile(CString strPath);
	void cmpTwoExcelInfo(map<CString, vector<vector<CString> > > curMapInfo, map<CString, vector<vector<CString> > > tmpExcelInfo);
	bool CmpSheetContent(VIVec tmpPreVec, VIVec tmpCurVec, CString strSheetName);
	bool WriteCsvFile();
	bool WriteStartFile(map<CString, vector<vector<CString> > > tmpExcelInfo);
private:
	vector<CCommonCell> m_CellInfo;
	map<CString, CExcelDifferInfo> m_DifferInfo;
	bool m_bIsError;
};

class CExcelDifferInfo
{
public:
	CExcelDifferInfo(void);
	~CExcelDifferInfo();

	void setSheetName(CString strSheetName);
	void setRange(CString strRange);
	void setOldValue(CString strOldValue);
	void setNewValue(CString strNewValue);
	void setDescription(CString strDescription);

	CString getSheetName() const;
	CString getRange() const;
	CString getOldValue() const;
	CString getNewValue() const;
	CString getDescription() const;

	CExcelDifferInfo& operator=(const CExcelDifferInfo& node);
	CString getOutPutString() const;
	CString getCsvOutString() const;
private:
	CString m_strSheetName;
	CString m_strRange;
	CString m_strOldValue;
	CString m_strNewValue;
	CString m_strDescription;
};