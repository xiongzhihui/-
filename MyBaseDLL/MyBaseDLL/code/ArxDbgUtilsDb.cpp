#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "MyBaseUtils.h"
//#include "SdSysVar.h"
#include "AcadStr.h"
#include "MyTransFunc.h"


/****************************************************************************
**
**	MyBaseUtils::objIdToEname
**
**	**jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::objIdToEname(const AcDbObjectId& objId, ads_name& ent)
{
    Acad::ErrorStatus es = acdbGetAdsName(ent, objId);
    ASSERT(es == Acad::eOk);
    if (es == Acad::eOk)
        MyBaseUtils::rxErrorMsg(es);

    return es;
}

/****************************************************************************
**
**	MyBaseUtils::enameToObjId
**
**	**jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::enameToObjId(ads_name ent, AcDbObjectId& objId)
{
    Acad::ErrorStatus es = acdbGetObjectId(objId, ent);
    ASSERT(es == Acad::eOk);
    if (es != Acad::eOk)
        MyBaseUtils::rxErrorMsg(es);
    
    return es;
}

/***********************************************************************
**
**	MyBaseUtils::handleStrToObjId
**		Convert a handle string to an objectId.
**
**	**jma
**
****************************************/

Acad::ErrorStatus
MyBaseUtils::handleStrToObjId(AcDbDatabase* db, LPCTSTR handleStr, AcDbObjectId& objId, bool speak)
{
    ASSERT(handleStr != NULL);
	ASSERT(db != NULL);

    AcDbHandle objHandle(handleStr);
    Acad::ErrorStatus es;

    es = db->getAcDbObjectId(objId, false, objHandle);
    if (es != Acad::eOk) {
        if (speak) {
            acutPrintf(_T("\nERROR: Could not translate handle to objectId (%s)"), MyBaseUtils::rxErrorStr(es));
        }
        return Acad::eInvalidInput;
    }
    else
        return Acad::eOk;
}

/***********************************************************************
**
**	MyBaseUtils::objIdToHandleStr
**
**	**jma
**
****************************************/

LPCTSTR
MyBaseUtils::objIdToHandleStr(const AcDbObjectId& objId, CString& str)
{
    AcDbObject* obj;
    Acad::ErrorStatus es;
    es = acdbOpenAcDbObject(obj, objId, AcDb::kForRead);
    if (es != Acad::eOk)
        str = ACDB_NULL_HANDLE;
    else {
        MyTransFunc::objToHandleStr(obj, str);
        obj->close();
    }
    return str;
}

