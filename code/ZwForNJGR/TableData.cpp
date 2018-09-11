#include "StdAfx.h"
#include "TableData.h"

CTableData::CTableData(void)
{
	m_ComVec.clear();
}

CTableData::~CTableData(void)
{
	m_ComVec.clear();
}


void CTableData::setNamCell(CNameCell nameCell)
{
	m_nameCell = nameCell;
}

void CTableData::setCommonCellVec(vector<vector<CCommonCell> > vec)
{
	m_ComVec.insert(m_ComVec.end(), vec.begin(), vec.end());
	vec.clear();
}

void CTableData::add(vector<CCommonCell> comCell)
{
	m_ComVec.push_back(comCell);
	comCell.clear();
}

void CTableData::clear()
{
	m_ComVec.clear();
}

CNameCell CTableData::getNameCell() const
{
	return m_nameCell;
}

vector<vector<CCommonCell> > CTableData::getCommonVec() const
{
	return m_ComVec;
}


