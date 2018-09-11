#pragma once
#include "MyExcel.h"
//生成接线表 和接线关系表

class CGenerateJxb
{
public:
	CGenerateJxb(void);
	~CGenerateJxb(void);
	//设置接线表数据
	void setType(bool bIsJxb);

	bool JudgeIsEmpty();
	bool openExcel();

	bool writeJxb();
	bool writeJxbgxb();
	bool writeDpInfoToExcel(int& nRow);
	bool writeRfInfoToExcel(int& nRow);
	bool writeMTjxbToExcel(int& nRow);
	//接线表
	bool writeRMTJxbToExcel(int& nRow);
	//接线关系表
	bool writeMTJxgxbToExcel(int& nRow);
	bool writeTInfoToExcel(int& nRow);
private:
	//先处理接线表的表格
	void doJxbExcel();
	//跳行数
	int skipRow(int nRow);
	bool insertRow(int nRow);

private:
	bool m_bIsJxb;//是否接线表
	CMyExcel m_Excel;
	int m_nSize;//dp和Rf的大小
	CString m_strFile;
	MPMAP m_dpOutInfo;
	MPMAP m_rfOutInfo;
	map<CString, vector<pair<CCommonCell, CCommonCell> > > m_mtOutPutMap;
};

