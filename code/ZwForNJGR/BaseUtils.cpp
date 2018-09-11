#include "StdAfx.h"
#include "BaseUtils.h"
#include "MyBaseUtils.h"
#include "CBaseInfoInDwgFile.h"

CBaseUtils::CBaseUtils(void)
{
}

CBaseUtils::~CBaseUtils(void)
{
}

AcGePoint3d CBaseUtils::startPt()
{
	AcGePoint3d pt;
	pt.set(0, 0, 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				pt = newRec->startPt();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return pt;
			}
			pt = newRec->startPt();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setStartPt(pt);
	}
	return pt;
}

void CBaseUtils::setStartPt(AcGePoint3d startPt)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setStartPt(startPt);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setStartPt(startPt);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

AcGePoint3d CBaseUtils::endPt()
{
	AcGePoint3d pt;
	pt.set(0, 0, 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				pt = newRec->endPt();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return pt;
			}
			pt = newRec->endPt();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setEndPt(pt);
	}
	return pt;
}

void CBaseUtils::setEndPt(AcGePoint3d endPt)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setEndPt(endPt);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setEndPt(endPt);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

CString CBaseUtils::frameType()
{
	CString  strType;
	strType = _T("A0");
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				strType = newRec->frameType();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return strType;
			}
			strType = newRec->frameType();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setFrameType(strType);
	}
	return strType;
}

void CBaseUtils::setFrameType(CString strType)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setFrameType(strType);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setFrameType(strType);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CBaseUtils::verFrameSpace()
{
	double  dVerSpace;
	dVerSpace = 10;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dVerSpace = newRec->verFrameSpace();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dVerSpace;
			}
			dVerSpace = newRec->verFrameSpace();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setVerFrameSpace(dVerSpace);
	}
	return dVerSpace;
}

void CBaseUtils::setVerFrameSpace(double dVerFrameSpace)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setVerFrameSpace(dVerFrameSpace);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setVerFrameSpace(dVerFrameSpace);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CBaseUtils::horFrameSpace()
{
	double  dHorSpace;
	dHorSpace = 10;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dHorSpace = newRec->horFrameSpace();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dHorSpace;
			}
			dHorSpace = newRec->horFrameSpace();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setHorFrameSpace(dHorSpace);
	}
	return dHorSpace;
}

void CBaseUtils::setHorFrameSpace(double dHorFrameSpace)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setHorFrameSpace(dHorFrameSpace);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setHorFrameSpace(dHorFrameSpace);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

bool CBaseUtils::jxbFlag()
{
	bool  bJxbFlag;
	bJxbFlag = false;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				bJxbFlag = newRec->JxbFlag();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return bJxbFlag;
			}
			bJxbFlag = newRec->JxbFlag();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setJxbFlag(bJxbFlag);
	}
	return bJxbFlag;
}

void CBaseUtils::setJxbFlag(bool bFlag)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setJxbFlag(bFlag);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setJxbFlag(bFlag);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CBaseUtils::hxFrameCount()
{
	double  nFrameCount;
	nFrameCount = 3;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nFrameCount = newRec->hxFrameCount();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nFrameCount;
			}
			nFrameCount = newRec->hxFrameCount();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setHxFrameCount(nFrameCount);
	}
	return nFrameCount;
}

void CBaseUtils::setHxFrameCount(double nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setHxFrameCount(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setHxFrameCount(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CBaseUtils::frameLength()
{
	double  nFrameCount;
	nFrameCount = 10.0;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nFrameCount = newRec->frameLength();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nFrameCount;
			}
			nFrameCount = newRec->frameLength();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setFrameLength(nFrameCount);
	}
	return nFrameCount;
}

void CBaseUtils::setFrameLength(double nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setFrameLength(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setFrameLength(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}


double CBaseUtils::frameHeight()
{
	double  nFrameCount;
	nFrameCount = 10.0;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nFrameCount = newRec->frameHeight();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nFrameCount;
			}
			nFrameCount = newRec->frameHeight();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setFrameHeight(nFrameCount);
	}
	return nFrameCount;
}

void CBaseUtils::setFrameHeight(double nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setFrameHeight(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setFrameHeight(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CBaseUtils::leftVerGap()
{
	double  dLeftVerGap;
	dLeftVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上纵向长"), 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dLeftVerGap = newRec->leftVerGap();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dLeftVerGap;
			}
			dLeftVerGap = newRec->leftVerGap();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setLeftVerGap(dLeftVerGap);
	}
	return dLeftVerGap;
}

void CBaseUtils::setLeftVerGap(double dLeftVerGap)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setLeftVerGap(dLeftVerGap);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setLeftVerGap(dLeftVerGap);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CBaseUtils::leftHorGap()
{
	double  dLeftVerGap;
	dLeftVerGap = gGlobal.GetIniValue(_T("基础设置"), _T("左上横向长"), 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dLeftVerGap = newRec->leftHorGap();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dLeftVerGap;
			}
			dLeftVerGap = newRec->leftHorGap();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setLeftHorGap(dLeftVerGap);
	}
	return dLeftVerGap;
}

void CBaseUtils::setLeftHorGap(double dLeftVerGap)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setLeftHorGap(dLeftVerGap);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setLeftHorGap(dLeftVerGap);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}


