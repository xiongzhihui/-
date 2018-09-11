#pragma once
#include <vector>
#include <map>
#include "CFrameInfo.h"
using namespace std;

class CDotPoint
{
public:
	CDotPoint(void);
	~CDotPoint(void);

	AcGePoint3d minPt() const;
	AcGePoint3d maxPt() const;

	AcGePoint3d LeftUpPoint() const;
	AcGePoint3d RightDownPoint() const;
	void setMinPt(AcGePoint3d pt);
	void setMaxPt(AcGePoint3d pt);
	
	void setExtents(AcDbExtents exts);
	CDotPoint& operator=(const CDotPoint& node);

	friend bool operator<(const CDotPoint &a, const CDotPoint &b)
	{
		return a.minPt().y < b.minPt().y;
	}

private:
	AcGePoint3d m_minPoint;
	AcGePoint3d m_maxPoint;
};

class CDrawDotted
{
public:
	CDrawDotted(void);
	~CDrawDotted(void);
	bool doIt();
	bool getData(AcDbObjectIdArray objIdArr);
	bool doData(int nType = 0);
	//处理图层
	bool doTcData();
	//处理母板
	bool doMBData();
	void drawlineByPt(AcGePoint3d leftUpPt, AcGePoint3d RightUpPt, AcDbObjectId layerId);
	void setGroupName(CString strGroupName);
	void makeGroup();
	AcDbObjectIdArray getIdArr();
	bool doLayer();
	bool doAlayer();
	bool getAllLayer();

	bool getAllExtLayer();
	AcGePoint3d getInsertPt(int nCount);
	double startXPos(int nCount);
	double endXPos(int nCount);
	double startYPos(int nCount);
	double endYPos(int nCount);

	AcGePoint3d GetLeftUpPt();
	AcGePoint3d GetLeftDownPt();
	AcGePoint3d GetRightUpPt();
	AcGePoint3d GetRightDownPt();
	AcGePoint3dArray getPointsArray();
	bool selEntForMb(int nType = 2);
	AcDbObjectId doPointsAndDrawPline(AcGePoint3dArray downPoints, AcGePoint3dArray upPoints);
	AcGePoint3dArray doPointsToTwoPoint(AcGePoint3dArray downPoints, bool bIsUp = true);
private:
	AcDbObjectIdArray m_idArrs;
	vector<map<pair<double, int>, vector<CDotPoint> > > m_data;
	map<int, vector<CDotPoint> > m_Info;
	CString m_strGroupName;
	vector<CString> m_LayerVec;
	AcGePoint3d m_startPt;
	map<CString, int> m_AInfo;
	vector<AcGePoint3d> m_ptVec;

	CFrameInfo m_FrameInfo;

	double m_dFrameLength;
	double m_dFrameHeight;
	AcGePoint3d m_InsertPt;
	double dLeftVerGap ;
	double dLeftHorGap ;
	double dSpaceHorGap;
	double dFrameHorGap;
	double dSpaceVerGap;
	vector<int> m_MBFrame;
	AcGePoint3d m_minPt;
	AcGePoint3d m_maxPt;

};
