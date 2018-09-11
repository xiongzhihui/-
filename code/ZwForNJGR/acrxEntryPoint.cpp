//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgTKSZ.h"
#include "MyExcel.h"
#include "CBaseInfoInDwgFile.h"
#include "DlgInitSetting.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")


#include "CAcadApplication.h"
#include "CAcadDocument.h"
#include "CAcadModelSpace.h"
#include "CAcadMenuBar.h"
#include "CAcadMenuGroup.h"
#include "CAcadMenuGroups.h"
#include "CAcadPopupMenu.h"
#include "CAcadPopupMenus.h"
#include "CDrawDotted.h"
#include "Utility.h"
#include "BaseUtils.h"
#include "DlgProress.h"
#include <shlwapi.h>
#include "DlgJswz.h"
#include "DlgExcelFileCmp.h"
#include "CBaseDocReactor.h"
#include "DlgSaveBM.h"
#include "CGlobalQuery.h"
#include "CGlobalRWriteFile.h"
#include "DlgEidtFrame.h"
#include "DlgCreateZuozi.h"
#include "CBaseDbReactor.h"
#include "DlgCreateTouzi.h"
#include "CGenerateJxb.h"
#include "DlgDrawResistance.h"
#include "CDrawLjx.h"
#include "CDrawOutLine.h"
#include "CQuickFindQuxiang.h"
#include "DlgEditTexing.h"
#include "DlgEditExcel.h"

extern CGlobalQuery gQuery;
CGlobalRWriteFile gRWriteFile;

CBaseDbReactor* g_baseDb = NULL;
//attach all
void attachDbReactorToAll()
{
	if (g_baseDb == NULL) 
	{
		g_baseDb = new CBaseDbReactor;
		AcDbDatabase* tmpDb;
		AcDbVoidPtrArray dbPtrs;
		MyBaseUtils::getAllDatabases(dbPtrs);

		//acutPrintf(_T("\nAttaching Database Reactor to all active databases..."));
		int len = dbPtrs.length();
		for (int i=0; i<len; i++) {
			tmpDb = static_cast<AcDbDatabase*>(dbPtrs[i]);
			tmpDb->addReactor(g_baseDb);
		}
	}
}
//Remove DbReactor
void removeDbReactorFromAll()
{
	if (g_baseDb != NULL) 
	{
		AcDbDatabase* tmpDb;
		AcDbVoidPtrArray dbPtrs;
		MyBaseUtils::getAllDatabases(dbPtrs);

		acutPrintf(_T("\nRemoving Database Reactor from all active databases..."));

		int len = dbPtrs.length();
		for (int i=0; i<len; i++) {
			tmpDb = static_cast<AcDbDatabase*>(dbPtrs[i]);
			tmpDb->removeReactor(g_baseDb);
		}
		if (g_baseDb != NULL)
		{
			delete g_baseDb;
			g_baseDb = NULL;
		}
	}
}
//加载cui文件
bool LoadPartialMenu(const TCHAR* filePath   //局部菜单文件名  
					 ,const TCHAR* menuGroupName  )

