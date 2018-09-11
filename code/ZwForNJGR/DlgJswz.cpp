//-----------------------------------------------------------------------------
//----- DlgJswz.cpp : Implementation of CDlgJswz
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgJswz.h"
#include "Utility.h"
#include <shlwapi.h>

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgJswz, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgJswz, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CDlgJswz::OnNMClickTree1)
	ON_BN_CLICKED(IDOK, &CDlgJswz::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgJswz::OnBnClickedButtonSave)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CDlgJswz::OnNMRClickTree1)
	ON_COMMAND(ID_JS_ADD, &CDlgJswz::OnMenuAdd)
	ON_COMMAND(ID_JS_DELETE, &CDlgJswz::OnMenuDel)
	ON_COMMAND(ID_JS_EDIT, &CDlgJswz::OnMenuEdit)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE1, &CDlgJswz::OnTvnBeginlabeleditTree1)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CDlgJswz::OnTvnEndlabeleditTree1)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COPY, &CDlgJswz::OnCopy)
	ON_COMMAND(ID_PASTE, &CDlgJswz::OnPaste)
	ON_COMMAND(ID_ADD_PATH, &CDlgJswz::OnMenuAddPath)
	ON_COMMAND(ID_JS_SAVE, &CDlgJswz::OnJsSave)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgJswz::CDlgJswz (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgJswz::IDD, pParent, hInstance) {
	AfxInitRichEdit();
}

//-----------------------------------------------------------------------------
void CDlgJswz::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearch);
	DDX_Control(pDX, IDC_RICHEDIT2_UPDATE, m_EditUpdate);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgJswz::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}


