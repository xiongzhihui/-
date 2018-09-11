#pragma once
#include "SingleSheetData.h"
#include <vector>
#include "CDataCell.h"
using namespace std;

class CDataAnalysis
{
public:
	CDataAnalysis(void);
	~CDataAnalysis(void);
	
	void setData(map<CString, CSingleSheetData> data);
	map<CString, CSingleSheetData> getData() const;

	void setDataSCell(vector<CDataCell> data);
	vector<CDataCell> getDataSCell() const;

	void setDataMCell(vector<CDataCell> data);
	vector<CDataCell> getDataMCell() const;
	
	void setDataRCell(vector<pair<int, CDataCell> > data);
	vector<pair<int, CDataCell> > getDataRCell() const;

	void setDataCCell(vector<CDataCell> data);
	vector<CDataCell> getDataCCell() const;

	void setDataRFCell(vector<CDataCell> data);
	vector<CDataCell> getDataRFCell() const;

	void setDataDPCell(vector<CDataCell> data);
	vector<CDataCell> getDataDPCell() const;

	bool doIt();

private:
	bool doSData();

	bool doCData();
	bool doRData();

	bool doRFData();
	bool doDPData();
private:
	map<CString, CSingleSheetData> m_data;
	vector<CDataCell> m_dataSCell;
	vector<CDataCell> m_dataMCell;
	vector<pair<int, CDataCell> > m_dataRCell;
	vector<CDataCell> m_dataCCell;
	vector<CDataCell> m_dataRFCell;
	vector<CDataCell> m_dataDPCell;
};
