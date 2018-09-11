#pragma once
#include "resource.h"
#include "BaseUtils.h"

// CDlgEditTexing 对话框

class CDlgEditTexing : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDlgEditTexing)

public:
	CDlgEditTexing(CString row,CString col,CString sheetname,int type,CString duanzi,CString xianhao,
		CString quxiang,CString texing,AcDbObjectId idEdit,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEditTexing();

// 对话框数据
	enum { IDD = IDD_DLG_EDITTX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_sCol;
	CString m_sDuanzi;
	CString m_sQxiang;
	CString m_sRow;
	CString m_sSheetName;
	CString m_sTexing;
	CString m_sTexingYuanyou;
	CString m_sXianhao;
	CString m_sType;
	AcDbObjectId m_idEdit;
	bool m_bEdit;
	bool m_bDelete;
	bool m_bAdd;
	void editTx();
	void deleteTx();
	void addTx();

	void init();

	double m_dTableLength;//表格长度

	double m_dFirstTableLength;//第一节数据
	double m_dSecondTableLength;//第二节数据
	double m_dThirdTableLength;//第三节数据

	double m_dRfSecondTableLength;//RF第二节长度
	double m_dRfThirdTableLength;//RF第三节长度
public:
	afx_msg void OnBnClickedOk();
};