BOOL CDlgJswz::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());
	// TODO:  在此添加额外的初始化
	m_TreeCtrl.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_EDITLABELS);
	m_strParentPath = MyBaseUtils::GetAppPath();
	m_TreeCtrl.SetParentPath(m_strParentPath);
	m_TreeCtrl.SetContent(m_strSearch);
	m_TreeCtrl.DisPlayTree(_T("技术文字"), TRUE);
	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
	//	m_TreeCtrl.Expand(hChild, TVE_EXPAND);
	// TODO:  在此添加额外的初始化
	//if (!m_strSearch.IsEmpty())
	//{
	//	HTREEITEM hItem = m_TreeCtrl.GetRootItem();
	//	hItem = FindItem(hItem);
	//	if (hItem == NULL)
	//	{
	//		HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	//		HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
	//		m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
	//		m_TreeCtrl.Expand(hChild, TVE_EXPAND);

	//		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
	//		m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".tj");
	//	}
	//	else
	//	{
	//		m_strSearch = m_TreeCtrl.GetItemText(hItem);	
	//		m_TreeCtrl.SelectItem(hItem); //设置选中结点
	//		m_strFileName = m_TreeCtrl.GetFullPath(hItem) + _T(".tj");
	//	}
	//}
	//else
	//{
	//	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	//	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
	//	m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
	//	m_TreeCtrl.Expand(hChild, TVE_EXPAND);

	//	HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
	//	m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".tj");
	//}

	//初始化置空
	if (OpenClipboard())
	{
		HGLOBAL clipBuffer;
		TCHAR* Buffer;
		//EmptyClipboard();
		CloseClipboard();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HTREEITEM CDlgJswz::FindItem(HTREEITEM ht)
{
	CString strText;
	HTREEITEM hItem = NULL;
	m_TreeCtrl.Expand(ht,TVE_EXPAND);
	hItem = m_TreeCtrl.GetNextItem(ht,TVGN_NEXTVISIBLE);
	if(hItem != NULL)
	{
		strText = m_TreeCtrl.GetItemText(hItem);
		if (strText.CompareNoCase(m_strSearch) == 0)
		{
			return hItem;
		}
		hItem = FindItem(hItem);
	}
	else
		return NULL;
	return hItem;
}


vector<CString> CDlgJswz::readStringFromFile(CString strFile)
{
	vector<CString> tmpVec;
	CStdioFile File;
	if (!File.Open(strFile, CFile::modeRead | CFile::typeText)) 
	{
		return tmpVec;
	}
	CString buffer;
	while (File.ReadString(buffer))
	{
		tmpVec.push_back(buffer);
	}
	File.Close();
	return tmpVec;
}

BOOL CDlgJswz::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((WM_KEYDOWN == pMsg-> message) && 
		(VK_RETURN ==  pMsg-> wParam))
	{
		if ( GetDlgItem(IDC_EDIT_SEARCH) == GetFocus())
		{
			GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(m_strSearch);
			m_TreeCtrl.SetContent(m_strSearch);
			m_TreeCtrl.DisPlayTree(_T("技术文字"), TRUE);
			HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
			m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
			return FALSE;
		}
	}
	if (pMsg->message == WM_KEYDOWN) 
	{
		switch(pMsg->wParam)
		{
		case VK_F2:
			OnMenuEdit();
			break;
		default:
			return CAcUiDialog::PreTranslateMessage(pMsg);
		}
		return TRUE;
	}
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

void CDlgJswz::UpdateMergeTreeCheck(const HTREEITEM hParent, const BOOL bCheck)
{
	CString strItem, strFileName;
	if (m_TreeCtrl.ItemHasChildren(hParent))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_TreeCtrl.GetChildItem(hParent);
		while (hChildItem != NULL)
		{
			//递归获取当前节点下的子节点
			m_TreeCtrl.SetCheck(hChildItem,bCheck);
			if (!m_TreeCtrl.ItemHasChildren(hChildItem))
			{
				strItem = m_TreeCtrl.GetItemText(hChildItem) ;//获得节点值
				strFileName = m_TreeCtrl.GetFullPath(hChildItem) + _T(".TJ");
				if (bCheck)//添加到右边
				{
					m_DataMap.insert(make_pair(strFileName, 1));
				}
				else//从右边数据中删除
				{
					map<CString,int>::iterator key = m_DataMap.find(strFileName);  
					if(key!=m_DataMap.end())  
					{  
						m_DataMap.erase(key);  
					}  
				}
			}

			UpdateMergeTreeCheck(hChildItem,bCheck);
			hNextItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
	}
}
void CDlgJswz::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint point;
	GetCursorPos(&point);//获得鼠标点击的位置
	m_TreeCtrl.ScreenToClient(&point);//转化为客户坐标
	UINT uFlags;

	HTREEITEM CurrentItem;
	CurrentItem=m_TreeCtrl.HitTest(point,&uFlags);//获得当前点击节点的ITEM
	//获得该节点的父节点
	// HTREEITEM ParentItem=m_videotree.GetParentItem(CurrentItem);
	CString strItem;
	if (uFlags & TVHT_ONITEMSTATEICON)
	{
		BOOL bCheck = !m_TreeCtrl.GetCheck(CurrentItem);
		if (m_TreeCtrl.ItemHasChildren(CurrentItem))
		{
			UpdateMergeTreeCheck(CurrentItem, bCheck);
		}
		else
		{
			m_TreeCtrl.SetCheck(CurrentItem, bCheck);
			strItem = m_TreeCtrl.GetItemText(CurrentItem) ;//获得节点值
			CString strFileName = m_TreeCtrl.GetFullPath(CurrentItem) + _T(".TJ");
			if (bCheck)//添加到右边
			{
				m_DataMap.insert(make_pair(strFileName, 1));
			}
			else//从右边数据中删除
			{
				map<CString,int>::iterator key = m_DataMap.find(strFileName);  
				if(key!=m_DataMap.end())  
				{  
					m_DataMap.erase(key);  
				}  
			}	
			m_TreeCtrl.SetCheck(CurrentItem, !bCheck);
		}
		
		//显示数据
		displayText();
	}	
	m_TreeCtrl.Select(CurrentItem, TVGN_DROPHILITE);
}


