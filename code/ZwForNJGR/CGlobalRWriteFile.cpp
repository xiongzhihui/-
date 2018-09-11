#include "StdAfx.h"
#include "CGlobalRWriteFile.h"
#include "Utility.h"
#include "Markup.h"

CGlobalRWriteFile::CGlobalRWriteFile(void)
{
	m_FrameIni.clear();
	m_AllFrame.clear();
}

CGlobalRWriteFile::~CGlobalRWriteFile(void)
{
	m_FrameIni.clear();
	m_AllFrame.clear();
}


void CGlobalRWriteFile::clear()
{
	m_FrameIni.clear();
	m_AllFrame.clear();
}

bool CGlobalRWriteFile::readFrameIniFile()
{
	CString strFile = CUtility::getFrameIniFile();
	if (!gGlobal.IsFileExist(strFile))
	{
		return false;
	}
	CMarkup xml;      //声明一个xml对象
	bool bIsLoaded = false;
	bIsLoaded = xml.Load(strFile);            //加载xml文件，成功返回true
	if (!bIsLoaded)
	{
		return false;
	}
	xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始

	CFrameInfo frameInfo;
	double dValue = 0.0;
	while (xml.FindChildElem(_T("Frame")))//不能使用if，因为要遍历所有的Student元素
	{	
		CString strTagName;//元素名称
		strTagName = xml.GetChildAttrib(_T("Name")); 
		frameInfo.setLrmName(strTagName);

		xml.IntoElem();
		CString strData;//元素内容
		xml.FindChildElem(_T("FrameType"));
		strData = xml.GetChildData();////获取元素内容
		if (strData.IsEmpty())
		{
			strData = gGlobal.GetIniValue(_T("图框设置"), _T("图框类型"));
		}
		frameInfo.setFrameType(strData);
		xml.ResetChildPos();

		xml.FindChildElem(_T("HorSpace"));
		strData = xml.GetChildData();////获取元素内容
		dValue = MyTransFunc::StringToDouble(strData);
		frameInfo.setHorSpace(dValue);
		xml.ResetChildPos();

		xml.FindChildElem(_T("VerSpace"));
		strData = xml.GetChildData();////获取元素内容
		dValue = MyTransFunc::StringToDouble(strData);
		frameInfo.setVerSpace(dValue);
		xml.ResetChildPos();

		xml.FindChildElem(_T("LeftHorGap"));
		strData = xml.GetChildData();////获取元素内容
		dValue = MyTransFunc::StringToDouble(strData);
		frameInfo.setLeftHorGap(dValue);
		xml.ResetChildPos();
		xml.FindChildElem(_T("LeftVerGap"));
		strData = xml.GetChildData();////获取元素内容
		dValue = MyTransFunc::StringToDouble(strData);
		frameInfo.setLeftVerGap(dValue);
		xml.ResetChildPos();

		xml.FindChildElem(_T("SpaceVerGap"));
		strData = xml.GetChildData();////获取元素内容
		dValue = MyTransFunc::StringToDouble(strData);
		frameInfo.setSpaceVerGap(dValue);
		xml.ResetChildPos();

		xml.FindChildElem(_T("Gap"));
		strData = xml.GetChildData();////获取元素内容
		dValue = MyTransFunc::StringToDouble(strData);
		frameInfo.setGap(dValue);
		xml.ResetChildPos();

		
		

		

		xml.OutOfElem();
		m_FrameIni.insert(make_pair(strTagName, frameInfo));
	}

	return true;
}

