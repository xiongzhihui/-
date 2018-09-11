#pragma once
#include "CCommonCell.h"
#include "CNameCell.h"
#include <vector>
using namespace std;

class CGLData
{
public:
	CGLData(void);
	~CGLData(void);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void setNamCell(CNameCell nameCell);
	void setCommonCellVec(vector<CCommonCell>  vec);
	void add(CCommonCell comCell);
	void setType(int nType);
	void setNumber(CString nNum);

	CNameCell getNameCell() const;
	vector<CCommonCell > getCommonVec() const;
	int type() const;
	CString number() const;
	CGLData& operator=(const CGLData& node);
	void clear();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

private:
	CNameCell m_nameCell;
	vector<CCommonCell>  m_ComVec;
	int m_nType;//集束光纤类型，0表示单光纤，1表示12光纤，2表示24光纤，3表示48光纤
	CString m_strNumber;//编号
};
