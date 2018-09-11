#include "StdAfx.h"
#include "MyTransFunc.h"
#include "MyBaseUtils.h"

MyTransFunc::MyTransFunc(void)
{
}

MyTransFunc::~MyTransFunc(void)
{
}


AcGePoint3d MyTransFunc::My2d23d(AcGePoint2d pt2d)
{
	AcGePoint3d pt;
	pt.x = pt2d.x;
	pt.y = pt2d.y;
	pt.z = 0;
	return pt;
}

AcGePoint2d MyTransFunc::My3d22d(AcGePoint3d pt3d)
{
	AcGePoint2d pt;
	pt.x = pt3d.x;
	pt.y = pt3d.y;
	return pt;
}

AcGePoint3dArray MyTransFunc::My2d23d(AcGePoint2dArray pt2dArr)
{
	AcGePoint3dArray ptArr;
	AcGePoint3d tmpPt;
	for (int i=0; i< pt2dArr.length(); i++)
	{
		tmpPt.x = pt2dArr.at(i).x;
		tmpPt.y = pt2dArr.at(i).y;
		tmpPt.z = 0;
		ptArr.append(tmpPt);
	}
	return ptArr;
}

AcGePoint2dArray MyTransFunc::My3d22d(AcGePoint3dArray pt3dArr)
{
	AcGePoint2dArray ptArr;
	AcGePoint2d tmpPt;
	for (int i=0; i< pt3dArr.length(); i++)
	{
		tmpPt.x = pt3dArr.at(i).x;
		tmpPt.y = pt3dArr.at(i).y;
		ptArr.append(tmpPt);
	}
	return ptArr;
}


AcGePoint3d MyTransFunc::MyMidPoint(AcGePoint3d ptStart, AcGePoint3d ptEnd)
{
	AcGePoint3d midPt;
	midPt.x = (ptStart.x + ptEnd.x)/2;
	midPt.y = (ptStart.y + ptEnd.y)/2;
	midPt.z = (ptStart.z + ptEnd.z)/2;
	return midPt;
}

//凸度计算函数
double MyTransFunc::GetBulgeOf3Pt(AcGePoint3d &startPt                        //起点
								 ,AcGePoint3d &interPt                //中间一点
								 ,AcGePoint3d &endPt                        //终点
							   )
{
	double dTmp;
	AcGeCircArc3d aa(startPt, interPt, endPt);
	dTmp = (startPt.x - endPt.x) * (interPt.y - endPt.y) - (interPt.x - endPt.x) * (startPt.y - endPt.y);
	acutPrintf(_T("\n起始弧度为%f, 终止弧度为%f"), aa.startAng(), aa.endAng());
	//圆弧逆时针方向
	if (dTmp > 0)        {
		return tan((aa.endAng() - aa.startAng()) * 0.25);
	} 
	//圆弧顺时针方向
	else if (dTmp < 0)        {
		return -tan((aa.endAng() - aa.startAng()) * 0.25);
	}
	return 0;
}

double MyTransFunc::CurveToRotation(double dCurve)
{
	double dRotation;
	dRotation = 180*dCurve/PI;
	return dRotation;
}

double MyTransFunc::RotationToCurve(double dRotation)
{
	double dCurve;
	dCurve = PI*dRotation/180;
	return dCurve;
}

AcGePoint3dArray MyTransFunc::OperateTwoPointsAndGetPoints(AcGePoint3d minPt, AcGePoint3d maxPt)
{
	AcGePoint3dArray points;
	points.removeAll();
	AcGePoint3d tmpPt1, tmpPt2;
	tmpPt1.x = minPt.x;
	tmpPt1.y = maxPt.y;
	tmpPt1.z = minPt.z;

	tmpPt2.x = maxPt.x;
	tmpPt2.y = minPt.y;
	tmpPt2.z = maxPt.z;

	points.append(minPt);
	points.append(tmpPt1);
	points.append(maxPt);
	points.append(tmpPt2);
	points.append(minPt);
	return points;
}

AcDbSortentsTable* MyTransFunc::get_sortents_table_of(AcDbEntity* pEnt)
{
	AcDbObjectId owner_id = pEnt->ownerId();
	if (AcDbObjectId::kNull == owner_id)
		return NULL;
	AcDbBlockTableRecord *pRec = NULL;
	if (Acad::eOk != acdbOpenObject( pRec, owner_id, AcDb::kForWrite ))
		return NULL;
	AcDbObjectId ext_id = pRec->extensionDictionary();
	if (AcDbObjectId::kNull == ext_id)
	{
		pRec->createExtensionDictionary();
		ext_id = pRec->extensionDictionary();
		if (AcDbObjectId::kNull == ext_id)
		{
			pRec->close();
			return NULL;
		}
	}
	pRec->close();
	AcDbDictionary *pExt = NULL;
	Acad::ErrorStatus es = acdbOpenObject( pExt, ext_id, AcDb::kForWrite );
	if (Acad::eOk != es)
		return NULL;
	AcDbObject *pObj;
	if (Acad::eOk != pExt->getAt( _T("ACAD_SORTENTS"), pObj, AcDb::kForWrite ))
	{
		AcDbSortentsTable *pSt = new AcDbSortentsTable;
		if (NULL == pSt)
		{
			pExt->close();
			return NULL;
		}

		AcDbObjectId new_id;
		if (Acad::eOk != pExt->setAt( _T("ACAD_SORTENTS"), pSt, new_id ))
		{
			delete pSt;
			pExt->close();
			return NULL;
		}
		pSt->setBlockId( owner_id );
		pObj = pSt;
	}
	pExt->close();

	if (!pObj->isKindOf( AcDbSortentsTable::desc() ))
	{
		pObj->close();
		return NULL;
	}
	return (AcDbSortentsTable*)pObj;
}

void MyTransFunc::DuiXiangPaiXu(AcDbObjectId ent_id, bool TopOrDow/*=TRUE*/)//对象排序
{	
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, ent_id, AcDb::kForWrite) != Acad::eOk)
	{
		return;
	}
	AcDbSortentsTable *pSt = get_sortents_table_of( pEnt );
	pEnt->close();
	if (NULL == pSt)
		return;	
	AcDbObjectIdArray entity_array;
	entity_array.append(ent_id);
	Acad::ErrorStatus es;
	if (TopOrDow==TRUE)
		es = pSt->moveToTop(entity_array);//移到最前面
	if (TopOrDow==FALSE)
		es = pSt->moveToBottom(entity_array);//移到底部
	pSt->close();
}

