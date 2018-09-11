#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "MyBaseXdata.h"
#include "MyBaseUtils.h"
#include "AcadStr.h"




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////  MyBaseAppXdata  //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

MyBaseAppXdata::MyBaseAppXdata(LPCTSTR appName, AcDbDatabase* db)
:   m_xdata(NULL)
{
    ASSERT(db != NULL);

        // make sure application name is registered
    AcDbRegAppTable* regAppTbl;
    Acad::ErrorStatus es = db->getSymbolTable(regAppTbl, AcDb::kForRead);
    if (es == Acad::eOk) {
        if (regAppTbl->has(appName) == false) {
            es = regAppTbl->upgradeOpen();
            if (es == Acad::eOk) {
                AcDbRegAppTableRecord* newRec = new AcDbRegAppTableRecord;
                es = newRec->setName(appName);
                if (es == Acad::eOk) {
                    es = regAppTbl->add(newRec);
                    if (es == Acad::eOk)
                        newRec->close();
                    else {
                        ASSERT(0);
                        delete newRec;
                    }
                }
                else {
                    ASSERT(0);
                    delete newRec;
                }
            }
            else {
                ASSERT(0);
            }
        }
        regAppTbl->close();
    }
    else {
        ASSERT(0);
    }

        // add 1001 code for application name
    m_appNameNode = acutNewRb(1001);
    acutNewString(appName, m_appNameNode->resval.rstring);
}


MyBaseAppXdata::~MyBaseAppXdata()
{
    if (m_appNameNode)
        acutRelRb(m_appNameNode);
}


resbuf*
MyBaseAppXdata::findItemStart(int tag)
{
    int nestedLists = 0;
    resbuf* tmp = m_xdata.data();
    while (tmp) {
			// keep track of nested lists
        if (tmp->restype == 1002) {
            if (!_tcscmp(tmp->resval.rstring, AcadString::openBrace))
                nestedLists++;
            else if(!_tcscmp(tmp->resval.rstring, AcadString::endBrace))
                nestedLists--;
            tmp = tmp->rbnext;
        }
			// make sure it is not within a list and is not the int value
			// of another tag
        else if ((tmp->restype == 1070) && (nestedLists == 0)) {
            if (tmp->resval.rint == tag)
                return tmp;

				// skip around value of the tag
            if (tmp->rbnext) {
                tmp = tmp->rbnext;
                if (tmp->restype == 1002)
                    ;    // start of a list, do nothing
                else {        // skip next rb which is tag value
                    if (tmp->rbnext == NULL)
                        return NULL;    // never found it
                    tmp = tmp->rbnext;
                }
            }
            else
                return NULL;        // never found it
        }
        else
            tmp = tmp->rbnext;
    }
    return NULL;    // never found it
}


resbuf*
MyBaseAppXdata::findItemEnd(resbuf* startPtr)
{
    int nestedLists = 0;
    resbuf* tmp = startPtr;

		// skip past tag node
    if (tmp->rbnext)
        tmp = tmp->rbnext;

    while (tmp) {
        if (tmp->restype == 1002) {
            if (!_tcscmp(tmp->resval.rstring, AcadString::openBrace))
                nestedLists++;
            else if (!_tcscmp(tmp->resval.rstring, AcadString::endBrace))
                nestedLists--;
        }
        if (nestedLists == 0)// found end of list or it was a single item, stop looking
            return(tmp);

        tmp = tmp->rbnext;
    }
    return NULL;
}


void
MyBaseAppXdata::addTagNode(int tag)
{
    resbuf* tagNode = acutNewRb(1070);
    tagNode->resval.rint = static_cast<short>(tag);
    m_xdata.addTail(tagNode);
}


bool
MyBaseAppXdata::getGenericReal(int tag, double& value, bool speakIfMissing, int dxfType)
{
    resbuf* itemPtr;

    if ((itemPtr = findItemStart(tag)) == NULL) {
        if (speakIfMissing)
            missingXdataTagWarning(tag);
        return false;
    }
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != dxfType)) {
            malformedXdataTagError(tag);
            return false;
        }
        else {
            value = itemPtr->rbnext->resval.rreal;
            return true;
        }
    }
}


bool
MyBaseAppXdata::getGenericString(int tag, CString& value, bool speakIfMissing, int dxfType)
{
    resbuf* itemPtr;

    if ((itemPtr = findItemStart(tag)) == NULL) {
        if (speakIfMissing)
            missingXdataTagWarning(tag);
        return false;
    }
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != dxfType)) {
            malformedXdataTagError(tag);
            return false;
        }
        else {
            value = itemPtr->rbnext->resval.rstring;
            return true;
        }
    }
}


bool
MyBaseAppXdata::getHandle(int tag, AcDbHandle& value, bool speakIfMissing)
{
    CString handleStr;
    if (getGenericString(tag, handleStr, speakIfMissing, 1005) == true) {
        value = handleStr;
        return true;
    }
    else
        return false;
}

