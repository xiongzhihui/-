/******************************************************************************
PUBLIC FUNCTION FORWARD DECLARATIONS
*****************************************************************************/

#include "stdafx.h"           // MFC stuff
#include <math.h>
#include "Mydrag.h"
#include "MyBaseUtils.h"

static int        g_nKwIndex = NOP;              // Index into kwtab
//int DragMode;
static ads_point  BasePoint,OldCur;     // BasePoint point for xform
static ads_real   scr_h;                      // current vport height
//static BOOL bLine = false;
/******************************************************************************
Set up an identity matrix.
******************************************************************************/
MyDrag::MyDrag(void)
{

}

MyDrag::~MyDrag()
{

}

void MyDrag::ads_mat_ident(ads_matrix matrix)
{
  int i,j;
	for(i=0;i<=3;i++)
		for (j=0;j<=3;j++)
		matrix[i][j]=0.0;
	for(i=0;i<=3;i++)
		matrix[i][i]=1.0;
}
/******************************************************************************
Subtract two ads_points.
******************************************************************************/
void MyDrag::ads_subvec(ads_point ap, ads_point bp, ads_point dp)
{
  dp[X] = ap[X] - bp[X];
  dp[Y] = ap[Y] - bp[Y];
  dp[Z] = ap[Z] - bp[Z];
}

/******************************************************************************
Get normal of a vector.
*******************************************************************************/
ads_real MyDrag::ads_fabsv(ads_point ap)
{
  return sqrt(ap[X] * ap[X] + ap[Y] * ap[Y] + ap[Z] * ap[Z]);
}

/******************************************************************************
Calculate distance between two points.
*******************************************************************************/
ads_real MyDrag::ads_dist(ads_point p1, ads_point p2)
{
  ads_point pd;
  
  ads_subvec(p1, p2, pd);
  return ads_fabsv(pd);
}

/******************************************************************************
Generate a matrix that rotates about a given axis.
******************************************************************************/
void MyDrag::ads_mat_rot(ads_real angle, int axis, ads_matrix m)
{
  int axp1, axp2;
  
  axp1 = (axis + 1) % 3;
  axp2 = (axis + 2) % 3;
  ads_mat_ident(m);
  m[axp1][axp1] = m[axp2][axp2] = cos(angle);
  m[axp1][axp2] = -(m[axp2][axp1] = sin(angle));
}

/******************************************************************************
Generate a matrix that scales the 3 axes by given amounts.
*******************************************************************************/
void MyDrag::ads_mat_scale(ads_real xscale, ads_real yscale, ads_real zscale,
                          ads_matrix m)
{
  ads_mat_ident(m);
  m[X][X] = xscale;
  m[Y][Y] = yscale;
  m[Z][Z] = zscale;
}

/******************************************************************************
Multiply matrix by a given vector.  Note that it does NO translation.
******************************************************************************/
void MyDrag::ads_mat_x_vec(ads_matrix mat, ads_point pin, ads_point pout)
{
  int i;
  ads_point temp;
  
  for (i = X; i <= Z; i++)
    temp[i] = mat[i][X] * pin[X] + mat[i][Y] * pin[Y] + mat[i][Z] * pin[Z];
  ads_point_set(temp,pout);
}