void CDlgJswz::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile;
	GetDlgItem(IDC_RICHEDIT2_UPDATE)->GetWindowText(strFile);
	if (strFile.Right(1).CompareNoCase(_T("\n")) !=0)
	{
		strFile+= _T("\n");
	}
	strFile.Replace(_T("\\"), _T("\\\\"));
	AcGePoint3d pt;
	BeginEditorCommand();
	int nRet = acedGetPoint(NULL, _T("\n制定插入点："), asDblArray(pt));
	if (nRet != RTNORM)
	{
		CancelEditorCommand();
		return;
	}
	AcDbObjectId textId = AcDbObjectId::kNull;
	double dlen,dTemp;
	dlen = dTemp = 0.0;
	int nFind = 0;
	CString strTemp,strNum;
	int i=1;
	AcGePoint3d cenPt;
	cenPt = pt;
	AcDbObjectIdArray objIdArr;
	CString strText;
	while (1)
	{
		nFind = strFile.Find(_T("\n"));
		if (nFind < 0)
		{
			break;
		}
		strTemp = strFile.Left(nFind);
		if (strTemp.GetLength() == 1)
		{
			strTemp.Trim();
			if (strTemp.IsEmpty())
			{
				strText += strTemp + _T("\n");
				pt.y -= 5;
				strFile = strFile.Right(strFile.GetLength()-nFind-1);
				continue;
			}
		}
		strNum.Format(_T("%d."), i);
		strTemp = strNum + strTemp + _T("\n");
		strText += strTemp;
		strFile = strFile.Right(strFile.GetLength()-nFind-1);
		i++;
		objIdArr.append(textId);
	}

	m_dJsTxtHeight = gGlobal.GetIniValue(_T("技术要求"), _T("技术要求字高"), 0);
	if (m_dJsTxtHeight < GeTol)
	{
		m_dJsTxtHeight = 4.0;
	}
	m_dTextGap = gGlobal.GetIniValue(_T("技术要求"), _T("文字间距"), 0);
	if (m_dTextGap < GeTol)
	{
		m_dTextGap = 5;
	}
	m_dTxtHeight = gGlobal.GetIniValue(_T("技术要求"), _T("技术文字"), 0);
	if (m_dTxtHeight < GeTol)
	{
		m_dTxtHeight = 3.0;
	}
	AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("文字设置"), _T("1"));
	textId = MyDrawEntity::DrawMText(pt, strText, m_dTxtHeight, textStyleId);
	objIdArr.append(textId);
	dlen = MyEditEntity::OpenObjAndGetLength(textId);

	BOOL bDraw = (int)gGlobal.GetIniValue(_T("技术要求"), _T("不绘制表头"), 0);
	if (!bDraw)
	{
		cenPt.x = pt.x + dlen/2;
		cenPt.y = pt.y + m_dTextGap;
		cenPt.z = 0;
		AcDbObjectId objId = MyDrawEntity::DrawText(cenPt, _T("技术要求"), m_dJsTxtHeight, textStyleId);
		objIdArr.append(objId);
	}
	
	int nUser = 0;
	MyBaseUtils::GetVar(_T("USERI5"), &nUser);
	CString strGroupName;
	strNum.Format(_T("%d"), nUser);
	strGroupName = JSWZ + strNum;
	MyDrawEntity::MakeGroup(objIdArr, false, strGroupName);
	nUser++;
	MyBaseUtils::SetVar(_T("USERI5"), nUser);
	CompleteEditorCommand();
	CAcUiDialog::OnOK();
}