/****************************************************************************
**
**	MyBaseUtils::cloneAndXformObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::cloneAndXformObjects(AcDbDatabase* db, const AcDbObjectId& entToClone,
                            const AcDbObjectId& ownerBlockId,
                            const AcGeMatrix3d& xformMat, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbObjectIdArray objIdList;
    objIdList.append(entToClone);

    return cloneAndXformObjects(db, objIdList, ownerBlockId, xformMat, debugSpeak);
}

/****************************************************************************
**
**	MyBaseUtils::cloneAndXformObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::cloneAndXformObjects(AcDbDatabase* db, const AcDbObjectIdArray& entsToClone,
                        const AcDbObjectId& ownerBlockId,
                        const AcGeMatrix3d& xformMat, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbIdMapping idMap;
    Acad::ErrorStatus es = db->deepCloneObjects(
                const_cast<AcDbObjectIdArray&>(entsToClone),
                const_cast<AcDbObjectId&>(ownerBlockId), idMap);

    if (es != Acad::eOk) {
        MyBaseUtils::rxErrorMsg(es);
        return es;
    }

    AcDbEntity* clonedEnt;
    AcDbIdPair idPair;
    AcDbIdMappingIter iter(idMap);
    for (iter.start(); !iter.done(); iter.next()) {
        if (!iter.getMap(idPair))
            return Acad::eInvalidInput;

        if (idPair.isCloned()) {
            es = acdbOpenAcDbEntity(clonedEnt, idPair.value(), AcDb::kForWrite);
            if (es == Acad::eOk) {
                if (idPair.isPrimary()) {
                    if (debugSpeak)
                        acutPrintf(_T("\nCloned And Transformed: %s"), MyTransFunc::objToClassStr(clonedEnt));

                    clonedEnt->transformBy(xformMat);
                }
                else if (debugSpeak)
                    acutPrintf(_T("\nCloned: %s"), MyTransFunc::objToClassStr(clonedEnt));

                clonedEnt->close();
            }
            else
                MyBaseUtils::rxErrorMsg(es);
        }
    }
    return Acad::eOk;
}

/****************************************************************************
**
**	MyBaseUtils::cloneObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::cloneObjects(AcDbDatabase* db, const AcDbObjectId& entToClone,
                        const AcDbObjectId& ownerBlockId, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbObjectIdArray objIdList;
    objIdList.append(entToClone);

    return cloneObjects(db, objIdList, ownerBlockId, debugSpeak);
}

/****************************************************************************
**
**	MyBaseUtils::cloneObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::cloneObjects(AcDbDatabase* db, const AcDbObjectIdArray& entsToClone,
                        const AcDbObjectId& ownerBlockId, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbIdMapping idMap;
    Acad::ErrorStatus es = db->deepCloneObjects(
                const_cast<AcDbObjectIdArray&>(entsToClone),
                const_cast<AcDbObjectId&>(ownerBlockId), idMap);

    if (es != Acad::eOk) {
        MyBaseUtils::rxErrorMsg(es);
        return es;
    }

    if (debugSpeak) {
        AcDbEntity* clonedEnt;
        AcDbIdPair idPair;
        AcDbIdMappingIter iter(idMap);
        for (iter.start(); !iter.done(); iter.next()) {
            if (!iter.getMap(idPair))
                return Acad::eInvalidInput;

            if (idPair.isCloned()) {
                es = acdbOpenAcDbEntity(clonedEnt, idPair.value(), AcDb::kForWrite);
                if (es == Acad::eOk) {
                    acutPrintf(_T("\nCloned: %s"), MyTransFunc::objToClassStr(clonedEnt));
                    clonedEnt->close();
                }
                else
                    MyBaseUtils::rxErrorMsg(es);
            }
        }
    }
    return Acad::eOk;
}

/****************************************************************************
**
**	MyBaseUtils::plineHasWidth
**
**	**jma
**
*************************************/

bool
MyBaseUtils::plineHasWidth(const AcDb2dPolyline* pline)
{
    ASSERT(pline != NULL);

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return false;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
                if ((vertex->startWidth() != 0.0) || (vertex->endWidth() != 0.0)) {
                    vertex->close();
                    delete vertexIter;
                    return true;        // bail out, we found what we needed
                }
            }
            vertex->close();
        }
    }
    delete vertexIter;
    return false;
}

/****************************************************************************
**
**	MyBaseUtils::plineHasWidth
**
**	**jma
**
*************************************/

bool
MyBaseUtils::plineHasWidth(const AcDbPolyline* pline)
{
    double startWidth, endWidth;
    Acad::ErrorStatus es;

    for (unsigned int i=0; i<pline->numVerts(); i++) {
        es = pline->getWidthsAt(i, startWidth, endWidth);
        if (es == Acad::eOk) {
            if ((startWidth > 0.0) || (endWidth > 0.0)) {
                return true;
            }
        }
    }
    return false;
}

/****************************************************************************
**
**  MyBaseUtils::plineHasZeroLengthSegments
**
**  **jma
**
*************************************/

bool
MyBaseUtils::plineHasZeroLengthSegments(const AcDbPolyline* pline)
{
    AcDbPolyline::SegType segType;

    int numSegs = MyBaseUtils::numSegments(pline);

    for (int i=0; i<numSegs; i++) {
        segType = pline->segType(i);
        if ((segType == AcDbPolyline::kCoincident) ||
            (segType == AcDbPolyline::kPoint) ||
            (segType == AcDbPolyline::kEmpty))
            return true;
    }

    return false;
}

/****************************************************************************
**
**	MyBaseUtils::numSegments
**		get the number of segments in a polyline.
**
**	**jma
**
*************************************/

int
MyBaseUtils::numSegments(const AcDbPolyline* pline)
{
    ASSERT(pline != NULL);

    int numSegs;
    if (pline->isClosed())
        numSegs = pline->numVerts();
    else
        numSegs = pline->numVerts() - 1;

    if (numSegs < 0)
        numSegs = 0;

    return numSegs;
}

/****************************************************************************
**
**	MyBaseUtils::collectVertices
**
**	**jma
**
*************************************/