void MyTransFunc::get_next(CString t, int next[ ])
{
 int t_len=t.GetLength();
 int i=0;         //求解每个next[i]
 next[0]=-1; //递推基本条件,然后求解next[i+1]
 int j=-1;     //向后递推位置下标
 /*
 next[i]=k =>T0...Tk-1=Ti-k...Ti-1
    求解next[i+1]
 1> 如果T0..Tk-1Tk=Ti-k...Ti-1Ti=>next[i+1]=k+1=next[i]+1;
 2>Tk<>Ti,next[k]=k', 如果Ti=Tk'=>next[i+1]=k'+1=next[k]+1=next[next[i]]+1;
 3>依次递推 最后情况next[i+1]=next[0]+1=0,即
 */
 while(i<t_len)
 {
   if(j==-1 ||t[i]==t[j])  //j==-1证明已经与t[0]不匹配了，此时next[i+1]=0
   {
    i++;
    j++;
    next[i]=j;
   }
   else
   {
       j=next[j]; 
   }
 }
}

void NEXT(CString&T,vector<int>&next)
{
	//按模式串生成vector,next(T.size())
	next[0]=-1;
	for(int i=1;i<T.GetLength();i++){
		int j=next[i-1];
		while(T[i]!=T[j+1]&&j>=0)
			j=next[j];//递推计算
		if(T[i]==T[j+1])
			next[i]=j+1;
		else 
			next[i]=0;//
	}
}
int MyTransFunc::KMP(CString S, CString T)  
{  
	//if (text.IsEmpty() || find.IsEmpty())
	//{
	//	return -1;
	//}
	//int find_len = find.GetLength();  
	//int text_len = text.GetLength();  
	//if (text_len < find_len)  
	//	return -1;  
	//int map[find_len];  
	//memset(map, 0, find_len*sizeof(int));  
	////initial the kmp base array: map  
	//map[0] = 0;  
	//map[1] = 0;  
	//int i = 2;  
	//int j = 0;  
	//for (i=2; i<find_len; i++)  
	//{  
	//	while (1)  
	//	{  
	//		if (find[i-1] == find[j])  
	//		{  
	//			j++;  
	//			if (find[i] == find[j])  
	//			{  
	//				map[i] = map[j];  
	//			}  
	//			else  
	//			{  
	//				map[i] = j;  
	//			}  
	//			break;  
	//		}  
	//		else  
	//		{  
	//			if (j == 0)  
	//			{  
	//				map[i] = 0;  
	//				break;  
	//			}  
	//			j = map[j];  
	//		}  
	//	}  
	//}  
	//i = 0;  
	//j = 0;  
	//for (i=0; i<text_len;)  
	//{  
	//	if (text[i] == find[j])  
	//	{  
	//		i++;  
	//		j++;  
	//	}  
	//	else  
	//	{  
	//		j = map[j];  
	//		if (j == 0)  
	//			i++;  
	//	}  
	//	if (j == (find_len))  
	//		return i-j;  
	//}  
	//return -1; 
	//利用模式串T的next函数求T在主串S中的个数count的KMP算法
	//其中T非空，
	S = S.MakeUpper();
	T = T.MakeUpper();
	vector<int> next(T.GetLength());
	if (next.size() < 1)
	{
		return 0;
	}
	NEXT(T,next);
	int index =0;
	int count =-1;
	for(index=0;index<S.GetLength();++index)
	{
		int pos=0;
		int iter=index;
		while(pos<T.GetLength()&&iter<S.GetLength())
		{
			if(S[iter]==T[pos]){
				++iter;++pos;
			}
			else
			{
				if(pos==0)
					++iter;
				else 
					pos=next[pos-1]+1;
			}
		}//whileend
		if(pos==T.GetLength()&&(iter-index)==T.GetLength())
			++count;
	}//forend
	return count;
}  

// int COUNT_KMP(CString&S, CString&T)
// {
// 	//利用模式串T的next函数求T在主串S中的个数count的KMP算法
// 	//其中T非空，
// 	vector<int>next(T.GetLength());
// 	NEXT(T,next);
// 	int index,count=0;
// 	for(index=0;index<S.GetLength();++index)
// 	{
// 		int pos=0;
// 		int iter=index;
// 		while(pos<T.GetLength()&&iter<S.GetLength())
// 		{
// 			if(S[iter]==T[pos]){
// 				++iter;++pos;
// 			}
// 			else
// 			{
// 				if(pos==0)
// 					++iter;
// 				else 
// 					pos=next[pos-1]+1;
// 			}
// 		}//whileend
// 		if(pos==T.GetLength()&&(iter-index)==T.GetLength())++count;
// 	}//forend
// 	return count;
// }

LPCTSTR
MyTransFunc::ptToStr(const AcGePoint3d& pt, CString& str, int unit, int prec)
{
	TCHAR xstr[100], ystr[100], zstr[100];

	acdbRToS(pt.x, unit, prec, xstr);
	acdbRToS(pt.y, unit, prec, ystr);
	acdbRToS(pt.z, unit, prec, zstr);

	str.Format(_T("(%s, %s, %s)"), xstr, ystr, zstr);
	return str;
}

