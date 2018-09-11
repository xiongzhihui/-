#pragma once
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif


class SERVERDLL_API MyEditEntity
{
public:
	MyEditEntity(void);
	~MyEditEntity(void);
	//将实体打开并修改其图层
	static AcDbObjectId openEntChangeLayer(AcDbObjectId entId, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//将POLYLINE的宽度
	static AcDbObjectId openPlineChangeWidth(AcDbObjectId plineId, double dWidth);
	//修改text, mtext, blockrefrence的rotation
	static AcDbObjectId openEntChangeRotation(AcDbObjectId entId, double dRotate);
	//blockrefrence的scale
	static AcDbObjectId openBlkRefChangeScale(AcDbObjectId entId, double dScale);

	//修改文字的textstyleId
	static AcDbObjectId openEntChangeTextStyle(AcDbObjectId entId, AcDbObjectId TextStyleId = AcDbObjectId::kNull);

	//修改文字高度
	static AcDbObjectId openTextChangeTextHeight(AcDbObjectId textId, double dTextHeight);

	//修改实体的线宽
	static AcDbObjectId openEntChangeLineWeight(AcDbObjectId entId, AcDb::LineWeight type = AcDb::kLnWt053);
	//修改实体颜色
	static AcDbObjectId openEntChangeColor(AcDbObjectId entId, int nColor);
	//修改实体线型
	static AcDbObjectId openEntChangeLineType(AcDbObjectId entId, CString strLineType, double dScale = 1.0);

	//[为修改线杆，新建管道，原有管道做的接口
	//返回实体所在的组的所有的特殊Id
	//使用该接口的时候需判定该接头的返回值是否为空
	static AcDbObjectIdArray openObjAndGetGroupIds(AcDbObjectId objId);

	static AcDbObjectIdArray openObjAndGetGroupIds(AcDbObject* obj);
	//从组中找到适合的组id里面实体集合
	static AcDbObjectIdArray openGroupIdsAndGetEntIds(AcDbObjectIdArray IdArrs, BOOL bIslimited = TRUE);
	//]
	//排序
	static AcDbSortentsTable* get_sortents_table_of(AcDbObjectId objId);

	//打开图层并添加类型数据
	static AcDbObjectId OpenLayerIdAppendDescription(AcDbObjectId objId, CString strDescription);
	static CString GetLayerIdDescription(AcDbObjectId objId);
	//列出所有的除0层外带说明文字的图层
	static void ListLayerNameWithDescription(CStringArray& strLayerNameArr, BOOL bIsMask = FALSE);

	static void ListLayerNameWithMetal(CStringArray& strLayerNameArr);
	//设置信息到obj
	static AcDbObjectId OpenObjAppendStrToXRecord(AcDbObjectId objId, CString strAppName, CString strTypeData);
	static AcDbObjectId OpenObjAppendIntToXRecord(AcDbObjectId objId, CString strAppName, int nTypeData);
	static AcDbObjectId OpenObjAppendDoubleToXRecord(AcDbObjectId objId, CString strAppName, double dTypeData);
	//static AcDbObjectId OpenObjAppendHandleToXRecord(AcDbObjectId objId, CString strAppName, AcDbHandle hData);
	
	static CString GetObjStrFromXRecord(AcDbObjectId objId, CString strAppName);
	static int GetObjIntFromXRecord(AcDbObjectId objId, CString strAppName);
	static double GetObjDoubleFromXRecord(AcDbObjectId objId, CString strAppName);
	//static AcDbHandle GetObjHandleFromXRecord(AcDbObjectId objId, CString strAppName);
	
	//[Special For gzsj
	//创建一个打印与不可打印的图层，返回其id
	static AcDbObjectId CreateNewNoPlotLayer(CString strLayer, int nColor, BOOL bIsModified = FALSE);
	
	static void FrozenLayer(CStringArray& strLayer, bool bIsModified = true);
	
	static void FrozenPassivationLayer(bool bIsFrozen = true);

	static void UnFrozenLayer();

	//操作dimstyle文字高度
	//[操作xdata
	//传入objid，将该实体打开并添加cstring到扩展数据
	static bool OpenObjAppendStrToXdata(AcDbObjectId objId, CString strAppName, CString strData);
	static bool OpenObjAppendIntToXdata(AcDbObjectId objId, CString strAppName, int nData);
	static bool OpenObjAppendDoubleToXdata(AcDbObjectId objId, CString strAppName, double dData);
	static bool OpenObjAppendHandleToXdata(AcDbObjectId objId, CString strAppName, AcDbHandle hData);
	//获取应用程序的
	static CString GetObjStrXdata(AcDbObjectId objId, CString strAppName);
	static double GetObjDoubleXdata(AcDbObjectId objId, CString strAppName);
	static int GetObjIntXdata(AcDbObjectId objId, CString strAppName);
	static AcDbHandle GetObjHandleXdata(AcDbObjectId objId, CString hData);

	static bool SetObjStrToXdata(AcDbObjectId objId, CString strAppName, CString strXdata);
	static bool SetObjDoubleToXdata(AcDbObjectId objId, CString strAppName, double dXdata);
	static bool SetObjIntToXdata(AcDbObjectId objId, CString strAppName, int nXdata);
	//]操作xdata

	//操作二维xoy平面矩形区域的相交情况
	static	BOOL JudgePointInArea(AcGePoint2d insertPt, AcGePoint2d minPt, AcGePoint2d maxPt);
	static int JudgeAreaInOtherArea(AcDbExtents curExts, AcDbExtents preExts);
	static int JudgeAreaInOtherArea(AcGePoint2d curMinPt,AcGePoint2d curMaxPt,AcGePoint2d preMinPt,AcGePoint2d preMaxPt);

	//获取实体的外框
	static AcDbExtents OpenObjAndGetExts(AcDbObjectId objId);
	static double OpenObjAndGetWidth(AcDbObjectId objId);
	static double OpenObjAndGetLength(AcDbObjectId objId);
	
	//@param objId为多行文字和单行文字的ID
	//@return 返回多行文字和单行文字的内容
	static CString OpenObjAndGetString(AcDbObjectId objId);

	//删除实体
	static bool EraseObj(AcDbObjectId);
	//删除选择集
	static bool EraseSSName(ads_name ssname);
	//删除所有实体
	static bool EraseAllEnt();

	//组相关的操作
	static CString openObjAndGetGroupName(AcDbObjectId objId);

	static bool EraseEntByGroupName(CString strGroupName);

	static bool AddObjToGroup(CString strLabel, AcDbObjectId objId);

};