bool CGlobalRWriteFile::EditFrameIniFile()
{
	CString strFile = CUtility::getFrameIniFile();
	if (!gGlobal.IsFileExist(strFile))
	{
		return false;
	}
	CMarkup xml;      //声明一个xml对象
	bool bIsLoaded = false;
	bIsLoaded = xml.Load(strFile);            //加载xml文件，成功返回true
	if (!bIsLoaded)
	{
		return false;
	}
	xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始
	xml.FindElem();//查找任意元素，此处查找到的第一个节点即为根节点
	xml.IntoElem();//进入Root
	CString strName;
	CFrameInfo frameInfo;
	for (map<CString, CFrameInfo>::iterator iter = m_FrameIni.begin();
		iter != m_FrameIni.end(); ++iter)
	{
		strName = iter->first;
		frameInfo = iter->second;
		while (xml.FindChildElem(_T("Frame")))
		{
			CString strTagName;//元素名称
			strTagName = xml.GetChildAttrib(_T("Name")); 
			if (strTagName.CompareNoCase(strName) == 0)
			{
				xml.IntoElem();
				strTagName = xml.GetChildTagName();//获取子元素名称
				if (strTagName.CompareNoCase(_T("HorSpace")) == 0)
				{
					xml.SetChildData(frameInfo.getHorSpace());
				}
				else if (strTagName.CompareNoCase(_T("VerSpace")) == 0)
				{
					xml.SetChildData(frameInfo.getVerSpace());
				}
				else if (strTagName.CompareNoCase(_T("SpaceHorGap")) == 0)
				{
					xml.SetChildData(frameInfo.getSpaceHorGap());
				}
				else if (strTagName.CompareNoCase(_T("SpaceVerGap")) == 0)
				{
					xml.SetChildData(frameInfo.getSpaceVerGap());
				}
				else if (strTagName.CompareNoCase(_T("Gap")) == 0)
				{
					xml.SetChildData(frameInfo.getGap());
				}
				else if (strTagName.CompareNoCase(_T("LeftVerGap")) == 0)
				{
					xml.SetChildData(frameInfo.getLeftVerGap());
				}
				else if (strTagName.CompareNoCase(_T("LeftHorGap")) == 0)
				{
					xml.SetChildData(frameInfo.getLeftHorGap());
				} 
				else if (strTagName.CompareNoCase(_T("FrameType")) == 0)
				{
					xml.SetChildData(frameInfo.getFrameType());
				}
				xml.OutOfElem();
				xml.Save(strFile);
				break;
			}
		}
	}
	return true;
}

bool CGlobalRWriteFile::addFrameIniFile(CString strName, CFrameInfo frameInfo)
{
	CString strFile = CUtility::getFrameIniFile();
	if (!gGlobal.IsFileExist(strFile))
	{
		return false;
	}
	CMarkup xml;      //声明一个xml对象
	bool bIsLoaded = false;
	bIsLoaded = xml.Load(strFile);            //加载xml文件，成功返回true
	if (!bIsLoaded)
	{
		return false;
	}
	xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始
	xml.FindElem();//查找任意元素，此处查找到的第一个节点即为根节点
	xml.IntoElem();//进入Root

	map<CString, CFrameInfo>::iterator iter = m_FrameIni.find(strName);
	if (iter != m_FrameIni.end())
	{
		//如果没有就插入
		xml.AddElem(_T("Frame"));
		xml.AddChildAttrib(_T("Name"), strName);
		xml.IntoElem();
		xml.AddChildElem(_T("FrameType"), frameInfo.getFrameType());
		xml.AddChildElem(_T("HorSpace"), frameInfo.getHorSpace());
		xml.AddChildElem(_T("VerSpace"), frameInfo.getVerSpace());
		xml.AddChildElem(_T("LeftVerGap"), frameInfo.getLeftVerGap());		
		xml.AddChildElem(_T("LeftHorGap"), frameInfo.getLeftHorGap());		
		xml.AddChildElem(_T("SpaceVerGap"), frameInfo.getSpaceVerGap());
		xml.AddChildElem(_T("Gap"), frameInfo.getGap());
		xml.OutOfElem();
		xml.Save(strFile);
	}
	else
	{
		//那么就编辑
		while (xml.FindChildElem(_T("Frame")))
		{
			CString strTagName;//元素名称
			strTagName = xml.GetChildAttrib(_T("Name")); 
			if (strTagName.CompareNoCase(strName) == 0)
			{
				xml.IntoElem();
				strTagName = xml.GetChildTagName();//获取子元素名称
				if (strTagName.CompareNoCase(_T("HorSpace")) == 0)
				{
					xml.SetChildData(frameInfo.getHorSpace());
				}
				else if (strTagName.CompareNoCase(_T("VerSpace")) == 0)
				{
					xml.SetChildData(frameInfo.getVerSpace());
				}
				else if (strTagName.CompareNoCase(_T("SpaceHorGap")) == 0)
				{
					xml.SetChildData(frameInfo.getSpaceHorGap());
				}
				else if (strTagName.CompareNoCase(_T("SpaceVerGap")) == 0)
				{
					xml.SetChildData(frameInfo.getSpaceVerGap());
				}
				else if (strTagName.CompareNoCase(_T("Gap")) == 0)
				{
					xml.SetChildData(frameInfo.getGap());
				}
				else if (strTagName.CompareNoCase(_T("LeftVerGap")) == 0)
				{
					xml.SetChildData(frameInfo.getLeftVerGap());
				}
				else if (strTagName.CompareNoCase(_T("LeftHorGap")) == 0)
				{
					xml.SetChildData(frameInfo.getLeftHorGap());
				} 
				else if (strTagName.CompareNoCase(_T("FrameType")) == 0)
				{
					xml.SetChildData(frameInfo.getFrameType());
				}
				xml.OutOfElem();
				xml.Save(strFile);
				break;
			}
		}
	}
	return true;
}

