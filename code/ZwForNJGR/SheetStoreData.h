#pragma once
#include "TableData.h"
#include "RFData.h"
#include "CommonData.h"
#include "GLData.h"
#include <vector>
using namespace std;

class CSheetStoreData
{
public:
	CSheetStoreData(void);
	~CSheetStoreData(void);

	void setSheetName(CString strSheetName);
	void setTableData(vector<CTableData> tableData);
	void setRFData(vector<CRFData> rfTableData);
	void setCommonData(vector<CCommonData> comData);
	void setGLData(vector<CGLData> glData);
	void setType(int nType);
	void setSheetType(int nType);
	void setLrmName(CString strLrmName);
	void setModLrmName(CString strModLrmName);
	//添加实体
	//************************************
	// Method:    addTableItem
	// FullName:  CSheetStoreData::addTableItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CTableData tblData
	//************************************
	void addTableItem(CTableData tblData);
	//************************************
	// Method:    addRfDataItem
	// FullName:  CSheetStoreData::addRfDataItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CRFData rfData
	//************************************
	void addRfDataItem(CRFData rfData);
	//************************************
	// Method:    addCommonDataItem
	// FullName:  CSheetStoreData::addCommonDataItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CCommonData comData
	//************************************
	void addCommonDataItem(CCommonData comData);
	//************************************
	// Method:    addGlDataItem
	// FullName:  CSheetStoreData::addGlDataItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CGLData glData
	//************************************
	void addGlDataItem(CGLData glData);

	CString getSheetName() const;
	vector<CTableData> getTableData() const;
	vector<CRFData> getRfData() const;
	vector<CCommonData> getCommonData() const;
	vector<CGLData> getGLData() const;
	int getType() const;
	int getSheetType() const;

	CString getLrmName() const;
	CString getModLrmName() const;

	void clear();
	CSheetStoreData& operator=(const CSheetStoreData& node);
private:
	vector<CTableData> m_tableData;//表格数据
	vector<CRFData> m_RfData;//射频数据
	vector<CCommonData> m_CommonData;//普通数据
	vector<CGLData> m_glData;//GL数据
	int m_nType;//sheet类型，如果有为0，表示A0,为1表示A1
	int m_nSheetType;//0为普通类型，1为Lrm类型，2为LRMV类型，3为数字光类型
	CString m_strSheetName;
	CString m_strLrmName;
	CString m_strModLrmName;
};
