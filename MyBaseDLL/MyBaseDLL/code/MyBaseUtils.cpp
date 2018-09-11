#include "StdAfx.h"
#include "MyBaseUtils.h"
#include <Windows.h>
#include <TlHelp32.h>

MyBaseUtils::MyBaseUtils(void)
{
}

MyBaseUtils::~MyBaseUtils(void)
{
}


const TCHAR* MyBaseUtils::rxErrorStr(Acad::ErrorStatus msgId)
{
	return acadErrorStatusText(msgId);
}

void MyBaseUtils::rxErrorMsg(Acad::ErrorStatus msgId)
{
	if (msgId != Acad::eOk)
		acutPrintf(_T("\nARX ERROR: %s"), acadErrorStatusText(msgId));
}

/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::SetVar(LPCTSTR szVName, ads_real rArg)
{
	struct resbuf res;
	res.restype = RTREAL;
	res.resval.rreal   = rArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::SetVar(LPCTSTR szVName, ads_point pArg)
{
	struct resbuf res;
	res.restype = RT3DPOINT;
	res.resval.rpoint[0] = pArg[0];
	res.resval.rpoint[1] = pArg[1];
	res.resval.rpoint[2] = pArg[2];
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::SetVar(LPCTSTR szVName, int nArg)
{
	struct resbuf res;
	res.restype = RTSHORT;
	res.resval.rint  = nArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::SetVar(LPCTSTR szVName, LPCTSTR szArg)
{
	ASSERT(AfxIsValidString(szArg));
	struct resbuf res;
	res.restype = RTSTR;
	res.resval.rstring =(LPTSTR) szArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::GetVar(LPCTSTR var, ads_real *rRes)
{
	ASSERT(rRes!=NULL);
	struct resbuf v;
	if(ads_getvar(var, &v)!=RTNORM)
		return FALSE;
	if (v.restype != RTREAL) 
		return FALSE;
	*rRes = v.resval.rreal;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::GetVar(LPCTSTR var, ads_point *pRes)
{
	ASSERT(pRes != NULL);
	struct resbuf v;
	if(ads_getvar(var, &v) !=RTNORM)
		return FALSE;
	if (v.restype != RT3DPOINT && v.restype != RTPOINT)
		return FALSE;
	ads_point_set(v.resval.rpoint, *pRes);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::GetVar(LPCTSTR var, int *nRes)
{
	ASSERT(nRes!=NULL);
	struct resbuf v;
	if(ads_getvar(var, &v)!=RTNORM)
		return FALSE;
	if (v.restype!=RTSHORT ) 
		return FALSE;
	*nRes = v.resval.rint;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
BOOL MyBaseUtils::GetVar(LPCTSTR var, LPTSTR sRes)
{
	struct resbuf v;
	if(ads_getvar(var, &v)!=RTNORM || v.restype!=RTSTR ) 
		return FALSE;
	_tcscpy(sRes, v.resval.rstring);
	//由于没有加下面这句造成2byte的内存泄露05-10-1
	ads_free (v.resval.rstring);	
	//#endif
	return TRUE;
}

void
MyBaseUtils::alertBox(LPCTSTR msg)
{
	AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION);
}

/****************************************************************************
**
**  ArxDbgUtils::stopAlertBox
**
**  
**
*************************************/

void
MyBaseUtils::stopAlertBox(LPCTSTR msg)
{
	AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
}

/****************************************************************************
**
**  ArxDbgUtils::infoAlertBox
**
**  
**
*************************************/

void
MyBaseUtils::infoAlertBox(LPCTSTR msg)
{
	AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
}

/****************************************************************************
**
**  ArxDbgUtils::stubAlert
**
**  
**
*************************************/

void
MyBaseUtils::stubAlert(LPCTSTR msg)
{
	CString str;
	str.Format(_T("Not Implemented Yet: %s"), msg);
	AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
}

/****************************************************************************
**
**  ArxDbgUtils::yesNoAlert
**      use when you have a yes/no question.  Pass in the default
**  button(which is No by default).
**
**  
**
*************************************/

UINT
MyBaseUtils::yesNoAlert(LPCTSTR msg, UINT defButton /*=no*/)
{
	UINT theDefault =(defButton == IDYES)? MB_DEFBUTTON1 : MB_DEFBUTTON2;
	return(AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | theDefault));
}

/****************************************************************************
**
**  ArxDbgUtils::yesNoCancelAlert
**      use when you have a yes/no/cancel question.  Pass in the default
**  button(which is Cancel by default).
**
**  
**
*************************************/

UINT
MyBaseUtils::yesNoCancelAlert(LPCTSTR msg, UINT defButton /*=cancel*/)
{
	UINT theDefault;

	if (defButton == IDYES)
		theDefault = MB_DEFBUTTON1;
	else if(defButton == IDNO)
		theDefault = MB_DEFBUTTON2;
	else
		theDefault = MB_DEFBUTTON3;

	return(AfxMessageBox(msg, MB_YESNOCANCEL | MB_ICONQUESTION | theDefault));
}

CString MyBaseUtils::GetAppPath()
{
	CString path;
	TCHAR buf[1000];
#ifdef ZRXSDK
	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("MyBaseDLL.zrx")),buf,1000);
#else
#ifdef ARX2014
	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("MyBaseDLL.arx")),buf,1000);
#else
	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("MyBaseDLL.dll")),buf,1000);
