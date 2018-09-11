#pragma once

//绘制总线连接线
//Date:2018年7月25日11:56:38

class CDrawLjx
{
public:
	CDrawLjx(void);
	~CDrawLjx(void);
	//处理
	bool doIt();
	//选择多个实体
	bool selEnt();
	
	//绘制竖线和横线
	bool drawSpecialLine();
private:
	map<double, AcGePoint3d> m_pointMap;
	AcDbObjectId m_LayerId;
};
