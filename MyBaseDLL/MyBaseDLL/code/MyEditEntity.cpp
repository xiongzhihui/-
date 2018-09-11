#include "StdAfx.h"
#include "MyEditEntity.h"
#include "MyBaseXdata.h"
#include "ArxDbgReferenceFiler.h"
#include "MyTransFunc.h"
MyEditEntity::MyEditEntity(void)
{
}

MyEditEntity::~MyEditEntity(void)
{
}

AcDbObjectId MyEditEntity::openEntChangeLayer(AcDbObjectId entId, AcDbObjectId LayerId)
{
	if (LayerId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	if (entId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}
	pEnt->setLayer(LayerId);
	pEnt->close();
	entId = pEnt->objectId();
	return entId;
}

AcDbObjectId MyEditEntity::openEntChangeTextStyle(AcDbObjectId entId, AcDbObjectId TextStyleId /* = AcDbObjectId::kNull */)
{
	if (TextStyleId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	if (entId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}

	if (pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText = AcDbText::cast(pEnt);
		pText->setTextStyle(TextStyleId);
		pText->close();
	}
	else if (pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMText = AcDbMText::cast(pEnt);
		pMText->setTextStyle(TextStyleId);
		pMText->close();
	}
	else
	{
		pEnt->close();
	}
	entId = pEnt->objectId();
	return entId;
}

AcDbObjectId MyEditEntity::openTextChangeTextHeight(AcDbObjectId textId, double dTextHeight)
{
	if (textId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, textId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}

	if (pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText = AcDbText::cast(pEnt);
		pText->setHeight(dTextHeight);
		pText->close();
	}
	else if (pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMText = AcDbMText::cast(pEnt);
		pMText->setTextHeight(dTextHeight);
		pMText->close();
	}
	else
	{
		pEnt->close();
	}
	textId = pEnt->objectId();
	return textId;
}

AcDbObjectId MyEditEntity::openPlineChangeWidth(AcDbObjectId plineId, double dWidth)
{
	if (plineId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	if (dWidth<0)
	{
		return AcDbObjectId::kNull;
	}
	AcDbPolyline* pLine = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pLine, plineId, AcDb::kForWrite)!=Acad::eOk)
	{
		pLine->close();
		return AcDbObjectId::kNull;
	}
	pLine->setConstantWidth(dWidth);
	pLine->close();
	plineId = pLine->objectId();
	return plineId;
}

AcDbObjectId MyEditEntity::openEntChangeRotation(AcDbObjectId entId, double dRotate)
{
	if (entId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}
	if (pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText = AcDbText::cast(pEnt);
		pText->setRotation(dRotate);
		pText->close();
		entId = pText->objectId();
	}
	else if (pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMtext = AcDbMText::cast(pEnt);
		pMtext->setRotation(dRotate);
		pMtext->close();
		entId = pMtext->objectId();
	}
	else if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
		pBlkRef->setRotation(dRotate);
		pBlkRef->close();
		entId = pBlkRef->objectId();
	}
	else
	{
		pEnt->close();
	}
	return entId;
}

AcDbObjectId MyEditEntity::openBlkRefChangeScale(AcDbObjectId entId, double dRotate)
{
	if (entId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
		pBlkRef->setScaleFactors(AcGeScale3d(dRotate, dRotate, dRotate));
		pBlkRef->close();
		entId = pBlkRef->objectId();
	}
	else
	{
		pEnt->close();
	}
	return entId;
}

AcDbObjectId MyEditEntity::openEntChangeLineWeight(AcDbObjectId entId, AcDb::LineWeight type)
{
	if (entId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}
	pEnt->setLineWeight(type);
	pEnt->close();
	return pEnt->objectId();
}

AcDbObjectId MyEditEntity::openEntChangeColor(AcDbObjectId entId, int nColor)
{
	if (entId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	Acad::ErrorStatus es ;
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}
	AcCmColor cmColor;
	es = cmColor.setColorIndex(nColor);
	es = pEnt->setColor(cmColor);
	pEnt->close();
	return pEnt->objectId();
}


bool MyEditEntity::OpenObjAppendStrToXdata(AcDbObjectId objId, CString strAppName, CString strData)
{
	if (objId.isNull())
	{
		return false;
	}

	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}
	MyBaseAppXdata connor(strAppName, pObj->database());
	connor.setString(XDT_XG_CSTRING, strData);
	connor.setXdata(pObj);
	 
	pObj->close();
	return true;
}

bool MyEditEntity::OpenObjAppendIntToXdata(AcDbObjectId objId, CString strAppName, int nData)
{
	if (objId.isNull())
	{
		return false;
	}

	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}
	MyBaseAppXdata connor(strAppName, pObj->database());
	connor.setInteger(XDT_XG_INT, nData);
	connor.setXdata(pObj);

	pObj->close();
	return true;
}

bool MyEditEntity::OpenObjAppendDoubleToXdata(AcDbObjectId objId, CString strAppName, double dData)
{
	if (objId.isNull())
	{
		return false;
	}

	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}
	MyBaseAppXdata connor(strAppName, pObj->database());
	connor.setReal(XDT_XG_DOUBLE, dData);
	connor.setXdata(pObj);
	
	pObj->close();
	return true;
}

