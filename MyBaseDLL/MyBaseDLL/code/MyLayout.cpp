#include "StdAfx.h"
#include "MyLayout.h"

MyLayout::MyLayout(void)
{
}

MyLayout::~MyLayout(void)
{
}

Acad::ErrorStatus MyLayout::DrawLayout(const AcGePoint3d& pt1, const AcGePoint3d& pt2, const AcGePoint3d& ptInsertBottomLeft, LPCTSTR lpszLayoutName, double dScaleFactor/* = 1.0*/, double dAngle /*= 0.0*/, AcDbPolyline* pBoundary /*= NULL*/)
{
	AcGeMatrix3d mat;
	//     mat.setToScaling(1 / dScaleFactor, pt1);
	mat *= AcGeMatrix3d::translation(ptInsertBottomLeft - pt1);

	AcGeVector3d dirRotate = AcGeVector3d::kXAxis;
	dirRotate.rotateBy(dAngle, AcGeVector3d::kZAxis);

	AcDbExtents ext(pt1, pt2);
	AcGePoint3d ptGeoMid = AcGePoint3d((pt2.x + pt1.x) / 2.0, (pt2.y + pt1.y) / 2.0, 0.0);
	AcGePoint3d ptModelCenter = ptGeoMid;

	mat *= AcGeMatrix3d::rotation(-dAngle, AcGeVector3d::kZAxis, ptModelCenter);

	if (NULL == pBoundary)
	{
		return Acad::eNotImplementedYet;
	}

	AcDbObjectId layerId = pBoundary->layerId();
	AcDbObjectIdArray layerIdArr;
	layerIdArr.append(layerId);

	actrTransactionManager->startTransaction();
	try
	{
		// 新建布局
		AcDbObjectId viewportId;
		AcDbObjectId layoutId;
		AcDbObjectId blkTabRecId;
		AcDbObjectId plineId;

		FindLayout(lpszLayoutName, layoutId, blkTabRecId);

		// 新建视口
		AcDbViewport* pViewPort = new AcDbViewport;
		pViewPort->setCustomScale(dScaleFactor);
		pViewPort->setCenterPoint(/*ptTKCen*/ptModelCenter);

		// 设置视口属性
		double dHeight = fabs(ext.maxPoint().y - ext.minPoint().y);
		pViewPort->setWidth(fabs(ext.maxPoint().x - ext.minPoint().x));
		pViewPort->setHeight(dHeight);
		//    pViewPort->setLayer(m_ComLayer_BuJuShiKou);
		pViewPort->setTwistAngle(-dAngle);
		pViewPort->setViewTarget(ptModelCenter);// 设置视口目标
		pViewPort->setViewHeight(dHeight);
		pViewPort->setCustomScale(dScaleFactor);
		pViewPort->freezeLayersInViewport(layerIdArr);    // 在当前视口中冻结裁图线所在的图层

		{
			// 在图纸空间中加入视口
			/*AcDbObjectPointer<AcDbBlockTableRecord> spBlk(blkTabRecId, AcDb::kForWrite);
			EOK_RETURN_ERROR(spBlk.openStatus());*/
			AcDbBlockTableRecord* spBlk;
			if (acdbOpenObject((AcDbObject*&)spBlk, blkTabRecId, AcDb::kForWrite) != Acad::eOk)
			{
				return Acad::eNotImplementedYet;
			}
			spBlk->appendAcDbEntity(viewportId, pViewPort);

			spBlk->close();
			Acad::ErrorStatus es = pViewPort->setOn();
		}

		{
			// 在图纸空间中加入闭合多边形pline线
			AcDbPolyline* pTempLine = AcDbPolyline::cast(pBoundary->clone()) ;
			pTempLine->setClosed(Adesk::kTrue);

			/*AcDbObjectPointer<AcDbBlockTableRecord> spBlk(blkTabRecId, AcDb::kForWrite);
			EOK_RETURN_ERROR(spBlk.openStatus());*/
			AcDbBlockTableRecord* spBlk;
			if (acdbOpenObject((AcDbObject*&)spBlk, blkTabRecId, AcDb::kForWrite) != Acad::eOk)
			{
				return Acad::eNotImplementedYet;
			}
			spBlk->appendAcDbEntity(plineId, pTempLine);
			spBlk->close();

			pTempLine->transformBy(mat);

			pTempLine->close();
		}

		pViewPort->transformBy(mat);
		pViewPort->setNonRectClipEntityId(plineId);
		pViewPort->setNonRectClipOn();
		pViewPort->setLocked();
		pViewPort->close();
	}
	catch (const Acad::ErrorStatus err)
	{
		actrTransactionManager->endTransaction();
	}
	catch (...)
	{
		acutPrintf(_T("未知错误！"));
		actrTransactionManager->endTransaction();
	}

	actrTransactionManager->endTransaction();

	//// 有时生成的裁图在视口以外的地方，需要缩放才能看到
	//　　// 但是每生成一副裁图，就要闪烁，先屏蔽了, 放在命令结束时做一次缩放
	//　　// acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("E"), 0);
	//ads_regen();

	return Acad::eOk;
}

