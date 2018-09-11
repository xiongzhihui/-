#pragma once

//绘制图形所需的一些必要数据
class CDrawBaseData
{
public:
	CDrawBaseData(void);
	~CDrawBaseData(void);
	void init();
	//************************************
	// Method:    frameLength
	// FullName:  CDrawBaseData::frameLength
	// Access:    public 
	// Returns:   double
	// Qualifier:图框长度
	//************************************
	double frameLength();
	//************************************
	// Method:    frameHeight
	// FullName:  CDrawBaseData::frameHeight
	// Access:    public 
	// Returns:   double
	// Qualifier:图框高度
	//************************************
	double frameHeight();
	//************************************
	// Method:    frameHorSpace
	// FullName:  CDrawBaseData::frameHorSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:图框横线间距
	//************************************
	double frameHorSpace();
	//************************************
	// Method:    frameVerSpace
	// FullName:  CDrawBaseData::frameVerSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:图框纵向间距
	//************************************
	double frameVerSpace();

	//************************************
	// Method:    tableLength
	// FullName:  CDrawBaseData::tableLength
	// Access:    public 
	// Returns:   double
	// Qualifier:表格长度
	//************************************
	double tableLength();
	//************************************
	// Method:    tableHeight
	// FullName:  CDrawBaseData::tableHeight
	// Access:    public 
	// Returns:   double
	// Qualifier:表格高度
	//************************************
	double tableHeight();

	//************************************
	// Method:    horSpace
	// FullName:  CDrawBaseData::horSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:横向间距
	//************************************
	double horSpace();

	//************************************
	// Method:    verSpace
	// FullName:  CDrawBaseData::verSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:纵向间距
	//************************************
	double verSpace();

	//************************************
	// Method:    lineLength
	// FullName:  CDrawBaseData::lineLength
	// Access:    public 
	// Returns:   double
	// Qualifier:文字线长
	//************************************
	double lineLength();
	double spaceHorGap();
	double spaceVerGap();
	int HxFrameCount();

	double firstTableLength();
	double secondTableLength();
	double thirdTableLength();
	double rfSecondTableLength();
	double rfThirdTableLength();

	AcGePoint3d getInsertPt();
	AcGePoint3d getContentPt(AcGePoint3d insertPt);
	void setInsertPt(AcGePoint3d insertPt);
private:
	//基础数据
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
};

