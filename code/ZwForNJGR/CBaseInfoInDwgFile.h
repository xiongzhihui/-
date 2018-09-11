#ifndef CBASEINFOINDWGFILE_H
#define CBASEINFOINDWGFILE_H
#include "MyBase.h"
#ifdef ZWFORNJGR_MODULE //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif
class WRQ_ENTITY_DLL CBaseInfoInDwgFile : public AcDbObject 
{

public:
                CBaseInfoInDwgFile();
    virtual     ~CBaseInfoInDwgFile();

    ACRX_DECLARE_MEMBERS(CBaseInfoInDwgFile);

        // access to data members

    LPCTSTR             label() const;
    Acad::ErrorStatus   setLabel(LPCTSTR newLabel);

	AcGePoint3d         startPt() const;
	Acad::ErrorStatus   setStartPt(AcGePoint3d startPt);

	AcGePoint3d			endPt() const;
	Acad::ErrorStatus	setEndPt(AcGePoint3d endPt);

	LPCTSTR				description() const;
	Acad::ErrorStatus	setDescription(LPCTSTR strDes);

	LPCTSTR				frameType() const;
	Acad::ErrorStatus   setFrameType(LPCTSTR strType);

	LPCTSTR				filePath() const;
	Acad::ErrorStatus   setFilePath(LPCTSTR strFile);

	double				verFrameSpace() const;
	Acad::ErrorStatus   setVerFrameSpace(double dVerSpace);

	double				horFrameSpace() const;
	Acad::ErrorStatus   setHorFrameSpace(double dVerSpace);

	double				frameLength() const;
	Acad::ErrorStatus   setFrameLength(double dLen);

	double				frameHeight() const;
	Acad::ErrorStatus   setFrameHeight(double dHeight);

	bool			    JxbFlag() const;
	Acad::ErrorStatus   setJxbFlag(bool bo1);

	double					hxFrameCount() const;
	Acad::ErrorStatus	setHxFrameCount(double nCount);

	double					leftVerGap() const;
	Acad::ErrorStatus	setLeftVerGap(double nCount);

	double					leftHorGap() const;
	Acad::ErrorStatus	setLeftHorGap(double nCount);

	double					spaceHorGap() const;
	Acad::ErrorStatus	setSpaceHorGap(double dGap);

	double					spaceVerGap() const;
	Acad::ErrorStatus	setSpaceVerGap(double dGap);

	double					HorGap() const;
	Acad::ErrorStatus	setHorGap(double dGap);

	double					VerGap() const;
	Acad::ErrorStatus   setVerGap(double dGap);

	Adesk::Int16			frameCount() const;
	Acad::ErrorStatus  setFrameCount(Adesk::Int16 nCount);

	Adesk::Int16			dpCount() const;
	Acad::ErrorStatus  setDpCount(Adesk::Int16 nCount);

	Adesk::Int16            rfCount() const;
	Acad::ErrorStatus  setRfCount(Adesk::Int16 nCount);

	Adesk::Int16			firstCount() const;
	Acad::ErrorStatus  setFirstCount(Adesk::Int16 nCount);

	Adesk::Int16            secondCount() const;
	Acad::ErrorStatus  setSecondCount(Adesk::Int16 nCount);

    virtual Acad::ErrorStatus   dwgInFields(AcDbDwgFiler* filer);
    virtual Acad::ErrorStatus   dwgOutFields(AcDbDwgFiler* filer) const;
    virtual Acad::ErrorStatus   dxfInFields(AcDbDxfFiler* filer);
    virtual Acad::ErrorStatus   dxfOutFields(AcDbDxfFiler* filer) const;

private:
    // data members
    CString             m_label;
    static Adesk::Int16 m_version;
	static LPCTSTR		m_dictName;
	CString				m_strDescription;
	CString				m_strFileName;//文件完整路径

	AcGePoint3d m_startPt;//图纸开始绘制的原点
	AcGePoint3d m_endPt;//图形绘制的终止点

	CString m_strFrameType;//图框类型
	double  m_dVerFrameSapce;//图框纵向间距
	double m_dHorFrameSpace;

	double m_dFrameLength;
	double m_dFrameHeight;
	double m_dHxFrameCount;
	double m_dLeftVerGap;//左上纵向长
	double m_dLeftHorGap;//左上横向长
	double m_dSpaceHorGap;//备用横向长
	double m_dSpaceVerGap;//备用纵向长
	double m_dHorSpace;//表格横向间距
	double m_dVerSpace;//表格纵向间距
    bool    m_bJxbFlag;//是否生成接线表的标志
	Adesk::Int16   m_nFrameCount;//图框数量
	Adesk::Int16   m_nRFCount;//RF数量
	Adesk::Int16   m_nDpCount;//dp数量
	Adesk::Int16   m_nFirstCount;//备用
	Adesk::Int16   m_nSecondCount;//备用
    // Dxf Codes
    enum 
	{
        kDxfLabel       = 1,
		kDxfStartPt     = 10,//10-39 双精度三维点值
		kDxfEndPt		= 11,
		//60-79     16 位整数值

		//40-59		双精度浮点值
		kDxfVerFrameSpace = 40,
		kDxfHorFrameSpace = 41,
		kDxfFrameLength	  = 42,
		kDxfFrameHeight	  = 43,	
		kDxfHxFrameCount  = 44,
		kDxfLeftVerGap	  = 45,
		kDxfLeftHorGap	  = 46,
		kDxfSpaceVerGap   = 47,
		kDxfSpaceHorGap	  = 48,
		kDxfFrameVerSpace = 49,
		kDxfFrameHorSpace = 50,

		kDxfFrameCount   = 60,
		kDxfRfCount		 = 61,
		kDxfDpCount      = 62,
		kDxfFirstCount	 = 63,
		kDxfSecondCount  = 64,
		//290-299		布尔标志值
		kDxfJxbFlag     = 290,
		kDxfDescription	=300,//300~309 任意字符串
		kDxfFrameType	= 301,
		kDxfFilePath    = 302,
    };

public:
	static LPCTSTR		dictName()	{ return m_dictName; }
	static AcDbObjectId	getStandardStyle(AcDbDatabase* db, bool makeIfNotThere);

};


#endif    // ARXDBGDBADESKLOGOSTYLE_H