{
	//如果有的话先卸载

	long i, menuGroupNum;   //菜单组数目

	VARIANT index;

	VariantInit(&index);

	index.vt = VT_I4;

	CString strGroupName(menuGroupName);

	CAcadApplication acadApp(acedGetAcadWinApp()->GetIDispatch(TRUE));

	CAcadMenuGroups menuGroups(acadApp.get_MenuGroups());

	CAcadMenuGroup menuGroup;

	menuGroupNum = menuGroups.get_Count();

	for (i = 0; i < menuGroupNum; i++) {

		index.lVal = i;

		menuGroup = menuGroups.Item(index);

		if (menuGroup.get_Name() == strGroupName) {

			menuGroup.Unload();

			break;
			//return false;

		}

	}

	//加载菜单

	VARIANT BaseMenu;   //是否加载为基础菜单

	VariantInit(&BaseMenu);

	BaseMenu.vt = VT_BOOL;

	BaseMenu.boolVal = FALSE;

	menuGroup = menuGroups.Load(CString(filePath), BaseMenu);
	// 把菜单在菜单条上显示出来

	CAcadMenuBar menuBar(acadApp.get_MenuBar());  //当前菜单条

	CAcadPopupMenus popupMenus(menuGroup.get_Menus()); //要加入的菜单条

	CAcadPopupMenu popupMenu;

	long curPopupMenuNum = menuBar.get_Count();   //当前菜单条上菜单的数目

	long n = popupMenus.get_Count();

	for (long i = 0; i < n; i++) {

		index.lVal = i;

		popupMenu = popupMenus.Item(index);

		index.lVal = i + curPopupMenuNum;

		popupMenu.InsertInMenuBar(index);

	}

	return true;

}
static void initLoadMenu()
{
	TCHAR buf[1000];
	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("ZwForNJGR.ARX")),buf,1000);

	if(pathLength)
	{
		CString path;
		path = buf;
		path=path.Left(path.ReverseFind('\\'));
		if(path!=_T(""))
		{
			path += _T("\\ICD.cuix");
		}
		LoadPartialMenu(path, _T("ICD"));
	}
}
CDlgProress *modelessDlg  = NULL;
Adesk::Boolean startDlg()
{
	CAcModuleResourceOverride resOverride;
	HWND hwndAcad = adsw_acadMainWnd();

	if ( !hwndAcad ) 
	{
		return Adesk::kFalse;
	}

	CWnd *pWnd = CWnd::FromHandle ( hwndAcad );
	if( modelessDlg == NULL ) 
	{
		if ( (modelessDlg = new CDlgProress ( pWnd )) == NULL ) 
		{
			return Adesk::kFalse;
		}

		BOOL succeeded = modelessDlg->Create ( pWnd );
		if ( !succeeded ) 
		{
			return Adesk::kFalse;
		}
		//gpDocToModReactor =new AsdkDocToModReactor();
	}
	modelessDlg->ShowWindow(SW_SHOWNORMAL);

	return Adesk::kTrue;
}
Adesk::Boolean endDlg()
{
	if ( !modelessDlg )
		return Adesk::kTrue;
	
	//modelessDlg->clear();
	BOOL ok = modelessDlg->DestroyWindow();
	delete modelessDlg;
	modelessDlg = NULL;

	/*delete gpDocToModReactor;
	gpDocToModReactor = NULL;*/
	return ok;
}
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
CBaseDocReactor* g_baseDoc = NULL;
class CZwForNJGRApp : public AcRxArxApp {

public:
	CZwForNJGRApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		gGlobal.SetIni(gGlobal.GetIniPath());
		CBaseInfoInDwgFile::rxInit();
		acrxBuildClassHierarchy();	
#ifndef ZRXSDK
		initLoadMenu();
#endif
#ifndef ARX2014
		acedCommand(RTSTR, _T("WSCURRENT"), RTSTR, _T("AutoCAD 经典"), RTNONE);
#endif
		MyBaseUtils::SetVar(_T("WHIPTHREAD"), 3);

		g_baseDoc = new CBaseDocReactor;
		g_baseDoc->Attach();