bool MyEditEntity::OpenObjAppendHandleToXdata(AcDbObjectId objId, CString strAppName, AcDbHandle hData)
{
	if (objId.isNull())
	{
		return false;
	}

	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}
	MyBaseAppXdata connor(strAppName, pObj->database());
	connor.setHandle(XDT_XG_HANDLE, hData);
	connor.setXdata(pObj);

	pObj->close();
	return true;
}

CString MyEditEntity::GetObjStrXdata(AcDbObjectId objId, CString strAppName)
{
	CString strXdata(_T(""));
	if (objId.isNull())
	{
		return strXdata;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return strXdata;
	}

	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	if (xdata.isEmpty())
	{
		pObj->close();
		return strXdata;
	}
	
	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(strAppName, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	int nSize = xdata.entries();
	if (nIndex > nSize-1)
	{
		pObj->close();
		return strXdata;
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	if (xdPtr != NULL)
	{
		bool bIsOk = xdPtr->getString(XDT_XG_CSTRING, strXdata);
		if (!bIsOk)
		{
			strXdata = _T("");
		}
	}
	pObj->close();
	return strXdata;
}

double MyEditEntity::GetObjDoubleXdata(AcDbObjectId objId, CString strAppName)
{
	double dXdata = 0.0;
	if (objId.isNull())
	{
		return dXdata;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return dXdata;
	}

	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	if (xdata.isEmpty())
	{
		pObj->close();
		return dXdata;
	}
	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(strAppName, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	int nSize = xdata.entries();
	if (nIndex > nSize-1)
	{
		pObj->close();
		return dXdata;
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	bool bIsOk = xdPtr->getReal(XDT_XG_DOUBLE, dXdata);
	if (!bIsOk)
	{
		dXdata = 0.0;
	}
	pObj->close();
	return dXdata;
}

int MyEditEntity::GetObjIntXdata(AcDbObjectId objId, CString strAppName)
{
	int nXdata = 0.0;
	if (objId.isNull())
	{
		return nXdata;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return nXdata;
	}

	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	if (xdata.isEmpty())
	{
		pObj->close();
		return nXdata;
	}
	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(strAppName, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	int nSize = xdata.entries();
	if (nIndex > nSize-1)
	{
		pObj->close();
		return nXdata;
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	bool bIsOk = xdPtr->getInteger(XDT_XG_INT, nXdata);
	if (!bIsOk)
	{
		nXdata = 0;
	}
	pObj->close();
	return nXdata;
}

AcDbHandle MyEditEntity::GetObjHandleXdata(AcDbObjectId objId, CString strAppName)
{
	AcDbHandle hXdata = 0.0;
	if (objId.isNull())
	{
		return hXdata;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return hXdata;
	}

	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	if (xdata.isEmpty())
	{
		pObj->close();
		return hXdata;
	}
	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(strAppName, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	int nSize = xdata.entries();
	if (nIndex > nSize-1)
	{
		pObj->close();
		return hXdata;
	}
	bool bIsOk = xdPtr->getHandle(XDT_XG_HANDLE, hXdata);
	/*if (!bIsOk)
	{
		hXdata = 0;
	}*/
	pObj->close();
	return hXdata;
}

bool MyEditEntity::SetObjStrToXdata(AcDbObjectId objId, CString strAppName, CString strXdata)
{
	if (objId.isNull())
	{
		return false;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}

	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(strAppName, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	xdPtr->setString(XDT_XG_CSTRING, strXdata);
	xdPtr->setXdata(pObj);
	pObj->close();
	return true;
}

bool MyEditEntity::SetObjDoubleToXdata(AcDbObjectId objId, CString strAppName, double dXdata)
{
	if (objId.isNull())
	{
		return false;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}

	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(strAppName, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	xdPtr->setDistance(XDT_XG_DOUBLE, dXdata);
	xdPtr->setXdata(pObj);
	pObj->close();
	return true;
}

bool MyEditEntity::SetObjIntToXdata(AcDbObjectId objId, CString strAppName, int nXdata)
{
	if (objId.isNull())
	{
		return false;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}

	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(strAppName, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	xdPtr->setInteger(XDT_XG_INT, nXdata);
	xdPtr->setXdata(pObj);
	pObj->close();
	return true;
}


AcDbObjectIdArray MyEditEntity::openObjAndGetGroupIds(AcDbObjectId objId)
{
	AcDbObjectIdArray GroupIdArrs;
	GroupIdArrs.removeAll();
	if (objId.isNull())
	{
		return GroupIdArrs;
	}

	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		return GroupIdArrs;
	}
	int nReactorCount = 0;
	AcDbVoidPtrArray* reactors = pObj->reactors();
	if (reactors != NULL)
		nReactorCount = reactors->length();

	if (nReactorCount<=0)
	{
		pObj->close();
		return GroupIdArrs;
	}
	ArxDbgReferenceFiler filer;
	pObj->dwgOut(&filer);
	pObj->close();
	for (int i=0; i<filer.m_softPointerIds.length(); i++)
	{
		objId = filer.m_softPointerIds.at(i);
		if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
		{
			continue;
		}
		if (pObj->isKindOf(AcDbGroup::desc()))
		{
			GroupIdArrs.append(objId);
			pObj->close();
		}
		else
		{
			pObj->close();
		}
	}

	return GroupIdArrs;
}

AcDbObjectIdArray MyEditEntity::openObjAndGetGroupIds(AcDbObject* pObj)
{
	AcDbObjectIdArray GroupIdArrs;
	GroupIdArrs.removeAll();
	
	int nReactorCount = 0;
	AcDbVoidPtrArray* reactors = pObj->reactors();
	if (reactors != NULL)
		nReactorCount = reactors->length();

	if (nReactorCount<=0)
	{
		pObj->close();
		return GroupIdArrs;
	}
	ArxDbgReferenceFiler filer;
	pObj->dwgOut(&filer);
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObject* obJ = NULL;
	for (int i=0; i<filer.m_softPointerIds.length(); i++)
	{
		objId = filer.m_softPointerIds.at(i);
		if (acdbOpenAcDbObject(obJ, objId, AcDb::kForWrite)!=Acad::eOk)
		{
			continue;
		}
		if (obJ->isKindOf(AcDbGroup::desc()))
		{
			GroupIdArrs.append(objId);
			obJ->close();
		}
		else
		{
			obJ->close();
		}
	}

	return GroupIdArrs;
}

AcDbObjectIdArray MyEditEntity::openGroupIdsAndGetEntIds(AcDbObjectIdArray IdArrs, BOOL bIslimited)
{
	AcDbObjectIdArray objIds;
	objIds.removeAll();
	if (IdArrs.length()<=0)
	{
		return objIds;
	}
	AcDbEntity* pEnt = NULL;
	AcDbGroup* pGroup = NULL;
	AcDbObjectId objId;
	AcDbObjectIdArray tmpIdArrs;
	tmpIdArrs.removeAll();
	for (int i=0; i<IdArrs.length(); i++)
	{	
		objId = IdArrs.at(i);
		if (acdbOpenAcDbObject((AcDbObject*&)pGroup, objId, AcDb::kForRead)!=Acad::eOk)
		{
			continue;
		}
		int nLength = 0;
		nLength = pGroup->allEntityIds(tmpIdArrs);
		if (bIslimited && (nLength<=0))
		{
			pGroup->close();
			continue;
		}
		else
		{
			for (int j=0; j<tmpIdArrs.length(); j++)
			{
				objIds.append(tmpIdArrs.at(j));
			}
		}
		pGroup->close();
	}
	return objIds;
}


AcDbSortentsTable* MyEditEntity::get_sortents_table_of(AcDbObjectId owner_id)
{
	if (AcDbObjectId::kNull == owner_id)
		return NULL;
	AcDbBlockTableRecord *pRec = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenObject( pRec, owner_id, AcDb::kForRead );
	if (Acad::eOk != es)
		return NULL;
	AcDbObjectId ext_id = pRec->extensionDictionary();
	if (AcDbObjectId::kNull == ext_id)
	{
		if (Acad::eOk != pRec->upgradeOpen())
		{
			pRec->close();
			return NULL;
		}
		ext_id = pRec->extensionDictionary();
		if (AcDbObjectId::kNull == ext_id)
		{
			pRec->close();
			return NULL;
		}
	}
	pRec->close();
	AcDbDictionary *pExt;
	es = acdbOpenObject( pExt, ext_id, AcDb::kForRead );
	if (Acad::eOk != es)return NULL;
	AcDbObject *pObj;
	if (Acad::eOk != pExt->getAt( _T("ACAD_SORTENTS"), pObj, AcDb::kForWrite ))
	{
		if (Acad::eOk != pExt->upgradeOpen())
		{
			pExt->close();
			return NULL;
		}
		AcDbSortentsTable *pSt = new AcDbSortentsTable;
		if (NULL == pSt)
		{
			pExt->close();
			return NULL;
		}


		AcDbObjectId new_id;
		if (Acad::eOk != pExt->setAt( _T("ACAD_SORTENTS"), pSt, new_id ))
		{
			delete pSt;
			pExt->close();
			return NULL;
		}
		pSt->setBlockId( owner_id );
		pObj = pSt;
	}
	pExt->close();


	if (!pObj->isKindOf( AcDbSortentsTable::desc() ))
	{
		pObj->close();
		return NULL;
	}
	return (AcDbSortentsTable*)pObj;
}

//void DuiXiangPaiXu(AcDbObjectId ent_id,bool TopOrDow/*=TRUE*/)//对象排序
//{	
//		AcDbSortentsTable *pSt = get_sortents_table_of( ent_id );
//		if (NULL == pSt)return;
//		AcDbObjectIdArray entity_array;
//		entity_array.append( ent_id );
//		if (TopOrDow==TRUE)pSt->moveToTop(entity_array);//移到最前面
//		if (TopOrDow==FALSE)pSt->moveToBottom(entity_array);//移到底部
//		pSt->close();
//}


//calculate函数
//判断点是否在区域内
//传入点pt,区域exts
BOOL MyEditEntity::JudgePointInArea(AcGePoint2d insertPt, AcGePoint2d minPt, AcGePoint2d maxPt)
{
	bool bo1,bo2,bo3,bo4;
	bo1 = ((insertPt.x - minPt.x)>=0);
	bo2 = ((insertPt.x - maxPt.x)<=0);
	bo3 = ((insertPt.y-minPt.y)>=0);
	bo4 = ((insertPt.y-maxPt.y)<=0);
	if (((insertPt.x - minPt.x)>=0)&&
		((insertPt.x - maxPt.x)<=0)&&
		((insertPt.y-minPt.y)>=0)&&
		((insertPt.y-maxPt.y)<=0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//判断图框是否在区域内
//返回1表示curExts在preExts内
//返回2表示preExts在curExts内
//返回3表示curExts与preExts没有关系
int MyEditEntity::JudgeAreaInOtherArea(AcDbExtents curExts, AcDbExtents preExts)
{
	AcGePoint3d curMinPt,curMaxPt,preMinPt,preMaxPt;
	curMaxPt = curExts.maxPoint();
	curMinPt = curExts.minPoint();

	preMinPt = preExts.minPoint();
	preMaxPt = preExts.maxPoint();
	BOOL bIsInArea1 = FALSE;
	BOOL bIsInArea2 = FALSE;
	bIsInArea1 = JudgePointInArea(MyTransFunc::My3d22d(curMinPt), MyTransFunc::My3d22d(preMinPt), MyTransFunc::My3d22d(preMaxPt));
	bIsInArea2 = JudgePointInArea(MyTransFunc::My3d22d(curMaxPt), MyTransFunc::My3d22d(preMinPt), MyTransFunc::My3d22d(preMaxPt));
	if (bIsInArea1&&bIsInArea2)
	{
		return 1;
	}

	bIsInArea1 = JudgePointInArea(MyTransFunc::My3d22d(preMinPt), MyTransFunc::My3d22d(curMinPt), MyTransFunc::My3d22d(curMaxPt));
	bIsInArea2 = JudgePointInArea(MyTransFunc::My3d22d(preMaxPt), MyTransFunc::My3d22d(curMinPt), MyTransFunc::My3d22d(curMaxPt));
	if (bIsInArea1&&bIsInArea2)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}


int MyEditEntity::JudgeAreaInOtherArea(AcGePoint2d curMinPt,AcGePoint2d curMaxPt,AcGePoint2d preMinPt,AcGePoint2d preMaxPt)
{
	BOOL bIsInArea1 = FALSE;
	BOOL bIsInArea2 = FALSE;
	bIsInArea1 = JudgePointInArea(curMinPt, preMinPt, preMaxPt);
	bIsInArea2 = JudgePointInArea(curMaxPt, preMinPt, preMaxPt);
	if (bIsInArea1&&bIsInArea2)
	{
		return 1;
	}

	bIsInArea1 = JudgePointInArea(preMinPt, curMinPt, curMaxPt);
	bIsInArea2 = JudgePointInArea(preMaxPt, curMinPt, curMaxPt);
	if (bIsInArea1&&bIsInArea2)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

AcDbExtents MyEditEntity::OpenObjAndGetExts(AcDbObjectId objId)
{
	AcDbExtents exts;
	if (objId.isNull())
	{
		return  exts;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead)!=Acad::eOk)
	{
		pEnt->close();
		return exts;
	}
	pEnt->getGeomExtents(exts);
	pEnt->close();
	return exts;
}

double MyEditEntity::OpenObjAndGetWidth(AcDbObjectId objId)
{
	double dHeight = 0.0;
	if (objId.isNull())
	{
		return dHeight;
	}
	AcDbExtents exts;
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
	if (es!=Acad::eOk)
	{
		return dHeight;
	}
	pEnt->getGeomExtents(exts);
	pEnt->close();
	dHeight = abs(exts.maxPoint().y - exts.minPoint().y);
	double dTmp = abs(exts.minPoint().y - exts.maxPoint().y);
	if (dHeight < dTmp)
	{
		return dHeight;
	}
	else
	{
		return dTmp;
	}
}

double MyEditEntity::OpenObjAndGetLength(AcDbObjectId objId)
{
	double dWidth = 0.0;
	if (objId.isNull())
	{
		return dWidth;
	}
	AcDbExtents exts;
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead)!=Acad::eOk)
	{
		return dWidth;
	}
	pEnt->getGeomExtents(exts);
	pEnt->close();
	dWidth = abs(exts.minPoint().x - exts.maxPoint().x);
	double dTmp = abs(exts.minPoint().y - exts.maxPoint().y);
	if (dWidth > dTmp)
	{
		return dWidth;
	}
	else
	{
		return dTmp;
	}
}

CString MyEditEntity::OpenObjAndGetString(AcDbObjectId objId)
{
	CString strText(_T(""));
	if (objId.isNull())
	{
		return strText;
	}
	AcDbExtents exts;
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead)!=Acad::eOk)
	{
		return strText;
	}
	if (pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMtext = AcDbMText::cast(pEnt);
		strText = pMtext->contents();
		pMtext->close();
	}
	else if (pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText = AcDbText::cast(pEnt);
		strText = pText->textString();
		pText->close();
	}
	else
	{
		pEnt->close();
	}
	return strText;
}
 
AcDbObjectId MyEditEntity::OpenLayerIdAppendDescription(AcDbObjectId objId, CString strDescription)
{
	if (objId.isNull())
	{
		return objId;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		return objId;
	}
	if (pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		AcDbLayerTableRecord* pLayerTblRec = AcDbLayerTableRecord::cast(pObj);
		pLayerTblRec->setDescription(strDescription);
		pLayerTblRec->close();
	}
	else
	{
		pObj->close();
	}
	return pObj->objectId();
}

CString MyEditEntity::GetLayerIdDescription(AcDbObjectId objId)
{
	CString strDescription(_T(""));
	if (objId.isNull())
	{
		return strDescription;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		return strDescription;
	}
	if (pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		AcDbLayerTableRecord* pLayerTblRec = AcDbLayerTableRecord::cast(pObj);
		strDescription = pLayerTblRec->description();
		pLayerTblRec->close();
	}
	else
	{
		pObj->close();
	}
	return strDescription;
}

void MyEditEntity::ListLayerNameWithDescription(CStringArray& strLayerNameArr, BOOL bIsMask)
{
	strLayerNameArr.RemoveAll();
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForRead)	!= Acad::eOk) 
		return;

	AcDbLayerTableRecord* pRecord;
	AcDbLayerTableIterator* iter;
	pTable->newIterator(iter);
	CString strLayerName,strDescription;
	for (; !iter->done(); iter->step())
	{
		iter->getRecord(pRecord, AcDb::kForRead);
		strDescription = pRecord->description();
		if (bIsMask)
		{
			if ((strDescription.CompareNoCase(_T("mask")) != 0))
			{
				pRecord->close();
				continue;
			}
		}
		else	
		{
			if (/*(strDescription.CompareNoCase(_T("Passivation")) != 0)
				&&*/(strDescription.Compare(_T("PI")) != 0)
				&&(strDescription.CompareNoCase(_T("Metal")) != 0)
				/*&&(strDescription.CompareNoCase(_T("mask")) != 0)*/)
			{
				pRecord->close();
				continue;
			}
		}
		
		const TCHAR* locName; 
		pRecord->getName(locName);
		strLayerName = locName;
		strLayerNameArr.Add(strLayerName);	
		pRecord->close();
	}
	pTable->close();
	delete iter;
}


void MyEditEntity::ListLayerNameWithMetal(CStringArray& strLayerNameArr)
{
	strLayerNameArr.RemoveAll();
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForRead)	!= Acad::eOk) 
		return;

	AcDbLayerTableRecord* pRecord;
	AcDbLayerTableIterator* iter;
	pTable->newIterator(iter);
	CString strLayerName,strDescription;
	for (; !iter->done(); iter->step())
	{
		iter->getRecord(pRecord, AcDb::kForRead);
		strDescription = pRecord->description();
	
		if ((strDescription.CompareNoCase(_T("Metal")) != 0))
		{
			pRecord->close();
			continue;
		}
		
		const TCHAR* locName; 
		pRecord->getName(locName);
		strLayerName = locName;
		strLayerNameArr.Add(strLayerName);	
		pRecord->close();
	}
	pTable->close();
	delete iter;
}

AcDbObjectId MyEditEntity::OpenObjAppendStrToXRecord(AcDbObjectId objId, CString strAppName, CString strTypeData)
{
	AcDbXrecord *pXrec = new AcDbXrecord;
	AcDbObject *pObj;
	AcDbObjectId dictObjId, xrecObjId;
	AcDbDictionary* pDict;

	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite) != Acad::eOk)
	{
		return AcDbObjectId::kNull;
	}

	pObj->createExtensionDictionary();

	dictObjId = pObj->extensionDictionary();
	pObj->close();

	acdbOpenObject(pDict, dictObjId, AcDb::kForWrite);
	pDict->setAt(strAppName, pXrec, xrecObjId);
	pDict->close();

	struct resbuf* head;
	head = acutBuildList(AcDb::kDxfText, strTypeData,
		/*AcDb::kDxfXCoord, testpt,
		AcDb::kDxfReal, 3.14159,
		AcDb::kDxfAngle, 3.14159,
		AcDb::kDxfColor, 1,
		AcDb::kDxfInt16, 180,*/
		0);

	pXrec->setFromRbChain(*head);
	pXrec->close();
	acutRelRb(head);
	return objId;
}

AcDbObjectId MyEditEntity::OpenObjAppendIntToXRecord(AcDbObjectId objId, CString strAppName, int nTypeData)
{
	AcDbXrecord *pXrec = new AcDbXrecord;
	AcDbObject *pObj;
	AcDbObjectId dictObjId, xrecObjId;
	AcDbDictionary* pDict;

	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite) != Acad::eOk)
	{
		return AcDbObjectId::kNull;
	}

	pObj->createExtensionDictionary();

	dictObjId = pObj->extensionDictionary();
	pObj->close();

	acdbOpenObject(pDict, dictObjId, AcDb::kForWrite);
	pDict->setAt(strAppName, pXrec, xrecObjId);
	pDict->close();

	struct resbuf* head;
	head = acutBuildList(AcDb::kDxfInt16, nTypeData, 0);
	pXrec->setFromRbChain(*head);
	pXrec->close();
	acutRelRb(head);
	return objId;
}

