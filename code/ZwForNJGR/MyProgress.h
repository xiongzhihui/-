#pragma once
#include <windef.h>


// CMyProgress

class CMyProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CMyProgress)

public:
	CMyProgress();
	virtual ~CMyProgress();
	
	void SetRange(int nStart, int nEnd);
	int SetPos(int nPos);
protected:
	DECLARE_MESSAGE_MAP()

public:
	COLORREF m_prgsColor; //进度条进度部分颜色
	COLORREF m_freeColor; //进度条后面空余部分颜色
	COLORREF m_prgsTextColor; //进度部分字体颜色
	COLORREF m_freeTextColor; //空白部分字体颜色

	int  m_iMin;    //进度条的最小值，通常是0
	int  m_iMax;    //进度条的最大值，通常是100
	int  m_iPos;    //当前的进度
	int  m_nBarWidth;  //进度条宽度
	afx_msg void OnPaint();
};