void CDlgJswz::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	CString strFile,strTemp;
	GetDlgItem(IDC_RICHEDIT2_UPDATE)->GetWindowText(strFile);
	if (strFile.IsEmpty())
	{
		AfxMessageBox(_T("编辑框里没有数据"));
		return;
	}

	CString strCurrentPath;
	strCurrentPath = MyBaseUtils::GetAppPath() + _T("技术文字\\");
	
	struct resbuf* result = NULL;
	int status = acedGetFileNavDialog(_T("技术文字保存目录"), strCurrentPath, _T("TJ"), _T("保存文件"), 1, &result);
	if (status == RTNORM)
	{
		strCurrentPath = result->resval.rstring;
		acutRelRb( result );
	}
	else
	{
		acutRelRb( result );
		return;
	}
	

	CStdioFile File;
	if(!File.Open(strCurrentPath,CFile::modeReadWrite))     
	{     
		if (!File.Open(strCurrentPath, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
		{
			return;
		} 
	}    	
	int nFind = 0;
	while (1)
	{
		nFind = strFile.Find(_T("\n"));
		if (nFind < 0)
		{
			break;
		}
		strTemp = strFile.Left(nFind);
		if (strTemp.GetLength() == 1)
		{
			strTemp.Trim();
			if (strTemp.IsEmpty())
			{
				strFile = strFile.Right(strFile.GetLength()-nFind-1);
				File.SeekToEnd();
				continue;
			}
		}
		File.SeekToEnd();
		File.WriteString(strTemp);
		strFile = strFile.Right(strFile.GetLength()-nFind-1);
	}
	
	File.Close();

	//m_TreeCtrl.UpdateWindow();
	m_TreeCtrl.SetParentPath(m_strParentPath);
	m_TreeCtrl.SetContent(m_strSearch);
	m_TreeCtrl.DisPlayTree(_T("技术文字"), TRUE);
	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
}

void CDlgJswz::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	LPPOINT lpoint=new tagPOINT;   
	::GetCursorPos(lpoint);                                //获得鼠标

	CPoint point = *lpoint;
	::ScreenToClient(m_TreeCtrl.m_hWnd,  &point);

	HTREEITEM Item;
	UINT nFlag = TVHT_ONITEM; // 当在 Item 上右击时
	Item = m_TreeCtrl.HitTest( point, &nFlag );
	m_TreeCtrl.SelectItem(Item);
	if (!Item)
	{
		return;
	}
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_MENU);
	CMenu* popMenu = menu.GetSubMenu(0);
	if (m_TreeCtrl.ItemHasChildren(Item))//文件夹
	{
		popMenu->EnableMenuItem(ID_ADD_PATH, MF_ENABLED);
		popMenu->EnableMenuItem(ID_JS_ADD, MF_GRAYED);
		popMenu->EnableMenuItem(ID_JS_SAVE, MF_GRAYED);
	}
	else
	{
		popMenu->EnableMenuItem(ID_ADD_PATH, MF_GRAYED);
		popMenu->EnableMenuItem(ID_JS_ADD, MF_ENABLED);
		popMenu->EnableMenuItem(ID_JS_SAVE, MF_ENABLED);
	}
	popMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lpoint->x, lpoint->y, this, NULL);
	*pResult = 0;
}


