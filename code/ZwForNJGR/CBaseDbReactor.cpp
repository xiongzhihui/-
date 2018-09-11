//-----------------------------------------------------------------------------
//----- CBaseDbReactor.cpp : Implementation of CBaseDbReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CBaseDbReactor.h"
#include "Utility.h"
#include "BaseUtils.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(CBaseDbReactor, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
CBaseDbReactor::CBaseDbReactor (AcDbDatabase *pDb) : AcDbDatabaseReactor(), mpDatabase(pDb) {
	if ( pDb )
		pDb->addReactor (this) ;
}

//-----------------------------------------------------------------------------
CBaseDbReactor::~CBaseDbReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CBaseDbReactor::Attach (AcDbDatabase *pDb) {
	Detach () ;
	if ( mpDatabase == NULL ) {
		if ( (mpDatabase =pDb) != NULL )
			pDb->addReactor (this) ;
	}
}

void CBaseDbReactor::Detach () {
	if ( mpDatabase ) {
		mpDatabase->removeReactor (this) ;
		mpDatabase =NULL ;
	}
}

AcDbDatabase *CBaseDbReactor::Subject () const {
	return (mpDatabase) ;
}

bool CBaseDbReactor::IsAttached () const {
	return (mpDatabase != NULL) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::proxyResurrectionCompleted(const AcDbDatabase * dwg, const ACHAR * appname, AcDbObjectIdArray & objects)
{
	AcDbDatabaseReactor::proxyResurrectionCompleted (dwg, appname, objects) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::goodbye(const AcDbDatabase * dwg)
{
	AcDbDatabaseReactor::goodbye (dwg) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::headerSysVarWillChange(const AcDbDatabase * dwg, const ACHAR * name)
{
	AcDbDatabaseReactor::headerSysVarWillChange (dwg, name) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::headerSysVarChanged(const AcDbDatabase * dwg, const ACHAR * name, Adesk::Boolean bSuccess)
{
	AcDbDatabaseReactor::headerSysVarChanged (dwg, name, bSuccess) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectUnAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectUnAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectReAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectReAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectOpenedForModify(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectOpenedForModify (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectModified(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectErased(const AcDbDatabase * dwg, const AcDbObject * dbObj, Adesk::Boolean pErased)
{
	if (pErased)
	{
		if (dbObj->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbExtents exts;
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(dbObj);
			AcDbObjectId blockId = pBlkRef->blockTableRecord();
			AcDbSymbolTableRecord* tblRec;
			CString strName;
			Acad::ErrorStatus es;
			es = acdbOpenObject(tblRec, blockId, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				const TCHAR* locName;
				tblRec->getName(locName);
				strName = locName;
				tblRec->close();
				pBlkRef->geomExtentsBestFit(exts);
			}
			if ((strName.CompareNoCase(_T("A0")) == 0)
				||(strName.CompareNoCase(_T("A1")) == 0)
				||(strName.CompareNoCase(_T("A2")) == 0)
				||(strName.CompareNoCase(_T("A3")) == 0)
				||(strName.CompareNoCase(_T("A4")) == 0))
			{
				//CDMXUtils::SetcreateBc(false);
				//删除本图框中的所有元素
				int nRow, nCol, nCount;
				CUtility::getRowAndColByPt(exts.maxPoint(), nRow, nCol);
				nCount = (nRow-1)*CBaseUtils::hxFrameCount() + nCol;
				acutPrintf(_T("\n第%d个图框被删除"), nCount);

				int nFirstCount = CBaseUtils::firstCount();
				nFirstCount++;
				CBaseUtils::setFirstCount(nFirstCount);
				CUtility::UpdatePageNo(nCount);
				ads_name ss;
				int nRet = acedSSGet(_T("C"), asDblArray(exts.minPoint()), asDblArray(exts.maxPoint()), NULL, ss);
				if (nRet != RTNORM)
				{
					return AcDbDatabaseReactor::objectErased (dwg, dbObj, pErased);
				}
				MyEditEntity::EraseSSName(ss);
				acedSSFree(ss);				
			}
		}
	}
	AcDbDatabaseReactor::objectErased (dwg, dbObj, pErased) ;
}

// -----------------------------------------------------------------------------
inline AcDbDatabaseReactor * CBaseDbReactor::cast(void)
{
	return NULL;
}