/****************************************************************************
**
**  MyTransFunc::ptToStr (2D)
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::ptToStr(const AcGePoint2d& pt, CString& str, int unit, int prec)
{
	TCHAR xstr[100], ystr[100];

	acdbRToS(pt.x, unit, prec, xstr);
	acdbRToS(pt.y, unit, prec, ystr);

	str.Format(_T("(%s, %s)"), xstr, ystr);
	return str;
}

/****************************************************************************
**
**  MyTransFunc::vectorToStr (3D)
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::vectorToStr(const AcGeVector3d& vec, CString& str, int unit, int prec)
{
	TCHAR xstr[100], ystr[100], zstr[100];

	acdbRToS(vec.x, unit, prec, xstr);
	acdbRToS(vec.y, unit, prec, ystr);
	acdbRToS(vec.z, unit, prec, zstr);

	str.Format(_T("(%s, %s, %s)"), xstr, ystr, zstr);
	return str;
}

/****************************************************************************
**
**  MyTransFunc::vectorToStr (2D)
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::vectorToStr(const AcGeVector2d& vec, CString& str, int unit, int prec)
{
	TCHAR xstr[100], ystr[100];

	acdbRToS(vec.x, unit, prec, xstr);
	acdbRToS(vec.y, unit, prec, ystr);

	str.Format(_T("(%s, %s)"), xstr, ystr);
	return str;
}

/****************************************************************************
**
**  MyTransFunc::angleToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::angleToStr(const double angle, CString& str, int unit, int prec)
{
	TCHAR tmpStr[100];

	acdbAngToS(angle, unit, prec, tmpStr);
	str = tmpStr;
	return str;
}

/****************************************************************************
**
**  MyTransFunc::doubleToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::doubleToStr(const double realVal, CString& str, int unit, int prec)
{
	TCHAR tmpStr[100];

	acdbRToS(realVal, unit, prec, tmpStr);
	str = tmpStr;
	return str;
}

CString MyTransFunc::doubleToStr(const double realVal, int unit)
{
	CString strTmp;
	if (unit < 0)
	{
		strTmp = _T("%f");
	}
	else
	{
		strTmp.Format(_T(".%df"), unit);
		strTmp = _T("%") + strTmp;
	}
	
	CString strReturn;
	strReturn.Format(strTmp, realVal);
	return strReturn;
}

/****************************************************************************
**
**  MyTransFunc::intToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::intToStr(const int intVal, CString& str)
{
	str.Format(_T("%d"), intVal);
	return str;
}

CString MyTransFunc::intToStr(const int intVal)
{
	CString str;
	str.Format(_T("%d"), intVal);
	return str;
}

/****************************************************************************
**
**  MyTransFunc::intDbIdToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::intDbIdToStr(const Adesk::IntDbId intVal, CString& str)
{
#ifndef _WIN64
	str.Format(_T("%d"), intVal);
#else
	str.Format(_T("%I64d"), intVal);
#endif
	return str;
}

/****************************************************************************
**
**  MyTransFunc::booleanToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::booleanToStr(bool b, CString& str)
{
	if (b)
		str = _T("True");
	else
		str = _T("False");

	return str;
}

/****************************************************************************
**
**  MyTransFunc::booleanToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::booleanToStr(Adesk::Boolean b, CString& str)
{
	if (b)
		str = _T("True");
	else
		str = _T("False");

	return str;
}

/***********************************************************************
**
**  MyTransFunc::colorToStr
**      Convert a color index into a string. You must pass in
**  the buffer for the string, but the string pointer is also returned
**  incase you want to use it in a cascading expression.
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::colorToStr(int color, CString& str, bool convertStandard)
{
	color = abs(color);    // in case used from Layer table

	ASSERT((color >= 0) && (color <= 256));

	if (color == MyBaseUtils::kByblock)
		str = _T("ByBlock");
	else if (color == MyBaseUtils::kBylayer)
		str = _T("ByLayer");
	else if (convertStandard) {
		if (color == MyBaseUtils::kRed)
			str = _T("1-Red");
		else if (color == MyBaseUtils::kYellow)
			str = _T("2-Yellow");
		else if (color == MyBaseUtils::kGreen)
			str = _T("3-Green");
		else if (color == MyBaseUtils::kCyan)
			str = _T("4-Cyan");
		else if (color == MyBaseUtils::kBlue)
			str = _T("5-Blue");
		else if (color == MyBaseUtils::kMagenta)
			str = _T("6-Magenta");
		else if (color == MyBaseUtils::kWhite)
			str = _T("7-White");
		else
			intToStr(color, str);
	}
	else
		intToStr(color, str);

	return str;
}

/***********************************************************************
**
**  MyTransFunc::objToHandleStr
**      Obtain an old-style handle string from an object pointer.
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::objToHandleStr(const AcDbObject* obj, CString& str)
{
	ASSERT(obj != NULL);

	AcDbHandle handle;

	obj->getAcDbHandle(handle);
	TCHAR tmpStr[256];
	handle.getIntoAsciiBuffer(tmpStr);
	str = tmpStr;
	return str;
}

/***********************************************************************
**
**  MyTransFunc::objToEnameStr
**      Obtain an old-style ename string from an object pointer.
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::objToEnameStr(const AcDbObject* obj, CString& str)
{
	ASSERT(obj != NULL);

	ads_name ent;
	acdbGetAdsName(ent, obj->objectId());
	str.Format(_T("%lx"), ent[0]);
	return str;
}

/***********************************************************************
**
**  MyTransFunc::objToClassStr
**      Obtain a class name string from an object pointer.
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::objToClassStr(const AcRxObject* obj)
{
	ASSERT(obj != NULL);

	AcRxClass* rxClass = obj->isA();
	if (rxClass == NULL) {
		ASSERT(0);
		MyBaseUtils::stopAlertBox(_T("AcRxObject class has not called rxInit()!"));
		return _T("*Unknown*");
	}

	return obj->isA()->name();
}

/****************************************************************************
**
**  MyTransFunc::objToClassAndHandleStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::objToClassAndHandleStr(const AcDbObjectId& objId, CString& str)
{
	CString tmpStr;
	AcDbObject* tmpObj;

	if (objId.isNull()) {
		str = _T("AcDbObjectId::kNull");
	}
	else {
		// open up even if erased
		Acad::ErrorStatus es = acdbOpenObject(tmpObj, objId, AcDb::kForRead, true);
		if (es == Acad::eOk) {
			str.Format(_T("<%-20s %-4s>"),
				MyTransFunc::objToClassStr(tmpObj), MyTransFunc::objToHandleStr(tmpObj, tmpStr));
			tmpObj->close();
		}
		else
			str = MyBaseUtils::rxErrorStr(es);
	}

	return str;
}

/****************************************************************************
**
**  MyTransFunc::objToClassAndHandleStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::objToClassAndHandleStr(const AcDbObject* obj, CString& str)
{
	CString tmpStr;

	str.Format(_T("<%-20s %-4s>"),
		MyTransFunc::objToClassStr(obj), MyTransFunc::objToHandleStr(obj, tmpStr));

	return str;
}

/****************************************************************************
**
**  MyTransFunc::ptrToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::ptrToStr(void* ptr, CString& str)
{
	str.Format(_T("%p"), ptr);
	return str;
}

/****************************************************************************
**
**  MyTransFunc::dbToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::dbToStr(AcDbDatabase* db, CString& str)
{
	if (db == NULL) {
		str = _T("NULL");
		return str;
	}

	const TCHAR* fname;
	Acad::ErrorStatus es = db->getFilename(fname);
	if (es == Acad::eOk)
		str.Format(_T("%p  \"%s\""), db, fname);
	else {
		// see if we can get name from a document
		AcApDocument* doc = acDocManager->document(db);
		if (doc) {
			str.Format(_T("%p  \"%s\""), db, doc->fileName());
		}
		else {
			// last resort, just use the pointer value.  eNotApplicable
			// happens frequently on temporary databases, otherwise we
			// would like to know what is going on.
			if (es == Acad::eNotApplicable)
				str.Format(_T("%p"), db);
			else {
				ASSERT(0);
				str.Format(_T("%p  %s"), db, MyBaseUtils::rxErrorStr(es));
			}
		}
	}

	return str;
}

/****************************************************************************
**
**  MyTransFunc::vertexTypeToStr
**      convert a vertex type to its string value
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::vertexTypeToStr(AcDb::Vertex3dType vertexType, CString& str)
{
	if (vertexType == AcDb::k3dSimpleVertex)
		str = _T("3D Vertex");
	else if (vertexType == AcDb::k3dControlVertex)
		str = _T("3D Control Vertex");
	else if (vertexType == AcDb::k3dFitVertex)
		str = _T("3D Fit Vertex");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::textModeToStr        HORIZONTAL
**      Convert a text mode value into a string for printing
**  purposes.
**
**    
**
****************************************/