void CDlgJswz::OnMenuAdd()
{
	//CString strFile,strTemp;
	//GetDlgItem(IDC_RICHEDIT2_UPDATE)->GetWindowText(strFile);
	//if (strFile.IsEmpty())
	//{
	//	AfxMessageBox(_T("编辑框里没有数据"));
	//	return;
	//}

	//CString strCurrentPath;
	////strCurrentPath = MyBaseUtils::GetAppPath() + _T("技术文字\\");
	//strCurrentPath =m_TreeCtrl.GetFullPath(m_TreeCtrl.GetSelectedItem());
	//CString strItem = m_TreeCtrl.GetItemText(m_TreeCtrl.GetSelectedItem());
	//strCurrentPath += _T(".tj");

	//struct resbuf* result = NULL;
	//int status = acedGetFileNavDialog(_T("技术文字保存目录"), strCurrentPath, _T("TJ"), _T("保存文件"), 1, &result);
	//if (status == RTNORM)
	//{
	//	strCurrentPath = result->resval.rstring;
	//	acutRelRb( result );
	//}
	//else
	//{
	//	acutRelRb( result );
	//	return;
	//}
	//

	//CStdioFile File;
	//if(!File.Open(strCurrentPath,CFile::modeReadWrite))     
	//{     
	//	if (!File.Open(strCurrentPath, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
	//	{
	//		return;
	//	} 
	//}    	
	//int nFind = 0;
	//while (1)
	//{
	//	nFind = strFile.Find(_T("\n"));
	//	if (nFind < 0)
	//	{
	//		if (!strTemp.IsEmpty())
	//		{
	//			File.SeekToEnd();
	//			File.WriteString(strTemp);
	//		}
	//		
	//		break;
	//	}
	//	strTemp = strFile.Left(nFind);
	//	if (strTemp.GetLength() == 1)
	//	{
	//		strTemp.Trim();
	//		if (strTemp.IsEmpty())
	//		{
	//			strFile = strFile.Right(strFile.GetLength()-nFind-1);
	//			File.SeekToEnd();
	//			continue;
	//		}
	//	}
	//	File.SeekToEnd();
	//	File.WriteString(strTemp);
	//	strFile = strFile.Right(strFile.GetLength()-nFind-1);
	//}
	//
	//File.Close();

	//m_TreeCtrl.SetParentPath(m_strParentPath);
	//m_TreeCtrl.SetContent(m_strSearch);
	//m_TreeCtrl.DisPlayTree(_T("技术文字"), TRUE);
	//HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	//m_TreeCtrl.Expand(hRoot, TVE_EXPAND);

	CString strItem,strFileName;
	HTREEITEM CurrentItem, hti, hParent;

	CurrentItem = m_TreeCtrl.GetSelectedItem();

	hParent = m_TreeCtrl.GetParentItem(CurrentItem);
	CString strPath = m_TreeCtrl.GetFullPath(CurrentItem);
	
	
	strPath = getNewFileName(strPath);

	HANDLE hFILE=CreateFile(strPath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	CloseHandle(hFILE);  
	hti = m_TreeCtrl.AddItem(hParent, strPath);
	m_TreeCtrl.SelectItem(hti);
	m_TreeCtrl.EditLabel(hti);
	m_EditUpdate.SetWindowText(_T(""));//置空
}

void CDlgJswz::OnMenuDel()
{	
	CString strItem,strFileName;
	HTREEITEM CurrentItem;

	CurrentItem = m_TreeCtrl.GetSelectedItem();
	if (m_TreeCtrl.ItemHasChildren(CurrentItem))
	{
		AfxMessageBox(_T("有子项，不允许删除文件夹"));
	}
	else
	{
		strFileName = m_TreeCtrl.GetFullPath(CurrentItem);
		if (::PathIsDirectory(strFileName))
		{
			m_TreeCtrl.DelFolder(strFileName);
		}
		else
		{
			strFileName += _T(".TJ");
			::DeleteFile(strFileName);
		}
		m_TreeCtrl.DeleteItem(CurrentItem);
	}
}

void CDlgJswz::OnMenuEdit()
{
	m_TreeCtrl.ModifyStyle(NULL,TVS_EDITLABELS);
	HTREEITEM hti;
	hti = m_TreeCtrl.GetSelectedItem();
	m_TreeCtrl.EditLabel(hti);
}



void CDlgJswz::OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CEdit *pEdit;

	pEdit = m_TreeCtrl.GetEditControl();
	pEdit->SetLimitText(100);	
	*pResult = 0;
}

void CDlgJswz::OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	TV_DISPINFO *ptvinfo;

	// Called at exit from Edit Mode
	ptvinfo = (TV_DISPINFO *)pNMHDR;
	CString strPre,strCur,strpreFileName,strCurFileName;
	bool bIsDir = false;
	if (ptvinfo->item.pszText != NULL)
	{		
		strpreFileName = m_TreeCtrl.GetFullPath(m_TreeCtrl.GetSelectedItem());
		if (::PathIsDirectory(strpreFileName))
		{
			//那么它是一个目录
			bIsDir = true;
		}
		else
		{
			strpreFileName +=  _T(".TJ");
		}

		ptvinfo->item.mask = TVIF_TEXT;
		m_TreeCtrl.SetItem(&ptvinfo->item);
		if (bIsDir)
		{
			strCurFileName = m_TreeCtrl.GetFullPath(m_TreeCtrl.GetSelectedItem());
			m_TreeCtrl.RenameFolder(strpreFileName, strCurFileName);
		}
		else
		{
			strCurFileName = m_TreeCtrl.GetFullPath(m_TreeCtrl.GetSelectedItem()) + _T(".TJ");
			::CFile::Rename(strpreFileName, strCurFileName);
		}
	}

	*pResult = TRUE; // TRUE => Accept the label
}