		attachDbReactorToAll();
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		g_baseDoc->Detach();
		if (g_baseDoc != NULL)
		{
			delete g_baseDoc;
			g_baseDoc = NULL;
		}
		deleteAcRxClass(CBaseInfoInDwgFile::desc());
		removeDbReactorFromAll();
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// - ZwForNJGR._TK command (do not rename)
	static void ZwForNJGR_TK(void)//图框
	{
		// Add your code for command ZwForNJGR._TK here
		gRWriteFile.clear();
		CAcModuleResourceOverride rs;
		//读取xml文件
		gRWriteFile.readFrameIniFile();

		CDlgTKSZ dlg;
		if (dlg.DoModal() == IDOK)
		{
			//将所有excel进程关闭
			CString strFile = CUtility::getLogPath() + _T("\\error.csv");
			if (gGlobal.IsFileExist(strFile))
			{
				::DeleteFile(strFile);
			}
			//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
			//////////////////////////////////////////////////////////////////////////

			CMyExcel myExcel;
			BOOL bRet = myExcel.Open(dlg.getFile());
			if (!bRet)
			{
				AfxMessageBox(_T("文件不能打开，请检查路径是否有效"));
				return;
			}

			//在初始化时，将数据置空
			gQuery.clear();
			CBaseUtils::setFrameCount(1);
			CBaseUtils::setRfCount(1);
			CBaseUtils::setDpCount(1);
			startDlg();
			Sleep(100);
			if (modelessDlg != NULL)
			{
				modelessDlg->SetWindowText(_T("自动校验"));
			}

			int nCount = myExcel.GetSheetCount();
			for (int i=1; i<=nCount; i++)
			{
				bRet = myExcel.OpenSheet(i);
				myExcel.GetArea();
				myExcel.readSpecailCol();	
				if (modelessDlg != NULL)
				{
					modelessDlg->setPos(i*50/nCount);
					Sleep(50);
				}

			}
			myExcel.Clear();

			if (!CUtility::verifyOther())
			{
				endDlg();
			}
			else
				endDlg();

			//如果有错误文件，打开该文件
			if (gGlobal.IsFileExist(strFile))
			{
				//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
				ShellExecute(NULL,_T("open"),strFile,NULL,NULL,SW_SHOWNORMAL );
				return;
			}
			else
			{
				int nRet = MessageBox(NULL, _T("Excel通过校验!\n是否生成电气特性连接表!"), _T("自动生成"), MB_YESNO|MB_DEFBUTTON1);
				if (nRet != IDYES)
				{
					return;
				}
				MyEditEntity::EraseAllEnt();
				//////////////////////////////////////////////////////////////////////////
				CBaseUtils::setStartPt(AcGePoint3d(0, 0, 0));
			}
			startDlg();
			Sleep(100);
			if (modelessDlg != NULL)
			{
				modelessDlg->SetWindowText(_T("电气特性连接表"));
			}

			CUtility::doIt();
			//myExcel.writeFile();
			if (modelessDlg != NULL)
			{
				modelessDlg->setPos(90);
				Sleep(50);
			}
			CUtility::UpdatePageNo();
			if (modelessDlg != NULL)
			{
				modelessDlg->setPos(95);
				Sleep(50);
			}
			CBaseUtils::setJxbFlag(true);
			MyBaseUtils::flushGraphics(acdbCurDwg());
			if (modelessDlg != NULL)
			{
				modelessDlg->setPos(100);
				Sleep(50);
			}
			endDlg();
			gRWriteFile.clear();//清理掉xml文件信息

#ifndef DEBUG
			CGenerateJxb jxb;
			jxb.writeJxb();
			jxb.writeJxbgxb();

			CDrawDotted dot;
			dot.doLayer();//模块虚线框
			dot.doAlayer();//母板虚线框
#endif	
			//删除临时文件，防止该文件阻止文件保存
			::DeleteFile(dlg.getFile());
		}
	}

