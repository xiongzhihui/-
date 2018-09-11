#include "StdAfx.h"
#include "CNameCell.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CNameCell::CNameCell(void)
{
	m_strName = _T("");
	m_strStatus = _T("0");
	m_nItem = 3;
	m_bDrawText = true;
	m_strWzNo = _T("");
	m_strWzNo = _T("");
}


CNameCell::~CNameCell()
{

}

void CNameCell::setName(CString strName)
{
	m_strName = strName;
}

void CNameCell::setStatus(CString strStatus)
{
	m_strStatus = strStatus;
}

void CNameCell::setItem(int nItem)
{
	m_nItem = nItem;
}
//
//void CNameCell::setModLjqName(CString strLjqName)
//{
//	m_strModLjqName = strLjqName;
//}
//
//void CNameCell::setModWzNo(CString strNo)
//{
//	m_strModWzNo = strNo;
//}

void CNameCell::setLjqName(CString strLjqName)
{
	m_strLjqName = strLjqName;
}

void CNameCell::setWzNo(CString strNo)
{
	m_strWzNo = strNo;
}

void CNameCell::setModLjqName(CString strLjqName)
{
	m_strModLjgName = strLjqName;
}

void CNameCell::setModWzNo(CString strNo)
{
	m_strModWzNo = strNo;
}

void CNameCell::setDrawText(bool bDrawText)
{
	m_bDrawText = bDrawText;
}

CString CNameCell::getName() const
{
	return m_strName;
}

CString CNameCell::getStatus() const
{
	return m_strStatus;
}

int CNameCell::getItem() const
{
	return m_nItem;
}


//CString CNameCell::getModLjqName() const
//{
//	return m_strModLjqName;
//}
//
//CString CNameCell::getModWzNo() const
//{
//	return m_strModWzNo;
//}

CString CNameCell::getLjqName() const
{
	return m_strLjqName;
}

CString CNameCell::getWzNo() const
{
	return m_strWzNo;
}

CString CNameCell::getModLjqName() const
{
	return m_strModLjgName;
}

CString CNameCell::getModWzNo() const
{
	return m_strModWzNo;
}

bool CNameCell::getDrawText() const
{
	return m_bDrawText;
}

CString CNameCell::getRealName() const
{
	CString strSheet;
	int nFind = m_strName.Find(_T("("));
	if (nFind >=0)
	{
		strSheet = m_strName.Left(nFind);
	}
	else
	{
		strSheet = m_strName;
	}
	CString	strLog = MyParserString::RemoveSymbol(_T("]"), strSheet);
	return strLog;
}

CString CNameCell::getSpecailaName() const
{
	CString strName;
	strName = m_strName;
	strName.Replace(_T("("), _T(""));
	strName.Replace(_T(")"), _T(""));
	return strName;
}

CNameCell& CNameCell::operator=(const CNameCell& node)
{
	m_strName = node.getName();
	m_strStatus = node.getStatus();
	m_nItem = node.getItem();
	m_strLjqName = node.getLjqName();
	m_strWzNo = node.getWzNo();
	m_bDrawText = node.getDrawText();
	m_strModWzNo = node.getModWzNo();
	m_strModLjgName = node.getModLjqName();
	return *this;
}

void CNameCell::clear()
{
	m_strName = _T("");
	m_strStatus = _T("0");
	m_strLjqName = _T("");
	m_strWzNo = _T("");
	m_strModWzNo = _T("");
	m_strModLjgName = _T("");
}