LPCTSTR
MyTransFunc::textModeToStr(AcDb::TextHorzMode horizMode, CString& str)
{
	switch (horizMode) {
		case AcDb::kTextLeft:
			str = _T("Left");
			break;
		case AcDb::kTextCenter:
			str = _T("Center");
			break;
		case AcDb::kTextRight:
			str = _T("Right");
			break;
		case AcDb::kTextAlign:
			str = _T("Align");
			break;
		case AcDb::kTextMid:
			str = _T("Mid");
			break;
		case AcDb::kTextFit:
			str = _T("Fit");
			break;
		default:
			ASSERT(0);
			str = _T("*Unknown*");
	}
	return str;
}

/***********************************************************************
**
**  MyTransFunc::textModeToStr       VERTICAL
**      Convert a text mode value into a string for printing
**  purposes.
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::textModeToStr(AcDb::TextVertMode vertMode, CString& str)
{
	switch (vertMode) {
		case AcDb::kTextBase:
			str = _T("Baseline");
			break;
		case AcDb::kTextBottom:
			str = _T("Bottom");
			break;
		case AcDb::kTextVertMid:
			str = _T("Vertical Mid");
			break;
		case AcDb::kTextTop:
			str = _T("Top");
			break;
		default:
			ASSERT(0);
			str = _T("*Unknown*");
	}
	return str;
}

/***********************************************************************
**
**  MyTransFunc::hatchTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::hatchTypeToStr(AcDbHatch::HatchPatternType hatchType, CString& str)
{
	if (hatchType == AcDbHatch::kUserDefined)
		str = _T("User Defined");
	else if (hatchType == AcDbHatch::kPreDefined)
		str = _T("Pre Defined");
	else if (hatchType == AcDbHatch::kCustomDefined)
		str = _T("Custom Defined");
	else {
		ASSERT(0);
		MyTransFunc::intToStr(hatchType, str);
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::hatchStyleToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::hatchStyleToStr(AcDbHatch::HatchStyle style, CString& str)
{
	if (style == AcDbHatch::kNormal)
		str = _T("Normal");
	else if (style == AcDbHatch::kOuter)
		str = _T("Outer");
	else if (style == AcDbHatch::kIgnore)
		str = _T("Ignore");
	else {
		ASSERT(0);
		MyTransFunc::intToStr(style, str);
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::hatchLoopTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::hatchLoopTypeToStr(Adesk::Int32 loopType, CString& str)
{
	if (loopType == AcDbHatch::kDefault)
		str = _T("Default");
	else if (loopType == AcDbHatch::kExternal)
		str = _T("External");
	else if (loopType == AcDbHatch::kPolyline)
		str = _T("Polyline");
	else if (loopType == AcDbHatch::kDerived)
		str = _T("Derived");
	else if (loopType == AcDbHatch::kTextbox)
		str = _T("Textbox");
	else if (loopType == AcDbHatch::kOutermost)
		str = _T("Outermost");
	else if (loopType == AcDbHatch::kNotClosed)
		str = _T("Not Closed");
	else if (loopType == AcDbHatch::kSelfIntersecting)
		str = _T("Self Intersecting");
	else {
		ASSERT(0);
		MyTransFunc::intToStr(loopType, str);
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::mtextAttachmentTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::mtextAttachmentTypeToStr(AcDbMText::AttachmentPoint type, CString& str)
{
	if (type == AcDbMText::kTopLeft)
		str = _T("Top Left");
	else if (type == AcDbMText::kTopCenter)
		str = _T("Top Center");
	else if (type == AcDbMText::kTopRight)
		str = _T("Top Right");
	else if (type == AcDbMText::kMiddleLeft)
		str = _T("Middle Left");
	else if (type == AcDbMText::kMiddleCenter)
		str = _T("Middle Center");
	else if (type == AcDbMText::kMiddleRight)
		str = _T("Middle Right");
	else if (type == AcDbMText::kBottomLeft)
		str = _T("Bottom Left");
	else if (type == AcDbMText::kBottomCenter)
		str = _T("Bottom Center");
	else if (type == AcDbMText::kBottomRight)
		str = _T("Bottom Right");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::mtextFlowDirTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::mtextFlowDirTypeToStr(AcDbMText::FlowDirection type, CString& str)
{
	if (type == AcDbMText::kLtoR)
		str = _T("Left to Right");
	else if (type == AcDbMText::kRtoL)
		str = _T("Right to Left");
	else if (type == AcDbMText::kTtoB)
		str = _T("Top to Bottom");
	else if (type == AcDbMText::kBtoT)
		str = _T("Bottom to Top");
	else if (type == AcDbMText::kByStyle)
		str = _T("By Style");
	else {
		ASSERT(0);
		str  = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::lineSpacingTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::lineSpacingTypeToStr(AcDb::LineSpacingStyle type, CString& str)
{
	if (type == AcDb::kAtLeast)
		str = _T("At Least");
	else if (type == AcDb::kExactly)
		str = _T("Exactly");
	else {
		ASSERT(0);
		str  = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::proxyMetafileTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::proxyMetafileTypeToStr(AcDbProxyEntity::GraphicsMetafileType type, CString& str)
{
	if (type == AcDbProxyEntity::kNoMetafile)
		str = _T("No Metafile");
	else if (type == AcDbProxyEntity::kBoundingBox)
		str = _T("Bounding Box");
	else if (type == AcDbProxyEntity::kFullGraphics)
		str = _T("Full Graphics");
	else {
		ASSERT(0);
		MyTransFunc::intToStr(type, str);
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::lineWeightTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::lineWeightTypeToStr(AcDb::LineWeight type, CString& str)
{
	if (type == AcDb::kLnWt000)
		str = _T("0.00 mm");
	else if (type == AcDb::kLnWt005)
		str = _T("0.05 mm");
	else if (type == AcDb::kLnWt009)
		str = _T("0.09 mm");
	else if (type == AcDb::kLnWt013)
		str = _T("0.13 mm");
	else if (type == AcDb::kLnWt015)
		str = _T("0.15 mm");
	else if (type == AcDb::kLnWt018)
		str = _T("0.18 mm");
	else if (type == AcDb::kLnWt020)
		str = _T("0.20 mm");
	else if (type == AcDb::kLnWt025)
		str = _T("0.25 mm");
	else if (type == AcDb::kLnWt030)
		str = _T("0.30 mm");
	else if (type == AcDb::kLnWt035)
		str = _T("0.35 mm");
	else if (type == AcDb::kLnWt040)
		str = _T("0.40 mm");
	else if (type == AcDb::kLnWt050)
		str = _T("0.50 mm");
	else if (type == AcDb::kLnWt053)
		str = _T("0.53 mm");
	else if (type == AcDb::kLnWt060)
		str = _T("0.60 mm");
	else if (type == AcDb::kLnWt070)
		str = _T("0.70 mm");
	else if (type == AcDb::kLnWt080)
		str = _T("0.80 mm");
	else if (type == AcDb::kLnWt090)
		str = _T("0.90 mm");
	else if (type == AcDb::kLnWt100)
		str = _T("1.00 mm");
	else if (type == AcDb::kLnWt106)
		str = _T("1.06 mm");
	else if (type == AcDb::kLnWt120)
		str = _T("1.20 mm");
	else if (type == AcDb::kLnWt140)
		str = _T("1.40 mm");
	else if (type == AcDb::kLnWt158)
		str = _T("1.58 mm");
	else if (type == AcDb::kLnWt200)
		str = _T("2.00 mm");
	else if (type == AcDb::kLnWt211)
		str = _T("2.11 mm");
	else if (type == AcDb::kLnWtByLayer)
		str = _T("ByLayer");
	else if (type == AcDb::kLnWtByBlock)
		str = _T("ByBlock");
	else if (type == AcDb::kLnWtByLwDefault)
		str = _T("ByLwDefault");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::mergeStyleToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::mergeStyleToStr(AcDb::DuplicateRecordCloning type, CString& str)
{
	if (type == AcDb::kDrcNotApplicable)
		str = _T("Not Applicable");
	else if (type == AcDb::kDrcIgnore)
		str = _T("Ignore");
	else if (type == AcDb::kDrcReplace)
		str = _T("Replace");
	else if (type == AcDb::kDrcXrefMangleName)
		str = _T("Xref Mangle Name");
	else if (type == AcDb::kDrcMangleName)
		str = _T("Mangle Name");
	else if (type == AcDb::kDrcUnmangleName)
		str = _T("Unmangle Name");
	else {
		//ASSERT(0);
		str.Format(_T("*Unknown* (%d)"), type);
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::standardScaleTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::standardScaleTypeToStr(AcDbViewport::StandardScaleType type, CString& str)
{
	if (type == AcDbViewport::kScaleToFit)
		str = _T("Scaled To Fit");
	else if (type == AcDbViewport::kCustomScale)
		str = _T("Custom Scale");
	else if (type == AcDbViewport::k1_1)
		str = _T("1:1");
	else if (type == AcDbViewport::k1_2)
		str = _T("1:2");
	else if (type == AcDbViewport::k1_4)
		str = _T("1:4");
	else if (type == AcDbViewport::k1_8)
		str = _T("1:8");
	else if (type == AcDbViewport::k1_10)
		str = _T("1:10");
	else if (type == AcDbViewport::k1_16)
		str = _T("1:16");
	else if (type == AcDbViewport::k1_20)
		str = _T("1:20");
	else if (type == AcDbViewport::k1_30)
		str = _T("1:30");
	else if (type == AcDbViewport::k1_40)
		str = _T("1:40");
	else if (type == AcDbViewport::k1_50)
		str = _T("1:50");
	else if (type == AcDbViewport::k1_100)
		str = _T("1:100");
	else if (type == AcDbViewport::k2_1)
		str = _T("2:1");
	else if (type == AcDbViewport::k4_1)
		str = _T("4:1");
	else if (type == AcDbViewport::k8_1)
		str = _T("8:1");
	else if (type == AcDbViewport::k10_1)
		str = _T("10:1");
	else if (type == AcDbViewport::k100_1)
		str = _T("100:1");
	else if (type == AcDbViewport::k1_128in_1ft)
		str = _T("1/128\"= 1'");
	else if (type == AcDbViewport::k1_64in_1ft)
		str = _T("1/64\"= 1'");
	else if (type == AcDbViewport::k1_32in_1ft)
		str = _T("1/32\"= 1'");
	else if (type == AcDbViewport::k1_16in_1ft)
		str = _T("1/16\"= 1'");
	else if (type == AcDbViewport::k3_32in_1ft)
		str = _T("3/32\"= 1'");
	else if (type == AcDbViewport::k1_8in_1ft)
		str = _T("1/8\" = 1'");
	else if (type == AcDbViewport::k3_16in_1ft)
		str = _T("3/16\"= 1'");
	else if (type == AcDbViewport::k1_64in_1ft)
		str = _T("1/64\" = 1'");
	else if (type == AcDbViewport::k1_4in_1ft)
		str = _T("1/4\" = 1'");
	else if (type == AcDbViewport::k3_8in_1ft)
		str = _T("3/8\" = 1'");
	else if (type == AcDbViewport::k1_2in_1ft)
		str = _T("1/2\" = 1'");
	else if (type == AcDbViewport::k3_4in_1ft)
		str = _T("3/4\" = 1'");
	else if (type == AcDbViewport::k1in_1ft)
		str = _T("1\"= 1'");
	else if (type == AcDbViewport::k3in_1ft)
		str = _T("3\"= 1'");
	else if (type == AcDbViewport::k6in_1ft)
		str = _T("6\"= 1'");
	else if (type == AcDbViewport::k1ft_1ft)
		str = _T("1'= 1'");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::dwgVersionToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::dwgVersionToStr(AcDb::AcDbDwgVersion type, CString& str)
{
	CString verStr;

	if (type == AcDb::kDHL_MC0_0)
		verStr = _T("kDHL_MC0_0");
	else if (type == AcDb::kDHL_AC1_2)
		verStr = _T("kDHL_AC1_2");
	else if (type == AcDb::kDHL_AC1_40)
		verStr = _T("kDHL_AC1_40");
	else if (type == AcDb::kDHL_AC1_50)
		verStr = _T("kDHL_AC1_50");
	else if (type == AcDb::kDHL_AC2_20)
		verStr = _T("kDHL_AC2_20");
	else if (type == AcDb::kDHL_AC2_10)
		verStr = _T("kDHL_AC2_10");
	else if (type == AcDb::kDHL_AC2_21)
		verStr = _T("kDHL_AC2_21");
	else if (type == AcDb::kDHL_AC2_22)
		verStr = _T("kDHL_AC2_22");
	else if (type == AcDb::kDHL_1001)
		verStr = _T("kDHL_1001");
	else if (type == AcDb::kDHL_1002)
		verStr = _T("kDHL_1002  (AutoCAD 2.5)");
	else if (type == AcDb::kDHL_1003)
		verStr = _T("kDHL_1003  (AutoCAD 2.6)");
	else if (type == AcDb::kDHL_1004)
		verStr = _T("kDHL_1004  (Release 9)");
	else if (type == AcDb::kDHL_1005)
		verStr = _T("kDHL_1005");
	else if (type == AcDb::kDHL_1006)
		verStr = _T("kDHL_1006  (Release 10)");
	else if (type == AcDb::kDHL_1007)
		verStr = _T("kDHL_1007");
	else if (type == AcDb::kDHL_1008)
		verStr = _T("kDHL_1008");
	else if (type == AcDb::kDHL_1009)
		verStr = _T("kDHL_1009  (R11 and R12)");
	else if (type == AcDb::kDHL_1010)
		verStr = _T("kDHL_1010");
	else if (type == AcDb::kDHL_1011)
		verStr = _T("kDHL_1011");
	else if (type == AcDb::kDHL_1012)
		verStr = _T("kDHL_1012  (Release 13)");
	else if (type == AcDb::kDHL_1013)
		verStr = _T("kDHL_1013  (R14 Mid Version)");
	else if (type == AcDb::kDHL_1014)
		verStr = _T("kDHL_1014  (R14 Final Version)");
	else if (type == AcDb::kDHL_1500)
		verStr = _T("kDHL_1500  (R2000 Development)");
	else if (type == AcDb::kDHL_1015)
		verStr = _T("kDHL_1015  (R2000 Final Version)");
	else if (type == AcDb::kDHL_1800a)
		verStr = _T("kDHL_1800a (R2004 Development Version)");
	else if (type == AcDb::kDHL_1800)
		verStr = _T("kDHL_1800 (R2004  Final Version)");
	else if (type == AcDb::kDHL_2100a)
		verStr = _T("kDHL_2100a (R2007 Development Version)");
	else if (type == AcDb::kDHL_1021)
		verStr = _T("kDHL_1021 (R2007 Final Version)");
	else if (type == AcDb::kDHL_2400a)
		verStr = _T("kDHL_2400a (R2010 Development Version)");
	else if (type == AcDb::kDHL_1024)
		verStr = _T("kDHL_1024 (R2010 Final Version)");
	else {
		ASSERT(0);
		ASSERT(AcDb::kDHL_Newest == AcDb::kDHL_2400a); // update needed above
		verStr = _T("*Unknown*");
	}

	str.Format(_T("%d = %s"), type, verStr);
	return str;
}

/***********************************************************************
**
**  MyTransFunc::orthoViewToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::orthoViewToStr(AcDb::OrthographicView type, CString& str)
{
	if (type == AcDb::kNonOrthoView)
		str = _T("Non Ortho");
	else if (type == AcDb::kTopView)
		str = _T("Top");
	else if (type == AcDb::kBottomView)
		str = _T("Bottom");
	else if (type == AcDb::kFrontView)
		str = _T("Front");
	else if (type == AcDb::kBackView)
		str = _T("Back");
	else if (type == AcDb::kLeftView)
		str = _T("Left");
	else if (type == AcDb::kRightView)
		str = _T("Right");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::renderModeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::renderModeToStr(AcDbAbstractViewTableRecord::RenderMode type, CString& str)
{
	if (type == AcDbAbstractViewTableRecord::k2DOptimized)
		str = _T("2D Optimized");
	else if (type == AcDbAbstractViewTableRecord::kWireframe)
		str = _T("Wireframe");
	else if (type == AcDbAbstractViewTableRecord::kHiddenLine)
		str = _T("Hidden Line");
	else if (type == AcDbAbstractViewTableRecord::kFlatShaded)
		str = _T("Flat Shaded");
	else if (type == AcDbAbstractViewTableRecord::kGouraudShaded)
		str = _T("Gouraud Sahded");
	else if (type == AcDbAbstractViewTableRecord::kFlatShadedWithWireframe)
		str = _T("Flat Shaded With Wireframe");
	else if (type == AcDbAbstractViewTableRecord::kGouraudShadedWithWireframe)
		str = _T("Gouraud Shaded With Wireframe");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::measurementTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::measurementTypeToStr(AcDb::MeasurementValue type, CString& str)
{
	if (type == AcDb::kEnglish)
		str = _T("English");
	else if (type == AcDb::kMetric)
		str = _T("Metric");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::unitTypeToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::unitTypeToStr(AcDb::UnitsValue type, CString& str)
{
	if (type == AcDb::kUnitsUndefined)
		str = _T("Undefined");
	else if (type == AcDb::kUnitsInches)
		str = _T("Inches");
	else if (type == AcDb::kUnitsFeet)
		str = _T("Feet");
	else if (type == AcDb::kUnitsMiles)
		str = _T("Miles");
	else if (type == AcDb::kUnitsMillimeters)
		str = _T("Millimeters");
	else if (type == AcDb::kUnitsCentimeters)
		str = _T("Centimeters");
	else if (type == AcDb::kUnitsMeters)
		str = _T("Meters");
	else if (type == AcDb::kUnitsKilometers)
		str = _T("Kilometers");
	else if (type == AcDb::kUnitsMicroinches)
		str = _T("Microinches");
	else if (type == AcDb::kUnitsMils)
		str = _T("Mils");
	else if (type == AcDb::kUnitsYards)
		str = _T("Yards");
	else if (type == AcDb::kUnitsAngstroms)
		str = _T("Angstroms");
	else if (type == AcDb::kUnitsNanometers)
		str = _T("Nanometers");
	else if (type == AcDb::kUnitsMicrons)
		str = _T("Microns");
	else if (type == AcDb::kUnitsDecimeters)
		str = _T("Decimeters");
	else if (type == AcDb::kUnitsDekameters)
		str = _T("Dekameters");
	else if (type == AcDb::kUnitsHectometers)
		str = _T("Hectometers");
	else if (type == AcDb::kUnitsGigameters)
		str = _T("Gigameters");
	else if (type == AcDb::kUnitsAstronomical)
		str = _T("Astronomical");
	else if (type == AcDb::kUnitsLightYears)
		str = _T("Light Years");
	else if (type == AcDb::kUnitsParsecs)
		str = _T("Parsecs");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/****************************************************************************
**
**  MyTransFunc::xrefStatusToSTr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::xrefStatusToStr(AcDb::XrefStatus stat, CString& str)
{
	if (stat == AcDb::kXrfNotAnXref)
		str = _T("Not An Xref");
	else if (stat == AcDb::kXrfResolved)
		str = _T("Xref Resolved");
	else if (stat == AcDb::kXrfUnloaded)
		str = _T("Xref Unloaded");
	else if (stat == AcDb::kXrfUnreferenced)
		str = _T("Xref Unreferenced");
	else if (stat == AcDb::kXrfFileNotFound)
		str = _T("Xref File Not Found");
	else if (stat == AcDb::kXrfUnresolved)
		str = _T("Xref Unresolved");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/****************************************************************************
**
**  MyTransFunc::saveFormatToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::saveFormatToStr(AcApDocument::SaveFormat type, CString& str)
{
	if (type == AcApDocument::kUnknown)
		str = _T("Invalid");
	else if (type == AcApDocument::kR12_dxf)
		str = _T("AutoCAD R12/LT2 DXF (*.dxf)");
	else if (type == AcApDocument::kR13_dwg)
		str = _T("AutoCAD R13/LT95 Drawing (*.dwg)");
	else if (type == AcApDocument::kR13_dxf)
		str = _T("AutoCAD R13/LT95 DXF (*.dxf)");
	else if (type == AcApDocument::kR14_dwg)
		str = _T("AutoCAD R14/LT97 Drawing (*.dwg)");
	else if (type == AcApDocument::kR14_dxf)
		str = _T("AutoCAD R14/LT97 DXF (*.dxf)");
	else if (type == AcApDocument::k2000_dwg)
		str = _T("AutoCAD 2000 Drawing (*.dwg)");
	else if (type == AcApDocument::k2000_dxf)
		str = _T("AutoCAD 2000 DXF (*.dxf)");
	else if (type == AcApDocument::k2000_Template)
		str = _T("AutoCAD 2000 Drawing Template File (*.dwt)");
	else if (type == AcApDocument::k2004_dwg)
		str = _T("AutoCAD 2004 Drawing (*.dwg)");
	else if (type == AcApDocument::k2004_dxf)
		str = _T("AutoCAD 2004 DXF (*.dxf)");
	else if (type == AcApDocument::k2004_Template)
		str = _T("AutoCAD 2004 Drawing Template File (*.dwt)");
	else if (type == AcApDocument::k2007_dwg)
		str = _T("AutoCAD 2007 Drawing (*.dwg)");
	else if (type == AcApDocument::k2007_dxf)
		str = _T("AutoCAD 2007 DXF (*.dxf)");
	else if (type == AcApDocument::k2007_Template)
		str = _T("AutoCAD 2007 Drawing Template File (*.dwt)");
	else if (type == AcApDocument::k2010_dwg)
		str = _T("AutoCAD 2010 Drawing (*.dwg)");
	else if (type == AcApDocument::k2010_dxf)
		str = _T("AutoCAD 2010 DXF (*.dxf)");
	else if (type == AcApDocument::k2010_Template)
		str = _T("AutoCAD 2010 Drawing Template File (*.dwt)");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/****************************************************************************
**
**  MyTransFunc::longTransactionTypeToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::longTransactionTypeToStr(int type, CString& str)
{
	if (type == AcDbLongTransaction::kSameDb)
		str = _T("Same Database");
	else if (type == AcDbLongTransaction::kXrefDb)
		str = _T("Xref Database");
	else if (type == AcDbLongTransaction::kUnrelatedDb)
		str = _T("Unrelated Database");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/****************************************************************************
**
**  MyTransFunc::docLockModeToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::docLockModeToStr(AcAp::DocLockMode mode, CString& str)
{
	str.Empty();

	if (mode & AcAp::kNone)
		str += _T("None");

	if (mode & AcAp::kAutoWrite) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("Auto Write");
	}

	if (mode & AcAp::kNotLocked) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("Not Locked");
	}

	if (mode & AcAp::kWrite) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("Write");
	}

	if (mode & AcAp::kProtectedAutoWrite) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("Protected Auto Write");
	}

	if (mode & AcAp::kRead) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("Read");
	}

	if (mode & AcAp::kXWrite) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("X Write");
	}


	return str;
}

/***********************************************************************
**
**  MyTransFunc::deepCloneContextToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::deepCloneContextToStr(AcDb::DeepCloneType context, CString& str)
{
	if (context == AcDb::kDcCopy)
		str = _T("Copy");
	else if (context == AcDb::kDcExplode)
		str = _T("Explode");
	else if (context == AcDb::kDcBlock)
		str = _T("Block");
	else if (context == AcDb::kDcSymTableMerge)
		str = _T("Sym Tbl Merge");
	else if (context == AcDb::kDcInsert)
		str = _T("Insert");
	else if (context == AcDb::kDcInsertCopy)
		str = _T("Insert Copy");
	else if (context == AcDb::kDcWblock)
		str = _T("Wblock");
	else if (context == AcDb::kDcObjects)
		str = _T("Objects");
	else if (context == AcDb::kDcXrefInsert)
		str = _T("XRef Insert");
	else if (context == AcDb::kDcXrefBind)
		str = _T("Xref Bind");
	else if (context == AcDb::kDcWblkObjects)
		str = _T("Wblock Objects");
	else {
		ASSERT(0);
		str = _T("*Unknown*");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::dateToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::dateToStr(const AcDbDate& date, CString& str)
{
	str.Format(_T("%d Hr., %d Min., %d Sec, %d Msec"),
		date.hour(), date.minute(), date.second(), date.millisecond());
	return str;
}

/***********************************************************************
**
**  MyTransFunc::promptStatusToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::promptStatusToStr(Acad::PromptStatus stat, CString& str)
{
	if (stat == Acad::eNone)
		str = _T("None");
	else if (stat == Acad::eNormal)
		str = _T("Normal");
	else if (stat == Acad::eError)
		str = _T("Error");
	else if (stat == Acad::eCancel)
		str = _T("Cancel");
	else if (stat == Acad::eRejected)
		str = _T("Rejected");
	else if (stat == Acad::eFailed)
		str = _T("Failed");
	else if (stat == Acad::eKeyword)
		str = _T("Keyword");
	else if (stat == Acad::eDirect)
		str = _T("Direct");
	else {
		ASSERT(0);
		str.Format(_T("*Unknown* (%d)"), stat);
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::initFlagsToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::initFlagsToStr(int initFlags, CString& str)
{
	str.Empty();

	if (initFlags & RSG_NONULL)
		str += _T("NoNull");

	if (initFlags & RSG_NOZERO) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("NoZero");
	}

	if (initFlags & RSG_NONEG) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("NoNeg");
	}

	if (initFlags & RSG_NOLIM) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("NoLim");
	}

	if (initFlags & RSG_GETZ) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("GetZ");
	}

	if (initFlags & RSG_DASH) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("Dash");
	}

	if (initFlags & RSG_2D) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("2D");
	}

	if (initFlags & RSG_OTHER) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("Other");
	}

	if (initFlags & RSG_DDISTFIRST) {
		if (str.IsEmpty() == false)
			str += _T(" | ");
		str += _T("DdDistFirst");
	}

	return str;
}

/***********************************************************************
**
**  MyTransFunc::promptStatusToStr
**
**  
**
****************************************/

