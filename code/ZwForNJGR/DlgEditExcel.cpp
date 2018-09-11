// DlgEditExcel.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgEditExcel.h"
#include "afxdialogex.h"


// CDlgEditExcel 对话框
extern Adesk::Boolean startDlg();
extern Adesk::Boolean endDlg();
extern CGlobalQuery gQuery;
extern CDlgProress *modelessDlg;

IMPLEMENT_DYNAMIC(CDlgEditExcel, CDialogEx)

CDlgEditExcel::CDlgEditExcel(bool refreshExcel, CWnd* pParent /*=NULL*/)
: CAcUiDialog(CDlgEditExcel::IDD, pParent)
{
	//获取excel地址
	m_sExcelPath = gGlobal.GetIniValue(_T("图框设置"), _T("路径"));
	m_bRefExcel = refreshExcel;
	init();
}

CDlgEditExcel::~CDlgEditExcel()
{
}

void CDlgEditExcel::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXCELPATH, m_sExcelPath);
}


BEGIN_MESSAGE_MAP(CDlgEditExcel, CAcUiDialog)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CDlgEditExcel::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDOK, &CDlgEditExcel::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEditExcel 消息处理程序


void CDlgEditExcel::init()
{
	m_dFirstTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第一段长度"), 0);
	m_dSecondTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第二段长度"), 0);
	m_dThirdTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("表格第三段长度"), 0);
	m_dTableLength = m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength;

	m_dRfSecondTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第二段长度"), 0);
	m_dRfThirdTableLength = gGlobal.GetIniValue(_T("基础设置"), _T("RF表格第三段长度"), 0);
}

void CDlgEditExcel::OnBnClickedBtnSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFileDialog importDlg(TRUE, _T(".xls"), NULL, 0,
		_T("excel格式(*.xls;*.xlsx) |*.xls;*.xlsx|"), acedGetAcadFrame());
	if (importDlg.DoModal() == IDOK)
	{
		m_sExcelPath = importDlg.GetPathName();
	}
	else
	{
		return;
	}
	UpdateData(FALSE);
}

void CDlgEditExcel::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bRefExcel)
	{
		if (GetModInfoForDwg())
		{
			EditExcelForModInfo();
		}
	}
	else
	{
		if (GetModInfoForExcel())
		{
			EditDwgForModInfo();
		}
	}
	CAcUiDialog::OnOK();
}

bool CDlgEditExcel::GetModInfoForDwg()
{
	m_vecModInfo.clear();
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("检索数据"));
	}
	AcDbObjectIdArray idArr = CBaseUtils::GetObjectIdArrayForXdata(ICD_XDATA_XIUGAI);
	for (int i = 0; i < idArr.length();i++)
	{
		ModExcelInfo modTemp;
		MyBaseAppXdata::getXdata(ICD_XDATA_SHEET, modTemp.sheetName, idArr[i]);
		MyBaseAppXdata::getXdata(ICD_XDATA_ROW, modTemp.row, idArr[i]);
		MyBaseAppXdata::getXdata(ICD_XDATA_COL, modTemp.col, idArr[i]);
		MyBaseAppXdata::getXdata(ICD_XDATA_TEXING, modTemp.texing, idArr[i]);
		int nType;
		MyBaseAppXdata::getXdata(ICD_XDATA_TYPE, nType, idArr[i]);
		modTemp.type = CBaseUtils::GetTexingType(nType);
		m_vecModInfo.push_back(modTemp);
		if (modelessDlg!=NULL)
		{
			modelessDlg->setPos((i+1) * 100 / idArr.length());
			Sleep(50);
		}
	}
	endDlg();
	if (m_vecModInfo.size()==0)
	{
		return false;
	}
	sort(m_vecModInfo.begin(), m_vecModInfo.end());
	return true;
}

void CDlgEditExcel::EditExcelForModInfo()
{
	CMyExcel excel;
	BOOL bRet = excel.Open(m_sExcelPath);
	if (!bRet)
	{
		AfxMessageBox(_T("excel打开失败!请检测excel文件。"));
		return;
	}
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("反向修改excel"));
	}
	for (int i = 0; i < m_vecModInfo.size(); i++)
	{
		ModExcelInfo modTemp = m_vecModInfo[i];
		if (!excel.OpenSheet(modTemp.sheetName))
		{
			continue;
		}
		excel.SetItemText(modTemp.row, modTemp.col, modTemp.type + modTemp.texing);
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos((i+1) * 100 / m_vecModInfo.size());
			Sleep(50);
		}
	}
	endDlg();
	excel.Save();
	excel.Exit();
}

bool CDlgEditExcel::GetModInfoForExcel()
{
	UpdateData(TRUE);

	CMyExcel myExcel;
	BOOL bRet = myExcel.Open(m_sExcelPath);
	if (!bRet)
	{
		AfxMessageBox(_T("文件不能打开，请检查路径是否有效"));
		return false;
	}

	//在初始化时，将数据置空
	gQuery.clear();
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("自动校验"));
	}

	int nCount = myExcel.GetSheetCount();
	for (int i = 1; i <= nCount; i++)
	{
		bRet = myExcel.OpenSheet(i);
		myExcel.GetArea();
		myExcel.readSpecailCol();
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(i * 50 / nCount);
			Sleep(50);
		}
	}
	myExcel.Clear();
	endDlg();

	return true;
}

void CDlgEditExcel::EditDwgForModInfo()
{
	gQuery.exchangeTexingCommonData();
}

BOOL CDlgEditExcel::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	if (!m_bRefExcel)
	{
		this->SetWindowText(_T("从excel局部刷新图纸"));
	}
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