double CBaseUtils::spaceHorGap()
{
	double  dSpaceHorGap;
	dSpaceHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用横向长"), 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dSpaceHorGap = newRec->spaceHorGap();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dSpaceHorGap;
			}
			dSpaceHorGap = newRec->spaceHorGap();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setSpaceHorGap(dSpaceHorGap);
	}
	return dSpaceHorGap;
}

void CBaseUtils::setSpaceHorGap(double dLeftVerGap)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setSpaceHorGap(dLeftVerGap);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setSpaceHorGap(dLeftVerGap);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}


double CBaseUtils::spaceVerGap()
{
	double  dSpaceHorGap;
	dSpaceHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("备用纵向长"), 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dSpaceHorGap = newRec->spaceVerGap();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dSpaceHorGap;
			}
			dSpaceHorGap = newRec->spaceVerGap();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setSpaceVerGap(dSpaceHorGap);
	}
	return dSpaceHorGap;
}

void CBaseUtils::setSpaceVerGap(double dLeftVerGap)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setSpaceVerGap(dLeftVerGap);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setSpaceVerGap(dLeftVerGap);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}


double CBaseUtils::HorSpace()
{
	double  dSpaceHorGap;
	dSpaceHorGap = gGlobal.GetIniValue(_T("基础设置"), _T("表格横向间距"), 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dSpaceHorGap = newRec->HorGap();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dSpaceHorGap;
			}
			dSpaceHorGap = newRec->HorGap();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setHorSpace(dSpaceHorGap);
	}
	return dSpaceHorGap;
}

void CBaseUtils::setHorSpace(double dLeftVerGap)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setHorGap(dLeftVerGap);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setHorGap(dLeftVerGap);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CBaseUtils::VerSpace()
{
	double  dSpaceHorGap;
	dSpaceHorGap =  gGlobal.GetIniValue(_T("基础设置"), _T("表格纵向间距"), 0);
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				dSpaceHorGap = newRec->VerGap();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return dSpaceHorGap;
			}
			dSpaceHorGap = newRec->VerGap();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setVerSpace(dSpaceHorGap);
	}
	return dSpaceHorGap;
}

void CBaseUtils::setVerSpace(double dLeftVerGap)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setVerGap(dLeftVerGap);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setVerGap(dLeftVerGap);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

int CBaseUtils::frameCount()
{
	int  nCount;
	nCount = 1;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nCount = newRec->frameCount();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nCount;
			}
			nCount = newRec->frameCount();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setFrameCount(nCount);
	}
	return nCount;
}

void CBaseUtils::setFrameCount(int nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setFrameCount(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setFrameCount(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}
int CBaseUtils::rfCount()
{
	int  nCount;
	nCount = 1;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nCount = newRec->rfCount();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nCount;
			}
			nCount = newRec->rfCount();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setRfCount(nCount);
	}
	return nCount;
}

void CBaseUtils::setRfCount(int nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setRfCount(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setRfCount(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}


int CBaseUtils::dpCount()
{
	int  nCount;
	nCount = 1;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nCount = newRec->dpCount();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nCount;
			}
			nCount = newRec->dpCount();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setDpCount(nCount);
	}
	return nCount;
}

void CBaseUtils::setDpCount(int nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setDpCount(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setDpCount(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}


int CBaseUtils::firstCount()
{
	int  nCount;
	nCount = 1;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nCount = newRec->firstCount();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nCount;
			}
			nCount = newRec->firstCount();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setFirstCount(nCount);
	}
	return nCount;
}

void CBaseUtils::setFirstCount(int nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setFirstCount(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setFirstCount(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}


int CBaseUtils::secondCount()
{
	int  nCount;
	nCount = 1;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nCount = newRec->secondCount();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nCount;
			}
			nCount = newRec->secondCount();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setSecondCount(nCount);
	}
	return nCount;
}

void CBaseUtils::setSecondCount(int nCount)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setSecondCount(nCount);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setSecondCount(nCount);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

CString CBaseUtils::filePath()
{
	CString  strType;
	strType = _T("");
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseInfoInDwgFile::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				strType = newRec->filePath();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return strType;
			}
			strType = newRec->filePath();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setFilePath(strType);
	}
	return strType;
}

void CBaseUtils::setFilePath(CString strFilePath)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseInfoInDwgFile::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
			newRec->setFilePath(strFilePath);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseInfoInDwgFile* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setFilePath(strFilePath);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