bool
MyBaseAppXdata::getInteger(int tag, int& value, bool speakIfMissing)
{
    resbuf* itemPtr;

    if ((itemPtr = findItemStart(tag)) == NULL) {
        if (speakIfMissing)
            missingXdataTagWarning(tag);
        return false;
    }
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != 1070)) {
            if (speakIfMissing)
                malformedXdataTagError(tag);
            return false;
        }
        else {
            value = itemPtr->rbnext->resval.rint;
            return true;
        }
    }
}


bool
MyBaseAppXdata::getLong(int tag, long& value, bool speakIfMissing)
{
    resbuf* itemPtr;

    if ((itemPtr = findItemStart(tag)) == NULL) {
        if (speakIfMissing)
            missingXdataTagWarning(tag);
        return false;
    }
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != 1071)) {
            if (speakIfMissing)
                malformedXdataTagError(tag);
            return false;
        }
        else {
            value = itemPtr->rbnext->resval.rlong;
            return true;
        }
    }
}


bool
MyBaseAppXdata::getGenericPoint(int tag, AcGePoint3d& value, bool speakIfMissing, int dxfType)
{
    resbuf* itemPtr;

    if ((itemPtr = findItemStart(tag)) == NULL) {
        if (speakIfMissing)
            missingXdataTagWarning(tag);
        return false;
    }
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != dxfType)) {
            malformedXdataTagError(tag);
            return false;
        }
        else {
            value = asPnt3d(itemPtr->rbnext->resval.rpoint);
            return true;
        }
    }
}


bool
MyBaseAppXdata::getDirection(int tag, AcGeVector3d& value, bool speakIfMissing)
{
    resbuf* itemPtr;

    if ((itemPtr = findItemStart(tag)) == NULL) {
        if (speakIfMissing)
            missingXdataTagWarning(tag);
        return false;
    }
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != 1013)) {
            malformedXdataTagError(tag);
            return false;
        }
        else {
            value = asVec3d(itemPtr->rbnext->resval.rpoint);
            return true;
        }
    }
}


void
MyBaseAppXdata::setInteger(int tag, int value)
{
    resbuf* itemPtr;

		// if it doesn't already exist, add tag node
		// and value node
    if ((itemPtr = findItemStart(tag)) == NULL) {
        addTagNode(tag);
        itemPtr = acutNewRb(1070);
        itemPtr->resval.rint = static_cast<short>(value);
        m_xdata.addTail(itemPtr);
    }
		// if it does exist, just set to new value
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != 1070)) {
            malformedXdataTagError(tag);
            return;
        }
        itemPtr->rbnext->resval.rint = static_cast<short>(value);
    }
}

void
MyBaseAppXdata::setGenericReal(int tag, double value, int dxfType)
{
    resbuf* itemPtr;

		// if it doesn't already exist, add tag node
		// and value node
    if ((itemPtr = findItemStart(tag)) == NULL) {
        addTagNode(tag);

        itemPtr = acutNewRb(dxfType);
        itemPtr->resval.rreal = value;
        m_xdata.addTail(itemPtr);
    }
		// if it does exist, just set to new value
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != dxfType)) {
            malformedXdataTagError(tag);
            return;
        }
        itemPtr->rbnext->resval.rreal = value;
    }
}

void
MyBaseAppXdata::setLong(int tag, long value)
{
    resbuf* itemPtr;

		// if it doesn't already exist, add tag node
		// and value node
    if ((itemPtr = findItemStart(tag)) == NULL) {
        addTagNode(tag);

        itemPtr = acutNewRb(1071);
        itemPtr->resval.rlong = value;
        m_xdata.addTail(itemPtr);
    }
		// if it does exist, just set to new value
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != 1071)) {
            malformedXdataTagError(tag);
            return;
        }
        itemPtr->rbnext->resval.rlong = value;
    }
}

void
MyBaseAppXdata::setGenericString(int tag, const TCHAR* value, int dxfType)
{
    resbuf* itemPtr;

		// if it doesn't already exist, add tag node
		// and value node
    if ((itemPtr = findItemStart(tag)) == NULL) {
        addTagNode(tag);

        itemPtr = acutNewRb(dxfType);
        acutNewString(value,itemPtr->resval.rstring);

        m_xdata.addTail(itemPtr);
    }
		// if it does exist, just set to new value
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != dxfType)) {
            malformedXdataTagError(tag);
            return;
        }

        if (itemPtr->rbnext->resval.rstring)
            acutDelString(itemPtr->rbnext->resval.rstring);
        acutNewString(value,itemPtr->rbnext->resval.rstring);
    }
}

void
MyBaseAppXdata::setLayer(int tag, const TCHAR* value)
{
    bool isValid = MyBaseUtils::symbolExists(AcDbLayerTableRecord::desc(), value, acdbHostApplicationServices()->workingDatabase());
    ASSERT(isValid == true);

    if (isValid)
        setGenericString(tag, value, 1003);
    else {
        acutPrintf(_T("\nERROR: Invalid layer %s"), value);
    }
}

