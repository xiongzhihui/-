#include "StdAfx.h"
#include "CIgnoreErrorCell.h"

CIgnoreErrorCell::CIgnoreErrorCell(void)
{
}

CIgnoreErrorCell::~CIgnoreErrorCell(void)
{
}

CString CIgnoreErrorCell::sheetName()
{
	return m_strSheetName;
}

CString CIgnoreErrorCell::row()
{	
	return m_strRow;
}

CString CIgnoreErrorCell::col()
{
	return m_strCol;
}

CString CIgnoreErrorCell::texing()
{
	return m_strTexing;
}

CString CIgnoreErrorCell::realTexing()
{
	return m_strRealTexing;
}

CString CIgnoreErrorCell::prompt()
{
	return m_strPrompt;
}

void CIgnoreErrorCell::SetSheetName(CString strSheetName)
{
	m_strSheetName = strSheetName;
}

void CIgnoreErrorCell::SetRow(CString strRow)
{
	m_strRow = strRow;
}

void CIgnoreErrorCell::SetCol(CString strCol)
{
	m_strCol = strCol;
}

void CIgnoreErrorCell::setTexing(CString strTexing)
{
	m_strTexing = strTexing;
}

void CIgnoreErrorCell::setRealTexing(CString strRealTexing)
{
	m_strRealTexing = strRealTexing;
}

void CIgnoreErrorCell::setPrompt(CString strPrompt)
{
	m_strPrompt = strPrompt;
}


void CIgnoreErrorCell::set(CString strSheetName, CString strRow, CString strCol, CString strTexing, CString strRealTexing, CString strPrompt)
{
	m_strSheetName = strSheetName;
	m_strRow = strRow;
	m_strCol = strCol;
	m_strTexing = strTexing;
	m_strRealTexing = strRealTexing;
	m_strPrompt = strPrompt;
}