bool CGlobalRWriteFile::delFrameIniByName(CString strName)
{
	CString strFile = CUtility::getFrameIniFile();
	if (!gGlobal.IsFileExist(strFile))
	{
		return false;
	}
	CMarkup xml;      //声明一个xml对象
	bool bIsLoaded = false;
	bIsLoaded = xml.Load(strFile);            //加载xml文件，成功返回true
	if (!bIsLoaded)
	{
		return false;
	}
	xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始
	xml.FindElem();//查找任意元素，此处查找到的第一个节点即为根节点
	xml.IntoElem();//进入Root

	map<CString, CFrameInfo>::iterator iter = m_FrameIni.find(strName);
	if (iter != m_FrameIni.end())
	{
		while (xml.FindChildElem(_T("Frame")))
		{
			CString strTagName;//元素名称
			strTagName = xml.GetChildAttrib(_T("Name")); 
			if (strTagName.CompareNoCase(strName) == 0)
			{
				xml.RemoveChildElem();
			}
		}
		xml.Save(strFile);
	}

	return true;
}

CFrameInfo CGlobalRWriteFile::getFrameInfoByName(CString strLrmName)
{
	CFrameInfo frameInfo;
	map<CString, CFrameInfo>::iterator iter = m_FrameIni.find(strLrmName);
	if (iter == m_FrameIni.end())
	{
		//如果没有找到
		return frameInfo;
	}
	else
	{
		frameInfo = iter->second;
	}
	return frameInfo;
}


map<int, CFrameInfo> CGlobalRWriteFile::getAllFrameInfo()
{
	return m_AllFrame;
}

void CGlobalRWriteFile::saveAllFrameInfo()
{
	m_AllFrame.clear();
	resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("INSERT"), 2, _T("A*"), -4,_T("and>"), RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen < 1)
	{
		return;
	}
	ads_name ename;
	AcDbObjectId blkId = AcDbObjectId::kNull;
	CString strBlockName;
	AcDbBlockReference* pBlkRef = NULL;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(blkId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, blkId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		pBlkRef->close();
		CString strSheetName;
		CFrameInfo frameInfo;

		strSheetName = MyEditEntity::GetObjStrXdata(blkId, ICD_SHEETNAME);
		if (strSheetName.IsEmpty())
		{
			continue;
		}
		int nCount = MyEditEntity::GetObjIntXdata(blkId, ICD_COUNT);
		frameInfo.setLrmName(MyEditEntity::GetObjStrXdata(blkId, ICD_LRM_NAME));

		frameInfo.setFrameType(MyEditEntity::GetObjStrXdata(blkId, ICD_FRAMETYPE));

		frameInfo.setHorSpace(MyEditEntity::GetObjDoubleXdata(blkId, ICD_HORSPACE));
		frameInfo.setVerSpace(MyEditEntity::GetObjDoubleXdata(blkId, ICD_VerSapce));
		//frameInfo.setSpaceHorGap(MyEditEntity::GetObjDoubleXdata(blkId, ICD_SPAREHORGAP));
		frameInfo.setSpaceVerGap(MyEditEntity::GetObjDoubleXdata(blkId, ICD_SPAREVERGAP));
		frameInfo.setLeftVerGap(MyEditEntity::GetObjDoubleXdata(blkId, ICD_LEFTVERGAP));
		frameInfo.setLeftHorGap(MyEditEntity::GetObjDoubleXdata(blkId, ICD_LEFTHORGAP));
		m_AllFrame.insert(std::make_pair(nCount, frameInfo));
	}
	acedSSFree(ssname);
}