void CDlgJswz::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	CRect rect1;
	LPPOINT lpoint=new tagPOINT;   
	::GetCursorPos(lpoint); 
	GetDlgItem(IDC_RICHEDIT2_UPDATE)->GetWindowRect(&rect1);
	if (!OpenClipboard())
	{
		return;
	}
	if (rect1.PtInRect(point))
	{
		CMenu* pPopup;
		CMenu popMenu;
		popMenu.LoadMenu(IDR_MENU_EDIT);
		pPopup = popMenu.GetSubMenu(0);

		TCHAR* buffer;
		CString strFromClipboard;
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		buffer = (TCHAR*)GlobalLock(hData);
		strFromClipboard = buffer;
		if (strFromClipboard.IsEmpty())
		{
			pPopup->EnableMenuItem(ID_COPY, MF_ENABLED);
			pPopup->EnableMenuItem(ID_PASTE, MF_GRAYED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_COPY, MF_GRAYED);
			pPopup->EnableMenuItem(ID_PASTE, MF_ENABLED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lpoint->x, lpoint->y, this, NULL);
		pPopup->Detach();
		pPopup->DestroyMenu();
	}
	CloseClipboard();
}


void CDlgJswz::OnCopy()
{
	// TODO: 在此添加命令处理程序代码
	long nStart,nEnd;
	m_EditUpdate.GetSel(nStart, nEnd);
	//m_EditUpdate.Copy();
	if (nStart == nEnd)
	{
		return;
	}
	else
	{
		CString strTemp,strTemp1,strTemp2,strTemp3;
		CString source;
		m_EditUpdate.GetWindowText(strTemp);
		
		int nSize1 = getEnterCode(nStart, strTemp);
		int nSize2 = getEnterCode(nEnd, strTemp);

		strTemp = strTemp.Mid(nStart + nSize1, nEnd+nSize2-nStart-nSize1);
		
		//strTemp = strTemp1 - strTemp2;
		if (OpenClipboard())
		{
			HGLOBAL clipBuffer;
			TCHAR* Buffer;
			EmptyClipboard();
			clipBuffer = GlobalAlloc(GMEM_MOVEABLE, strTemp.GetLength()*2+2);
			Buffer = (TCHAR*)GlobalLock(clipBuffer);
			_tcscpy(Buffer, LPCTSTR(strTemp));
			GlobalUnlock(clipBuffer);
			SetClipboardData(CF_UNICODETEXT, clipBuffer);//unicode编码
			CloseClipboard();
		}
	}
}


void CDlgJswz::OnPaste()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR* buffer = NULL;
	CString strFromClipboard;
	if (OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		buffer = (TCHAR*)GlobalLock(hData);
		strFromClipboard = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
	}
	if (!strFromClipboard.IsEmpty())
	{	
		m_EditUpdate.Paste();
		//粘贴一次后清空数据
		if (OpenClipboard())
		{
			HGLOBAL clipBuffer;
			TCHAR* Buffer;
			EmptyClipboard();
			CloseClipboard();
		}
	}
}

void CDlgJswz::OnMenuAddPath()
{
	CString strItem,strFileName;
	HTREEITEM CurrentItem, hti;

	CurrentItem = m_TreeCtrl.GetSelectedItem();
	
	CString strPath = m_TreeCtrl.GetFullPath(CurrentItem);
	strPath += _T("\\新建文件夹");
	CString strTemp = strPath;
	int nIndex = 1;
	CString strNo;
	while (PathFileExists(strTemp))
	{
		strNo.Format(_T("%d"), nIndex);
		strTemp = strPath + strNo;
		nIndex++;
	}
	strPath = strTemp;
	::CreateDirectory(strPath, NULL);
	
	hti = m_TreeCtrl.AddItem(CurrentItem, strPath);
	m_TreeCtrl.SelectItem(hti);
	m_TreeCtrl.EditLabel(hti);
}