	// - ZwForNJGR._Test command (do not rename)
	static void ZwForNJGR_Test(void)
	{
	//	// Add your code for command ZwForNJGR._Test here
		AcGePoint3d pt;
		int nRet = acedGetPoint(NULL, _T("\n请选择点："), asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		int nRow,nCol, nCount;
		nCount = CUtility::getRowAndColByPt(pt, nRow, nCol);
		acutPrintf(_T("\nCount:---%d;Row:%d---Col:%d"), nCount, nRow, nCol);
		/*ads_name ssname;
		int nRet = acedSSGet(NULL, NULL, NULL, NULL, ssname);
		if (nRet != RTNORM)
		{
		return;
		}

		long sslen;
		acedSSLength(ssname, &sslen);
		ads_name ename;
		AcDbObjectId objId = AcDbObjectId::kNull;
		AcDbObjectIdArray objIdArrs;
		objIdArrs.removeAll();
		for (int i=0; i<sslen; i++)
		{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		objIdArrs.append(objId);
		}

		MyDrawEntity::MakeGroup(objIdArrs, false, _T("abcd"));
		acedSSFree(ssname);*/
	}

	// - ZwForNJGR._InitDlg command (do not rename)
	static void ZwForNJGR_InitDlg(void)//初始数据设置
	{
		// Add your code for command ZwForNJGR._InitDlg here
		CAcModuleResourceOverride rs;
		CDlgInitSetting dlg;
		dlg.DoModal();
	}

	// - ZwForNJGR._CLOSEEXCEL command (do not rename)
	static void ZwForNJGR_CLOSEEXCEL(void)
	{
		// Add your code for command ZwForNJGR._CLOSEEXCEL here
		MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
	}

	// - ZwForNJGR._ADDXXK command (do not rename)
	static void ZwForNJGR_ADDXXK(void)
	{
		// Add your code for command ZwForNJGR._ADDXXK here
		CDrawDotted dot;
		dot.doIt();
	}

	// - ZwForNJGR._ADDLAYERXXK command (do not rename)
	static void ZwForNJGR_ADDLAYERXXK(void)
	{
		// Add your code for command ZwForNJGR._ADDLAYERXXK here
		CDrawDotted dot;
		dot.doLayer();
	}
	// - ZwForNJGR._ADDMBLAYER command (do not rename)
	static void ZwForNJGR_ADDMBLAYER(void)
	{
		// Add your code for command ZwForNJGR._ADDMBLAYER here
		CDrawDotted dot;
		dot.doAlayer();
	}

	// - ZwForNJGR._SETLENGTH command (do not rename)
	static void ZwForNJGR_SETLENGTH(void)
	{
		// Add your code for command ZwForNJGR._SETLENGTH here
		int nCount;
		int nRet = acedGetInt(_T("\n请输入字符限制长度："), &nCount);
		if (nRet != RTNORM)
		{
			return;
		}
		gGlobal.SetIniValue(_T("验证设置"), _T("字符长度"), nCount);
		gGlobal.WriteIniFile();
	}

	// - ZwForNJGR._SCJXB command (do not rename)
	static void ZwForNJGR_SCJXB(void)
	{
		// Add your code for command ZwForNJGR._SCJXB here
		//MyBaseUtils::GetVar(_T("USERI1"),&nCount);//记录count值
		bool bJxb = CBaseUtils::jxbFlag();
		if (!bJxb)
		{
			AfxMessageBox(_T("图纸还没有验证通过(或者图纸重新打开），请用电气特性连接表命令验证数据"));
			return;
		}
	
		CGenerateJxb jxb;
		jxb.writeJxb();
	}

	// - ZwForNJGR._SAVEFILE command (do not rename)
	static void ZwForNJGR_SAVEFILE(void)//保存文件
	{
		// Add your code for command ZwForNJGR._SAVEFILE here
		CAcModuleResourceOverride rs;
		CDlgSaveBM dlg;
		if (dlg.DoModal() != IDOK)
		{
			return;
		}
		
		CString strPath = CUtility::getBmPath();
		if (!PathIsDirectory(strPath))  
		{  
			::CreateDirectory(strPath, 0);  
		}  
		
		CString strData = CUtility::getDate();
		CString strBmNo = gGlobal.GetIniValue(_T("图框设置"), _T("BM"));
		strPath = strPath + strBmNo + _T("\\");
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, 0);
		}
		