AcDbObjectId MyEditEntity::OpenObjAppendDoubleToXRecord(AcDbObjectId objId, CString strAppName, double dTypeData)
{
	AcDbXrecord *pXrec = new AcDbXrecord;
	AcDbObject *pObj;
	AcDbObjectId dictObjId, xrecObjId;
	AcDbDictionary* pDict;

	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite) != Acad::eOk)
	{
		return AcDbObjectId::kNull;
	}

	pObj->createExtensionDictionary();

	dictObjId = pObj->extensionDictionary();
	pObj->close();

	acdbOpenObject(pDict, dictObjId, AcDb::kForWrite);
	pDict->setAt(strAppName, pXrec, xrecObjId);
	pDict->close();

	struct resbuf* head;
	head = acutBuildList(AcDb::kDxfReal, dTypeData, 0);
	pXrec->setFromRbChain(*head);
	pXrec->close();
	acutRelRb(head);
	return objId;
}

//AcDbObjectId MyEditEntity::OpenObjAppendHandleToXRecord(AcDbObjectId objId, CString strAppName, AcDbHandle hData)
//{
//	AcDbXrecord *pXrec = new AcDbXrecord;
//	AcDbObject *pObj;
//	AcDbObjectId dictObjId, xrecObjId;
//	AcDbDictionary* pDict;
//
//	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite) != Acad::eOk)
//	{
//		return AcDbObjectId::kNull;
//	}
//
//	pObj->createExtensionDictionary();
//
//	dictObjId = pObj->extensionDictionary();
//	pObj->close();
//
//	acdbOpenObject(pDict, dictObjId, AcDb::kForWrite);
//	pDict->setAt(strAppName, pXrec, xrecObjId);
//	pDict->close();
//
//	struct resbuf* head;
//	head = acutBuildList(AcDb::kDxfXdHandle, hData, 0);
//	pXrec->setFromRbChain(*head);
//	pXrec->close();
//	acutRelRb(head);
//}
//
//CString MyEditEntity::GetObjStrFromXRecord(AcDbObjectId objId, CString strAppName)
//{
//	AcDbObject *pObj;
//	AcDbXrecord *pXrec;
//	AcDbObjectId dictObjId;
//	AcDbDictionary *pDict;
//
//	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForRead) != Acad::eOk)
//	{
//		return AcDbObjectId::kNull;
//	}
//
//	// Get the object ID of the object's extension dictionary.
//	//
//	dictObjId = pObj->extensionDictionary();
//	pObj->close();
//
//	// Open the extension dictionary and get the xrecord
//	// associated with the key ASDK_XREC1.
//	// 
//	acdbOpenObject(pDict, dictObjId, AcDb::kForRead);
//	pDict->getAt(_T("ASDK_XREC1"), (AcDbObject*&)pXrec,
//		AcDb::kForRead);
//	pDict->close();
//
//	// Get the xrecord's data list and then close the xrecord.
//	//
//	struct resbuf *pRbList;
//	pXrec->rbChain(&pRbList);
//	pXrec->close();
//
//	int rt, i;
//	for (i = 0;pRbList != NULL;i++, pRbList = pRbList->rbnext) {
//		if (pRbList->restype < 0)
//			rt = pRbList->restype;
//		else if (pRbList->restype < 10)
//			rt = RTSTR;
//		else if (pRbList->restype < 38)
//			rt = RT3DPOINT;
//		else if (pRbList->restype < 60)
//			rt = RTREAL;
//		else if (pRbList->restype < 80)
//			rt = RTSHORT;
//		else if (pRbList->restype < 100)
//			rt = RTLONG;
//		else if (pRbList->restype < 106)
//			rt = RTSTR;
//		else if (pRbList->restype < 148)
//			rt = RTREAL;
//		else if (pRbList->restype < 290)
//			rt = RTSHORT;
//		else if (pRbList->restype < 330)
//			rt = RTSTR;
//		else if (pRbList->restype < 370)
//			rt = RTENAME;
//		else if (pRbList->restype < 999)
//			rt = RT3DPOINT;
//		else
//			rt = pRbList->restype;
//
//		switch (rt) {
//		case RTSHORT:
//			if (pRbList->restype == RTSHORT)
//				acutPrintf(_T("RTSHORT : %d\n"),
//				pRbList->resval.rint);
//			else
//				acutPrintf(_T("(%d . %d)\n"), pRbList->restype,
//				pRbList->resval.rint);
//			break;
//		case RTREAL:
//			if (pRbList->restype == RTREAL)
//				acutPrintf(_T("RTREAL : %0.3f\n"),
//				pRbList->resval.rreal);
//			else
//				acutPrintf(_T("(%d . %0.3f)\n"), pRbList->restype,
//				pRbList->resval.rreal);
//			break;
//		case RTSTR:
//			if (pRbList->restype == RTSTR)
//				acutPrintf(_T("RTSTR : %s\n"),
//				pRbList->resval.rstring);
//			else
//				acutPrintf(_T("(%d . \"%s\")\n"), pRbList->restype,
//				pRbList->resval.rstring);
//			break;
//		case RT3DPOINT:
//			if (pRbList->restype == RT3DPOINT)
//				acutPrintf(
//				_T("RT3DPOINT : %0.3f, %0.3f, %0.3f\n"),
//				pRbList->resval.rpoint[X],
//				pRbList->resval.rpoint[Y],
//				pRbList->resval.rpoint[Z]);
//			else
//				acutPrintf(
//				_T("(%d %0.3f %0.3f %0.3f)\n"),
//				pRbList->restype,
//				pRbList->resval.rpoint[X],
//				pRbList->resval.rpoint[Y],
//				pRbList->resval.rpoint[Z]);
//			break;
//		case RTLONG:
//			acutPrintf(_T("RTLONG : %dl\n"),
//				pRbList->resval.rlong);
//			break;
//		case -1:
//		case RTENAME:
//			acutPrintf(_T("(%d . <Entity name: %8lx>)\n"),
//				pRbList->restype, pRbList->resval.rlname[0]);
//			break;
//		case -3:
//			acutPrintf(_T("(-3)\n"));
//		}
//
//		if ((i == 23) && (pRbList->rbnext != NULL)) {
//			i = 0;
//			acedGetString(0,
//				_T("Press <ENTER> to continue..."), buf);
//		}
//	}
//	acutRelRb(pRbList);
//}



