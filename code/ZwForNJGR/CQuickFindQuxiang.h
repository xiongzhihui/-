#pragma once
#include "CCommonCell.h"
#include "CIgnoreErrorCell.h"
//快速查找去向所在的原始excel位置，方便修改

class CQuickFindQuxiang
{
public:
	CQuickFindQuxiang(void);
	~CQuickFindQuxiang(void);
	bool doIt();
	//单选
	bool sel();
	//分析数据
	bool AnalysisData();
	//打印信息
	void print();
	//显示对话框
	bool showDialog();
private:
	void addDlgInfo(CCommonCell curComcell);
private:
	CString m_strContent;//内容
	CString m_strSheetName;
	COMVEC m_VecInfo;//内容
	vector<CIgnoreErrorCell> m_dlgInfo;
};

