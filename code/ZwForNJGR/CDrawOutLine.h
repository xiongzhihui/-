#pragma once
//处理外框虚框
//日期：2018年7月26日11:55:07

#include "map"
using namespace std;

class CDrawOutLine
{
public:
	CDrawOutLine(void);
	~CDrawOutLine(void);
	//处理数据
	bool doIt();

private:
	//获取所有图层名
	bool getAllLayerName();
	//获取图层的点信息
	void getLayerPoints(CString strName);
	//处理图层相关信息
	void doLayerNameInfo(CString strName);
	//根据两个角点绘制多段线
	void drawPlineByTwoPoints(AcGePoint3d startPt, AcGePoint3d endPt);
	void EraseAllGroup();
private:
	map<CString, vector<CString> > m_MapName;
	AcDbObjectIdArray m_idArrs;
	map<double, vector<AcGePoint3d> > m_pointsMap;
	int m_nColor;
};