void
MyBaseUtils::collectVertices(const AcDb2dPolyline* pline, AcGePoint3dArray& pts,
            AcGeDoubleArray& bulges, bool asWcsPts)
{
    ASSERT(pline != NULL);
    ASSERT(pts.isEmpty() && bulges.isEmpty());

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
                if (asWcsPts)
                    pts.append(pline->vertexPosition(*vertex));        // returns WCS
                else
                    pts.append(vertex->position());                    // returns ECS
                bulges.append(vertex->bulge());
            }
            vertex->close();
        }
    }
    delete vertexIter;

    ASSERT(pts.isEmpty() == Adesk::kFalse);

    if (pline->isClosed()) {
        AcGePoint3d tmpPt = pts[0];        // used to be a bug in dynamic arrays (not sure if its still there??)
        pts.append(tmpPt);
        bulges.append(0.0);
    }
}

/****************************************************************************
**
**	MyBaseUtils::collectVertices
**
**	**csa
**
*************************************/

void
MyBaseUtils::collectVertices(const AcDb2dPolyline* pline, AcGePoint3dArray& pts,
                        AcDbIntArray& types, AcGeDoubleArray& bulges,
                        AcGeDoubleArray& startWidths, 
                        AcGeDoubleArray& endWidths, bool& hasWidth)
{
    ASSERT(pline != NULL);
    ASSERT(pts.isEmpty() && bulges.isEmpty());

    hasWidth = false;

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
                pts.append(pline->vertexPosition(*vertex));        // returns WCS
                bulges.append(vertex->bulge());
                startWidths.append(vertex->startWidth());
                endWidths.append(vertex->endWidth());

                if (vertex->startWidth() || vertex->endWidth())
                    hasWidth = true;
                types.append(vertex->vertexType());
            }
            vertex->close();
        }
    }
    delete vertexIter;

    ASSERT(pts.isEmpty() == false);

    if (pline->isClosed()) {
        AcGePoint3d tmpPt = pts[0];        // used to be a bug in dynamic arrays (not sure if its still there??)
        pts.append(tmpPt);

        bulges.append(0.0);

        int tmpType = types[0];
        types.append(tmpType);

        double tmpWidth = startWidths[0];
        startWidths.append(tmpWidth);

        tmpWidth = endWidths[0];
        endWidths.append(tmpWidth);
    }
}

/****************************************************************************
**
**	MyBaseUtils::collectVertices
**
**	**jma
**
*************************************/

void
MyBaseUtils::collectVertices(const AcDb3dPolyline* pline, AcGePoint3dArray& pts)
{
    ASSERT(pline != NULL);
    ASSERT(pts.isEmpty());

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return;

    AcDb3dPolylineVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k3dControlVertex)
                pts.append(vertex->position());
            vertex->close();
        }
    }
    delete vertexIter;

    ASSERT(pts.isEmpty() == false);

    if (pline->isClosed()) {
        AcGePoint3d tmpPt = pts[0];        // used to be a bug in dynamic arrays (not sure if its still there??)
        pts.append(tmpPt);
    }
}

/****************************************************************************
**
**	MyBaseUtils::collectVertices
**
**	**jma
**
*************************************/

void
MyBaseUtils::collectVertices(const AcDbPolyFaceMesh* pface, AcDbObjectIdArray& vfaces, AcGePoint3dArray& pts)
{
    AcDbObjectIterator*  vertexIter = pface->vertexIterator();
    if (vertexIter == NULL)
        return;

    AcDbFaceRecord* vface;
    AcDbPolyFaceMeshVertex* pfaceVertex;
    AcDbObject* obj;

        // walk through and seperate vfaces and vertices into two
        // seperate arrays
    Acad::ErrorStatus es;
    for (; !vertexIter->done(); vertexIter->step()) {
        es = acdbOpenObject(obj, vertexIter->objectId(), AcDb::kForRead);
        if (es == Acad::eOk) {
            if ((vface = AcDbFaceRecord::cast(obj)) != NULL)
                vfaces.append(obj->objectId());
            else if ((pfaceVertex = AcDbPolyFaceMeshVertex::cast(obj)) != NULL)
                pts.append(pfaceVertex->position());
            else {
                ASSERT(0);
            }
            obj->close();
        }
        else
            MyBaseUtils::rxErrorMsg(es);
    }
    delete vertexIter;
}

