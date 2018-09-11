#pragma once

class CLrmvTitleInfo
{
public:
	CLrmvTitleInfo(void);
	~CLrmvTitleInfo(void);
	int startIndex() const;
	int endIndex() const;
	int type() const;
	CString name() const;

	void setStartIndex(int nIndex);
	void setEndIndex(int nIndex);
	void setType(int nType);
	void setName(CString strName);
private:
	int m_nStart;
	int m_nEnd;
	int m_nType;//类型0为普通数据，1为射频数据，2为集束光纤数据
	CString m_strName;//头名比如P0这种
};