Acad::ErrorStatus MyLayout::CreateNewLayout(LPCTSTR lpszName, AcDbObjectId& layoutId, AcDbObjectId& blkTabRecId)
{
	AcApLayoutManager* pLayMan = (AcApLayoutManager *) acdbHostApplicationServices()->layoutManager();
	if (!pLayMan)
	{
		return Acad::eNotImplementedYet;
	}

	AcDbLayout* pLayout = NULL;
	if (pLayout = pLayMan->findLayoutNamed(lpszName))
	{
		layoutId = pLayout->objectId();
		blkTabRecId = pLayout->getBlockTableRecordId();

		pLayMan->setCurrentLayoutId(layoutId);

		return Acad::eNotImplementedYet;
	}
	else
	{
		//         pLayMan->createLayout(lpszName, layoutId, blkTabRecId);
		//         pLayMan->updateLayoutTabs();
		//         pLayMan->setCurrentLayoutId(layoutId);

		Acad::ErrorStatus es = Acad::eOk;
		AcDbBlockTable*    pBlockTable;
		es = acdbCurDwg()->getSymbolTable(pBlockTable,AcDb::kForWrite);
		//EOK_RETURN_ERROR(es);

		AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord;
		pBlockTableRecord->setName(_T("*Paper_Space"));
		pBlockTable->add(blkTabRecId, pBlockTableRecord);
		pBlockTable->close();
		pBlockTableRecord->close();

		pLayout = new AcDbLayout;
		if(!pLayout)
		{
			return es;
		}
		es = pLayout->setLayoutName(lpszName);            
		if (es != Acad::eOk)
		{
			delete pLayout;
			return es;
		}

		es = pLayout->addToLayoutDict(acdbCurDwg(), blkTabRecId);            
		if (es != Acad::eOk)
		{
			delete pLayout;
			return es;
		}

		pLayout->close();
		pLayMan->updateLayoutTabs();
		pLayMan->setCurrentLayout(lpszName);
	}

	return Acad::eOk;
}

Acad::ErrorStatus MyLayout::AllLayout()
{
	//遍历所有布局
	{
		//获得布局字典，布局字典里包含了模型和布局
		AcDbDictionary *pLayoutDic=NULL;
		Acad::ErrorStatus es=acdbHostApplicationServices()
			->workingDatabase()->getLayoutDictionary(pLayoutDic,AcDb::kForRead);
		if(es != Acad::eOk)
		{
			pLayoutDic->close();
			acutPrintf(_T("\n获取图纸空间字典错误"));
			return Acad::eOk;
		}
		if(pLayoutDic->numEntries()==0)
		{
			pLayoutDic->close();
			acutPrintf(_T("\n当前图形不存在图纸空间"));
			return Acad::eOk;
		}

		//创建布局字典遍历器
		AcDbDictionaryIterator *pLayoutDicItr=pLayoutDic->newIterator();

		//创建循环，遍历布局
		for(int i = 1; !pLayoutDicItr->done(); pLayoutDicItr->next())
		{
			acutPrintf(_T("\n第[ %d ]个布局，布局名称为[ %s ]"),i,pLayoutDicItr->name());
			i++;
		}
		delete pLayoutDicItr;    //删除遍历器对象
		pLayoutDic->close();
	}

	return Acad::eOk;
}

Acad::ErrorStatus MyLayout::FindLayout(LPCTSTR lpszName, AcDbObjectId& layoutId, AcDbObjectId& blkTabRecId)
{
	AcApLayoutManager* pLayMan = (AcApLayoutManager *) acdbHostApplicationServices()->layoutManager();
	if (!pLayMan)
	{
		return Acad::eNotImplementedYet;
	}

	AcDbLayout* pLayout = NULL;
	if (pLayout = pLayMan->findLayoutNamed(lpszName))
	{
		layoutId = pLayout->objectId();
		blkTabRecId = pLayout->getBlockTableRecordId();

		pLayMan->setCurrentLayoutId(layoutId);

		return Acad::eOk;
	}

	return Acad::eNotImplementedYet;
}