AcDbObjectId MyEditEntity::CreateNewNoPlotLayer(CString strLayer,int nColor, BOOL bIsModified)
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return AcDbObjectId::kNull;

	AcDbLayerTableRecord* pRecord;
	if(pTable->has(strLayer))//这个层已经有了	
	{
		if ( pTable->getAt(strLayer, pRecord, AcDb::kForWrite) != Acad::eOk )
		{
			pTable->close();
			return AcDbObjectId::kNull;
		}

		pRecord->setIsPlottable(false);
		if (bIsModified)
		{
			AcCmColor layerColor;
			layerColor.setColorIndex(nColor);
			pRecord->setColor(layerColor);
		}
		pRecord->close();
	}
	else					//这个层没有
	{
		pRecord = new AcDbLayerTableRecord;		
		pRecord->setName(strLayer);	
		Acad::ErrorStatus es;
		pRecord->setIsFrozen(Adesk::kFalse);	//解冻
		pRecord->setIsLocked(Adesk::kFalse);	//解锁
		pRecord->setIsOff(Adesk::kFalse);		//on
		pRecord->setIsPlottable(false);
		AcCmColor layerColor;
		layerColor.setColorIndex(nColor);
		pRecord->setColor(layerColor);
		pTable->add(pRecord);
		pRecord->close();
	}
	pTable->close();

	acdbCurDwg()->getLayerTable(pTable,AcDb::kForRead);
	AcDbObjectId layerId;
	pTable->getAt(strLayer, layerId);
	pTable->close();
	return layerId;	
}

