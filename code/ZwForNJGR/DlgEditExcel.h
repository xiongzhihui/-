#pragma once
#include "resource.h"
#include "BaseUtils.h"
#include "MyExcel.h"
#include <algorithm>
#include "DlgProress.h"
#include "CGlobalQuery.h"
#include "Utility.h"

// CDlgEditExcel 对话框
struct ModExcelInfo
{
	CString sheetName;
	int row;
	int col;
	CString texing;
	CString type;
	bool operator<(const ModExcelInfo&b)
	{
		if (this->sheetName<b.sheetName)
		{
			return true;
		}
		else if (this->sheetName>b.sheetName)
		{
			return false;
		}
		else
		{
			if (this->row<b.row)
			{
				return true;
			}
			else if (this->row>b.row)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
};

class CDlgEditExcel : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDlgEditExcel)

public:
	CDlgEditExcel(bool refreshExcel,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEditExcel();

// 对话框数据
	enum { IDD = IDD_DLG_EDITEXCEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sExcelPath;
private:
	bool m_bRefExcel;//如果为true 则从dwg刷新信息到excel 如果为false 则从excel反向修改特性

	double m_dTableLength;//表格长度

	double m_dFirstTableLength;//第一节数据
	double m_dSecondTableLength;//第二节数据
	double m_dThirdTableLength;//第三节数据

	double m_dRfSecondTableLength;//RF第二节长度
	double m_dRfThirdTableLength;//RF第三节长度

	std::vector<ModExcelInfo> m_vecModInfo;
	bool GetModInfoForDwg();//从图纸中获取修改信息
	void EditExcelForModInfo();//通过修改信息刷新excel

	bool GetModInfoForExcel();//反向修改从excel中获取刷新去向信息
	void EditDwgForModInfo();//通过修改信息刷新dwg
public:
	void init();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
