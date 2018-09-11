#pragma once

//图框配置文件
class CFrameInfo
{
public:
	CFrameInfo(void);
	~CFrameInfo(void);
	//获取数据
	double getTableLength() const;
	double getTableHeight() const;
	double getLineLength() const;
	double getFristTableLength() const;
	double getSecondTableLength() const;
	double getThirdTableLength() const;
	double getRfSecondTableLength() const;
	double getRfThirdTableLength() const;
	
	double getHorSpace() const;
	double getVerSpace() const;
	double getSpaceHorGap() const;
	double getSpaceVerGap() const;
	double getGap() const;
	double getLeftVerGap() const;
	double getLeftHorGap() const;
	CString getFrameType() const;
	CString getLrmName() const;

	//设置数据
	void setTableLength(double dTableLength);
	void setTableHeight(double dTableHeight);
	void setLineLength(double dLineLength);
	void setFirstTableLength(double dFirstTableLength);
	void setSecondTableLength(double dSecondTableLength);
	void setThirdTableLength(double dThirdTableLength);
	void setRfSecondTableLength(double dRfSecondTableLength);
	void setRfThirdTableLength(double dRfThirdTableLength);
	void setHorSpace(double dHorSpace);
	void setVerSpace(double dVerSpace);
	void setSpaceHorGap(double dSpaceHorGap);
	void setSpaceVerGap(double dSpaceVerGap);
	void setGap(double dGap);
	void setLeftVerGap(double dLeftVerGap);
	void setLeftHorGap(double dLeftHorGap);
	void setFrameType(CString strFrameType);
	void setLrmName(CString strLrmName);

	CFrameInfo& operator=(const CFrameInfo& node);

private:
	//可修改	
	CString m_strFrameType;//图框类型
	CString m_strLrmName;
	double m_dHorSpace;//表格横向间距
	double m_dVerSapce;//表格纵向间距
	double m_dSpareHorGap;//备用横向长
	double m_dSpareVerGap;//备用纵向长
	double m_dLeftVerGap;//左上纵向长
	double m_dLeftHorGap;//左上横向长
	//暂时固定数据，不允许修改
	double m_dGap;//地图框与底线的间隙
	double m_dTableLength;//表格长度
	double m_dTableHeight;//表格高度
	double m_dLineLength;//文字线长
	double m_dFirstTableLength;//第一节数据
	double m_dSecondTableLength;//第二节数据
	double m_dThirdTableLength;//第三节数据
	double m_dRfSecondTableLength;//RF第二节长度
	double m_dRfThirdTableLength;//RF第三节长度
	int m_nCount;//数量
};


//图框表格内容
class CFrameTitle
{
public:
	CFrameTitle(void);
	~CFrameTitle(void);
	//获取数据
	CString getBmText() const;
	CString getDesigner() const;
	CString getProName() const;
	CString getProGrade() const;
	CString getProStage() const;
	CString getGZJ() const;
	CString getVerifier() const;
	CString getFirstEmpty() const;
	CString getStandard() const;
	CString getSencondEmpty() const;
	CString getApproval() const;

	//设置数据
	void setBmText(CString strBmText);
	void setDesigner(CString strDesigner);
	void setProName(CString strProName);
	void setProGrade(CString strGrade);
	void setProStage(CString strProStage);
	void setGZJ(CString strGZJ);
	void setVerifier(CString strVerifier);
	void setFirstEmpty(CString strFirstEmpty);
	void setStandard(CString strStandard);
	void setSencondEmpty(CString strEmpty);
	void setApproval(CString strApproval);

	CFrameTitle& operator=(const CFrameTitle& node);

private:
	//固定数据
	CString m_strBmText;//bm文字
	CString m_strDesigner;//设计师
	CString m_strProName;//产品名称
	CString m_strProGrade;//项目等级
	CString m_strProStage;//项目阶段
	CString m_strGZJ;//关重件
	CString m_strVerifier;//审核人
	CString m_strFirstEmpty;//标记
	CString m_strStandard;//标准化
	CString m_strSecondEmpty;//第二个空白处
	CString m_strApproval;//批准
};