void MyEditEntity::FrozenLayer(CStringArray& strLayerArr, bool bIsModified)
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return;
	AcDbLayerTableRecord* pRecord;
	AcDbLayerTableIterator* iter;
	pTable->newIterator(iter);
	CString strLayerName;
	const TCHAR* locName;
	for (; !iter->done(); iter->step())
	{	
		bool bIsFrozen = true;
		iter->getRecord(pRecord, AcDb::kForWrite);
		pRecord->getName(locName);
		strLayerName = locName;
		if (bIsModified)
		{
			for (int i=0; i<strLayerArr.GetSize(); i++)
			{
				if (strLayerName.CompareNoCase(strLayerArr.GetAt(i)) == 0)
				{
					bIsFrozen = false;
					break;
				}
			}
		}
		else
		{
			bIsFrozen = false;
		}
		pRecord->setIsFrozen(bIsFrozen);

		pRecord->close();
	}
	pTable->close();
	delete iter;
}

void MyEditEntity::FrozenPassivationLayer(bool bIsFrozen)
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return;
	AcDbLayerTableRecord* pRecord;
	AcDbLayerTableIterator* iter;
	pTable->newIterator(iter);
	CString strLayerName,strDescription;
	for (; !iter->done(); iter->step())
	{
		iter->getRecord(pRecord, AcDb::kForRead);
		strDescription = pRecord->description();

		if ((strDescription.CompareNoCase(_T("Passivation")) == 0))
		{
			pRecord->upgradeOpen();
			pRecord->setIsFrozen(bIsFrozen);
			pRecord->close();
			break;
		}
		pRecord->close();
	}
	pTable->close();
	delete iter;
}

