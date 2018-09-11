#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include "CFrameInfo.h"

class CDrawEntity
{
public:
	CDrawEntity(void);
	~CDrawEntity(void);
	//提取基础数据
	void Init();
	bool drawFrame();
	void setNewFrameFlag(bool bIsAdded = true);

	void setCount(int nCount);
	bool draw(map<CNameCell, vector<CCommonCell> > mapInfo);
	AcGePoint3d getContentPt(AcGePoint3d insertPt);

	bool drawTable(CNameCell nameCell, vector<CCommonCell> comVec);
	void setSheetName(CString strSheetName);
	//处理表格点
	void doTablePoint(double dDist);
	void getTablePt(bool bIsDrawText = false);

	void doRFPoint();

	void doGapPoint();
	void drawContent(CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao = _T(""), bool bDrawLine = true,int nRow=-1,int nCol=-1,int nType=-1);

	//绘制RF图形
	void drawRF(CCommonCell, bool bisRF = true);

	//设置绘制mod的标志
	void setModFlag(bool bDrawModFlag);
	//绘制图块
	void drawTu(vector<CString> tuVec);
private:
	int getCount();
	AcGePoint3d getInsertPt();
	bool drawGL(CCommonCell comCell);
	void drawK(CCommonCell comCell);
	bool IsStartPt();
	bool getBlkPt(double dLen, double dHeight);
	AcDbObjectId drawblkRef(CString strFile);
	void moveToPt(AcDbObjectId objId);

	AcGePoint3d getblkStartPt(int nCount);
private:
	double m_dFrameLength;//图框长度
	double m_dFrameHeight;//图框高度
	double m_dFrameHorSpace;//图框横向间距
	double m_dFrameVerSpace;//图框纵向间距
	double m_dTableLength;//表格长度
	double m_dTableHeight;//表格高度
	double m_dHorSpace;//横向间距
	double m_dVerSapce;//纵向间距
	double m_dLineLength;//文字线长
	double m_dSpareHorGap;//备用横向长
	double m_dSpareVerGap;//备用纵向长
	double m_dGap;//地图框与底线的间隙
	AcGePoint3d m_basePt;//基础点
	AcGePoint3d m_InsertPt;//图框插入基点
	CString m_strSheetName;//sheet名称
	AcDbObjectId m_layerId;
	AcDbObjectIdArray m_objIdArr;
	AcDbObjectIdArray m_ignoreErrorArr;
	AcDbObjectId m_contentId;//表格id
	double m_dLeftVerGap;
	double m_dLeftHorGap;
	bool m_bIsDrawVerLine;//是否绘制竖着线
	bool m_bIsDrawHorLine;//是否绘制横着的线
	int  m_nHxCount;//横向图框数
	double m_dFirstTableLength;//第一节数据
	double m_dSecondTableLength;//第二节数据
	double m_dThirdTableLength;//第三节数据

	double m_dRfSecondTableLength;//RF第二节长度
	double m_dRfThirdTableLength;//RF第三节长度

	bool m_bIsDrawMod;//是否绘制模块
	bool m_bIsDrawXxk;//是否绘制虚线框
	bool m_bFirstDrawRf;//是否第一次绘制RF
	double m_dBlkLen;
	double m_dBlkHeight;
	bool m_bIsMuban;//是否母版
	double m_dOffsetDist;//偏移变量
	map<CString, int> m_dictGroup;
	CString m_strRealName;
	int m_nCount;
	bool m_bIsNewFrame;//增加了新的图框

	CFrameInfo m_FrameInfo;
	CString m_strPreName;
	bool m_bIsHor;//横向放置

};
