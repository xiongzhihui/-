#include "StdAfx.h"
#include "CDataCell.h"

CDataCell::CDataCell(void)
{
}

CDataCell::~CDataCell(void)
{
}

void CDataCell::setNameCell(CNameCell nameCell)
{
	m_nameCell = nameCell;
}

void CDataCell::setIndex(int nIndex)
{
	m_nIndex = nIndex;
}

void CDataCell::setComCell(CCommonCell comCell)
{
	m_ComCell = comCell;
}

void CDataCell::setSheetName(CString strSheetName)
{
	m_strSheetName = strSheetName;
}

void CDataCell::setItem(int nItem)
{
	m_nItem = nItem;
}

CNameCell CDataCell::getNameCell() const
{
	return m_nameCell;
}

int CDataCell::getIndex() const
{
	return m_nIndex;
}

CCommonCell CDataCell::getComCell() const
{
	return m_ComCell;
}

CString CDataCell::getSheetName() const
{
	return m_strSheetName;
}

CString CDataCell::getSheet() const
{
	CString strSheet;
	int nFind = m_strSheetName.Find(_T("("));
	if (nFind >=0)
	{
		strSheet = m_strSheetName.Left(nFind);
	}
	else
	{
		strSheet = m_strSheetName;
	}
	return strSheet;
}

CString CDataCell::getSheetLrmv() const
{
	CString strSheet;
	int nFind = m_strSheetName.Find(_T("("));
	if (nFind >=0)
	{
		strSheet = m_strSheetName.Left(nFind);
	}
	else
	{
		strSheet = m_strSheetName;
	}
	nFind = m_strSheetName.Find(_T("_"));
	if (nFind >= 0)
	{
		strSheet = m_strSheetName.Left(nFind);
		strSheet.Replace(_T("("), _T(""));
	}
	else
	{
		strSheet = MyParserString::RemoveSymbol(_T("("), _T(")"));
	}
	return strSheet;
}

CString CDataCell::getSheetMod() const
{
	CString strPre,strFix, strNextFix, strEnd, strOut, strNum;

	int nFind = m_strSheetName.Find(_T("("));
	if (nFind>=0)
	{
		strPre = m_strSheetName.Left(nFind);
		return strPre;
	}
	else
	{
		return m_strSheetName;
	}
}

int CDataCell::getItem() const
{
	return m_nItem;
}

CDataCell& CDataCell::operator=(const CDataCell& node)
{
	m_nameCell = node.getNameCell();
	m_nIndex = node.getIndex();
	m_ComCell = node.getComCell();
	m_strSheetName = node.getSheetName();
	m_nItem = node.getItem();
	return *this;
}
