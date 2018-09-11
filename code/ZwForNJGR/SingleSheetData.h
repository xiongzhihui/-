#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include "TableData.h"
#include "RFData.h"
#include "CommonData.h"
#include "GLData.h"
#include <vector>
#include "SheetStoreData.h"
using namespace std;

class CSingleSheetData
{
public:
	CSingleSheetData(void);
	~CSingleSheetData(void);

	void setMapInfo(map<CNameCell, vector<CCommonCell> > info);
	void setSheetStoreData(CSheetStoreData sheetData);
	map<CNameCell, vector<CCommonCell> > getMapInfo() const;
	CSheetStoreData getSheetStoreData() const;
	CSingleSheetData& operator=(const CSingleSheetData& node);

	void clear();
private:
	map<CNameCell, vector<CCommonCell> > m_MapInfo;
	CSheetStoreData m_sheetData;
};