void
MyBaseAppXdata::setHandle(int tag, const AcDbHandle& value)
{
    TCHAR handleStr[256];

    value.getIntoAsciiBuffer(handleStr);
    setGenericString(tag, handleStr, 1005);
}

void
MyBaseAppXdata::setGenericPoint(int tag, const AcGePoint3d& value, int dxfType)
{
    resbuf* itemPtr;

		// if it doesn't already exist, add tag node
		// and value node
    if ((itemPtr = findItemStart(tag)) == NULL) {
        addTagNode(tag);

        itemPtr = acutNewRb(dxfType);
        itemPtr->resval.rpoint[0] = value.x;
        itemPtr->resval.rpoint[1] = value.y;
        itemPtr->resval.rpoint[2] = value.z;
        m_xdata.addTail(itemPtr);
    }
		// if it does exist, just set to new value
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != dxfType)) {
            malformedXdataTagError(tag);
            return;
        }

        itemPtr->rbnext->resval.rpoint[0] = value.x;
        itemPtr->rbnext->resval.rpoint[1] = value.y;
        itemPtr->rbnext->resval.rpoint[2] = value.z;
    }
}

void
MyBaseAppXdata::setDirection(int tag, const AcGeVector3d& value)
{
    resbuf* itemPtr;

		// if it doesn't already exist, add tag node
		// and value node
    if ((itemPtr = findItemStart(tag)) == NULL) {
        addTagNode(tag);

        itemPtr = acutNewRb(1013);
        itemPtr->resval.rpoint[0] = value.x;
        itemPtr->resval.rpoint[1] = value.y;
        itemPtr->resval.rpoint[2] = value.z;
        m_xdata.addTail(itemPtr);
    }
		// if it does exist, just set to new value
    else {
        if ((itemPtr->rbnext == NULL) || (itemPtr->rbnext->restype != 1013)) {
            malformedXdataTagError(tag);
            return;
        }

        itemPtr->rbnext->resval.rpoint[0] = value.x;
        itemPtr->rbnext->resval.rpoint[1] = value.y;
        itemPtr->rbnext->resval.rpoint[2] = value.z;
    }
}

resbuf*
MyBaseAppXdata::detach(int tag)
{
    resbuf* startPtr = findItemStart(tag);
    if (startPtr == NULL)
        return(NULL);    // not on the list

    ASSERT(startPtr->rbnext != NULL);
    if (startPtr->rbnext == NULL) {
        malformedXdataTagError(tag);
        return(NULL);
    }

    resbuf* endPtr = findItemEnd(startPtr);
    ASSERT(endPtr != NULL);
    if (endPtr == NULL) {
        malformedXdataTagError(tag);
        return(NULL);
    }

    return m_xdata.detach(startPtr, endPtr);
}

bool
MyBaseAppXdata::remove(int tag)
{
    resbuf* rb = detach(tag);
    if (rb != NULL) {
        acutRelRb(rb);
        return true;
    }
    else
        return false;
}

bool
MyBaseAppXdata::isBeginList(resbuf* rb)
{
    return (rb->restype == 1002) && (!_tcscmp(rb->resval.rstring, AcadString::openBrace));
}


bool
MyBaseAppXdata::isEndList(resbuf* rb)
{
    return (rb->restype == 1002) && (!_tcscmp(rb->resval.rstring, AcadString::endBrace));
}


bool
MyBaseAppXdata::getList(int tag, MyBaseXdataList& xdataList)
{
    xdataList.removeAll();    // remove one if it is already there

    resbuf* startPtr = findItemStart(tag);
    resbuf* tagPtr = startPtr;    // keep original start point
    if (startPtr == NULL)
        return false;    // not on the list

    ASSERT(startPtr->rbnext != NULL);
    if (startPtr->rbnext == NULL) {
        malformedXdataTagError(tag);
        return false;
    }
    // skip past tag, ensure that it is an open brace
    startPtr = startPtr->rbnext;
    ASSERT(isBeginList(startPtr));
    if (isBeginList(startPtr) == false) {
        malformedXdataTagError(tag);
        return false;
    }
    // find end of item, ensure that it is an end brace
    resbuf* endPtr = findItemEnd(tagPtr);
    ASSERT(endPtr != NULL);
    if (endPtr == NULL){
        malformedXdataTagError(tag);
        return false;
    }
    ASSERT(isEndList(endPtr));
    if (isEndList(endPtr)== false) {
        malformedXdataTagError(tag);
        return false;
    }

    MyBaseRbList xdata(duplicateResbufChain(startPtr, endPtr));
    xdata.removeHead();    // remove opening brace
    xdata.removeTail();    // remove ending brace
    xdataList.adoptResbufChain(xdata.orphanData());

    return true;
}