		strPath = strPath + strData + _T("\\");
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, 0);
		}
		CString strExcelFile = CBaseUtils::filePath();
		CString strTemp = CUtility::getFileNameByFullName(strExcelFile);

		CString strFileName;
		strFileName = strPath + strTemp;
		::CopyFile(strExcelFile, strFileName, FALSE);

		//保存cad图纸
		strFileName = strPath + strBmNo + _T(".dwg");
		AcDbDatabase* pDb = acdbCurDwg();
		Acad::ErrorStatus es;
		es = pDb->saveAs(strFileName);
		if (es != Acad::eOk)
		{
			acutPrintf(_T("\n错误：%d"), es);
		}
	}

	static void ZwForNJGR_JSWZ(void)//技术文字
	{
		CAcModuleResourceOverride rs;
		CDlgJswz dlg;
		dlg.DoModal();
	}

	static void ZwForNJGR_CMP(void)//文件比对
	{
		CString strPath = MyBaseUtils::GetAppPath() + _T("GWCompareExcelContent.exe");
		ShellExecute(NULL, _T("Open"), strPath,NULL,NULL,SW_SHOWNORMAL );
	}

	// - ZwForNJGR._ABOUT command (do not rename)
	static void ZwForNJGR_DESCRIPTION(void)//版本号
	{
		// Add your code for command ZwForNJGR._ABOUT here
		CString strPrompt,strTime;
		CString strFile;
		TCHAR buf[1000];
		GetModuleFileName(GetModuleHandle(_T("ZwForNJGR.ARX")),buf,1000);
		strFile = buf;
		HANDLE hFile = CreateFile(strFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
		SYSTEMTIME stLocal;
		FILETIME ftCreate, ftModify, ftAccess;
		if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
		{
			return;
		}	
		CloseHandle(hFile);		

		FileTimeToSystemTime(&ftModify, &stLocal);
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour+8, stLocal.wMinute, stLocal.wSecond);
		#ifdef _WIN64
				strPrompt.Format(_T("\nvernum = %s_x64(只读)"), strTime);
		#else
				strPrompt.Format(_T("\nvernum = %s_x32(只读)"), strTime);
		#endif
		
		acutPrintf(strPrompt);
	}
	// - ZwForNJGR._SETMUBAN command (do not rename)
	static void ZwForNJGR_SETMUBAN(void)//设置母板名称
	{
		// Add your code for command ZwForNJGR._SETMUBAN here
		CString strTemp = gGlobal.GetIniValue(_T("基础设置"), _T("母板"));
		acutPrintf(_T("\n现在%s都可以是母版"), strTemp);
		acutPrintf(_T("\n母板名称可以多个，多个用逗号\"、\"隔开,"));
		TCHAR pName[256];
		int nRet = acedGetString(0, _T("\n请输入母板名称:"), pName);
		if (nRet != RTNORM)
		{
			return;
		}
		CString strName = pName;
		gGlobal.SetIniValue(_T("基础设置"), _T("母板"), strName);
		gGlobal.WriteIniFile();
	}
	// - ZwForNJGR._QXQZ command (do not rename)
	static void ZwForNJGR_QZ(void)//取消群组功能
	{
		// Add your code for command ZwForNJGR._QXQZ here
		AcDbObjectId objId = AcDbObjectId::kNull;
		ads_name ename;
		AcGePoint3d pt;
		int nRet = acedEntSel(_T("\n请选择一个实体..."), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(objId, ename);

		CString strGroupName(_T(""));
		AcDbObjectIdArray objIdArrs;
		objIdArrs.removeAll();
		objIdArrs = MyEditEntity::openObjAndGetGroupIds(objId);
		if ((objIdArrs.length() <=0) || (objIdArrs.length() > 1))
		{
			return;
		}
		AcDbObjectId groupId;
		groupId = objIdArrs.at(0);
		bool bIsSel = false;
		AcDbGroup* pGroup = NULL;
		if (acdbOpenAcDbObject((AcDbObject*&)pGroup, groupId, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		bIsSel = pGroup->isSelectable();
		pGroup->setSelectable(!bIsSel);
		pGroup->close();
	}

	// - ZwForNJGR._XGTK command (do not rename)
	static void ZwForNJGR_XGTK(void)//修改图框
	{
		// Add your code for command ZwForNJGR._XGTK here
		CAcModuleResourceOverride rs;
		CDlgEidtFrame dlg;
		if (dlg.sel())
		{
			dlg.DoModal();
		}
		
	}
	// - ZwForNJGR._XGTK command (do not rename)
	static void ZwForNJGR_SCJXLJGXB(void)//生成接线连接关系表
	{
		// Add your code for command ZwForNJGR._XGTK here
		bool bJxb = CBaseUtils::jxbFlag();
		if (!bJxb)
		{
			AfxMessageBox(_T("图纸还没有验证通过(或者图纸重新打开），请用电气特性连接表命令验证数据"));
			return;
		}
		CGenerateJxb jxb;
		jxb.writeJxbgxb();
	}
	// - ZwForNJGR._XGTK command (do not rename)
	static void ZwForNJGR_SCZZ(void)//生成座子
	{
		// Add your code for command ZwForNJGR._XGTK here
		CAcModuleResourceOverride rs;
		CDlgCreateZuozi dlg;
		if (dlg.DoModal() == IDOK)
		{
			//将所有excel进程关闭
			CString strFile = CUtility::getLogPath() + _T("\\error.csv");
			if (gGlobal.IsFileExist(strFile))
			{
				::DeleteFile(strFile);
			}
			//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
			//////////////////////////////////////////////////////////////////////////

			CMyExcel myExcel;
			BOOL bRet = myExcel.Open(dlg.getFile());
			if (!bRet)
			{
				AfxMessageBox(_T("文件不能打开，请检查路径是否有效"));
				return;
			}

			int nCount = myExcel.GetSheetCount();
			for (int i=1; i<=nCount; i++)
			{
				bRet = myExcel.OpenSheet(i);
				myExcel.GetArea();
				myExcel.readLrmvpCol();	
			}
			//如果有错误文件，打开该文件
			if (gGlobal.IsFileExist(strFile))
			{
				myExcel.Clear();
				//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
				ShellExecute(NULL,_T("open"),strFile,NULL,NULL,SW_SHOWNORMAL );
				return;
			}
	
			myExcel.writeLrmvFile();
			myExcel.Clear();
			::DeleteFile(strFile);
		}
	}

	// - ZwForNJGR._XGTK command (do not rename)
	static void ZwForNJGR_SVXML(void)//增加xml文件
	{
		// Add your code for command ZwForNJGR._XGTK here
		CString strValue;
		CFrameInfo frameInfo;
		TCHAR pName[256];
		int nRet = acedGetString(0, _T("\n请输入lrm字符"), pName);
		if (nRet != RTNORM)
		{
			return;
		}
		strValue = pName;
		
		frameInfo.setFrameType(strValue);
		double dValue;
		nRet = acedGetReal(_T("\n请输入表格横向间距"), &dValue);
		if (nRet != RTNORM)
		{
			return;
		}
		frameInfo.setHorSpace(dValue);

		nRet = acedGetReal(_T("\n请输入表格纵向间距"), &dValue);
		if (nRet != RTNORM)
		{
			return;
		}
		frameInfo.setVerSpace(dValue);

		nRet = acedGetReal(_T("\n请输入linelength"), &dValue);
		if (nRet != RTNORM)
		{
			return;
		}
		frameInfo.setLineLength(dValue);

		nRet = acedGetReal(_T("\n请输入备用横向长"), &dValue);
		if (nRet != RTNORM)
		{
			return;
		}
		frameInfo.setSpaceHorGap(dValue);

		nRet = acedGetReal(_T("\n请输入备用纵向长"), &dValue);
		if (nRet != RTNORM)
		{
			return;
		}
		frameInfo.setSpaceVerGap(dValue);

		nRet = acedGetReal(_T("\n请输入左上横向长"), &dValue);
		if (nRet != RTNORM)
		{
			return;
		}
		frameInfo.setLeftVerGap(dValue);

		nRet = acedGetString(0, _T("\n请输入图框类型"), pName);
		if (nRet != RTNORM)
		{
			return;
		}
		CString strTemp = pName;
		frameInfo.setFrameType(strTemp);
		gRWriteFile.addFrameIniFile(strValue, frameInfo);
	}

	static void ZwForNJGR_SCTZ(void)//生成座子
	{
		// Add your code for command ZwForNJGR._XGTK here
		CAcModuleResourceOverride rs;
		CDlgCreateTouzi dlg;
		if (dlg.DoModal() == IDOK)
		{
			//将所有excel进程关闭
			CString strFile = CUtility::getLogPath() + _T("\\error.csv");
			if (gGlobal.IsFileExist(strFile))
			{
				::DeleteFile(strFile);
			}
			//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
			//////////////////////////////////////////////////////////////////////////

			CMyExcel myExcel;
			BOOL bRet = myExcel.Open(dlg.getFile());
			if (!bRet)
			{
				AfxMessageBox(_T("文件不能打开，请检查路径是否有效"));
				return;
			}

			int nCount = myExcel.GetSheetCount();
			for (int i=1; i<=nCount; i++)
			{
				bRet = myExcel.OpenSheet(i);
				myExcel.GetArea();
				myExcel.readLrmvCol();	
			}
			//如果有错误文件，打开该文件
			if (gGlobal.IsFileExist(strFile))
			{
				myExcel.Clear();
				//MyBaseUtils::CloseProcess(_T("EXCEL.EXE"));
				ShellExecute(NULL,_T("open"),strFile,NULL,NULL,SW_SHOWNORMAL );
				return;
			}

			myExcel.writeFile();
			myExcel.Clear();
			::DeleteFile(strFile);
		}
	}

	// - ZwForNJGR._HYYS command (do not rename)
	//还原颜色
	static void ZwForNJGR_HYYS(void)
	{
		// Add your code for command ZwForNJGR._HYYS here
		int nColor = gGlobal.GetIniValue(_T("验证设置"), _T("颜色"), 0);
		resbuf* filter = acutBuildList(-4,_T("<and"), 62, nColor, -4,_T("and>"), RTNONE);
		ads_name ssname;
		int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
		acutRelRb(filter);
		if (nRet != RTNORM)
		{
			return;
		}
		long sslen;
		acedSSLength(ssname, &sslen);
		if (sslen < 1)
		{
			return;
		}
		ads_name ename;
		AcDbObjectId objId = AcDbObjectId::kNull;
		AcDbEntity* pEnt = NULL;
		for (int i=0; i<sslen; i++)
		{
			acedSSName(ssname, i, ename);
			acdbGetObjectId(objId, ename);
			if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite) != Acad::eOk)
			{
				continue;
			}
			pEnt->setColorIndex(256);//bylayer
			pEnt->close();
		}
		acedSSFree(ssname);
	}

	//图形校验
	static void ZwForNJGR_TXJY(void)
	{
		if (CBaseUtils::jxbFlag())
		{
			CString strFile = CUtility::getFilePath() + _T("info.csv");
			if (gGlobal.IsFileExist(strFile))
			{
				::DeleteFile(strFile);
			}
			gQuery.writeCellInfoToFile();
		}
		else
		{
			AfxMessageBox(_T("先通过电气特性连接表命令生成CAD图形"));
		}
	}

	//接地电阻
	static void ZwForNJGR_JDDZ(void)
	{
		CAcModuleResourceOverride rs;
		CDlgDrawResistance dlg;
		dlg.DoModal();
	}
	//总线连接线
	static void ZwForNJGR_ZXLJX(void)
	{
		CDrawLjx ljx;
		ljx.doIt();
	}

	// - ZwForNJGR.WK command (do not rename)
	//外框是否显示
	static void ZwForNJGRWK(void)
	{
		// Add your code for command ZwForNJGR.WK here
		CDrawOutLine outLine;
		outLine.doIt();
	}
	// - ZwForNJGR.WK command (do not rename)
	//方便修改
	static void ZwForNJGRFBXG(void)
	{
		// Add your code for command ZwForNJGR.WK here
		CQuickFindQuxiang qcf;
		qcf.doIt();
	}

	static void ZwForNJGR_EDITTX(void)
	{
		ads_name entname;
		ads_point pt;
		if (acedEntSel(_T("\n选择需要修改的特性信息,请选择边框图块:"), entname, pt) != RTNORM)
		{
			return;
		}
		AcDbObjectId entid;
		acdbGetObjectId(entid, entname);
		Acad::ErrorStatus es;
		AcDbEntity *pEnt=NULL;
		es = acdbOpenAcDbEntity(pEnt, entid, AcDb::kForWrite);
		if (es!=eOk)
		{
			return;
		}
		if (!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			acutPrintf(_T("\n所选的图形不是图块!"));
			pEnt->close();
			return;
		}
		pEnt->close();
		int nType, nRow, nCol;
		CString sRow, sCol;
		CString sheetName, duanzi, texing, quxiang, xianhao;
		MyBaseAppXdata::getXdata(ICD_XDATA_TYPE, nType, entid);
		MyBaseAppXdata::getXdata(ICD_XDATA_SHEET, sheetName, entid);
		MyBaseAppXdata::getXdata(ICD_XDATA_ROW, nRow, entid);
		MyBaseAppXdata::getXdata(ICD_XDATA_COL, nCol, entid);
		MyBaseAppXdata::getXdata(ICD_XDATA_DUANZI, duanzi, entid);
		MyBaseAppXdata::getXdata(ICD_XDATA_TEXING, texing, entid);
		MyBaseAppXdata::getXdata(ICD_XDATA_QUXIANG, quxiang, entid);
		MyBaseAppXdata::getXdata(ICD_XDATA_XIANHAO, xianhao, entid);
		sRow.Format(_T("%d"), nRow);
		sCol.Format(_T("%d"), nCol);
		CAcModuleResourceOverride rs;
		CDlgEditTexing dlg(sRow, sCol, sheetName, nType, duanzi, xianhao, quxiang, texing, entid, acedGetAcadFrame());
		dlg.DoModal();
	}

	static void ZwForNJGR_REFRESHEXCEL(void)
	{
		CAcModuleResourceOverride rs;
		CDlgEditExcel dlg(true);
		dlg.DoModal();
	}

	static void ZwForNJGR_REFRESHDWG()
	{
		CAcModuleResourceOverride rs;
		CDlgEditExcel dlg(false);
		dlg.DoModal();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CZwForNJGRApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _TK, TK, ACRX_CMD_TRANSPARENT, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _Test, Test, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _InitDlg, INIT, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _ADDXXK, XXK, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _ADDLAYERXXK, TCXXK, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _ADDMBLAYER, MBXXK, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SETLENGTH, SLEN, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SCJXB, SCJXB, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SAVEFILE, SFILE, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _JSWZ, JSWZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _CMP, CMP, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _DESCRIPTION, VERNUM, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SETMUBAN, SZMB, ACRX_CMD_TRANSPARENT, NULL)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _QZ, QZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _CLOSEEXCEL, CEXCEL, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _XGTK, XGTK, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SCJXLJGXB, SCJXLJB, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SCZZ, SCZZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SVXML, SVXML, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _SCTZ, SCTZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _HYYS, HYYS, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _TXJY, TXJY, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _JDDZ, JDDZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _ZXLJX, ZXLJX, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, WK, WK, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, FBXG, FBXG, ACRX_CMD_TRANSPARENT, NULL)

//修改特性 刷新特性值
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _EDITTX, EDITTX, ACRX_CMD_TRANSPARENT, NULL)
//反向映射到excel信息
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _REFRESHEXCEL, REFRESHEXCEL, ACRX_CMD_TRANSPARENT, NULL)
//通过excel局部刷新特性
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForNJGRApp, ZwForNJGR, _REFRESHDWG, REFRESHDWG, ACRX_CMD_TRANSPARENT, NULL)