
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "CBaseInfoInDwgFile.h"
#include "Utility.h"

     // MDI safe statics
Adesk::Int16    CBaseInfoInDwgFile::m_version = 3;
LPCTSTR			CBaseInfoInDwgFile::m_dictName = _T("ZW_BASE_FOR_NJGRIDC");

ACRX_DXF_DEFINE_MEMBERS(CBaseInfoInDwgFile, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        0x400, BASE_FOR_NJGRIDC, ZwForTRSApp)

CBaseInfoInDwgFile::CBaseInfoInDwgFile()
: m_label(_T("BASE"))
{
	m_startPt.set(0, 0, 0);
	m_strDescription = _T("Author：mazaiguo; Email：mazaiguo@zwcad.com");
	m_strFrameType = _T("A0");
	double dTemp = 0.0;
	CUtility::getIniFromFile(m_strFrameType, m_dFrameLength, m_dFrameHeight, dTemp);
	
	m_strFrameType = gGlobal.GetIniValue(_T("图框设置"), _T("图框类型"));;	
	m_dVerFrameSapce = gGlobal.GetIniValue(_T("基础设置"), _T("图框纵向间距"), 0);
	m_dHorFrameSpace = gGlobal.GetIniValue(_T("基础设置"), _T("图框横向间距"), 0);
	m_dHxFrameCount = (int)gGlobal.GetIniValue(_T("基础设置"), _T("横向图框数"), 0);
	m_dLeftHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上横向长"), 0);
	m_dLeftVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上纵向长"), 0);
	m_dSpaceVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用纵向长"), 0);
	m_dSpaceHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用横向长"), 0);
	m_dHorSpace = gGlobal.GetIniValue(_T("基础设置"), _T("表格横向间距"), 0);
	m_dVerSpace = gGlobal.GetIniValue(_T("基础设置"), _T("表格纵向间距"), 0);
	m_strFileName = gGlobal.GetIniValue(_T("图框设置"), _T("路径"));
	m_bJxbFlag = false;
	m_bJxbFlag = false;
	m_nFirstCount = 1;
	m_nSecondCount = 1;
	m_nFrameCount = 1;
	m_nDpCount = 1;
	m_nRFCount = 1;
}



CBaseInfoInDwgFile::~CBaseInfoInDwgFile()
{
	m_startPt.set(0, 0, 0);
	m_strDescription = _T("Author：mazaiguo; Email：mazaiguo@zwcad.com");
	m_dFrameLength = 1189;
	m_dFrameHeight = 841;

	m_strFrameType = gGlobal.GetIniValue(_T("图框设置"), _T("图框类型"));;	
	m_dVerFrameSapce = gGlobal.GetIniValue(_T("基础设置"), _T("图框纵向间距"), 0);
	m_dHorFrameSpace = gGlobal.GetIniValue(_T("基础设置"), _T("图框横向间距"), 0);
	m_dHxFrameCount = (int)gGlobal.GetIniValue(_T("基础设置"), _T("横向图框数"), 0);
	m_dLeftVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上纵向长"), 0);
	m_dLeftHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上横向长"), 0);
	m_dSpaceVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用纵向长"), 0);
	m_dSpaceHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用横向长"), 0);
	m_dHorSpace = gGlobal.GetIniValue(_T("基础设置"), _T("表格横向间距"), 0);
	m_dVerSpace = gGlobal.GetIniValue(_T("基础设置"), _T("表格纵向间距"), 0);
	m_strFileName = gGlobal.GetIniValue(_T("图框设置"), _T("路径"));
	m_bJxbFlag = false;
	m_bJxbFlag = false;
	m_nFirstCount = 1;
	m_nSecondCount = 1;
	m_nFrameCount = 1;
	m_nDpCount = 1;
	m_nRFCount = 1;
}



LPCTSTR
CBaseInfoInDwgFile::label() const
{
    assertReadEnabled();
    return m_label;
}


Acad::ErrorStatus
CBaseInfoInDwgFile::setLabel(LPCTSTR newLabel)
{
        // we allow a null string label, so check for NULL
        // and turn it into NullString
    if (newLabel == NULL) 
	{
        if (m_label.IsEmpty() == FALSE)
		{   // make sure it will actually change
            assertWriteEnabled();
            m_label.Empty();
        }
        return Acad::eOk;
    }

        // don't allow to be longer than 255 for DXF simplicity
    int len = _tcslen(newLabel);
    if (len > 255)
        return Acad::eStringTooLong;

    if (m_label.Compare(newLabel))
	{
        assertWriteEnabled();
        m_label = newLabel;
    }

    return Acad::eOk;
}


