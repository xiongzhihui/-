#pragma once
#include "SheetStoreData.h"
#include "CFrameInfo.h"

//绘制数据光
class CDrawDigitalOptical
{
public:
	CDrawDigitalOptical(void);
	~CDrawDigitalOptical(void);

	void setData(CSheetStoreData data);
	void doIt();
	//将A0和A1的图纸排序，A0放在A0里面，A1放在A1里面
	void sortData();
	bool draw();
	void setFrameCount(int nFrameCount);
private:
	bool drawGLData(CGLData glData);
	void drawGLEnt(AcGePoint3d tmpPt, CString strNumber, vector<CCommonCell> tmpVec);
	void drawGlContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec);
	void drawContent(AcGePoint3d tmpPt, CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao, bool bIsDrawLine = true);
	void drawGlExts(CString strText);
private:
	int m_nFrameType;//图框类型，默认为0，0表示A0，1表示A1
	CSheetStoreData m_sheetData;//sheet的数据
	//CDrawBaseData m_drawBaseData;//基础绘制数据
	CFrameInfo m_FrameInfo;//基础数据
	CString m_strRealName;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	AcGePoint3d m_basePt;//基础点
	AcGePoint3d m_InsertPt;//图框插入基点
	AcDbObjectId m_layerId;//图层id
	AcDbObjectIdArray m_objIdArr;
	AcDbObjectIdArray m_ignoreErrorArr;
	AcGePoint3d m_glMinPt;
	AcGePoint3d m_glMaxPt;


	bool m_bIsDrawHorLine;
	bool m_bIsDrawVerLine;
	bool m_bIsUp;
	CNameCell m_nameCell;

	double m_dYPos;//记录起始y位置
	int m_nFrameCount;//横向数量
	int m_nCount;
	int m_nGlCount;//Gl个数
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
};

