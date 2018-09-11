#include "StdAfx.h"
#include "RFData.h"

CRFData::CRFData(void)
{
	m_ComVec.clear();
}

CRFData::~CRFData(void)
{
	m_ComVec.clear();
}


void CRFData::setNamCell(CNameCell nameCell)
{
	m_nameCell = nameCell;
}

void CRFData::setCommonCellVec(vector<CCommonCell> vec)
{
	m_ComVec.insert(m_ComVec.end(), vec.begin(), vec.end());
	vec.clear();
}

void CRFData::add(CCommonCell comCell)
{
	m_ComVec.push_back(comCell);
}

void CRFData::clear()
{
	m_ComVec.clear();
}

CNameCell CRFData::getNameCell() const
{
	return m_nameCell;
}

vector<CCommonCell> CRFData::getCommonVec() const
{
	return m_ComVec;
}


CRFData& CRFData::operator=(const CRFData& node)
{
	vector<CCommonCell> ComVec = node.getCommonVec();
	m_ComVec.insert(m_ComVec.end(), ComVec.begin(), ComVec.end());
	m_nameCell = node.getNameCell();
	ComVec.clear();
	return *this;
}
