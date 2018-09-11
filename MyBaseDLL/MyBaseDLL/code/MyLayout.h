#pragma once
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

class SERVERDLL_API MyLayout
{
public:
	MyLayout(void);
	~MyLayout(void);
	// --------------------------------------------------------------------------------------------
	// Summary: 绘制布局裁图
	// Parameters:
	//     pt1                -    裁图区域所在的外包最小点
	//     pt2                -    裁图区域所在的外包最大点
	//     ptInsertTopLeft    -    视口的左下脚点
	//     dScaleFactor        -    缩放比例
	//     dAngle                -    裁图角度
	//     pBoundary            -    裁图边界
	// Returns: 
	//     成功返回 Acad::eOk, 否则返回 Acad::ErrorStatus 错误状态
	// --------------------------------------------------------------------------------------------
	static Acad::ErrorStatus DrawLayout(const AcGePoint3d& pt1, const AcGePoint3d& pt2, const AcGePoint3d& ptInsertBottomLeft, LPCTSTR lpszLayoutName, double dScaleFactor, double dAngle, AcDbPolyline* pBoundary);

	// --------------------------------------------------------------------------------------------
	// Summary: 创建新布局
	// Parameters:
	//     lpszName        -    布局的名称
	//     layoutId        -    布局id
	//     blkTabRecId    -    布局块表记录
	// Returns: 
	//     成功返回 Acad::eOk, 否则返回 Acad::ErrorStatus 错误状态
	// --------------------------------------------------------------------------------------------
	static Acad::ErrorStatus CreateNewLayout(LPCTSTR lpszName, AcDbObjectId& layoutId, AcDbObjectId& blkTabRecId);

	// --------------------------------------------------------------------------------------------
	// Summary: 遍历所有布局
	// --------------------------------------------------------------------------------------------
	Acad::ErrorStatus AllLayout();

	// --------------------------------------------------------------------------------------------
	// Summary: 查找指定名称的布局
	// Parameters:
	//     lpszName        -    布局的名称
	//     layoutId        -    布局id
	//     blkTabRecId    -    布局块表记录
	// Returns: 
	//     成功返回 Acad::eOk, 否则返回 Acad::ErrorStatus 错误状态
	// --------------------------------------------------------------------------------------------
	static Acad::ErrorStatus FindLayout(LPCTSTR lpszName, AcDbObjectId& layoutId, AcDbObjectId& blkTabRecId);
};