AcGePoint3d CBaseInfoInDwgFile::startPt() const
{
	assertReadEnabled();
	return m_startPt;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setStartPt(AcGePoint3d startPt)
{
	assertWriteEnabled();
	m_startPt = startPt;
	return Acad::eOk;
}

AcGePoint3d CBaseInfoInDwgFile::endPt() const
{
	assertReadEnabled();
	return m_endPt;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setEndPt(AcGePoint3d endPt)
{
	assertWriteEnabled();
	m_endPt = endPt;
	return Acad::eOk;
}

LPCTSTR CBaseInfoInDwgFile::description() const
{
	assertReadEnabled();
	return m_strDescription;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setDescription(LPCTSTR strDes)
{
	if (strDes == NULL) 
	{
		if (m_strDescription.IsEmpty() == FALSE)
		{   // make sure it will actually change
			assertWriteEnabled();
			m_strDescription.Empty();
		}
		return Acad::eOk;
	}

	// don't allow to be longer than 255 for DXF simplicity
	int len = _tcslen(strDes);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strDescription.Compare(strDes))
	{
		assertWriteEnabled();
		m_strDescription = strDes;
	}

	return Acad::eOk;
}

LPCTSTR CBaseInfoInDwgFile::frameType() const
{
	assertReadEnabled();
	return m_strFrameType;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setFrameType(LPCTSTR strType)
{
	assertWriteEnabled();
	int len = _tcslen(strType);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strFrameType.Compare(strType))
	{
		assertWriteEnabled();
		m_strFrameType = strType;
	}

	return Acad::eOk;
}

LPCTSTR CBaseInfoInDwgFile::filePath() const
{
	assertReadEnabled();
	return m_strFileName;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setFilePath(LPCTSTR strFile)
{
	assertWriteEnabled();
	int len = _tcslen(strFile);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strFileName.Compare(strFile))
	{
		assertWriteEnabled();
		m_strFileName = strFile;
	}

	return Acad::eOk;
}	

double CBaseInfoInDwgFile::verFrameSpace() const
{
	assertReadEnabled();
	return m_dVerFrameSapce;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setVerFrameSpace(double dVerSpace)
{
	assertWriteEnabled();
	m_dVerFrameSapce = dVerSpace;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::horFrameSpace() const
{
	assertReadEnabled();
	return m_dHorFrameSpace;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setHorFrameSpace(double dVerSpace)
{
	assertWriteEnabled();
	m_dHorFrameSpace = dVerSpace;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::frameLength() const
{
	assertReadEnabled();
	return m_dFrameLength;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setFrameLength(double dLen)
{
	assertWriteEnabled();
	m_dFrameLength = dLen;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::frameHeight() const
{
	assertReadEnabled();
	return m_dFrameHeight;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setFrameHeight(double dHeight)
{
	assertWriteEnabled();
	m_dFrameHeight = dHeight;
	return Acad::eOk;
}

bool CBaseInfoInDwgFile::JxbFlag() const
{
	assertReadEnabled();
	return m_bJxbFlag;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setJxbFlag(bool bo1)
{
	assertWriteEnabled();
	m_bJxbFlag = bo1;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::hxFrameCount() const
{
	assertReadEnabled();
	return m_dHxFrameCount;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setHxFrameCount(double dCount)
{
	assertWriteEnabled();
	m_dHxFrameCount = dCount;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::leftVerGap() const
{
	assertReadEnabled();
	return m_dLeftVerGap;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setLeftVerGap(double dCount)
{
	assertWriteEnabled();
	m_dLeftVerGap = dCount;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::leftHorGap() const
{
	assertReadEnabled();
	return m_dLeftHorGap;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setLeftHorGap(double nCount)
{
	assertWriteEnabled();
	m_dLeftHorGap = nCount;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::spaceHorGap() const
{
	assertReadEnabled();
	return m_dSpaceHorGap;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setSpaceHorGap(double dGap)
{
	assertWriteEnabled();
	m_dSpaceHorGap = dGap;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::spaceVerGap() const
{
	assertReadEnabled();
	return m_dSpaceVerGap;
}


Acad::ErrorStatus CBaseInfoInDwgFile::setSpaceVerGap(double dGap)
{
	assertWriteEnabled();
	m_dSpaceVerGap = dGap;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::HorGap() const
{
	assertReadEnabled();
	return m_dHorSpace;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setHorGap(double dGap)
{
	assertWriteEnabled();
	m_dHorSpace = dGap;
	return Acad::eOk;
}

double CBaseInfoInDwgFile::VerGap() const
{
	assertReadEnabled();
	return m_dVerSpace;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setVerGap(double dGap)
{
	assertWriteEnabled();
	m_dVerSpace = dGap;
	return Acad::eOk;
}

Adesk::Int16 CBaseInfoInDwgFile::frameCount() const
{
	assertReadEnabled();
	return m_nFrameCount;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setFrameCount(Adesk::Int16 nCount)
{
	assertWriteEnabled();
	m_nFrameCount = nCount;
	return Acad::eOk;
}

Adesk::Int16 CBaseInfoInDwgFile::dpCount() const
{
	assertReadEnabled();
	return m_nDpCount;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setDpCount(Adesk::Int16 nCount)
{
	assertWriteEnabled();
	m_nDpCount = nCount;
	return Acad::eOk;
}

Adesk::Int16 CBaseInfoInDwgFile::rfCount() const
{	
	assertReadEnabled();
	return m_nRFCount;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setRfCount(Adesk::Int16 nCount)
{
	assertWriteEnabled();
	m_nRFCount = nCount;
	return Acad::eOk;
}

Adesk::Int16 CBaseInfoInDwgFile::firstCount() const
{
	assertReadEnabled();
	return m_nFirstCount;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setFirstCount(Adesk::Int16 nCount)
{
	assertWriteEnabled();
	m_nFirstCount = nCount;
	return Acad::eOk;
}

Adesk::Int16 CBaseInfoInDwgFile::secondCount() const
{
	assertReadEnabled();
	return m_nSecondCount;
}

Acad::ErrorStatus CBaseInfoInDwgFile::setSecondCount(Adesk::Int16 nCount)
{
	assertWriteEnabled();
	m_nSecondCount = nCount;
	return Acad::eOk;
}

Acad::ErrorStatus
CBaseInfoInDwgFile::dwgInFields(AcDbDwgFiler* filer)
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgInFields(filer)) != Acad::eOk)
        return es;
	assertWriteEnabled();
    Adesk::Int16 version;
    filer->readItem(&version);
    if (version > m_version)
        return Acad::eMakeMeProxy;

    TCHAR* tmpStr = NULL;    // must explicitly set to NULL or readItem() crashes!

    filer->readItem(&tmpStr);
    m_label = tmpStr;
    acutDelString(tmpStr);
	
	filer->readItem(&m_startPt);
	filer->readItem(&m_endPt);
	filer->readItem(&tmpStr);
	m_strDescription = tmpStr;
	acutDelString(tmpStr);
	
	if (version == 2)
	{
		filer->readItem(&tmpStr);
		m_strFrameType = tmpStr;
		acutDelString(tmpStr);

		filer->readItem(&m_dVerFrameSapce);
		filer->readItem(&m_dHorFrameSpace);
		filer->readItem(&m_dFrameLength);
		filer->readItem(&m_dFrameHeight);
		filer->readItem(&m_dHxFrameCount);
		filer->readItem(&m_dLeftVerGap);
		filer->readItem(&m_dLeftHorGap);
		filer->readItem(&m_dSpaceVerGap);
		filer->readItem(&m_dSpaceHorGap);
		filer->readItem(&m_dVerSpace);
		filer->readItem(&m_dHorSpace);
		filer->readItem(&m_bJxbFlag);
	}
	else if (version > 2)
	{
		filer->readItem(&tmpStr);
		m_strFrameType = tmpStr;
		acutDelString(tmpStr);
		filer->readItem(&tmpStr);
		m_strFileName = tmpStr;
		acutDelString(tmpStr);

		filer->readItem(&m_dVerFrameSapce);
		filer->readItem(&m_dHorFrameSpace);
		filer->readItem(&m_dFrameLength);
		filer->readItem(&m_dFrameHeight);
		filer->readItem(&m_dHxFrameCount);
		filer->readItem(&m_dLeftVerGap);
		filer->readItem(&m_dLeftHorGap);
		filer->readItem(&m_dSpaceVerGap);
		filer->readItem(&m_dSpaceHorGap);
		filer->readItem(&m_dVerSpace);
		filer->readItem(&m_dHorSpace);

		filer->readItem(&m_nFrameCount);
		filer->readItem(&m_nRFCount);
		filer->readItem(&m_nDpCount);
		filer->readItem(&m_nFirstCount);
		filer->readItem(&m_nSecondCount);
		filer->readItem(&m_bJxbFlag);
	}

    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseInfoInDwgFile::dwgOutFields(AcDbDwgFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk)
        return es;
	
	assertReadEnabled();

    filer->writeItem(m_version);

    filer->writeItem(static_cast<const TCHAR*>(m_label));
	filer->writeItem(m_startPt);
	filer->writeItem(m_endPt);
	filer->writeItem(static_cast<const TCHAR*>(m_strDescription));
	if (m_version > 1)
	{
		filer->writeItem(static_cast<const TCHAR*>(m_strFrameType));
		filer->writeItem(static_cast<const TCHAR*>(m_strFileName));
		filer->writeItem(m_dVerFrameSapce);
		filer->writeItem(m_dHorFrameSpace);
		filer->writeItem(m_dFrameLength);
		filer->writeItem(m_dFrameHeight);
		filer->writeItem(m_dHxFrameCount);
		filer->writeItem(m_dLeftVerGap);
		filer->writeItem(m_dLeftHorGap);
		filer->writeItem(m_dSpaceVerGap);
		filer->writeItem(m_dSpaceHorGap);
		filer->writeItem(m_dVerSpace);
		filer->writeItem(m_dHorSpace);
		filer->writeItem(m_nFrameCount);
		filer->writeItem(m_nRFCount);
		filer->writeItem(m_nDpCount);
		filer->writeItem(m_nFirstCount);
		filer->writeItem(m_nSecondCount);
		filer->writeItem(m_bJxbFlag);
	}
    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseInfoInDwgFile::dxfInFields(AcDbDxfFiler* filer)
{
    Acad::ErrorStatus es;
    if (((es = AcDbObject::dxfInFields(filer)) != Acad::eOk) ||
        (filer->atSubclassData(this->desc()->name()) != Adesk::kTrue))
        return filer->filerStatus();

	assertWriteEnabled();

	struct resbuf rb;
	es = Acad::eOk;

    while ((es == Acad::eOk) && ((es = filer->readResBuf(&rb)) == Acad::eOk)) 
	{
        if (rb.restype == kDxfLabel)
            setLabel(rb.resval.rstring);        // NOTE: normally have to free string from resbuf... not in this case, DXF Filer does
		else if (rb.restype == kDxfDescription)
		{
			setDescription(rb.resval.rstring);
		}
		else if (rb.restype == kDxfStartPt)
		{
			setStartPt(asPnt3d(rb.resval.rpoint));
		}
		else if (rb.restype == kDxfEndPt)
		{
			setEndPt(asPnt3d(rb.resval.rpoint));
		}
		else if (rb.restype == kDxfFrameType)
		{
			setFrameType(rb.resval.rstring);
		}
		else if (rb.restype == kDxfVerFrameSpace)
		{
			setVerFrameSpace(rb.resval.rreal);
		}
		else if (rb.restype == kDxfJxbFlag)
		{
			setJxbFlag(rb.resval.rint);
		}
		else if (rb.restype == kDxfHorFrameSpace)
		{
			setHorFrameSpace(rb.resval.rreal);
		}
		else if (rb.restype == kDxfHxFrameCount)
		{
			setHxFrameCount(rb.resval.rreal);
		}
		else if (rb.restype == kDxfFrameLength)
		{
			setFrameLength(rb.resval.rreal);
		}
		else if (rb.restype == kDxfFrameHeight)
		{
			setFrameHeight(rb.resval.rreal);
		}
		else if (rb.restype == kDxfLeftVerGap)
		{
			setLeftVerGap(rb.resval.rreal);
		}
		else if (rb.restype == kDxfLeftHorGap)
		{
			setLeftHorGap(rb.resval.rreal);
		}
		else if (rb.restype == kDxfSpaceHorGap)
		{
			setSpaceHorGap(rb.resval.rreal);
		}
		else if (rb.restype == kDxfSpaceVerGap)
		{
			setSpaceVerGap(rb.resval.rreal);
		}
		else if (rb.restype == kDxfFrameHorSpace)
		{
			setHorGap(rb.resval.rreal);
		}
		else if (rb.restype == kDxfFrameVerSpace)
		{
			setVerGap(rb.resval.rreal);
		}
		else if (rb.restype == kDxfFrameCount)
		{
			setFrameCount(rb.resval.rlong);
		}
		else if (rb.restype == kDxfRfCount)
		{
			setRfCount(rb.resval.rlong);
		}
		else if (rb.restype == kDxfDpCount)
		{
			setDpCount(rb.resval.rlong);
		}
		else if (rb.restype == kDxfFirstCount)
		{
			setFirstCount(rb.resval.rlong);
		}
		else if (rb.restype == kDxfSecondCount)
		{
			setSecondCount(rb.resval.rlong);
		}
		else if (rb.restype == kDxfFilePath)
		{
			setFilePath(rb.resval.rstring);
		}
		else 
		{
            filer->pushBackItem();
            es = Acad::eEndOfFile;
        }
    }

    if (es != Acad::eEndOfFile)
        return Acad::eInvalidResBuf;

    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseInfoInDwgFile::dxfOutFields(AcDbDxfFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfOutFields(filer)) != Acad::eOk)
        return es;

	assertReadEnabled();

    filer->writeItem(AcDb::kDxfSubclass, this->desc()->name());

    filer->writeItem(kDxfLabel, static_cast<const TCHAR*>(m_label));
	filer->writeItem(kDxfStartPt, m_startPt);
	filer->writeItem(kDxfEndPt, m_endPt);
	filer->writeItem(kDxfDescription, static_cast<const TCHAR*>(m_strDescription));
	if (m_version > 1)
	{
		filer->writeItem(kDxfFrameType, static_cast<const TCHAR*>(m_strFrameType));
		filer->writeItem(kDxfFilePath, static_cast<const TCHAR*>(m_strFileName));
		filer->writeItem(kDxfVerFrameSpace, m_dVerFrameSapce);
		filer->writeItem(kDxfHorFrameSpace, m_dHorFrameSpace);
		filer->writeItem(kDxfFrameLength, m_dFrameLength);
		filer->writeItem(kDxfFrameHeight, m_dFrameHeight);
		filer->writeItem(kDxfHxFrameCount, m_dHxFrameCount);
		filer->writeItem(kDxfLeftVerGap, m_dLeftVerGap);
		filer->writeItem(kDxfLeftHorGap, m_dLeftHorGap);
		filer->writeItem(kDxfSpaceVerGap, m_dSpaceVerGap);
		filer->writeItem(kDxfSpaceHorGap,m_dSpaceHorGap);
		filer->writeItem(kDxfFrameVerSpace, m_dVerSpace);
		filer->writeItem(kDxfFrameHorSpace, m_dHorSpace);
		filer->writeItem(kDxfFrameCount, m_nFrameCount);
		filer->writeItem(kDxfRfCount, m_nRFCount);
		filer->writeItem(kDxfDpCount, m_nDpCount);
		filer->writeItem(kDxfFirstCount, m_nFirstCount);
		filer->writeItem(kDxfSecondCount, m_nSecondCount);
		filer->writeItem(kDxfJxbFlag, m_bJxbFlag);
	}
    return filer->filerStatus();
}


AcDbObjectId
CBaseInfoInDwgFile::getStandardStyle(AcDbDatabase* db, bool makeIfNotThere)
{
    Acad::ErrorStatus es;
    AcDbObjectId styleId;
    AcDbDictionary* dict;

    const TCHAR* standardStrPtr = acdbGetReservedString(AcDb::kStandard, false);
    ASSERT(standardStrPtr != NULL);
    
	dict = MyBaseUtils::openDictionaryForRead(m_dictName, db);
    if (dict) 
	{
        es = dict->getAt(standardStrPtr, styleId);
        dict->close();

        if (es == Acad::eOk)
            return styleId;

        if (makeIfNotThere == false)
            return AcDbObjectId::kNull;
    }

	dict = MyBaseUtils::openDictionaryForWrite(m_dictName, true, db);
    if (dict) 
	{
        CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
        es = dict->setAt(standardStrPtr, newRec, styleId);
        dict->close();
        if (es == Acad::eOk) 
		{
            newRec->close();
            return styleId;
        }
        else 
		{
            delete newRec;
        }
    }

    return AcDbObjectId::kNull;
}