//将pt投影到pt1和pt2决定的线上
BOOL MyDrag::getProjectedPt(ads_point pt, ads_point pt1,ads_point pt2,ads_point result)
{
	double angle= ads_angle(pt1,pt2);

	ads_point to1;
	ads_polar(pt,angle+PI/2,100,to1);

	BOOL boo;
	int rc=ads_inters(pt,to1,pt1,pt2,0,result);
	if(rc!=RTNORM)
		boo=FALSE;
	else
		boo=TRUE;

	return boo;
}
/******************************************************************************
ads_DRAGGEN sampler function.  This function is called every time the
digitizer is moved.  It receives two arguments, the first is pt, and
the second is mat.  The argument pt is the current digitizer location,
and the arguemnt mat is the transformation matrix which is being used
by AutoCAD to display the current selection set of objects.

  The global g_nKwIndex is an integer variable which contains a code
  representing the current drag mode.  The possible values are MXYZ,
  SXYZ, RX, RY, RZ, NOP, and EXIT.  This variable is set in the function
  testdraggen.
  
    The global CurMatrix is set to the value of the argument mat before
    returning from the dragsampler.  This variable is used by testdraggen
    when performing an ads_XFORSS call.
******************************************************************************/
int MyDrag::DragFunc(ads_point pt, ads_matrix mat)
{
  MyBaseUtils utils;
  ads_real        D;
  ads_real        angle;
  ads_point       tp;
  int             axis;

  ads_point_set(BasePoint,OldCur);
  switch( g_nKwIndex ) {
  case MXYZ: // generate translation matrix
	  ads_subvec(pt, BasePoint, tp);
	  ads_mat_x_vec(mat, tp, tp);
	  mat[X][T] = tp[X];
	  mat[Y][T] = tp[Y];
	  mat[Z][T] = tp[Z];

	  break;
  case SXYZ: // generate uniformly scaled matrix
	  D = ads_dist(pt, BasePoint)/scr_h*10;
	  // SCALE = DIST / (SCR_H/10)
	  if (D < .0001) D = .0001;
	  ads_mat_scale(D, D, D, mat);

	  // calculate new translation point
	  ads_mat_x_vec(mat, BasePoint, tp);
	  ads_subvec(BasePoint, tp, tp);
	  mat[X][T] = tp[X];
	  mat[Y][T] = tp[Y];
	  mat[Z][T] = tp[Z];
	  break;

  case RX:
  case RY:
  case RZ:
	  switch (g_nKwIndex) {
  case RX:
	  axis = X;
	  break;
  case RY:
	  axis = Y;
	  break;
  case RZ:
	  axis = Z;
	  break;
	  }

	  // calculate angle of line between LastPoint and tp
	  ads_subvec(pt, BasePoint, tp);
	  angle = ads_angle(BasePoint, pt);

	  //[对旋转点进行修正 modified by lwx at 2005-6-17
	  int bOrtho;
	  utils.GetVar(_T("orthomode"),&bOrtho);
	  if(bOrtho==1)
	  {
		  ads_point projectpt1,projectpt2;
		  ads_point result1,result2;
		  double dDist1,dDist2;
		  ads_polar(BasePoint,0,100,projectpt1);
		  ads_polar(BasePoint,PI/2,100,projectpt2);
		  getProjectedPt(pt,BasePoint,projectpt1,result1);
		  getProjectedPt(pt,BasePoint,projectpt2,result2);
		  dDist1=ads_distance(pt,result1);
		  dDist2=ads_distance(pt,result2);
		  if(dDist1<dDist2)
		  {
			  angle = ads_angle(BasePoint, result1);
		  }
		  else
		  {
			  angle = ads_angle(BasePoint, result2);
		  }
	  }
	  //]对旋转点进行修正 modified by lwx at 2005-6-17
	  // generate rotation matrix about axis
	  ads_mat_rot(angle, axis, mat);

	  // calculate new translation point
	  ads_mat_x_vec(mat, BasePoint, tp);
	  ads_subvec(BasePoint, tp, tp);
	  mat[X][T] = tp[X];
	  mat[Y][T] = tp[Y];
	  mat[Z][T] = tp[Z];

	  break;
  case EXIT:
	  return RTCAN;
  case NOP:
  default:
	  return RTNONE;
	}
	ads_point_set(pt,OldCur);
	return RTNORM;
}

/******************************************************************************
move objects on XY-plan
ssname -- the objects
bpt -- the base point
res -- the final object point(result)
return:
RTNORM -- normal case
RTCAN  -- user press cancel
RTERROR -- error detected
*******************************************************************************/
int MyDrag::DragMove(CString Prompt, ads_name ssname, ads_point bpt, ads_point rPoint)
{
  int         retVal, ret_tag, i, CursorMode;
  long        slen=0;
  ads_name    ett;
  ads_matrix  CurMatrix;
  
  ads_mat_ident(CurMatrix);
  CursorMode = 0;
  
  g_nKwIndex = MXYZ; //NOP;
  retVal = ads_sslength(ssname, &slen);
  if (retVal != RTNORM || slen == 0L)
    return RTERROR;
  
  //Sleep(10000);
  acTransactionManagerPtr()->queueForGraphicsFlush();
  actrTransactionManager->flushGraphics();
  for(i=0; i<slen; i++)
  {
	  ads_ssname(ssname, i, ett);
	  ads_redraw(ett, 2);
  }
  
  ads_point_set(bpt, BasePoint);
  
  //  if (bLine) ads_grdraw(BasePoint, OldCur, -1 , 1);
  retVal = ads_draggen(ssname, Prompt, CursorMode, DragFunc, rPoint);
  //  if (bLine) ads_grdraw(BasePoint, rPoint, -1 , 1);
  
  switch ( retVal )
  {
  case RTNORM:  
  case RTNONE:
	DragFunc(rPoint,CurMatrix);
	if (ads_xformss(ssname, CurMatrix)!= RTNORM )
      ads_command(RTSTR, _T("MOVE"), RTPICKS, ssname, RTSTR, _T(""), RT3DPOINT, bpt, RT3DPOINT, rPoint, RTNONE);
    ret_tag = RTNORM;
    break;
  case RTCAN:
	  ret_tag = retVal;
	  break;
  case RTSTR:
  case RTKWORD:
    ret_tag = retVal;
	ads_point_set(OldCur,rPoint);
	DragFunc(rPoint,CurMatrix);
	ads_xformss(ssname, CurMatrix);
    break;
    
  case RTERROR:
  default:
    ret_tag = RTERROR;
    break;
  }
  
  return ret_tag;
}