bool
MyBaseAppXdata::detachList(int tag, MyBaseXdataList& xdataList)
{
    resbuf* startPtr = detach(tag);
    if(startPtr == NULL)
        return false;

    ASSERT(startPtr->rbnext != NULL);
    ASSERT(isBeginList(startPtr->rbnext));
    if ((startPtr->rbnext == NULL) || (isBeginList(startPtr->rbnext) == false)) {
        malformedXdataTagError(tag);
        return false;
    }
    resbuf* endPtr = tailOfResbufChain(startPtr);

    ASSERT(endPtr != NULL);
    ASSERT(isEndList(endPtr));
    if ((endPtr == NULL) || (isEndList(endPtr) == false)) {
        malformedXdataTagError(tag);
        return false;
    }

    MyBaseRbList xdata(startPtr);      // adopts the resbuf
    xdata.removeHead();            // remove tag
    xdata.removeHead();            // remove opening brace
    xdata.removeTail();            // remove ending brace
    xdataList.adoptResbufChain(xdata.orphanData());
    return true;
}

void
MyBaseAppXdata::setList(int tag, MyBaseXdataList& xdataList)
{
    remove(tag);    // remove one if it is already there

    MyBaseRbList appendList(duplicateResbufChain(xdataList.m_xdata.data()));
    resbuf* newNode;

		// add the opening brace
    newNode = acutNewRb(1002);
    acutNewString(AcadString::openBrace,newNode->resval.rstring);
    appendList.addHead(newNode);

		// add the end brace
    newNode = acutNewRb(1002);
    acutNewString(AcadString::endBrace,newNode->resval.rstring);
    appendList.addTail(newNode);

		// put tag on the front
    newNode = acutNewRb(1070);
    newNode->resval.rint = static_cast<short>(tag);
    appendList.addHead(newNode);

		// glue onto the xdata
    m_xdata.append(appendList);
}

void
MyBaseAppXdata::adoptResbufChain(resbuf* xdata)
{
    resbuf* tmp;

		// empty previous list
    m_xdata.removeAll();

		// pop -3 code off if it exists(should never happen in Rx)
    if (xdata &&(xdata->restype == -3)) {
        tmp = xdata;
        xdata = xdata->rbnext;
        tmp->rbnext = NULL;
        acutRelRb(tmp);
    }
		// pop 1001 code off if it exists
    if (xdata &&(xdata->restype == 1001)) {
        tmp = xdata;
        xdata = xdata->rbnext;
        tmp->rbnext = NULL;
        acutRelRb(tmp);
    }

    m_xdata.adoptData(xdata);
}

void
MyBaseAppXdata::getXdata(AcDbObject* obj)
{
    ASSERT(obj != NULL);

		// empty previous list
    m_xdata.removeAll();
    resbuf* xdata = obj->xData(getAppName());
    if (xdata != NULL)
        m_xdata.adoptData(xdata);
}

Acad::ErrorStatus
MyBaseAppXdata::setXdata(AcDbObject* obj)
{
    ASSERT(obj != NULL);
    ASSERT(obj->isWriteEnabled());

		// hook all the components together into one list
    m_appNameNode->rbnext = m_xdata.data();

		// set in object xdata
    Acad::ErrorStatus es = obj->setXData(m_appNameNode);

		// break them apart again
    m_appNameNode->rbnext = NULL;

    if (es != Acad::eOk) {
        acutPrintf(_T("\nERROR: could not set Xdata (%s)"), MyBaseUtils::rxErrorStr(es));
    }
    return es;
}

bool MyBaseAppXdata::setXdata(CString appname, CString value, AcDbEntity *pEnt)
{
	acdbRegApp(appname);
	struct resbuf*rb = acutBuildList(AcDb::kDxfRegAppName, appname, AcDb::kDxfXdAsciiString, value, RTNONE);
	pEnt->setXData(rb);
	acutRelRb(rb);
	return true;
}

bool MyBaseAppXdata::setXdata(CString appname, int value, AcDbEntity*pEnt)
{
	acdbRegApp(appname);
	struct resbuf*rb = acutBuildList(AcDb::kDxfRegAppName, appname, AcDb::kDxfXdInteger32, value, RTNONE);
	pEnt->setXData(rb);
	acutRelRb(rb);
	return true;
}

bool MyBaseAppXdata::setXdata(CString appname, AcGePoint3d value, AcDbEntity*pEnt)
{
	acdbRegApp(appname);
	struct resbuf*rb = acutBuildList(AcDb::kDxfRegAppName, appname, AcDb::kDxfXdWorldXCoord, asDblArray(value), RTNONE);
	pEnt->setXData(rb);
	acutRelRb(rb);
	return true;
}

bool MyBaseAppXdata::setXdata(CString appname, CString value, AcDbObjectId idEnt)
{
	bool bRet;
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es!=eOk)
	{
		return false;
	}
	bRet=setXdata(appname, value, pEnt);
	pEnt->close();
	return bRet;
}