/****************************************************************************
**
**	MyBaseUtils::collectVertices
**		grab the vertices for a polygon mesh and insert close vertices if
**	appropriate.  The new mSize and nSize counts will be returned in mSize/nSize
**
**	**jma
**
*************************************/

bool
MyBaseUtils::collectVertices(const AcDbPolygonMesh* pmesh, AcGePoint3dArray& pts,
                            int& mSize, int& nSize)
{
    AcDbObjectIterator*  vertexIter = pmesh->vertexIterator();
    if (vertexIter == NULL)
        return false;

    Acad::ErrorStatus es;
    AcDbPolygonMeshVertex* pVertex;
    AcGePoint3d tmpPt;

    if (pmesh->polyMeshType() == AcDb::kSimpleMesh) {
        mSize = pmesh->mSize();
        nSize = pmesh->nSize();
    }
    else {
        mSize = pmesh->mSurfaceDensity();
        nSize = pmesh->nSurfaceDensity();
    }

    int nCount = 0;
    int totalCount = 0;
    for (; !vertexIter->done(); vertexIter->step()) {
        es = acdbOpenObject(pVertex, vertexIter->objectId(), AcDb::kForRead);
        if (es != Acad::eOk) {
            MyBaseUtils::rxErrorMsg(es);
            delete vertexIter;
            return false;
        }
        pts.append(pVertex->position());
        nCount++;
        totalCount++;
        if (nCount == nSize) {
            if (pmesh->isNClosed()) {
                tmpPt = pts[totalCount - nSize];        // work around bug by making temp copy (not sure if still a bug??)
                pts.append(tmpPt);
                totalCount++;
            }
            nCount = 0;
        }
    }
    delete vertexIter;

    if (pmesh->isNClosed())
        nSize++;
    if (pmesh->isMClosed()) {
        mSize++;
        for (int i=0; i<nSize; i++) {
            tmpPt = pts[i];        // see note above
            pts.append(tmpPt);    
        }
    }

    return true;
}

/****************************************************************************
**
**	MyBaseUtils::isPaperSpace
**
**	**jma
**
*************************************/

bool
MyBaseUtils::isPaperSpace(AcDbDatabase* db)
{
	if (db->tilemode() == 1)
		return false;

	if (db->paperSpaceVportId() == acedGetCurViewportObjectId())
		return true;
	else
		return false;
}

/****************************************************************************
**
**	MyBaseUtils::transformToWcs
**
**	**jma
**
*************************************/

void
MyBaseUtils::transformToWcs(AcDbEntity* ent, AcDbDatabase* db)
{
    ASSERT(ent->isWriteEnabled());
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getUcsToWcsMatrix(m, db);
    Acad::ErrorStatus es = ent->transformBy(m);

    ASSERT(es == Acad::eOk);
}

/****************************************************************************
**
**	MyBaseUtils::transformToUcs
**
**	**jma
**
*************************************/

void
MyBaseUtils::transformToUcs(AcDbEntity* ent, AcDbDatabase* db)
{
    ASSERT(ent->isWriteEnabled());
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getWcsToUcsMatrix(m, db);
    Acad::ErrorStatus es = ent->transformBy(m);

    ASSERT(es == Acad::eOk);
}

/****************************************************************************
**
**	MyBaseUtils::getUcsXAxis
**		returns current UCS X Axis, even if currently the paperSpace viewport
**
**	**jma
**
*************************************/

AcGeVector3d
MyBaseUtils::getUcsXAxis(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		return xDir;
	}
	else {
		ASSERT(0);
		return AcGeVector3d::kXAxis;
	}
}

/****************************************************************************
**
**	MyBaseUtils::getUcsYAxis
**		returns current UCS Y Axis, even if currently the paperSpace viewport
**
**	**jma
**
*************************************/

AcGeVector3d
MyBaseUtils::getUcsYAxis(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		return yDir;
	}
	else {
		ASSERT(0);
		return AcGeVector3d::kYAxis;
	}
}

/****************************************************************************
**
**	MyBaseUtils::getUcsZAxis
**
**	**jma
**
*************************************/

AcGeVector3d
MyBaseUtils::getUcsZAxis(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		return zDir;
	}
	else {
		ASSERT(0);
		return AcGeVector3d::kZAxis;
	}
}

