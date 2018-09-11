#include "StdAfx.h"
#include "MySymble.h"

MySymble::MySymble(void)
{
}

MySymble::~MySymble(void)
{
}


AcDbObjectId MySymble::GetLayer(CString strlayer)
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForRead)	!= Acad::eOk) 
		return AcDbObjectId::kNull;

	AcDbObjectId layerId;
	pTable->getAt(strlayer, layerId);
	pTable->close();
	return layerId;
}
AcDbObjectId MySymble::CreateNewLayer(CString strLayer, int nColor, BOOL bIsModColor, CString strLineTypeName, AcDb::LineWeight Lweight)
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

		///////////
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
			pRecord->setLinetypeObjectId(ltId);
		}
		/////////////////

		if (bIsModColor)
		{
			AcCmColor layerColor;
			layerColor.setColorIndex(nColor);
			pRecord->setColor(layerColor);
		}
		pRecord->setLineWeight(Lweight);
		pRecord->close();
	}
	else					//这个层没有
	{
		pRecord = new AcDbLayerTableRecord;		
		pRecord->setName(strLayer);	
		AcCmColor layerColor;
		Acad::ErrorStatus es;
		es = layerColor.setColorIndex(nColor);
		pRecord->setColor(layerColor);
		pRecord->setIsFrozen(Adesk::kFalse);	//解冻
		pRecord->setIsLocked(Adesk::kFalse);	//解锁
		pRecord->setIsOff(Adesk::kFalse);		//on
		pRecord->setLineWeight(Lweight);
		///////
		if (strLineTypeName.CompareNoCase(_T("Continuous")) != 0)
		{
			AcDbLinetypeTable   *pLinetypeTbl; 
			AcDbObjectId   ltId; 
			acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
			if(pLinetypeTbl-> getAt(strLineTypeName,ltId)!=Acad::eOk) 
			{ 
				pLinetypeTbl-> close(); 
#ifdef ARX
				es = acdbCurDwg()-> loadLineTypeFile(strLineTypeName, _T("acad.lin")); 
#else
				es = acdbCurDwg()->loadLineTypeFile(strLineTypeName, _T("zwcad.lin"));
#endif
				acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
				pLinetypeTbl-> getAt(strLineTypeName,ltId); 
			} 
			pLinetypeTbl-> close(); 
			pRecord->setLinetypeObjectId(ltId);
		}
		//////////
		es = pTable->add(pRecord);
		pRecord->close();
	}
	pTable->close();

	acdbCurDwg()->getLayerTable(pTable,AcDb::kForRead);
	AcDbObjectId layerId;
	pTable->getAt(strLayer, layerId);
	pTable->close();
	return layerId;	
}

AcDbObjectId MySymble::CreateTextStyle(CString stylename,CString strFont,CString strBigfont,double dTextScale, double dTextSize, BOOL bIsModified)
{
	Acad::ErrorStatus es;
	AcDbTextStyleTable *pTextStyleTbl = new AcDbTextStyleTable;
	if (acdbCurDwg()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite) == Acad::eOk)
	{	
		AcDbTextStyleTableRecord *pTextStyleTblRcd = new AcDbTextStyleTableRecord;
		if(pTextStyleTbl->has(stylename) == Adesk::kFalse)
		{
			TCHAR sPath[MAX_PATH];
			if( acedFindFile( strFont, sPath ) != RTNORM )
			{
				es = pTextStyleTblRcd->setFont(strFont, FALSE, FALSE, 1, 1);
				if (es != Acad::eOk)
				{
					acutPrintf(_T("\n setFont出错!"));
				}
				es = pTextStyleTblRcd->setName(stylename);
				es = pTextStyleTblRcd->setXScale(dTextScale);
				es = pTextStyleTblRcd->setTextSize(dTextSize);
				es = pTextStyleTbl->add(pTextStyleTblRcd);
				es = pTextStyleTblRcd->close();
			}
			else
			{
				es = pTextStyleTblRcd->setFileName(strFont); 
				es = pTextStyleTblRcd->setBigFontFileName(strBigfont);				
				es = pTextStyleTblRcd->setName(stylename);
				es = pTextStyleTblRcd->setXScale(dTextScale);
				es = pTextStyleTblRcd->setTextSize(dTextSize);
				es = pTextStyleTbl->add(pTextStyleTblRcd);
				es = pTextStyleTblRcd->close();
			}
			es = pTextStyleTbl->close();
		}
		else
		{
			if (bIsModified)
			{
				if ( pTextStyleTbl->getAt(stylename, pTextStyleTblRcd, AcDb::kForWrite) != Acad::eOk )
				{
					pTextStyleTbl->close();
					return AcDbObjectId::kNull;
				}
				if (_tcslen(strBigfont)!=0)
				{
					pTextStyleTblRcd->setFileName(strFont);
					pTextStyleTblRcd->setBigFontFileName(strBigfont);
				}
				else
				{
					if(pTextStyleTblRcd->setFont(strFont, FALSE, FALSE, 1, 1)!= Acad::eOk)
					{
						pTextStyleTblRcd->close();
						pTextStyleTbl->close();
						return AcDbObjectId::kNull;
					}
				}
				es = pTextStyleTblRcd->setXScale(dTextScale);
				es = pTextStyleTblRcd->setTextSize(dTextSize);
				pTextStyleTblRcd->close();
			}
			pTextStyleTbl->close();
		}
	}

	acdbCurDwg()->getTextStyleTable(pTextStyleTbl,AcDb::kForRead);
	AcDbObjectId TextStyleId;
	pTextStyleTbl->getAt(stylename, TextStyleId);
	es = pTextStyleTbl->close();
	return TextStyleId;
}