void MyEditEntity::UnFrozenLayer()
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return;
	AcDbLayerTableRecord* pRecord;
	AcDbLayerTableIterator* iter;
	pTable->newIterator(iter);
	CString strLayerName,strDescription;
	for (; !iter->done(); iter->step())
	{
		iter->getRecord(pRecord, AcDb::kForWrite);
		pRecord->setIsFrozen(false);
		pRecord->close();
	}
	pTable->close();
	delete iter;
}




bool MyEditEntity::EraseObj(AcDbObjectId objId)
{
	if (objId.isNull())
	{
		return false;
	}
	AcDbObject* pObj = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		return false;
	}
	pObj->erase();
	pObj->close();
	return true;
}

bool MyEditEntity::EraseSSName(ads_name ssname)
{
	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen < 1)
	{
		return false;
	}
	AcDbObjectId objId = AcDbObjectId::kNull;

	ads_name ename;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		EraseObj(objId);
	}
	return true;
}

bool MyEditEntity::EraseAllEnt()
{
	ads_name ssname;
	int nRet = acedSSGet(_T("X"), NULL, NULL, NULL, ssname);
	if (nRet != RTNORM)
	{
		return false;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	AcDbObjectId objId;
	ads_name ename;
	AcDbEntity* pEnt = NULL;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite) != Acad::eOk)
		{
			continue;
		}
		pEnt->erase();
		pEnt->close();
	}
	acedSSFree(ssname);
	return true;
}