/*******************************************************************************
Scale objects on XY-plan
ssname -- the objects
bpt -- the base point
rPoint -- the final scale(result)
return:
RTNORM -- normal case
RTCAN  -- user press cancel
RTERROR -- error detected
*******************************************************************************/
int MyDrag::DragScale(CString Prompt, ads_name ssname, ads_point bpt, ads_point rPoint)
{
  int         retVal, ret_tag, i, CursorMode;
  long        slen=0;
  //  char        tstr[512];
  struct resbuf viewsize;
  ads_real    rReal;
  ads_name    ett;
  ads_matrix CurMatrix;
  
  ads_mat_ident(CurMatrix);
  CursorMode = 0;
  ads_getvar(_T("VIEWSIZE"), &viewsize);
  scr_h = viewsize.resval.rreal;
  g_nKwIndex = SXYZ; //NOP;
  retVal = ads_sslength(ssname, &slen);
  if (retVal != RTNORM || slen == 0L) {
    return RTERROR;
  }

  //  actrTransactionManager->flushGraphics();
  for(i=0; i<slen; i++) {
    ads_ssname(ssname, i, ett);
    ads_redraw(ett, 2);
  }
  
  ads_point_set(bpt, BasePoint);
  //  SET_PT(OldCur, bpt);
  
  //  if (bLine) ads_grdraw(BasePoint, OldCur, -1 , 1);
  retVal = ads_draggen(ssname, Prompt, CursorMode, DragFunc, rPoint);
  //  if (bLine) ads_grdraw(BasePoint, rPoint, -1 , 1);
  
  switch ( retVal ) {
  case RTNORM:
    ret_tag = retVal;
    rReal = ads_distance(bpt, rPoint)/scr_h*10.0;
	DragFunc(rPoint,CurMatrix);
    if(ads_xformss(ssname, CurMatrix)!=RTNORM )
      ads_command(RTSTR, _T("SCALE"), RTPICKS, ssname, RTSTR, _T(""), RT3DPOINT, bpt, RTREAL, rReal, RTNONE);
    break;
  case RTCAN:
  case RTNONE:
	  ret_tag = retVal;
	  break;
  case RTSTR:
  case RTKWORD:
    ret_tag = retVal;
	ads_point_set(OldCur,rPoint);
	DragFunc(rPoint,CurMatrix);
	ads_xformss(ssname, CurMatrix);
    break;
    
  case RTERROR:
  default:
    ret_tag = RTERROR;
    break;
  }
  return ret_tag;
}

/*******************************************************************************
rotate objects on XY-plan around Z axis
ssname -- the objects
bpt -- the base point
rPoint -- the final rotate angle(result), 角度
return:
RTNORM -- normal case
RTCAN  -- user press cancel
RTERROR -- error detected
*******************************************************************************/
int MyDrag::DragRotate(CString Prompt, ads_name ssname, ads_point bpt, ads_point rPoint)
{
  int         retVal, ret_tag, i, CursorMode;
  long        slen=0;
  //  char        tstr[512];
  ads_name    ett;
  ads_real    rReal;
  ads_matrix CurMatrix;
  ads_mat_ident(CurMatrix);
  
  CursorMode = 0;
  g_nKwIndex = RZ; //NOP;
  retVal = ads_sslength(ssname, &slen);
  if (retVal != RTNORM || slen == 0L) {
    return RTERROR;
  }

  //  actrTransactionManager->flushGraphics();
  for(i=0; i<slen; i++) {
    ads_ssname(ssname, i, ett);
    ads_redraw(ett, 2);
  }
  
  ads_point_set(bpt, BasePoint);
  //  SET_PT(OldCur, bpt);
  
  //  if (bLine) ads_grdraw(BasePoint, OldCur, -1 , 1);
  retVal = ads_draggen(ssname, Prompt, CursorMode, DragFunc, rPoint);
  //  if (bLine) ads_grdraw(BasePoint, rPoint, -1 , 1);
  
  switch (retVal) {
  case RTNORM:
    ret_tag = retVal;
    rReal = ads_angle(bpt, rPoint)*180.0/3.14159;
	DragFunc(rPoint,CurMatrix);
    if(ads_xformss(ssname, CurMatrix)!=RTNORM )
      ads_command(RTSTR, _T("ROTATE"), RTPICKS, ssname, RTSTR, _T(""), RT3DPOINT, bpt, RTREAL, rReal, RTNONE);
    break;
  case RTCAN:
  case RTNONE:
	  ret_tag = retVal;
	  break;
  case RTSTR:
  case RTKWORD:
    ret_tag = retVal;
	ads_point_set(OldCur,rPoint);
	DragFunc(rPoint,CurMatrix);
	ads_xformss(ssname, CurMatrix);
    break;
    
  case RTERROR:
  default:
    ret_tag = RTERROR;
    break;
  }
  
  return ret_tag;
}