bool MyBaseAppXdata::setXdata(CString appname, int value, AcDbObjectId idEnt)
{
	bool bRet;
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es != eOk)
	{
		return false;
	}
	bRet=setXdata(appname, value, pEnt);
	pEnt->close();
	return bRet;
}

bool MyBaseAppXdata::setXdata(CString appname, AcGePoint3d value, AcDbObjectId idEnt)
{
	bool bRet;
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es != eOk)
	{
		return false;
	}
	bRet = setXdata(appname, value, pEnt);
	pEnt->close();
	return bRet;
}

bool MyBaseAppXdata::getXdata(CString appname, CString &value, AcDbObjectId idEnt)
{
	bool bRet;
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es != eOk)
	{
		return false;
	}
	bRet = getXdata(appname, value, pEnt);
	pEnt->close();
	return bRet;
}

bool MyBaseAppXdata::getXdata(CString appname, int &value, AcDbObjectId idEnt)
{
	bool bRet;
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es != eOk)
	{
		return false;
	}
	bRet = getXdata(appname, value, pEnt);
	pEnt->close();
	return bRet;
}

bool MyBaseAppXdata::getXdata(CString appname, AcGePoint3d &value, AcDbObjectId idEnt)
{
	bool bRet;
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForWrite);
	if (es != eOk)
	{
		return false;
	}
	bRet = getXdata(appname, value, pEnt);
	pEnt->close();
	return bRet;
}

bool MyBaseAppXdata::getXdata(CString appname, CString &value, AcDbEntity *pEnt)
{
	struct resbuf *pRb = pEnt->xData(appname);
	if (pRb)
	{
		struct resbuf *pTemp = pRb;
		pTemp = pTemp->rbnext;
		while (pTemp)
		{
			if (AcDb::kDxfXdAsciiString == pTemp->restype)
			{
				value = pTemp->resval.rstring;
				acutRelRb(pRb);
				return true;
			}
			pTemp = pTemp->rbnext;
		}
	}
	acutRelRb(pRb);
	return false;
}

bool MyBaseAppXdata::getXdata(CString appname, int &value, AcDbEntity *pEnt)
{
	struct resbuf *pRb = pEnt->xData(appname);
	if (pRb)
	{
		struct resbuf *pTemp = pRb;
		pTemp = pTemp->rbnext;
		while (pTemp)
		{
			if (AcDb::kDxfXdInteger32 == pTemp->restype)
			{
				value = pTemp->resval.rint;
				acutRelRb(pRb);
				return true;
			}
			pTemp = pTemp->rbnext;
		}
	}
	acutRelRb(pRb);
	return false;
}

bool MyBaseAppXdata::getXdata(CString appname, AcGePoint3d &value, AcDbEntity*pEnt)
{
	struct resbuf *pRb = pEnt->xData(appname);
	if (pRb)
	{
		struct resbuf *pTemp = pRb;
		pTemp = pTemp->rbnext;
		while (pTemp)
		{
			if (AcDb::kDxfXdWorldXCoord == pTemp->restype)
			{
				value[X] = pTemp->resval.rpoint[X];
				value[Y] = pTemp->resval.rpoint[Y];
				value[Z] = pTemp->resval.rpoint[Z];
				acutRelRb(pRb);
				return true;
			}
			pTemp = pTemp->rbnext;
		}
	}
	acutRelRb(pRb);
	return false;
}

resbuf*
MyBaseAppXdata::allocResbufChain()
{
    resbuf* xdata = duplicateResbufChain(m_appNameNode);
    resbuf* xdataContent = duplicateResbufChain(m_xdata.data());

    // connect them together
    xdata->rbnext = xdataContent;
    return xdata;
}

const TCHAR*
MyBaseAppXdata::getAppName()const
{
    ASSERT(m_appNameNode != NULL);
    return m_appNameNode->resval.rstring;
}

bool
MyBaseAppXdata::getReal(int tag, double& value, bool speakIfMissing)
{
    return getGenericReal(tag, value, speakIfMissing, 1040);
}


bool
MyBaseAppXdata::getDistance(int tag, double& value, bool speakIfMissing)
{
    return getGenericReal(tag, value, speakIfMissing, 1041);
}


bool
MyBaseAppXdata::getScale(int tag, double& value, bool speakIfMissing)
{
    return getGenericReal(tag, value, speakIfMissing, 1042);
}


bool
MyBaseAppXdata::getString(int tag, CString& value, bool speakIfMissing)
{
    return getGenericString(tag, value, speakIfMissing, 1000);
}

bool
MyBaseAppXdata::getLayer(int tag, CString& value, bool speakIfMissing)
{
    return getGenericString(tag, value, speakIfMissing, 1003);
}


bool
MyBaseAppXdata::getPoint(int tag, AcGePoint3d& value, bool speakIfMissing)
{
    return getGenericPoint(tag, value, speakIfMissing, 1010);
}

bool
MyBaseAppXdata::getPosition(int tag, AcGePoint3d& value, bool speakIfMissing)
{
    return getGenericPoint(tag, value, speakIfMissing, 1011);
}


