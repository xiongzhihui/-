#ifndef __MYJ_DRAGGER_H__
#define __MYJ_DRAGGER_H__

#define MXYZ 0
#define SXYZ 1
#define RX   2
#define RY   3
#define RZ   4
#define NOP  5
#define EXIT 6
//#include <zds.h>
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

class SERVERDLL_API MyDrag
{
public:
	MyDrag(void);
	~MyDrag(void);
	
	static void ads_mat_ident(ads_matrix matrix);
	static void ads_subvec(ads_point ap, ads_point bp, ads_point dp);
	static ads_real ads_fabsv(ads_point ap);
	static ads_real ads_dist(ads_point p1, ads_point p2);
	static void ads_mat_rot(ads_real angle, int axis, ads_matrix m);
	static void ads_mat_scale(ads_real xscale, ads_real yscale, ads_real zscale, ads_matrix m);
	static void ads_mat_x_vec(ads_matrix mat, ads_point pin, ads_point pout);
	static int DragFunc(ads_point pt, ads_matrix mat);
	static int DragMove(CString Prompt, ads_name ssname, ads_point bpt, ads_point rPoint);
	static int DragScale(CString Prompt, ads_name ssname, ads_point bpt, ads_point rPoint);
	static int DragRotate(CString Prompt, ads_name ssname, ads_point bpt, ads_point rPoint);
	static int Draggen(CString Prompt, ads_name ss, ads_point pt, ads_point PtRet, CString str, int ret_tag=-1);
	static BOOL MoveStuff(ads_name ssname,ads_point moveVec);
	static BOOL ScaleStuff(ads_name ssname,ads_point ptBase,double dScale);
	static BOOL getProjectedPt(ads_point pt, ads_point pt1,ads_point pt2,ads_point result);
};
						

#endif	