CString MyEditEntity::openObjAndGetGroupName(AcDbObjectId objId)
{
	CString strGroupName(_T(""));
	AcDbObjectIdArray objIdArrs;
	objIdArrs.removeAll();
	objIdArrs = openObjAndGetGroupIds(objId);
	if ((objIdArrs.length() <=0) || (objIdArrs.length() > 1))
	{
		return strGroupName;
	}
	AcDbObjectId groupId;
	groupId = objIdArrs.at(0);
	AcDbGroup* pGroup = NULL;
	if (acdbOpenAcDbObject((AcDbObject*&)pGroup, groupId, AcDb::kForRead) != Acad::eOk)
	{
		return strGroupName;
	}
	strGroupName = pGroup->name();
	pGroup->close();
	return strGroupName;
}

bool MyEditEntity::EraseEntByGroupName(CString strGroupName)
{
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	Acad::ErrorStatus es;
	es = acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	es = pGroupDict->getAt(strGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		pGroupDict->close();
		return false;
	}

	AcDbEntity* pEnt = NULL;
	AcDbObjectId objId;
	AcDbObjectIdArray objIds;
	objIds.removeAll();
	int nLength = 0;
	nLength = pGroup->allEntityIds(objIds);
	for (int i=0; i<objIds.length(); i++)
	{
		objId = objIds.at(i);
		es = acdbOpenAcDbEntity((AcDbEntity*&)pEnt, objId, AcDb::kForRead);
		if (es!= Acad::eOk)
		{
			pEnt->close();
		}
		else
		{
			pEnt->upgradeOpen();
			pEnt->erase();
			pEnt->close();
		}
	}
	pGroup->erase();
	pGroup->close();

	pGroupDict->close();
	return true;
}