Acad::ErrorStatus MySymble::GetTextStyleInfo(AcDbObjectId ObjId, double& dTextScale, double& dTextSize)
{
	Acad::ErrorStatus es;
	if (ObjId.isNull())
	{
		return Acad::eNotApplicable;
	}

	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, ObjId, AcDb::kForRead)!=Acad::eOk)
	{
		return Acad::eNotApplicable;
	}
	if (pObj->isKindOf(AcDbTextStyleTableRecord::desc()))
	{
		AcDbTextStyleTableRecord* pTextStyleTblRcd = AcDbTextStyleTableRecord::cast(pObj);
		dTextScale = pTextStyleTblRcd->xScale();
		dTextSize = pTextStyleTblRcd->textSize();
		pTextStyleTblRcd->close();
		return Acad::eOk;
	}
	else
	{
		pObj->close();
		return Acad::eNotApplicable;
	}
}

AcDbObjectId MySymble::CreateLineType(CString strLtName, BOOL bIsModified)
{
	AcDbLinetypeTable   *pLinetypeTbl; 
	AcDbObjectId   ltId; 
	Acad::ErrorStatus es;
	acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForWrite); 
	if(pLinetypeTbl-> getAt(strLtName,ltId)!=Acad::eOk) 
	{ 
		pLinetypeTbl-> close(); 
#ifdef UNICODE
		es = acdbCurDwg()-> loadLineTypeFile(strLtName, _T("acad.lin")); 
#else
		es = acdbCurDwg()->loadLineTypeFile(strLtName, _T("zwcad.lin"));
#endif
		acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
		pLinetypeTbl-> getAt(strLtName,ltId); 
	} 
	pLinetypeTbl-> close(); 
	return ltId;
}

void MySymble::GetAllTextStyleName(CStringArray& StrArr)
{
	CString strName;
	AcDbTextStyleTable* pTextTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()  ->getSymbolTable(pTextTbl, AcDb::kForRead); 

	AcDbTextStyleTableIterator* pTblIter = NULL;
	AcDbTextStyleTableRecord* pTextTblRec = NULL;
	pTextTbl->newIterator(pTblIter);
	for (; !pTblIter->done(); pTblIter->step())
	{
		const TCHAR* locName;
		pTblIter->getRecord(pTextTblRec, AcDb::kForRead);
		pTextTblRec->getName(locName);
		strName = locName;
		StrArr.Add(strName);
		pTextTblRec->close();
	}
	pTextTbl->close();
	delete pTblIter;
}



AcDbObjectId MySymble::CreateDimstyle(CString strDimName, double dHeight, double dDimlfac /* = 1.0 */, bool bDimTih, bool bDimToh)
{
	AcDbDimStyleTable* pTable;
	if (acdbCurDwg()->getDimStyleTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return AcDbObjectId::kNull;

	AcDbDimStyleTableRecord* pRecord;
	Acad::ErrorStatus es;
	if(pTable->has(strDimName))//这个层已经有了	
	{
		if ( pTable->getAt(strDimName, pRecord, AcDb::kForWrite) != Acad::eOk )
		{
			pTable->close();
			return AcDbObjectId::kNull;
		}
		//if (dDimlfac - 1.0 >0.000001)
		//{
		pRecord->setDimlfac(dDimlfac);
		//}
		AcCmColor color;
		color.setColorIndex(5);
		pRecord->setDimclrd(color);
		color.setColorIndex(5);
		pRecord->setDimclre(color);
		color.setColor(5);
		pRecord->setDimclrt(color);//标注文字颜色

		pRecord->setDimexo(200);//设置尺寸线
		pRecord->setDimexe(200);

		pRecord->setDimtxt(dHeight);
		pRecord->setDimasz(dHeight);
		pRecord->setDimdec(1);//显示小数点位数
		pRecord->setDimtih(bDimTih);
		pRecord->setDimtoh(bDimToh);
		pRecord->close();
	}
	else					//这个层没有
	{
		pRecord = new AcDbDimStyleTableRecord;	

		pRecord->setName(strDimName);	

		//if (dDimlfac - 1.0 >0.000001)
		//{
		es = pRecord->setDimlfac(dDimlfac);
		//}
		AcCmColor color;
		color.setColorIndex(5);
		es = pRecord->setDimclrd(color);
		color.setColorIndex(5);
		es = pRecord->setDimclre(color);
		color.setColor(5);
		es = pRecord->setDimclrt(color);//标注文字颜色

		es = pRecord->setDimtxt(dHeight);
		es = pRecord->setDimasz(dHeight);
		es = pRecord->setDimdec(1);//显示小数点位数

		pRecord->setDimexo(200);//设置尺寸线
		pRecord->setDimexe(200);
		pRecord->setDimtih(bDimTih);
		pRecord->setDimtoh(bDimToh);

		es = pTable->add(pRecord);
		pRecord->close();
	}
	pTable->close();

	acdbCurDwg()->getDimStyleTable(pTable,AcDb::kForRead);
	AcDbObjectId DimstyleId;
	pTable->getAt(strDimName, DimstyleId);
	pTable->close();
	return DimstyleId;	
}

