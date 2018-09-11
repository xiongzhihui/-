#pragma once
#include "SheetStoreData.h"
#include "TableData.h"
#include "GLData.h"
#include "RFData.h"
#include "CommonData.h"
#include "CFrameInfo.h"

//绘制特殊的LRMV图形
class CDrawLrmvEnt
{
public:
	CDrawLrmvEnt(void);
	~CDrawLrmvEnt(void);

	void setData(CSheetStoreData data);
	void doIt();
	//将A0和A1的图纸排序，A0放在A0里面，A1放在A1里面
	void sortData();
	bool draw();
	void setModFlag(bool bIsMod);
	void setCount(int nCount);
	void setLastDrawFlag(bool bIsLast);

private:
	bool drawTable(CTableData tblData);
	bool drawGLData(CGLData glData);
	bool drawRfData(CRFData rfData);
	void drawComData(CCommonData comData);
	//在该点绘制表格图形
	void drawTableContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec);
	void drawContent(AcGePoint3d tmpPt, CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao, bool bIsDrawLine = true);
	void drawRF(AcGePoint3d tmpPt, CCommonCell comCell, bool bIsRF = true);
	void drawK(AcGePoint3d tmpPt, CCommonCell comCell);
	void drawGLEnt(AcGePoint3d tmpPt, CString strNumber, vector<CCommonCell> tmpVec);
	void drawGlContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec, bool param3);
	void drawGlExts(CString strText);
	void drawGL(AcGePoint3d tmpPt, CCommonCell param2);
private:
	int m_nFrameType;//图框类型，默认为0，0表示A0，1表示A1
	CSheetStoreData m_sheetData;//sheet的数据
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
	CNameCell m_nameCell;
	bool m_bIsDrawMod;//是否绘制模块
	int m_nCount;//-1为默认
	bool m_bIsLastDraw;//是否最后一个绘制
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

};

