#pragma once
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

class SERVERDLL_API MyDrawEntity
{
public:
	MyDrawEntity(void);
	~MyDrawEntity(void);
	static AcDbObjectId DrawLine(AcGePoint3d ptStart, AcGePoint3d ptEnd, AcDbObjectId LayerId = AcDbObjectId::kNull);
	static AcDbObjectId DrawText(AcGePoint3d basePt, 
									CString strText, 
									double dHeight = 3.0,
									AcDbObjectId textStyleId = AcDbObjectId::kNull,
									AcDb::TextHorzMode HorMode = AcDb::kTextLeft, 
									AcDb::TextVertMode VerMode = AcDb::kTextBase);
	//根据给定的基点，文字内容和文字样式及文字附加模式绘制多行文字
	static AcDbObjectId DrawMText(AcGePoint3d basePt, CString strText, double dHeight = 3.0, 
									AcDbObjectId textStyleId = AcDbObjectId::kNull, 
									AcDbMText::AttachmentPoint attachMode = AcDbMText::kTopLeft);
	//绘制覆盖的多行文字
	static AcDbObjectId DrawMTextCover(AcGePoint3d basePt, CString strText, double dHeight = 3.0, 
										AcDbObjectId textStyleId = AcDbObjectId::kNull, 
										AcDbMText::AttachmentPoint attachMode = AcDbMText::kTopLeft);
	//static void DrawPolyline(AcGePoint3dArray ptArr);
	/*
		传入参数组名和选择集objIds，并将这些实体添加到一个组里面
		返回组的objectId
	*/
	static AcDbObjectId	MakeGroup(AcDbObjectIdArray& objIds, bool bIsSelectable = false, CString strGroupName = _T(""));
	//绘制圆
	static AcDbObjectId DrawCircle(AcGePoint3d cenPt, double dRadius, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//绘制圆弧,根据三点绘制圆弧
	static AcDbObjectId DrawSemiCircle(AcGePoint3d cenPt, double dRadius, double dRotate, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//通过圆心，半径及三点绘制圆弧
	static AcDbObjectId DrawArc(AcGePoint3d cenPt, double dRadius, AcGePoint3d startPt, AcGePoint3d endPt, 
									AcDbObjectId LayerId = AcDbObjectId::kNull);
	//通过圆心，半径及起始弧度和终止弧度绘制圆弧
	static AcDbObjectId DrawArc(AcGePoint3d cenPt, double dRadius, double startAng, double endAng, 
									int nColor = 5, AcDbObjectId LayerId = AcDbObjectId::kNull);

	static AcDbObjectId DrawEllipse(AcGePoint3d cenPt, AcGeVector3d unitVec, AcGeVector3d majorAxis, double dRadio);
	//根据给定的文件路径和插入点插入块
	static AcDbObjectId InsertBlkRef(CString strFilePathName, AcGePoint3d ptInsert, double inputscale =1 ,
										double inputrotation =0.0 , AcDbObjectId LayerId = AcDbObjectId::kNull);
	//根据给定的文件路径和插入点得到块参照id
	static AcDbObjectId GetBlkRef(CString strFilePathName);
	//根据两点绘制多段线
	static AcDbObjectId DrawPlineByTwoPoint(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//根据点组绘制多段线
	static AcDbObjectId DrawPlineByPoints(AcGePoint3dArray points, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//创建面域
	static AcDbObjectIdArray CreateRegion(const AcDbObjectIdArray& curveIds);

};
