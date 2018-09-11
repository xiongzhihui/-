#include "StdAfx.h"
#include "GLData.h"

CGLData::CGLData(void)
{
	m_ComVec.clear();
	m_nType = 0;
}

CGLData::~CGLData(void)
{
	m_ComVec.clear();
	m_nType = 0;
}


void CGLData::setNamCell(CNameCell nameCell)
{
	m_nameCell = nameCell;
}

void CGLData::setCommonCellVec(vector<CCommonCell> vec)
{
	m_ComVec.insert(m_ComVec.end(), vec.begin(), vec.end());
	vec.clear();
}

void CGLData::add(CCommonCell comCell)
{
	m_ComVec.push_back(comCell);
}

void CGLData::setType(int nType)
{
	m_nType = nType;
}

void CGLData::setNumber(CString nNum)
{
	m_strNumber = nNum;
}

CNameCell CGLData::getNameCell() const
{
	return m_nameCell;
}

vector<CCommonCell > CGLData::getCommonVec() const
{
	return m_ComVec;
}


int CGLData::type() const
{
	return m_nType;
}


CString CGLData::number() const
{
	return m_strNumber;
}

CGLData& CGLData::operator=(const CGLData& node)
{
	m_nameCell = node.getNameCell();
	m_nType = node.type();
	m_strNumber = node.number();
	m_ComVec.clear();
	vector<CCommonCell> tmpVec = node.getCommonVec();
	m_ComVec.insert(m_ComVec.end(), tmpVec.begin(), tmpVec.end());
	tmpVec.clear();
	return *this;
}

void CGLData::clear()
{
	m_ComVec.clear();
}
