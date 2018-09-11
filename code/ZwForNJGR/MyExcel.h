//---------------------------------------------------------
#pragma once
#include "excel9.h" 
#include <comdef.h> 
#include <vector>
#include "CNameCell.h"
#include "SingleSheetData.h"
#include "CSheetInfo.h"
#include "SheetStoreData.h"
using namespace excel;
using namespace std;

class CMyExcel 
{ 
public: 
	//Excel的应用 
	_Application   MyApp;   
	_Workbook   MyBook; 
	_Worksheet   MySheet;    
	Workbooks   MyBooks;    
	Worksheets   MySheets;    
	Range   MyRange;   
	CString strFilePath; 
	//构造函数 
	CMyExcel(); 
	//析构函数 
	~CMyExcel(); 
	//打开新的Excel文件 
	BOOL Open(); 
	//打开strFile文件 
	BOOL Open(CString strFile); 
	BOOL openSpecailFile(CString strFile);

	int GetSheetCount();
	//打开名为strSheet的表 
	BOOL OpenSheet(CString strSheet = _T(""), bool bIsAdded = true); 
	bool OpenSheet(int nCount);
	BOOL OpenExcelDoc();
	//设置(ROW,COL)的字符strText 
	BOOL SetItemText(long Row,long Col,CString strText); 
	//取得(ROW,COL)的字符 
	CString GetItemText(long Row,long Col); 
	bool selectRange(long Row, long Col);
	//获取border值
	vector<int> GetBorderItem(long Row,long Col, bool& bAdd);
	//获取对齐方式 
	//设置列宽 
	double GetColumnWidth(long Row,long Col); 
	//设置行高 
	double GetRowHeight(long Row,long Col); 
	//获取有效区域
	void GetArea();
	
	bool verifyData();

	//退出excel 
	void Exit(); 
	//显示excel 
	void SetVisible(BOOL blnVisible); 
	//查找此文件是否存在 
	BOOL IsFileExist(CString strFn, BOOL bDir); 
	//自动保存(针对打开已经存在的文件) 
	void Save(); 
	//保存为strPath 
	void SaveAs(CString strPath); 
	//添加新的表 
	void AddSheet(CString strSheet); 
	//得到新的选择区域 
	void GetRange(CString strBegin,CString strEnd); 
	//得到Range范围
	void GetRangeArea(CString strBegin,CString strEnd);

	//列自动展开 
	void AutoColFit(); 
	//行自动展开 
	void AutoRowFit(); 
	//设置自动换行 
	void SetWrapText(BOOL blnTrue); 
	//得到整个区域 
	void AutoRange(); 
	//合并单元格 
	void SetMergeCells(BOOL blnTrue); 
	//合并单元格处理
	void MergeCells(long nRow, long nCol, long nEndRow, long nEndCol);
	//得到有效列数 
	long GetRowS(); 
	//得到有效行数 
	long GetColS(); 
	//设置列宽 
	void SetColumnWidth(int intWidth); 
	//设置行高 
	void SetRowHeight(int intHeight); 
	//复制行并插入
	void InsertRow(CString strBegin, CString strEnd, long nStart);
	//返回当前程序所在路径 
	CString GetAppPath(); 
	//void UpdateData(AcDbObjectId blkRecId);

	void Clear();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//单独为项目增加的函数
	void readSpecailCol();
	//读取LRMV-p数据
	void readLrmvpCol();
	//读取lrmv数据
	void readLrmvCol();

	bool verifyItemLength(CString strItem);
	void readLrmvdata(int nLen);
	void writeFile();
	void writeLrmvFile();

	map<CString, vector<vector<CString> > > ReadAllData();
	//读取当前活动页面的数据
	vector<vector<CString> > ReadActiveSheetData(CString& strSheetName);
	void readDigitalData(int nLen, int nType = 0);
	void readCommonData(int nLen);
	void readLrmCommonData(int nLen);
	void readLrmvpData(int nLen);
	void readLrmvData(int nLen);

	//处理名称信息
	vector<vector<CString> > doNameInfo(int nLen);
	void _Excel_Find_(LPCTSTR lpszFind);
	bool setVisible(bool bIsVisible);
	void CloseExcelFile(BOOL if_save = FALSE);
private:
	long iRowNum;
	long iColNum;
	long iStartRow;
	long iStartCol;

	int m_nItem;
	map<int, CNameCell > m_MapData;
	map<CString, CSingleSheetData> m_datainfo;
	vector<CString> m_nameVec;
	vector<CString> m_modNameVec;//全是模块的数据

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//记录lrmv数据
	vector<pair<CString, vector<vector<CString> > > > m_ExcelVec;
	//记录块名
	vector<CString> m_blkNameVec;
};


