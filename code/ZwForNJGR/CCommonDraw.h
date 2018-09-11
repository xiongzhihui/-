#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"

class CCommonDraw
{
public:
	CCommonDraw(void);
	~CCommonDraw();

	void setNameCell(CNameCell cell);
	void setCommonCell(CCommonCell cell);

	CNameCell getNameCell();
	CCommonCell getCommonCell();
private:
	CNameCell m_nameCell;
	CCommonCell m_commonCell;
};