LPCTSTR
MyTransFunc::filerTypeToStr(AcDb::FilerType type, CString& str)
{
	if (type == AcDb::kFileFiler)
		str = _T("File");
	else if (type == AcDb::kCopyFiler)
		str = _T("Copy");
	else if (type == AcDb::kUndoFiler)
		str = _T("Undo");
	else if (type == AcDb::kBagFiler)
		str = _T("Bag");
	else if (type == AcDb::kIdXlateFiler)
		str = _T("IdXlate");
	else if (type == AcDb::kPageFiler)
		str = _T("Page");
	else if (type == AcDb::kIdFiler)
		str = _T("Id");
	else if (type == AcDb::kDeepCloneFiler)
		str = _T("DeepClone");
	else if (type == AcDb::kPurgeFiler)
		str = _T("Purge");
	else if (type == AcDb::kWblockCloneFiler)
		str = _T("WblockClone");
	else {
		ASSERT(0);
		str.Format(_T("*Unknown* (%d)"), type);
	}

	return str;
}

/****************************************************************************
**
**  MyTransFunc::osnapToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::osnapToStr(int osnapMode, CString& str)
{
	if (osnapMode == AcDb::kOsModeEnd)
		str = _T("End");
	else if (osnapMode == AcDb::kOsModeMid)
		str = _T("Mid");
	else if (osnapMode == AcDb::kOsModeCen)
		str = _T("Cen");
	else if (osnapMode == AcDb::kOsModeNode)
		str = _T("Node");
	else if (osnapMode == AcDb::kOsModeQuad)
		str = _T("Quad");
	else if (osnapMode == AcDb::kOsModeIns)
		str = _T("Ins");
	else if (osnapMode == AcDb::kOsModePerp)
		str = _T("Perp");
	else if (osnapMode == AcDb::kOsModeTan)
		str = _T("Tan");
	else if (osnapMode == AcDb::kOsModeNear)
		str = _T("Near");
	else
		str.Format(_T("*Unknown* (%d)"), osnapMode);

	return str;
}

/****************************************************************************
**
**  MyTransFunc::doubleArray3dToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::doubleArray3dToStr(double array[], CString& str)
{
	str.Format(_T("(%.15g, %.15g, %.15g)"), array[0], array[1], array[2]);
	return str;
}

/****************************************************************************
**
**  MyTransFunc::doubleArray2dToStr
**
**  
**
*************************************/

LPCTSTR
MyTransFunc::doubleArray2dToStr(double array[], CString& str)
{
	str.Format(_T("(%.15g, %.15g)"), array[0], array[1]);
	return str;
}


int MyTransFunc::StringToInt(CString str)
{
	int nRet = 0;
//#ifdef UNICODE
	nRet = _ttoi(str);
//#else
//	nRet = atoi(str);
//#endif
	return nRet;
}

double MyTransFunc::StringToDouble(CString str)
{
	double dRet =0.0;
//#ifdef UNICODE
	dRet = _tcstod(str, 0);
//#else
//	dRet = atof(str);
//#endif
	return dRet;
}

