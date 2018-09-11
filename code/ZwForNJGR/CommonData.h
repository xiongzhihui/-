#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include <vector>
using namespace std;

class CCommonData
{
public:
	CCommonData(void);
	~CCommonData(void);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void setNamCell(CNameCell nameCell);
	void setCommonCellVec(vector<CCommonCell> vec);
	void add(CCommonCell comCell);
	
	CNameCell getNameCell() const;
	vector<CCommonCell> getCommonVec() const;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

private:
	CNameCell m_nameCell;
	vector<CCommonCell> m_ComVec;
};
