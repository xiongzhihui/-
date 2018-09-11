#pragma once
#include "CFrameInfo.h"
//绘制图框
//由于标题栏是固定的大小45*180；
//而我们的所有图框都是以右下角点作为基础点，故而其它的文字数据位置就根据右下角点偏移，
//这个具体的偏移数据是固定的，故而全部选用固定的数值
class CDrawFrame
{
public:
	CDrawFrame(void);
	~CDrawFrame(void);

	//图层颜色
	//void setSheetName(CString strSheetName);
	//void setSheetCount(int nCount);
	void setFrameIni(CString strSheetName, CFrameInfo frameIni, int nCount = -1);

	bool draw();
	void setAddFlag(bool bIsAdded = true);
	AcGePoint3d getInsertPt();

	AcGePoint3d getContentPt();
private:
	AcGePoint3d getPoint();
	bool drawFrame(AcGePoint3d basePt);
	//BMUP BMDOWN
	bool drawBmText(AcGePoint3d basePt);
	//单张
	bool drawCountText(AcGePoint3d basePt);
	//产品名称
	void drawProName(AcGePoint3d basePt);
	//设计
	void drawDesigner(AcGePoint3d basePt);
	//审核
	void drawVerifier(AcGePoint3d basePt);
	//绘制第一个空白区域
	void drawFirstEmpty(AcGePoint3d basePt);
	//绘制标准化
	void drawStandard(AcGePoint3d basePt);
	//绘制第二个空白区域
	void drawSecondEmpty(AcGePoint3d basePt);
	//绘制批准
	void drawApproval(AcGePoint3d basePt);
	//绘制关重件
	void drawGzj(AcGePoint3d basePt);
	//绘制项目等级
	void drawProGrade(AcGePoint3d basePt);
	//绘制项目阶段
	void drawProStage(AcGePoint3d basePt);

	//绘制文字
	AcGePoint3d drawBasicText(CString strDescription, double& dSize);

private:
	int m_nCount;//第几个图框	
	//int m_nSheetCount;//sheet内的count，默认为1
	AcGePoint3d m_InsertPt;
	CString m_strSheetName;//sheet名称
	CFrameInfo m_FrameIni;
	CFrameTitle m_frameTitle;
	AcDbObjectId m_TextStyleId;
	bool m_bIsAdded;//是否增加
};