bool
MyBaseAppXdata::getDisplacement(int tag, AcGePoint3d& value, bool speakIfMissing)
{
    return getGenericPoint(tag, value, speakIfMissing, 1012);
}


void
MyBaseAppXdata::setReal(int tag, double value)
{
    setGenericReal(tag, value, 1040);
}


void
MyBaseAppXdata::setDistance(int tag, double value)
{
    setGenericReal(tag, value, 1041);
}


void
MyBaseAppXdata::setScale(int tag, double value)
{
    setGenericReal(tag, value, 1042);
}


void
MyBaseAppXdata::setString(int tag, LPCTSTR value)
{
    setGenericString(tag, value, 1000);
}

void
MyBaseAppXdata::setPoint(int tag, const AcGePoint3d& value)
{
    setGenericPoint(tag, value, 1010);
}

void
MyBaseAppXdata::setPosition(int tag, const AcGePoint3d& value)
{
    setGenericPoint(tag, value, 1011);
}


void
MyBaseAppXdata::setDisplacement(int tag, const AcGePoint3d& value)
{
    setGenericPoint(tag, value, 1012);
}

bool
MyBaseAppXdata::isEmpty()
{
    return m_xdata.isEmpty();
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////  MyBaseXdataList  //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

MyBaseXdataList::MyBaseXdataList()
{
}


MyBaseXdataList::~MyBaseXdataList()
{
}

void
MyBaseXdataList::appendInteger(int value)
{
    resbuf* newNode = acutNewRb(1070);

    newNode->resval.rint = static_cast<short>(value);
    m_xdata.addTail(newNode);
}


void
MyBaseXdataList::appendLong(long value)
{
    resbuf* newNode = acutNewRb(1071);
    newNode->resval.rlong = value;
    m_xdata.addTail(newNode);
}


void
MyBaseXdataList::appendGenericReal(double value, int dxfType)
{
    resbuf* newNode = acutNewRb(dxfType);
    newNode->resval.rreal = value;
    m_xdata.addTail(newNode);
}


void
MyBaseXdataList::appendString(const TCHAR* value)
{
    resbuf* newNode = acutNewRb(1000);
    acutNewString(value, newNode->resval.rstring);
    m_xdata.addTail(newNode);
}


void
MyBaseXdataList::appendLayer(const TCHAR* value)
{
    bool isValid = MyBaseUtils::symbolExists(AcDbLayerTableRecord::desc(), value,
						acdbHostApplicationServices()->workingDatabase());
    ASSERT(isValid == true);

    if (isValid) {
        resbuf* newNode = acutNewRb(1003);
        acutNewString(value, newNode->resval.rstring);
        m_xdata.addTail(newNode);
    }
    else {
        acutPrintf(_T("\nERROR: could not append layer %s in Xdata"), value);
    }
}


void
MyBaseXdataList::appendHandle(const AcDbHandle& value)
{
    TCHAR handleStr[256];

    resbuf* newNode = acutNewRb(1005);
    value.getIntoAsciiBuffer(handleStr);
    acutNewString(handleStr, newNode->resval.rstring);
    m_xdata.addTail(newNode);
}


void
MyBaseXdataList::appendGenericPoint(const AcGePoint3d& value, int dxfType)
{
    resbuf* newNode = acutNewRb(dxfType);

    newNode->resval.rpoint[0] = value.x;
    newNode->resval.rpoint[1] = value.y;
    newNode->resval.rpoint[2] = value.z;

    m_xdata.addTail(newNode);
}


void
MyBaseXdataList::appendDirection(const AcGeVector3d& value)
{
    resbuf* newNode = acutNewRb(1013);

    newNode->resval.rpoint[0] = value.x;
    newNode->resval.rpoint[1] = value.y;
    newNode->resval.rpoint[2] = value.z;

    m_xdata.addTail(newNode);
}

void
MyBaseXdataList::appendReal(double value)
{
    appendGenericReal(value, 1040);
}


void
MyBaseXdataList::appendDistance(double value)
{
    appendGenericReal(value, 1041);
}


void
MyBaseXdataList::appendScale(double value)
{
    appendGenericReal(value, 1042);
}


void
MyBaseXdataList::appendPoint(const AcGePoint3d& value)
{
    appendGenericPoint(value, 1010);
}



void
MyBaseXdataList::appendPosition(const AcGePoint3d& value)
{
    appendGenericPoint(value, 1011);
}


void
MyBaseXdataList::appendDisplacement(const AcGePoint3d& value)
{
    appendGenericPoint(value, 1012);
}



void
MyBaseXdataList::removeAll()
{
    m_xdata.removeAll();
}



void
MyBaseXdataList::adoptResbufChain(resbuf* xdata)
{
    m_xdata.adoptData(xdata);
}



resbuf*
MyBaseXdataList::orphanResbufChain()
{
    return(m_xdata.orphanData());
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////  MyBaseXdataListIterator  //////////////////////////
/////////////////////////////////////////////////////////////////////////////



MyBaseXdataListIterator::MyBaseXdataListIterator(MyBaseXdataList& xdata)
{
		// deal with raw resbufs only
    m_xdata = xdata.m_xdata.data();
    m_curPtr = m_xdata;
}


MyBaseXdataListIterator::~MyBaseXdataListIterator()
{
    // somebody else owns the list, so just leave it alone
}



bool
MyBaseXdataListIterator::getInteger(int& value)
{
    if (m_curPtr && (m_curPtr->restype == 1070)) {
        value = m_curPtr->resval.rint;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


bool
MyBaseXdataListIterator::getLong(long& value)
{
    if (m_curPtr &&(m_curPtr->restype == 1071)) {
        value = m_curPtr->resval.rlong;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


bool
MyBaseXdataListIterator::getReal(double& value)
{
    return getGenericReal(value, 1040);
}



bool
MyBaseXdataListIterator::getString(CString& value)
{
    return getGenericString(value, 1000);
}



bool
MyBaseXdataListIterator::getGenericReal(double& value, int dxfType)
{
    if (m_curPtr && (m_curPtr->restype == dxfType)) {
        value = m_curPtr->resval.rreal;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}



bool
MyBaseXdataListIterator::getGenericString(CString& value, int dxfType)
{
    if (m_curPtr && (m_curPtr->restype == dxfType)) {
        value = m_curPtr->resval.rstring;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}



bool
MyBaseXdataListIterator::getHandle(AcDbHandle& value)
{
    CString handleStr;
    if (getGenericString(handleStr, 1005)) {
        value = handleStr;
        return true;
    }
    else
        return false;
}



bool
MyBaseXdataListIterator::getGenericPoint(AcGePoint3d& value, int dxfType)
{
    if (m_curPtr && (m_curPtr->restype == dxfType)) {
        value = asPnt3d(m_curPtr->resval.rpoint);
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


bool
MyBaseXdataListIterator::getDirection(AcGeVector3d& value)
{
    if (m_curPtr && (m_curPtr->restype == 1013)) {
        value = asVec3d(m_curPtr->resval.rpoint);
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}



bool
MyBaseXdataListIterator::setInteger(int value)
{
    if (m_curPtr && (m_curPtr->restype == 1070)) {
        m_curPtr->resval.rint = static_cast<short>(value);
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


bool
MyBaseXdataListIterator::setLong(long value)
{
    if (m_curPtr && (m_curPtr->restype == 1071)) {
        m_curPtr->resval.rlong = value;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


bool
MyBaseXdataListIterator::setGenericReal(double value, int dxfType)
{
    if (m_curPtr && (m_curPtr->restype == dxfType)) {
        m_curPtr->resval.rreal = value;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


bool
MyBaseXdataListIterator::setGenericString(const TCHAR* value, int dxfType)
{
    if (m_curPtr && (m_curPtr->restype == dxfType)) {
        if (m_curPtr->resval.rstring)
            acutDelString(m_curPtr->resval.rstring);
        acutNewString(value,m_curPtr->resval.rstring);
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}

bool
MyBaseXdataListIterator::setLayer(const TCHAR* value)
{
    bool isValid = MyBaseUtils::symbolExists(AcDbLayerTableRecord::desc(), value,
					acdbHostApplicationServices()->workingDatabase());
    ASSERT(isValid == true);

    if (isValid)
        return setGenericString(value, 1003);

    else {
        acutPrintf(_T("\nERROR: could not set layer %s in Xdata"), value);
        return false;
    }
}



bool
MyBaseXdataListIterator::setHandle(const AcDbHandle& value)
{
    TCHAR handleStr[256];
    value.getIntoAsciiBuffer(handleStr);
    return setGenericString(handleStr, 1005);
}


bool
MyBaseXdataListIterator::setGenericPoint(const AcGePoint3d& value, int dxfType)
{
    if (m_curPtr && (m_curPtr->restype == dxfType)) {
        m_curPtr->resval.rpoint[0] = value.x;
        m_curPtr->resval.rpoint[1] = value.y;
        m_curPtr->resval.rpoint[2] = value.z;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


bool
MyBaseXdataListIterator::setDirection(const AcGeVector3d& value)
{
    if (m_curPtr && (m_curPtr->restype == 1013)) {
        m_curPtr->resval.rpoint[0] = value.x;
        m_curPtr->resval.rpoint[1] = value.y;
        m_curPtr->resval.rpoint[2] = value.z;
        m_curPtr = m_curPtr->rbnext;
        return true;
    }
    else
        return false;
}


void
MyBaseXdataListIterator::start()
{
    m_curPtr = m_xdata;
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////  MyBaseAppXdataList  /////////////////////////////
/////////////////////////////////////////////////////////////////////////////


MyBaseAppXdataList::MyBaseAppXdataList(AcDbObject* const obj)
:	m_obj(obj),
	m_xdataStart(NULL)
{
    ASSERT(obj != NULL);

    divideApps(obj->xData());
}


MyBaseAppXdataList::~MyBaseAppXdataList()
{
    if (m_xdataStart != NULL)
        acutRelRb(m_xdataStart);

    removeAndDestroy();
}


bool
MyBaseAppXdataList::contains(LPCTSTR appName)const
{
    if (find(appName) == NULL)
        return false;
    else
        return true;
}


int
MyBaseAppXdataList::find(LPCTSTR appName)const
{
    for (int i = 0; i < m_list.length(); i ++) {
        if (!_tcsicmp(appName, m_list[i]->getAppName()))    
            return i;
    }
    return -1;
}



void
MyBaseAppXdataList::removeAndDestroy(LPCTSTR appName)
{
    int pos = find(appName);
    if (pos != -1) {
        delete m_list[pos];
        m_list.removeAt(pos);
    }
}


void
MyBaseAppXdataList::removeAndDestroy()
{
    for (int i = 0; i < m_list.length(); i ++) {
        delete m_list[i];
    }
    m_list.setLogicalLength(0);
}


void
MyBaseAppXdataList::divideApps(resbuf* xdata)
{
    MyBaseAppXdata* newApp;

    resbuf* lagPtr = NULL;
    resbuf* appPtr = NULL;
    resbuf* tmp = xdata;
    while (tmp) {
			// if beginning of xdata, save this node for later use
        if (tmp->restype == -3) {
            m_xdataStart = tmp;
            tmp = tmp->rbnext;
            lagPtr = tmp;
            m_xdataStart->rbnext = NULL;
        }
			// start of a new application
        else if (tmp->restype == 1001) {
            if (appPtr != NULL) {            // already seen at least one app
                lagPtr->rbnext = NULL;       // detach this app's list
                newApp = new MyBaseAppXdata(appPtr->resval.rstring, m_obj->database());
                newApp->adoptResbufChain(appPtr);
                append(newApp);
            }
            appPtr = tmp;
            lagPtr = tmp;
            tmp = tmp->rbnext;
        }
        // at end of xdata
        else if (tmp->rbnext == NULL) {
            if (appPtr != NULL) {            // already seen at least one app
                newApp = new MyBaseAppXdata(appPtr->resval.rstring, m_obj->database());
                newApp->adoptResbufChain(appPtr);
                append(newApp);
            }
            tmp = NULL;    // done walking list
        }
			// just advance to next node, looking for next app...
        else {
            lagPtr = tmp;
            tmp = tmp->rbnext;
        }
    }
}


void
MyBaseAppXdataList::append(MyBaseAppXdata* newApp)
{
    m_list.append(newApp);
}



void
MyBaseAppXdataList::prepend(MyBaseAppXdata* newApp)
{
    m_list.insertAt(0,newApp);
}


int
MyBaseAppXdataList::entries() const
{
    return m_list.length();
}


bool
MyBaseAppXdataList::isEmpty() const
{
    return m_list.isEmpty();
}



MyBaseAppXdata*
MyBaseAppXdataList::at(int index)
{
    ASSERT(index >= 0 && index < m_list.length());
    return m_list[index];
}



/////////////////////////////////////////////////////////////////////////////
///////////////////////  MyBaseAppXdataListIterator  ////////////////////////
/////////////////////////////////////////////////////////////////////////////



MyBaseAppXdataListIterator::MyBaseAppXdataListIterator(const MyBaseAppXdataList& list)
:   m_xdataList(list)
{
    if (m_xdataList.isEmpty())
        m_pos = -1;
    else 
        m_pos = 0;
}


MyBaseAppXdataListIterator::~MyBaseAppXdataListIterator()
{
}


void
MyBaseAppXdataListIterator::toFirst()
{
    if (m_xdataList.isEmpty())
        m_pos = -1;
    else 
        m_pos = 0;
}


void
MyBaseAppXdataListIterator::toLast()
{
    if (m_xdataList.isEmpty())
        m_pos = -1;
    else 
        m_pos = m_xdataList.m_list.length() - 1;
}


void
MyBaseAppXdataListIterator::previous()
{
    m_pos --;
}


void
MyBaseAppXdataListIterator::next()
{
    m_pos ++;
    if (m_pos == m_xdataList.m_list.length())
        m_pos = -1;
}


MyBaseAppXdata*
MyBaseAppXdataListIterator::item() const
{
    if (m_pos == -1)
        return NULL;

    return m_xdataList.m_list[m_pos];
}

bool
MyBaseAppXdataListIterator::done() const
{
    return m_pos == -1;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////  GLOBAL FUNCTIONS  ////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
malformedXdataTagError(int tag)
{
    acutPrintf(_T("\nERROR: malfomed Xdata tag: %d"), tag);
}


void
missingXdataTagWarning(int tag)
{
    acutPrintf(_T("\nWARNING: Xdata tag missing: %d"), tag);
}