/****************************************************************************
**
**	MyBaseUtils::getUcsPlane
**
**	**jma
**
*************************************/

AcGePlane
MyBaseUtils::getUcsPlane(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		AcGePlane ucsPlane(origin, xDir, yDir);
		return ucsPlane;
	}
	else {
		ASSERT(0);
        AcGePlane ucsPlane(AcGePoint3d::kOrigin, AcGeVector3d::kIdentity);
        return ucsPlane;
	}
}

/****************************************************************************
**
**    MyBaseUtils::getUcsToWcsMatrix
**
**    **jma
**
*************************************/

void
MyBaseUtils::getUcsToWcsMatrix(AcGeMatrix3d& m, AcDbDatabase* db)
{
	ASSERT(db != NULL);

	if (!acdbUcsMatrix(m, db)) {
		m.setToIdentity();
		ASSERT(0);
	}
}

/****************************************************************************
**
**	MyBaseUtils::getWcsToUcsMatrix
**
**	**jma
**
*************************************/

void
MyBaseUtils::getWcsToUcsMatrix(AcGeMatrix3d& m, AcDbDatabase* db)
{
    getUcsToWcsMatrix(m, db);
    m.invert();
}

/****************************************************************************
**
**	MyBaseUtils::getUcsToWcsOriginMatrix
**		move an object form one coordinate system to another.
**
**	NOTE:    The basePt must be passed in as a WCS point!
**
**	**jma
**
*************************************/

void
MyBaseUtils::getUcsToWcsOriginMatrix(AcGeMatrix3d& m,
                        const AcGePoint3d& wcsBasePt, AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d tmpMat;

	if (acdbUcsMatrix(tmpMat, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		tmpMat.getCoordSystem(origin, xDir, yDir, zDir);
		origin += wcsBasePt.asVector();

	    m.setToAlignCoordSys(origin, xDir, yDir, zDir,
            AcGePoint3d::kOrigin, AcGeVector3d::kXAxis,
            AcGeVector3d::kYAxis, AcGeVector3d::kZAxis);
	}
	else {
		ASSERT(0);
        m = AcGeMatrix3d::kIdentity;
	}
}

/****************************************************************************
**
**	MyBaseUtils::getEcsPlane
**		This is the ECS plane for an AutoCAD entity. It can then be used to
**	calculate angles relative to the ECS plane, for instance, angles of 
**	AcDbArc, AcDbBlockReference
**
**	**jma
**
*************************************/

void
MyBaseUtils::getEcsPlane(const AcGeVector3d& entNormal, AcGePlane& ecsPlane)
{
    AcGeMatrix3d ecsMat;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, entNormal, ecsMat);

    AcGePoint3d origin;
    AcGeVector3d xAxis, yAxis, zAxis;
    ecsMat.getCoordSystem(origin, xAxis, yAxis, zAxis);
    ecsPlane.set(origin, xAxis, yAxis);
}

/****************************************************************************
**
**	MyBaseUtils::getEcsPlane
**	same as above, but also return the XAxis of ECS, and use the origin
**	passed in for the plane
**
**	**jma
**
*************************************/

void
MyBaseUtils::getEcsPlane(const AcGePoint3d& origin,
                        const AcGeVector3d& entNormal, AcGePlane& ecsPlane,
                        AcGeVector3d& ecsXAxis)
{
    AcGeMatrix3d ecsMat;
    getEcsToWcsMatrix(origin, entNormal, ecsMat);

    AcGePoint3d tmpOrigin;
    AcGeVector3d yAxis, zAxis;
    ecsMat.getCoordSystem(tmpOrigin, ecsXAxis, yAxis, zAxis);
    ecsPlane.set(tmpOrigin, ecsXAxis, yAxis);
}

/****************************************************************************
**
**	MyBaseUtils::getEcsXAxis
**
**	**jma
**
*************************************/

void
MyBaseUtils::getEcsXAxis(const AcGeVector3d& ecsZAxis, AcGeVector3d& xAxis)
{
    AcGeMatrix3d arbMat;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, ecsZAxis, arbMat);

    AcGePoint3d origin;
    AcGeVector3d yAxis, zAxis;

    arbMat.getCoordSystem(origin, xAxis, yAxis, zAxis);    // return xAxis;
}