CString CBaseUtils::GetTexingType(int nType)
{
	switch (nType)
	{
	case -1:
		return _T("[D]");
	case 0:
		return _T("[普通数据]");
	case 1:
		return _T("[DP]");
	case 2:
		return _T("[RF]");
	case 3:
		return _T("[数字光]");
	case 4:
		return _T("[模拟光]");
	case 5:
		return _T("[集束光]");
	case 6:
		return _T("[R层级]");
	case 7:
		return _T("[S]");
	case 8:
		return _T("[M]");
	case 9:
		return _T("[R+S]");
	case 10:
		return _T("[R+M]");
	case 11:
		return _T("[光纤数据]");
	case 12:
		return _T("[LRMV]");
	default:
		break;
	}
	return _T("");
}

int CBaseUtils::GetTypeTexing(CString sType)
{
	if (sType == _T("[D]"))
	{
		return -1;
	}
	else if (sType == _T("[普通数据]"))
	{
		return 0;
	}
	else if (sType == _T("[DP]"))
	{
		return 1;
	}
	else if (sType == _T("[RF]"))
	{
		return 2;
	}
	else if (sType == _T("[数字光]"))
	{
		return 3;
	}
	else if (sType == _T("[模拟光]"))
	{
		return 4;
	}
	else if (sType == _T("[集束光]"))
	{
		return 5;
	}
	else if (sType == _T("[R层级]"))
	{
		return 6;
	}
	else if (sType == _T("[S]"))
	{
		return 7;
	}
	else if (sType == _T("[M]"))
	{
		return 8;
	}
	else if (sType == _T("[R+S]"))
	{
		return 9;
	}
	else if (sType == _T("[R+M]"))
	{
		return 10;
	}
	else if (sType == _T("[光纤数据]"))
	{
		return 11;
	}
	else if (sType == _T("[LRMV]"))
	{
		return 12;
	}
	else
	{
		return 100;
	}
}

AcDbObjectIdArray CBaseUtils::GetObjectIdArrayForXdata(CString xdataTag)
{
	AcDbObjectIdArray IDARRRET;
	struct resbuf *rb;
	rb = acutBuildList(1001, xdataTag, RTNONE);
	ads_name textArr;
	acedSSGet(TEXT("X"), NULL, NULL, rb, textArr);
	long Lenth;
	acedSSLength(textArr, &Lenth);
	for (int i = 0; i < Lenth; i++)
	{
		ads_name ent;
		acedSSName(textArr, i, ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId, ent);
		IDARRRET.append(objId);
	}
	acutRelRb(rb);
	acedSSFree(textArr);
	return IDARRRET;
}

AcDbExtents CBaseUtils::GetExtentsForEntity(AcDbObjectId idEnt)
{
	AcDbExtents ext;
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es!=eOk)
	{
		return ext;
	}
	pEnt->getGeomExtents(ext);
	pEnt->close();
	return ext;
}

AcDbObjectIdArray CBaseUtils::GetObjectIdArrayForRange(AcDbExtents ext)
{
	AcDbObjectIdArray IDARRRET;
	AcGePoint3d ptMin,ptMax;
	ptMin = ext.minPoint();
	ptMax = ext.maxPoint();
	ads_point ads_Min, ads_Max;
	ads_Min[X] = ptMin[X];
	ads_Min[Y] = ptMin[Y];
	ads_Min[Z] = ptMin[Z];
	ads_Max[X] = ptMax[X];
	ads_Max[Y] = ptMax[Y];
	ads_Max[Z] = ptMax[Z];

	ads_name textArr;
	acedSSGet(TEXT("W"),ads_Min , ads_Max, NULL, textArr);
	long Lenth;
	acedSSLength(textArr, &Lenth);
	for (int i = 0; i < Lenth; i++)
	{
		ads_name ent;
		acedSSName(textArr, i, ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId, ent);
		IDARRRET.append(objId);
	}
	acedSSFree(textArr);
	return IDARRRET;
}

AcGePoint3d CBaseUtils::GetBlockRefPostion(AcDbObjectId idBlock)
{
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	AcGePoint3d ptRet = AcGePoint3d::kOrigin;
	es = acdbOpenAcDbEntity(pEnt, idBlock, AcDb::kForWrite);
	if (es!=eOk)
	{
		return ptRet;
	}
	if (!pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		pEnt->close();
		return ptRet;
	}
	AcDbBlockReference *pRef = static_cast<AcDbBlockReference*>(pEnt);
	ptRet = pRef->position();
	pEnt->close();
	return ptRet;
}

AcDbObjectId CBaseUtils::GetEntityLayer(AcDbObjectId idEnt)
{
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	AcDbObjectId idRet = AcDbObjectId::kNull;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es != eOk)
	{
		return idRet;
	}
	idRet = pEnt->layerId();
	pEnt->close();
	return idRet;
}
