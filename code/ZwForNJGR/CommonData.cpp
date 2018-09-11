#include "StdAfx.h"
#include "CommonData.h"

CCommonData::CCommonData(void)
{
	m_ComVec.clear();
}

CCommonData::~CCommonData(void)
{
	m_ComVec.clear();
}

void CCommonData::setNamCell(CNameCell nameCell)
{
	m_nameCell = nameCell;
}

void CCommonData::setCommonCellVec(vector<CCommonCell> vec)
{
	m_ComVec.insert(m_ComVec.end(), vec.begin(), vec.end());
	vec.clear();
}

void CCommonData::add(CCommonCell comCell)
{
	m_ComVec.push_back(comCell);
}

CNameCell CCommonData::getNameCell() const
{
	return m_nameCell;
}

vector<CCommonCell> CCommonData::getCommonVec() const
{
	return m_ComVec;
}