bool MyEditEntity::AddObjToGroup(CString strLabel, AcDbObjectId objId)
{
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strLabel, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return false;
	}
	pGroup->append(objId);
	pGroup->close();
	pGroupDict->close();
	return true;
}

AcDbObjectId MyEditEntity::openEntChangeLineType(AcDbObjectId entId, CString strLineTypeName, double dScale)
{
	if (entId.isNull())
	{
		return AcDbObjectId::kNull;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
	{
		pEnt->close();
		return AcDbObjectId::kNull;
	}	

	if (strLineTypeName.CompareNoCase(_T("Continuous")) != 0)
	{
		AcDbLinetypeTable   *pLinetypeTbl; 
		AcDbObjectId   ltId; 
		acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
		if(pLinetypeTbl-> getAt(strLineTypeName,ltId)!=Acad::eOk) 
		{ 
			pLinetypeTbl-> close(); 
			Acad::ErrorStatus es;
#ifdef ARX
			es = acdbCurDwg()-> loadLineTypeFile(strLineTypeName, _T("acad.lin")); 
#else
			es = acdbCurDwg()->loadLineTypeFile(strLineTypeName, _T("zwcad.lin"));
#endif
			acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
			pLinetypeTbl-> getAt(strLineTypeName,ltId); 
		} 
		pLinetypeTbl-> close(); 
		pEnt->setLinetype(ltId);
	}
	else
	{
		pEnt->setLinetype(strLineTypeName);
	}
	pEnt->setLinetypeScale(dScale);
	pEnt->close();
	return pEnt->objectId();
}
