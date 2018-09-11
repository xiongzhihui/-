#include "StdAfx.h"
#include "SingleSheetData.h"

CSingleSheetData::CSingleSheetData(void)
{
	m_MapInfo.clear();
	m_sheetData.clear();
}

CSingleSheetData::~CSingleSheetData(void)
{
	m_MapInfo.clear();
	m_sheetData.clear();
}

void CSingleSheetData::setMapInfo(map<CNameCell, vector<CCommonCell> > info)
{
	m_MapInfo.clear();
	for (map<CNameCell, vector<CCommonCell> >::iterator iter = info.begin(); iter != info.end(); ++iter)
	{
		m_MapInfo.insert(make_pair(iter->first, iter->second));
	}
	info.clear();
}

void CSingleSheetData::setSheetStoreData(CSheetStoreData sheetData)
{
	m_sheetData = sheetData;
}

map<CNameCell, vector<CCommonCell> > CSingleSheetData::getMapInfo() const
{
	return m_MapInfo;
}

CSheetStoreData CSingleSheetData::getSheetStoreData() const
{
	return m_sheetData;
}

CSingleSheetData& CSingleSheetData::operator=(const CSingleSheetData& node)
{
	m_MapInfo.clear();
	map<CNameCell, vector<CCommonCell> > tmpComCell = node.getMapInfo();
	for (map<CNameCell, vector<CCommonCell> >::iterator iter = tmpComCell.begin(); iter != tmpComCell.end(); ++iter)
	{
		m_MapInfo.insert(make_pair(iter->first, iter->second));
	}

	m_sheetData = node.getSheetStoreData();
	tmpComCell.clear();

	return *this;
}

void CSingleSheetData::clear()
{
	m_MapInfo.clear();
	m_sheetData.clear();
}
