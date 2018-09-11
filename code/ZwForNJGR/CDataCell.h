#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"

class CDataCell
{
public:
	CDataCell(void);
	~CDataCell(void);
	void setNameCell(CNameCell nameCell);
	void setIndex(int nIndex);
	void setComCell(CCommonCell comCell);
	void setSheetName(CString strSheetName);

	void setItem(int nItem);
	CNameCell getNameCell() const;
	int getIndex() const;
	CCommonCell getComCell() const;
	CString getSheetName() const;
	CDataCell& operator=(const CDataCell& node);
	
	CString getSheet() const;
	CString getSheetLrmv() const;
	CString getSheetMod() const;
	int getItem() const;

private:
	CNameCell m_nameCell;
	int m_nIndex;//Ë÷Òý
	CCommonCell m_ComCell;//ÆÕÍ¨
	CString m_strSheetName;
	int m_nItem;
};