/////////////////////////////////////////////////////////////////////////////
//功  能：拖动
//参  数：选择集，基点，返回的点, 拖动的方式
//返  回：-2取消-1失败 0回车 1返回点 2返回字符
int MyDrag::Draggen(CString Prompt, ads_name ss, ads_point pt, 
						ads_point PtRet, CString str, int ret_tag)
{
	//	gCyz_OsnapOn();
	switch(ret_tag)
	{
	case -1:
		ret_tag = DragMove(Prompt, ss, pt, PtRet);
		break;
	case 1:
		ret_tag = DragScale(Prompt, ss, pt, PtRet);
		break;
	default:
		ret_tag = DragRotate(Prompt, ss, pt, PtRet);
		break;
	}
	//	gCyz_OsnapOff();

	int fh = -1;
	TCHAR szTmp[32];
	switch (ret_tag)
	{
	case RTCAN:
		//ads_command(RTSTR, _T(".erase"), RTPICKS, ss, RTSTR, _T(""), NULL);
		//ads_ssfree(ss);
		fh = -2;
		break;
	case RTNONE:
		//ads_command(RTSTR, _T(".erase"), RTPICKS, ss, RTSTR, _T(""), NULL);
		//ads_ssfree(ss);
		fh = 0;
		break;
	case RTSTR:
	case RTKWORD:
		if(ads_getinput(szTmp) == RTNORM)
		{
			str = szTmp;
			fh = 2;
		}
		else
		{
			ads_command(RTSTR, _T(".erase"), RTPICKS, ss, RTSTR, _T(""), NULL);
			ads_ssfree(ss);
			fh = 0;
		}
		break;
	default:
		fh = 1;
		break;
	}
	return fh;
}
/////////////////////////////////////////////////////////////////////////////

BOOL MyDrag::MoveStuff(ads_name ssname,ads_point moveVec)
{
	ads_matrix  CurMatrix;
	ads_mat_ident(CurMatrix);
    ads_mat_x_vec(CurMatrix, moveVec, moveVec);
    CurMatrix[X][T] = moveVec[X];
    CurMatrix[Y][T] = moveVec[Y];
    CurMatrix[Z][T] = moveVec[Z];
	
	if (ads_xformss(ssname, CurMatrix)!= RTNORM )
		return FALSE;
		//		ads_command(RTSTR, "MOVE", RTPICKS, ssname, RTSTR, "", RT3DPOINT, bpt, RT3DPOINT, rPoint, RTNONE);
	
	return TRUE;
}

BOOL MyDrag::ScaleStuff(ads_name ssname,ads_point ptBase,double dScale)
{
	ads_point       tp;
	ads_matrix  CurMatrix;
	ads_mat_ident(CurMatrix);
	CurMatrix[X][X] = dScale;
	CurMatrix[Y][Y] = dScale;
	CurMatrix[Z][Z] = dScale;
	
    ads_mat_x_vec(CurMatrix, ptBase, tp);
	ads_subvec(ptBase, tp, tp);

    CurMatrix[X][T] = tp[X];
    CurMatrix[Y][T] = tp[Y];
    CurMatrix[Z][T] = tp[Z];
	
	if (ads_xformss(ssname, CurMatrix)!= RTNORM )
		return FALSE;
		//		ads_command(RTSTR, "MOVE", RTPICKS, ssname, RTSTR, "", RT3DPOINT, bpt, RT3DPOINT, rPoint, RTNONE);
	
	return TRUE;
}