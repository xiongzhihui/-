#include "StdAfx.h"
#include "SheetStoreData.h"

CSheetStoreData::CSheetStoreData(void)
{
	m_tableData.clear();
	m_glData.clear();
	m_CommonData.clear();
	m_RfData.clear();
	m_nSheetType = 1;
}

CSheetStoreData::~CSheetStoreData(void)
{
}

void CSheetStoreData::setSheetName(CString strSheetName)
{
	strSheetName.Trim();
	m_strSheetName = strSheetName;
}

void CSheetStoreData::setTableData(vector<CTableData> tableData)
{
	m_tableData.clear();
	m_tableData.insert(m_tableData.end(), tableData.begin(), tableData.end());
	tableData.clear();
}

void CSheetStoreData::setRFData(vector<CRFData> rfTableData)
{
	m_RfData.clear();
	m_RfData.insert(m_RfData.end(), rfTableData.begin(), rfTableData.end());
	rfTableData.clear();
}

void CSheetStoreData::setCommonData(vector<CCommonData> comData)
{
	m_CommonData.clear();
	m_CommonData.insert(m_CommonData.end(), comData.begin(), comData.end());
	comData.clear();
}	

void CSheetStoreData::setGLData(vector<CGLData> glData)
{
	m_glData.clear();
	m_glData.insert(m_glData.end(), glData.begin(), glData.end());
	glData.clear();
}

void CSheetStoreData::setType(int nType)
{
	m_nType = nType;
}

void CSheetStoreData::setSheetType(int nType)
{
	m_nSheetType = nType;
}

void CSheetStoreData::setLrmName(CString strLrmName)
{
	m_strLrmName = strLrmName;
}

void CSheetStoreData::setModLrmName(CString strModLrmName)
{
	m_strModLrmName = strModLrmName;
}

void CSheetStoreData::addTableItem(CTableData tblData)
{
	m_tableData.push_back(tblData);
}

void CSheetStoreData::addRfDataItem(CRFData rfData)
{
	m_RfData.push_back(rfData);
}

void CSheetStoreData::addCommonDataItem(CCommonData comData)
{
	m_CommonData.push_back(comData);
}

void CSheetStoreData::addGlDataItem(CGLData glData)
{
	m_glData.push_back(glData);
}

CString CSheetStoreData::getSheetName() const
{
	return m_strSheetName;
}

vector<CTableData> CSheetStoreData::getTableData() const
{
	return m_tableData;
}

vector<CRFData> CSheetStoreData::getRfData() const
{
	return m_RfData;
}

vector<CCommonData> CSheetStoreData::getCommonData() const
{
	return m_CommonData;
}

vector<CGLData> CSheetStoreData::getGLData() const
{
	return m_glData;
}

int CSheetStoreData::getType() const
{
	return m_nType;
}

int CSheetStoreData::getSheetType() const
{
	return m_nSheetType;
}

CString CSheetStoreData::getLrmName() const
{
	return m_strLrmName;
}

CString CSheetStoreData::getModLrmName() const
{
	return m_strModLrmName;
}

void CSheetStoreData::clear()
{
	m_tableData.clear();
	m_glData.clear();
	m_RfData.clear();
	m_CommonData.clear();
}

CSheetStoreData& CSheetStoreData::operator=(const CSheetStoreData& node)
{
	vector<CTableData> tableData = node.getTableData();//表格数据
	vector<CRFData> RfData = node.getRfData();//射频数据
	vector<CCommonData> CommonData = node.getCommonData();//普通数据
	vector<CGLData> glData = node.getGLData();//GL数据
	clear();

	m_tableData.insert(m_tableData.end(), tableData.begin(), tableData.end());
	m_RfData.insert(m_RfData.end(), RfData.begin(), RfData.end());
	m_CommonData.insert(m_CommonData.end(), CommonData.begin(), CommonData.end());
	m_glData.insert(m_glData.end(), glData.begin(), glData.end());
	m_nType = node.getType();
	m_strSheetName = node.getSheetName();
	m_nSheetType = node.getSheetType();

	m_strLrmName = node.getLrmName();
	m_strModLrmName = node.getModLrmName();
	tableData.clear();
	RfData.clear();
	CommonData.clear();
	glData.clear();
	return *this;
}
