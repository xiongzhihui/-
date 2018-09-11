#include "StdAfx.h"
#include "CCommonDraw.h"

void CCommonDraw::setNameCell(CNameCell cell)
{
	m_nameCell = cell;
}

void CCommonDraw::setCommonCell(CCommonCell cell)
{
	m_commonCell = cell;
}

CNameCell CCommonDraw::getNameCell()
{
	return m_nameCell;
}

CCommonCell CCommonDraw::getCommonCell()
{
	return m_commonCell;
}


