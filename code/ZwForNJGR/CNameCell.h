#pragma once

class CNameCell
{
public:	 
	CNameCell(void);

	~CNameCell();

	void setName(CString strName);
	void setStatus(CString strStatus);
	void setItem(int nItem);
	void setLjqName(CString strLjqName);
	void setWzNo(CString strNo);
	void setModLjqName(CString strLjqName);
	void setModWzNo(CString strNo);
	void setDrawText(bool bDrawText);

	CString getName() const;
	CString getStatus() const;
	int getItem() const;
	CString getLjqName() const;
	CString getWzNo() const;
	CString getModLjqName() const;
	CString getModWzNo() const;
	bool getDrawText() const;

	void clear();
	CNameCell& operator=(const CNameCell& node);


	friend bool operator<(const CNameCell &a, const CNameCell &b)
	{
		return a.m_nItem < b.m_nItem;
	}

	CString getRealName() const;
	CString getSpecailaName() const;
private:
	CString m_strName;
	CString m_strStatus;
	int m_nItem;
	CString m_strLjqName;//母版连接器名称
	CString m_strWzNo;//物资代码

	CString m_strModLjgName;
	CString m_strModWzNo;//
	bool m_bDrawText;//是否绘制文字
};