int CDlgJswz::getEnterCode(int nFlag, CString strText)
{
	int nCout = 0;
	CString strTemp;
	int i=0;
	int nFind = 0;
	while (1)
	{
		nFind = strText.Find(_T("\n"));
		if (nFind < 0)
		{
			break;
		}
		if (nFind > nFlag)
		{
			break;
		}
		nFlag -= nFind;
		strTemp = strText.Left(nFind);
		if (strTemp.GetLength() == 1)
		{
			strTemp.Trim();
			if (strTemp.IsEmpty())
			{
				strText = strText.Right(strText.GetLength()-nFind-1);
				continue;
			}
		}
		strText = strText.Right(strText.GetLength()-nFind-1);
		i++;
	} ;

	return i;
}

void CDlgJswz::displayText()
{
	CString strFile;
	m_EditUpdate.SetWindowText(_T(""));//置空
	for (map<CString, int>::iterator iTer = m_DataMap.begin(); iTer != m_DataMap.end(); ++iTer)
	{
		vector<CString> tmpVec;
		strFile = iTer->first;
		tmpVec = readStringFromFile(strFile);
		m_EditUpdate.SetTargetDevice(NULL,0);
		int i=1;
		for (vector<CString>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
		{
			m_EditUpdate.SetSel(-1, -1);
			m_EditUpdate.ReplaceSel(*iter);
			m_EditUpdate.SetSel(-1, -1);
			m_EditUpdate.ReplaceSel(_T("\n"));
		}
		tmpVec.clear();
		m_EditUpdate.ShowScrollBar(SB_VERT,FALSE);
		int nFirstVisible = m_EditUpdate.GetFirstVisibleLine();
		if (nFirstVisible > 0)
		{
			m_EditUpdate.LineScroll(-nFirstVisible, 0);
		}
	}
}

void CDlgJswz::OnJsSave()
{
	// TODO: Add your command handler code here
	CString strFile,strTemp;
	GetDlgItem(IDC_RICHEDIT2_UPDATE)->GetWindowText(strFile);
	if (strFile.IsEmpty())
	{
		AfxMessageBox(_T("编辑框里没有数据"));
		return;
	}

	HTREEITEM CurrentItem;
	CurrentItem = m_TreeCtrl.GetSelectedItem();
	CString strCurrentPath = m_TreeCtrl.GetFullPath(CurrentItem) + _T(".TJ");

	//写入文件
	CStdioFile File;
	if(!File.Open(strCurrentPath,CFile::modeReadWrite))     
	{     
		if (!File.Open(strCurrentPath, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
		{
			return;
		} 
	}    	
	
	strTemp = strFile;
	int nFind = 0;
	while (1)
	{
		nFind = strFile.Find(_T("\n"));
		if (nFind < 0)
		{
			if (!strTemp.IsEmpty())
			{
				//File.SeekToEnd();
				File.WriteString(strTemp);
			}
			
			break;
		}
		strTemp = strFile.Left(nFind);
		if (strTemp.GetLength() == 1)
		{
			strTemp.Trim();
			if (strTemp.IsEmpty())
			{
				strFile = strFile.Right(strFile.GetLength()-nFind-1);
				//File.SeekToEnd();
				continue;
			}
		}
		//File.SeekToEnd();
		File.WriteString(strTemp);
		strFile = strFile.Right(strFile.GetLength()-nFind-1);
	}
	
	File.Close();
}

CString CDlgJswz::getNewFileName(CString strPath)
{
	CString strTemp = strPath.Left(strPath.ReverseFind('\\'));
	int nValue;
	MyBaseUtils::GetVar(_T("USERI2"), &nValue);
	nValue++;

	
	CString strFileName;
	strFileName.Format(_T("%s\\新技术文字%d.TJ"), strTemp, nValue);

	while(gGlobal.IsFileExist(strFileName))
	{
		nValue++;
		strFileName.Format(_T("%s\\新技术文字%d.TJ"), strTemp, nValue);
	}
	MyBaseUtils::SetVar(_T("USERI2"), nValue);
	return strFileName;
}