/****************************************************************************
**
**	MyBaseUtils::getEcsToWcsMatrix
**		run AutoCAD's arbitrary matrix algorithm for ECS entities
**
**	**jma
**
*************************************/

void
MyBaseUtils::getEcsToWcsMatrix(const AcGePoint3d& origin,
                    const AcGeVector3d& zAxis, AcGeMatrix3d& mat)
{
    const double kArbBound = 0.015625;         //  1/64th

        // short circuit if in WCS already
    if (zAxis == AcGeVector3d::kZAxis) {
        mat.setToIdentity();
        return;
    }

    AcGeVector3d xAxis, yAxis;

    ASSERT(zAxis.isUnitLength());

    if ((fabs(zAxis.x) < kArbBound) && (fabs(zAxis.y) < kArbBound))
        xAxis = AcGeVector3d::kYAxis.crossProduct(zAxis);
    else
        xAxis = AcGeVector3d::kZAxis.crossProduct(zAxis);

    xAxis.normalize();
    yAxis = zAxis.crossProduct(xAxis);
    yAxis.normalize();

    mat.setCoordSystem(AcGePoint3d::kOrigin, xAxis, yAxis, zAxis);
}

/****************************************************************************
**
**	MyBaseUtils::wcsToUcs
**
**	**jma
**
*************************************/

AcGePoint3d
MyBaseUtils::wcsToUcs(const AcGePoint3d& pt)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getWcsToUcsMatrix(m, db);
    return m * pt;
}

/****************************************************************************
**
**	MyBaseUtils::wcsToUcs
**
**	**jma
**
*************************************/

AcGeVector3d
MyBaseUtils::wcsToUcs(const AcGeVector3d& vec)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getWcsToUcsMatrix(m, db);

    AcGeVector3d newv = vec;
    newv.transformBy(m);

    return newv;
}

/****************************************************************************
**
**	MyBaseUtils::wcsToUcs
**
**	**jma
**
*************************************/

void
MyBaseUtils::wcsToUcs(AcGePoint3dArray& ptArray)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getWcsToUcsMatrix(m, db);

    int len = ptArray.length();
    for (int i=0; i<len; i++)
        ptArray[i] = m * ptArray[i];
}

/****************************************************************************
**
**	MyBaseUtils::ucsToWcs
**
**	**jma
**
*************************************/

AcGePoint3d
MyBaseUtils::ucsToWcs(const AcGePoint3d& pt)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getUcsToWcsMatrix(m, db);
    return m * pt;
}

/****************************************************************************
**
**	MyBaseUtils::ucsToWcs
**
**	**jma
**
*************************************/

AcGeVector3d
MyBaseUtils::ucsToWcs(const AcGeVector3d& vec)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getUcsToWcsMatrix(m, db);

    AcGeVector3d newv = vec;
    newv.transformBy(m);

    return newv;
}

/****************************************************************************
**
**	MyBaseUtils::ucsToWcs
**
**	**jma
**
*************************************/

void
MyBaseUtils::ucsToWcs(AcGePoint3dArray& ptArray)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getUcsToWcsMatrix(m, db);

    int len = ptArray.length();
    for (int i=0; i<len; i++)
        ptArray[i] = m * ptArray[i];
}

/************************************************************************
**
**	MyBaseUtils::ecsToWcs
**
**	**jma
**
***************************************/

AcGePoint3d
MyBaseUtils::ecsToWcs(const AcGePoint3d& pt, const AcGeVector3d& entNormal)
{
    AcGeMatrix3d m;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, entNormal, m);

    return m * pt;
}

/************************************************************************
**
**	MyBaseUtils::ecsToWcs
**
**	**jma
**
***************************************/

AcGeVector3d
MyBaseUtils::ecsToWcs(const AcGeVector3d& vec, const AcGeVector3d& entNormal)
{
    AcGeMatrix3d m;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, entNormal, m);

    return m * vec;
}

/************************************************************************
**
**	MyBaseUtils::dcsToUcs
**
**	**jma
**
***************************************/

AcGePoint3d
MyBaseUtils::dcsToUcs(const AcGePoint2d& pt)
{
    resbuf fromRb, toRb;
    ads_point newPt;

    fromRb.restype = RTSHORT;
    fromRb.resval.rint = AcDb::kCurDisplayCS;

    toRb.restype = RTSHORT;
    toRb.resval.rint = AcDb::kUserCS;

    short result = acedTrans(asDblArray(pt), &fromRb, &toRb, FALSE, newPt);
    ASSERT(result == RTNORM);

    return asPnt3d(newPt);
}

