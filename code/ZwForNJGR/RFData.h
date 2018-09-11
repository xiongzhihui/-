#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include <vector>
using namespace std;

class CRFData
{
public:
	CRFData(void);
	~CRFData(void);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void setNamCell(CNameCell nameCell);
	void setCommonCellVec(vector<CCommonCell> vec);
	void add(CCommonCell comCell);
	void clear();
	CNameCell getNameCell() const;
	vector<CCommonCell> getCommonVec() const;
	CRFData& operator=(const CRFData& node);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

private:
	CNameCell m_nameCell;
	vector<CCommonCell> m_ComVec;
};