#endif
#endif

	if (pathLength)
	{
		path = buf;
		path = path.Left(path.ReverseFind('\\'));
		path+=_T("\\");
	}
	else
	{
		path =_T("");
	}
	return path;
}

//获得当前的视图设置
void MyBaseUtils::GetCurrentView(AcDbViewTableRecord& view)
{
	struct resbuf rb;
	struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统标记

	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	// 获得当前视口的“查看”模式
	acedGetVar(_T("VIEWMODE"), &rb);
	view.setPerspectiveEnabled(rb.resval.rint & 1);
	view.setFrontClipEnabled(rb.resval.rint & 2);
	view.setBackClipEnabled(rb.resval.rint & 4);
	view.setFrontClipAtEye(!(rb.resval.rint & 16)); 

	// 当前视口中视图的中心点（UCS坐标）
	acedGetVar(_T("VIEWCTR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	view.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X], rb.resval.rpoint[Y])); 

	// 当前视口透视图中的镜头焦距长度（单位为毫米）
	acedGetVar(_T("LENSLENGTH"), &rb);
	view.setLensLength(rb.resval.rreal);

	// 当前视口中目标点的位置（以 UCS 坐标表示）
	acedGetVar(_T("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setTarget(AcGePoint3d(rb.resval.rpoint[X],	rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的观察方向（UCS）
	acedGetVar(_T("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	view.setViewDirection(AcGeVector3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的视图高度（图形单位）
	acedGetVar(_T("VIEWSIZE"), &rb);
	view.setHeight(rb.resval.rreal);
	double height = rb.resval.rreal;

	// 以像素为单位的当前视口的大小（X 和 Y 值）
	acedGetVar(_T("SCREENSIZE"), &rb);
	view.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);

	// 当前视口的视图扭转角
	acedGetVar(_T("VIEWTWIST"), &rb);
	view.setViewTwist(rb.resval.rreal);

	// 将模型选项卡或最后一个布局选项卡置为当前
	acedGetVar(_T("TILEMODE"), &rb);
	int tileMode = rb.resval.rint;
	// 设置当前视口的标识码
	acedGetVar(_T("CVPORT"), &rb);
	int cvport = rb.resval.rint;


	// 是否是模型空间的视图
	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
	view.setIsPaperspaceView(paperspace);

	if (!paperspace)
	{
		// 当前视口中前向剪裁平面到目标平面的偏移量
		acedGetVar(_T("FRONTZ"), &rb);
		view.setFrontClipDistance(rb.resval.rreal);

		// 获得当前视口后向剪裁平面到目标平面的偏移值
		acedGetVar(_T("BACKZ"), &rb);
		view.setBackClipDistance(rb.resval.rreal);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}
}
bool MyBaseUtils::getCurViewExt(AcDbExtents& dcsExt)
{
	resbuf rbSCREENSIZE;
	int rt = acedGetVar(_T("SCREENSIZE"), &rbSCREENSIZE);

	double dblHwSca = rbSCREENSIZE.resval.rpoint[1] / rbSCREENSIZE.resval.rpoint[0];

	resbuf rbVIEWSIZE;
	rt = acedGetVar(_T("VIEWSIZE"), &rbVIEWSIZE);

	double dx = rbVIEWSIZE.resval.rreal / dblHwSca / 2.0;
	double dy = rbVIEWSIZE.resval.rreal / 2.0;

	resbuf rbVIEWCTR;
	rt = acedGetVar(_T("VIEWCTR"), &rbVIEWCTR);

	ads_point pntScopCen;
	ads_point_set(rbVIEWCTR.resval.rpoint, pntScopCen);

	acdbUcs2Wcs(pntScopCen, pntScopCen, 0);

	ads_point pntScopMin;
	pntScopMin[0] = pntScopCen[0] - dx;
	pntScopMin[1] = pntScopCen[1] - dy;
	pntScopMin[2] = 0.0;

	ads_point pntScopMax;
	pntScopMax[0] = pntScopCen[0] + dx;
	pntScopMax[1] = pntScopCen[1] + dy;
	pntScopMax[2] = 0.0;

	resbuf wcs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;

	resbuf dcs;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;
	acedTrans(pntScopMin, &wcs, &dcs, 0, pntScopMin);
	acedTrans(pntScopMax, &wcs, &dcs, 0, pntScopMax);

	dcsExt.addPoint(asPnt3d(pntScopMin));
	dcsExt.addPoint(asPnt3d(pntScopMax));

	return true;

}

void MyBaseUtils::ZOOMEXT(double dScale)
{
	//缩放前纪录视图数据
	AcDbExtents dcsLastViewExt;
	getCurViewExt(dcsLastViewExt);

	resbuf rbLIMMIN;
	acedGetVar(_T("LIMMIN"), &rbLIMMIN);

	resbuf rbLIMMAX;
	acedGetVar(_T("LIMMAX"), &rbLIMMAX);

	acdbCurDwg()->updateExt();

	resbuf rbEXTMIN;
	acedGetVar(_T("EXTMIN"), &rbEXTMIN);

	resbuf rbEXTMAX;
	acedGetVar(_T("EXTMAX"), &rbEXTMAX);

	resbuf rbVIEWDIR;
	acedGetVar(_T("VIEWDIR"), &rbVIEWDIR);

	AcGeVector3d vecViewDir = asVec3d(rbVIEWDIR.resval.rpoint);

	bool m_bHasEntInCurrView = false;
	ads_name lastename;	
	if (RTNORM  == acdbEntLast(lastename))
	{
		m_bHasEntInCurrView = true;
	}

	AcDbExtents maxExt;

	if (m_bHasEntInCurrView)
	{
		AcGeVector3d wcsViewDir;
		acdbUcs2Wcs(asDblArray(vecViewDir), asDblArray(wcsViewDir), true);
		wcsViewDir = wcsViewDir.normal();

		if (wcsViewDir.isEqualTo(AcGeVector3d::kZAxis))
		{
			maxExt.addPoint(asPnt3d(rbLIMMIN.resval.rpoint));
			maxExt.addPoint(asPnt3d(rbLIMMAX.resval.rpoint));
		}

		if (rbEXTMAX.resval.rpoint[0] >= rbEXTMIN.resval.rpoint[0] &&
			rbEXTMAX.resval.rpoint[1] >= rbEXTMIN.resval.rpoint[1])
		{
			maxExt.addPoint(asPnt3d(rbEXTMIN.resval.rpoint));
			maxExt.addPoint(asPnt3d(rbEXTMAX.resval.rpoint));
		}
	}
	else
	{
		maxExt.addPoint(asPnt3d(rbLIMMIN.resval.rpoint));
		maxExt.addPoint(asPnt3d(rbLIMMAX.resval.rpoint));
	}

	// 计算长方形最小包围盒的顶点
	ads_point pt[/*7*/8];
	pt[0][X] = pt[3][X] = pt[4][X] = pt[7][X] = maxExt.minPoint().x;
	pt[1][X] = pt[2][X] = pt[5][X] = pt[6][X] = maxExt.maxPoint().x;
	pt[0][Y] = pt[1][Y] = pt[4][Y] = pt[5][Y] = maxExt.minPoint().y;
	pt[2][Y] = pt[3][Y] = pt[6][Y] = pt[7][Y] = maxExt.maxPoint().y;
	pt[0][Z] = pt[1][Z] = pt[2][Z] = pt[3][Z] = maxExt.minPoint().z;
	pt[4][Z] = pt[5][Z] = pt[6][Z] = pt[7][Z] = maxExt.maxPoint().z; 

	// 将长方体的所有角点转移到DCS中
	struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统标记
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	acedTrans(pt[0], &wcs, &ucs, 0, pt[0]);
	acedTrans(pt[1], &wcs, &ucs, 0, pt[1]);
	acedTrans(pt[2], &wcs, &ucs, 0, pt[2]);
	acedTrans(pt[3], &wcs, &ucs, 0, pt[3]);
	acedTrans(pt[4], &wcs, &ucs, 0, pt[4]);
	acedTrans(pt[5], &wcs, &ucs, 0, pt[5]);
	acedTrans(pt[6], &wcs, &ucs, 0, pt[6]);
	acedTrans(pt[7], &wcs, &ucs, 0, pt[7]);

	acedTrans(pt[0], &ucs, &dcs, 0, pt[0]);
	acedTrans(pt[1], &ucs, &dcs, 0, pt[1]);
	acedTrans(pt[2], &ucs, &dcs, 0, pt[2]);
	acedTrans(pt[3], &ucs, &dcs, 0, pt[3]);
	acedTrans(pt[4], &ucs, &dcs, 0, pt[4]);
	acedTrans(pt[5], &ucs, &dcs, 0, pt[5]);
	acedTrans(pt[6], &ucs, &dcs, 0, pt[6]);
	acedTrans(pt[7], &ucs, &dcs, 0, pt[7]);

	// 获得所有角点在DCS中最小的包围矩形
	double xMax = pt[0][X], xMin = pt[0][X];
	double yMax = pt[0][Y], yMin = pt[0][Y];
	for (int i = 1; i <= 7; i++)
	{
		if (pt[i][X] > xMax)
			xMax = pt[i][X];
		if (pt[i][X] < xMin)
			xMin = pt[i][X];
		if (pt[i][Y] > yMax)
			yMax = pt[i][Y];
		if (pt[i][Y] < yMin)
			yMin = pt[i][Y];
	}

	//创建VIEW作为缩放参数
	AcDbViewTableRecord view;
	GetCurrentView(view);

	// 设置视图的中心点
	view.setCenterPoint(AcGePoint2d((xMin + xMax) / 2, (yMin + yMax) / 2));
	// 设置视图的高度和宽度
	view.setHeight(fabs(yMax - yMin)*dScale);
	view.setWidth(fabs(xMax - xMin)*dScale);
	// 将视图对象设置为当前视图
	Acad::ErrorStatus es = acedSetCurrentView(&view, NULL);

}
void MyBaseUtils::ZOOMWINDOW(AcGePoint3d minPt, AcGePoint3d maxPt)
{
	// get the extents of the drawing
	AcDbViewTableRecord view;

	AcGePoint2d max_2d (maxPt[X], maxPt[Y]);
	AcGePoint2d min_2d (minPt[X], minPt[Y]);
	// now set the view centre point
	view.setCenterPoint (min_2d + (max_2d - min_2d) / 2.0);
	// now height and width of view
	view.setHeight(max_2d[Y] - min_2d[Y]);
	view.setWidth (max_2d[X] - min_2d[X]);
	// set the view
	acedSetCurrentView (&view, NULL);
	// updates the extents
	acdbHostApplicationServices()->workingDatabase()->updateExt(TRUE);
}

void  MyBaseUtils::ZOOMWindowByDeFault(AcGePoint3d cenPt)
{
	AcDbExtents dcsLastViewExt;
	getCurViewExt(dcsLastViewExt);

	AcGePoint3d minPt,maxPt;
	minPt = dcsLastViewExt.minPoint();
	maxPt = dcsLastViewExt.maxPoint();

	double dHeight = maxPt.y - minPt.y;
	double dLength = maxPt.x - minPt.x;
	AcDbViewTableRecord view;


	// now set the view centre point
	view.setCenterPoint (cenPt.convert2d(AcGePlane::kXYPlane));
	// now height and width of view
	view.setHeight(dHeight);
	view.setWidth (dLength);
	// set the view
	acedSetCurrentView (&view, NULL);
	// updates the extents
	acdbHostApplicationServices()->workingDatabase()->updateExt(TRUE);
}

void MyBaseUtils::ZoomToWindow(AcGePoint3d wmin, AcGePoint3d wmax)
{	
	AcDbViewTableRecord view;
	struct resbuf rb;
	struct resbuf wcs, dcs, ccs;
	ads_point vpDir;
	ads_point wdcsmax,wdcsmin;
	AcGeVector3d viewDir;
	AcGePoint2d cenPt;
	ads_real lenslength,viewtwist,frontz,backz;
	ads_point target;
	int viewmode,tilemode,cvport;

	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	
	ccs.restype = RTSHORT;
	ccs.resval.rint = 1;

	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	acedGetVar(_T("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ccs, &wcs, 0, vpDir);

	viewDir.set(vpDir[X], vpDir[Y], vpDir[Z]);
	acedTrans(asDblArray(wmax), &ccs, &dcs, 0, wdcsmax);
	acedTrans(asDblArray(wmin), &ccs, &dcs, 0, wdcsmin);

	cenPt.set(((wdcsmax[X] + wdcsmin[X])/2.0), ((wdcsmax[Y] + wdcsmin[Y])/2.0));
	
	view.setCenterPoint(cenPt);
	view.setHeight(fabs(wdcsmax[Y] - wdcsmin[Y]));
	view.setWidth(fabs(wdcsmax[X] - wdcsmin[X]));
	view.setViewDirection(viewDir);

	acedGetVar(_T("LENSLENGTH"), &rb);
	lenslength = rb.resval.rreal;

	view.setLensLength(lenslength);

	acedGetVar(_T("VIEWTWIST"), &rb);
	viewtwist = rb.resval.rreal;

	view.setViewTwist(viewtwist);

	acedGetVar(_T("FRONTZ"), &rb);
	frontz = rb.resval.rreal;
	acedGetVar(_T("BACKZ"), &rb);
	backz = rb.resval.rreal;

	acedGetVar(_T("VIEWMODE"), &rb);
	viewmode = rb.resval.rint;
	view.setPerspectiveEnabled(viewmode & 1);

	view.setFrontClipEnabled(viewmode & 2);
	view.setBackClipEnabled(viewmode & 4);
	view.setFrontClipAtEye(!(viewmode & 16));

	acedGetVar(_T("TILEMODE"), &rb);
	tilemode = rb.resval.rint;
	acedGetVar(_T("CVPORT"), &rb);
	cvport = rb.resval.rint;

	Adesk::Boolean paperspace = ((tilemode == 0) && (cvport == 1)) ? Adesk::kTrue : Adesk::kFalse;
	view.setIsPaperspaceView(paperspace);
	if (Adesk::kFalse == paperspace)
	{
		view.setFrontClipDistance(frontz);
		view.setBackClipDistance(backz);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}
	acedGetVar(_T("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ccs, &wcs, 0, target);
	view.setTarget(AcGePoint3d(target[X], target[Y], target[Z]));
	acedSetCurrentView(&view, NULL);

}	

Acad::ErrorStatus MyBaseUtils::flushGraphics(AcDbObjectId objId)
{	
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es;

	es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		return es;
	}
	pEnt->recordGraphicsModified();
	pEnt->downgradeOpen();
	pEnt->draw();
	pEnt->close();
	return Acad::eOk;
}

Acad::ErrorStatus MyBaseUtils::flushGraphics(AcDbObjectIdArray objIds)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es;
	for (int i=0; i<objIds.length(); i++)
	{
		objId = objIds.at(i);
		es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			continue;
		}
		pEnt->recordGraphicsModified();
		pEnt->downgradeOpen();
		pEnt->draw();
		pEnt->close();
	}
	return Acad::eOk;
}

Acad::ErrorStatus MyBaseUtils::flushGraphics(AcDbDatabase* pDb)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();

	AcDbBlockTable* pBlockTable = NULL;
	AcDbBlockTableRecord* pSpaceRecord = NULL;
	Acad::ErrorStatus es;
	es = pDb->getBlockTable(pBlockTable, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		actrTransactionManager->abortTransaction();
		return es; 
	}
	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		actrTransactionManager->abortTransaction();
		return es;
	}
	pBlockTable->close();

	AcDbBlockTableRecordIterator* pIter;
	es = pSpaceRecord->newIterator(pIter);
	if (es != Acad::eOk)
	{
		pSpaceRecord->close();
		actrTransactionManager->abortTransaction();
		return es;
	}
	es = pSpaceRecord->close();
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObject* pObj = NULL;
	for (; !pIter->done(); pIter->step())
	{
		es = pIter->getEntityId(objId);
		if (es == Acad::eOk)
		{
			flushGraphics(objId);
		}
	}
	delete pIter;
	pIter = NULL;
	actrTransactionManager->endTransaction();
	return es;
}


bool MyBaseUtils::RegInfoExit()
{	
	bool n_GInfo = true;
#ifdef ARX
#else
	TCHAR strTemp[256];
	CString strTmp;
	GetVar(_T("VERAUTH"), strTemp);
	strTmp = strTemp;
	if (strTmp.CompareNoCase(_T("NONE")) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
#endif
	return n_GInfo;
}

void MyBaseUtils::CloseProcess(CString strProName)
{
	if (strProName.IsEmpty())  
	{  
		AfxMessageBox(_T("进程名字不能为空!"));  
		return;  
	}  
	HANDLE SnapShot, ProcessHandle;    
	SHFILEINFO shSmall;    
	PROCESSENTRY32 ProcessInfo;     
	CString strExeFile; //进程名字  
	SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);    
	if(SnapShot != NULL)     
	{    
		ProcessInfo.dwSize = sizeof(ProcessInfo);  // 设置ProcessInfo的大小    
		BOOL Status = Process32First(SnapShot, &ProcessInfo);   

		while(Status)    
		{    
			// 获取进程文件信息    
			SHGetFileInfo(ProcessInfo.szExeFile, 0, &shSmall, sizeof(shSmall), SHGFI_ICON|SHGFI_SMALLICON);    

			// 检测进程是否需要关闭    
			strExeFile = ProcessInfo.szExeFile;  

			if(strProName.CompareNoCase(strExeFile) == 0)     
			{    
				// 获取进程句柄，强行关闭    
				ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessInfo.th32ProcessID);    
				TerminateProcess(ProcessHandle, 1);    
				//break;    
			}    

			// 获取下一个进程的信息    
			Status = Process32Next(SnapShot, &ProcessInfo);    
		}    
	}   
} 