/************************************************************************
**
**	MyBaseUtils::ucsToDcs
**
**	**jma
**
***************************************/

AcGePoint2d
MyBaseUtils::ucsToDcs(const AcGePoint3d& pt)
{
    resbuf fromRb, toRb;
    ads_point newPt;

    fromRb.restype = RTSHORT;
    fromRb.resval.rint = AcDb::kUserCS;

    toRb.restype = RTSHORT;
    toRb.resval.rint = AcDb::kCurDisplayCS;

    short result = acedTrans(asDblArray(pt), &fromRb, &toRb, FALSE, newPt);
    ASSERT(result == RTNORM);

    return asPnt2d(newPt);
}

/************************************************************************
**
**	MyBaseUtils::getAllDatabases
**
**	**jma
**
***************************************/

void
MyBaseUtils::getAllDatabases(AcDbVoidPtrArray& dbPtrs)
{
	AcDbVoidPtrArray* dbs = acdbActiveDatabaseArray();
	if (dbs) {
		dbPtrs = *dbs;
	}
	else {
		ASSERT(0);
	}
}

/************************************************************************
**
**	MyBaseUtils::getAllDocuments
**
**	**jma
**
***************************************/

void
MyBaseUtils::getAllDocuments(AcDbVoidPtrArray& docPtrs)
{
    AcApDocumentIterator* iter = acDocManager->newAcApDocumentIterator();
    if (iter) {
        AcApDocument* tmpDoc;
        for (; !iter->done(); iter->step()) {
            tmpDoc = iter->document();
            if (tmpDoc) {
                docPtrs.append(tmpDoc);
            }
            else {
                ASSERT(0);
            }
        }
        delete iter;
	}
}

/****************************************************************************
**
**  MyBaseUtils::verifyClassType
**
**  **jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::verifyClassType(const AcDbObject* obj, AcRxClass* classType)
{
    ASSERT(classType != NULL);

    if (obj->isAProxy()) {
            // we can't really know if this is correct or not, so just leave it
        return Acad::eOk;
    }
    else {
        if (obj->isKindOf(classType))
            return Acad::eOk;
    }

    return Acad::eWrongObjectType;
}

/****************************************************************************
**
**  MyBaseUtils::verifyEntityReference
**
**  **jma
**
*************************************/

Acad::ErrorStatus
MyBaseUtils::verifyEntityReference(const AcDbEntity* ent, const AcDbObjectId& idToValidate,
                 AcRxClass* classType, bool allowNull, bool allowErased)
{
    if (ent == NULL) {
        ASSERT(0);
        return Acad::eInvalidInput;
    }

        // first check for NULL
    if (idToValidate == AcDbObjectId::kNull) {
        if (allowNull)
            return Acad::eOk;
        else
            return Acad::eNullObjectId;
    }

        // now open it up and see what it is
    Acad::ErrorStatus es;
    AcDbEntity* entToValidate;
    es = acdbOpenAcDbEntity(entToValidate, idToValidate, AcDb::kForRead);
    if (es == Acad::eOk) {
            // make sure all references are from the same database
            // which should always be the case!
        if (ent->database() != entToValidate->database()) {
            entToValidate->close();
            return Acad::eWrongDatabase;
        }
            // make sure that references are from the same block
            // This can fail from a simple deepClone() to make a
            // blockTableRecord.  If the user didn't pick all the
            // referenced objects in the anchor at the time of
            // making the block, they will still point to the
            // entities in modelSpace... which will eventually crash
            // the whole thing.
        if (ent->blockId() != entToValidate->blockId()) {
            entToValidate->close();
            return Acad::eNotInBlock;
        }

            // see if they want to validate based on object type
        if (classType != NULL) {
            es = verifyClassType(entToValidate, classType);
            if (es != Acad::eOk) {
                entToValidate->close();
                return es;
            }
        }

        entToValidate->close();
        return Acad::eOk;               // passed our test    
    }
    else if (es == Acad::eWasErased) {
        if (allowErased)
            return Acad::eOk;           // passed under relaxed constraints
        else
            return Acad::eWasErased;    // failed
    }
    else {
        return es;                      // failed
    }
}


