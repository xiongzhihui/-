#include "StdAfx.h"
#include "CLrmvTitleInfo.h"

CLrmvTitleInfo::CLrmvTitleInfo(void)
{

}

CLrmvTitleInfo::~CLrmvTitleInfo(void)
{

}

int CLrmvTitleInfo::startIndex() const
{
	return m_nStart;
}

int CLrmvTitleInfo::endIndex() const
{
	return m_nEnd;
}

int CLrmvTitleInfo::type() const
{
	return m_nType;
}

CString CLrmvTitleInfo::name() const
{
	return m_strName;
}

void CLrmvTitleInfo::setStartIndex(int nIndex)
{
	m_nStart = nIndex;
}

void CLrmvTitleInfo::setEndIndex(int nIndex)
{
	m_nEnd = nIndex;
}

void CLrmvTitleInfo::setType(int nType)
{
	m_nType = nType;
}


void CLrmvTitleInfo::setName(CString strName)
{
	m_strName = strName;
}
