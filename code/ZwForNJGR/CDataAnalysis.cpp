#include "StdAfx.h"
#include "CDataAnalysis.h"
#include "DlgProress.h"

extern CDlgProress* modelessDlg;
CDataAnalysis::CDataAnalysis(void)
{
	m_data.clear();
	m_dataSCell.clear();
	m_dataRCell.clear();
	m_dataCCell.clear();
	m_dataMCell.clear();
	m_dataRFCell.clear();
	m_dataDPCell.clear();
}

CDataAnalysis::~CDataAnalysis(void)
{
	m_data.clear();
	m_dataSCell.clear();
	m_dataRCell.clear();
	m_dataCCell.clear();
	m_dataMCell.clear();
	m_dataRFCell.clear();
	m_dataDPCell.clear();
}

void CDataAnalysis::setData(map<CString, CSingleSheetData> data)
{
	m_data.clear();
	for (map<CString, CSingleSheetData>::iterator iter = data.begin(); iter != data.end(); ++iter)
	{
		m_data.insert(make_pair(iter->first, iter->second));
	}
}

map<CString, CSingleSheetData> CDataAnalysis::getData() const
{
	return m_data;
}

void CDataAnalysis::setDataSCell(vector<CDataCell> data)
{
	m_dataSCell.clear();
	m_dataSCell.insert(m_dataSCell.end(), data.begin(), data.end());
}

vector<CDataCell> CDataAnalysis::getDataSCell() const
{
	return m_dataSCell;
}

void CDataAnalysis::setDataMCell(vector<CDataCell> data)
{
	m_dataMCell.clear();
	m_dataMCell.insert(m_dataMCell.end(), data.begin(), data.end());
}

vector<CDataCell> CDataAnalysis::getDataMCell() const
{
	return m_dataMCell;
}


void CDataAnalysis::setDataRCell(vector<pair<int, CDataCell> > data)
{
	m_dataRCell.clear();
	m_dataRCell.insert(m_dataRCell.end(), data.begin(), data.end());
}

vector<pair<int, CDataCell> > CDataAnalysis::getDataRCell() const
{
	return m_dataRCell;
}

void CDataAnalysis::setDataCCell(vector<CDataCell> data)
{
	m_dataCCell.clear();
	m_dataCCell.insert(m_dataCCell.end(), data.begin(), data.end());
}

vector<CDataCell> CDataAnalysis::getDataCCell() const
{
	return m_dataCCell;
}

void CDataAnalysis::setDataRFCell(vector<CDataCell> data)
{
	m_dataRFCell.clear();
	m_dataRFCell.insert(m_dataRFCell.end(), data.begin(), data.end());
}

vector<CDataCell> CDataAnalysis::getDataRFCell() const
{
	return m_dataRFCell;
}

void CDataAnalysis::setDataDPCell(vector<CDataCell> data)
{
	m_dataDPCell.clear();
	m_dataDPCell.insert(m_dataDPCell.end(), data.begin(), data.end());
}

vector<CDataCell> CDataAnalysis::getDataDPCell() const
{
	return m_dataDPCell;
}

bool CDataAnalysis::doIt()
{
	//acedSetStatusBarProgressMeterPos(10);
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(1);
	}
	if (!doSData())
	{
		return false;
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(10);
	}
	//acedSetStatusBarProgressMeterPos(20);
	

	if (!doCData())
	{
		return false;
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(20);
	}
	//acedSetStatusBarProgressMeterPos(25);
	
	if (!doRData())
	{
		return false;
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(25);
	}
	if (!doDPData())
	{
		return false;
	}

	if (!doRFData())
	{
		return false;
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(30);
	}
	return true;
}

bool CDataAnalysis::doSData()
{
	for (vector<CDataCell>::iterator iter = m_dataSCell.begin(); iter != m_dataSCell.end(); ++iter)
	{
		CDataCell pSData = *iter;
		CString strSTexing = pSData.getComCell().getRealTexing();
		for (vector<CDataCell>::iterator Itr = m_dataMCell.begin(); Itr != m_dataMCell.end(); ++Itr)
		{
			CDataCell pMData = *Itr;
			CString strMTexing = pMData.getComCell().getRealTexing();
			if (strMTexing.IsEmpty())
			{
				continue;
			}
			CString strSSheetName = pSData.getSheetName();
			CString strMSheetName = pMData.getSheetName();
			if (strSSheetName.CompareNoCase(strMSheetName) != 0)
			{
				if (strMTexing.CompareNoCase(strSTexing) == 0)
				{	
					//修改S对应的去向
					map<CString, CSingleSheetData>::iterator SIr = m_data.find(strSSheetName);
					if (SIr == m_data.end())
					{
						continue;
					}
					CSingleSheetData sSingleData = SIr->second;
					map<CNameCell, vector<CCommonCell> > sMap = sSingleData.getMapInfo();
					CNameCell sNameCell = pSData.getNameCell();
					map<CNameCell, vector<CCommonCell> >::iterator SItr = sMap.find(sNameCell);
					if (SItr == sMap.end())
					{
						continue;
					}

					vector<CCommonCell> sCom = SItr->second;
					CCommonCell sComCell = sCom.at(pSData.getIndex());
					CString strQuxiang;
					//strQuxiang = pMData.getSheet() + pMData.getNameCell().getRealName() + _T(":") + pMData.getComCell().getDuanzi();
					strQuxiang = pMData.getComCell().getTempQuxiang();
					sComCell.setQuxiang(strQuxiang);//先设置为端子
					sCom.at(pSData.getIndex()) = sComCell;
					SItr->second = sCom;
					sSingleData.setMapInfo(sMap);
					m_data[strSSheetName] = sSingleData;
					//修改M对应的去向
					map<CString, CSingleSheetData>::iterator MIr = m_data.find(strMSheetName);
					if (MIr == m_data.end())
					{
						continue;
					}
					CSingleSheetData mSingleData = MIr->second;
					map<CNameCell, vector<CCommonCell> > mMap = mSingleData.getMapInfo();
					CNameCell mNameCell = pSData.getNameCell();
					map<CNameCell, vector<CCommonCell> >::iterator MItr = mMap.find(mNameCell);
					if (MItr == mMap.end())
					{
						continue;
					}

					vector<CCommonCell> mCom = MItr->second;
					CCommonCell mComCell = mCom.at(pMData.getIndex());
					strQuxiang = mComCell.getQuxiang();
					if (strQuxiang.IsEmpty())
					{
						//strQuxiang = pSData.getSheet() + pSData.getNameCell().getRealName() + _T(":") + pSData.getComCell().getDuanzi();
						strQuxiang = pSData.getComCell().getTempQuxiang();
						mComCell.setQuxiang(strQuxiang);//先设置为端子
						mCom.at(pMData.getIndex()) = mComCell;
						MItr->second = mCom;
						mSingleData.setMapInfo(mMap);
						m_data[strMSheetName] = mSingleData;
					}
				}
			}
		}
	}
	return true;
}

bool CDataAnalysis::doCData()
{
	int i, j, temp;
	int nSize = m_dataCCell.size();
	CDataCell preCell, curCell;
	CCommonCell preComCell, curComCell;
	CString strPreTexing,strCurTexing, strPreQuxiang, strCurQuxiang, strPreSheetName, strCurSheetName;
	for (i = 0; i < nSize - 1; i++)
	{
		for (j = i+1; j < nSize; j++)
		{
			preCell = m_dataCCell[i];
			curCell = m_dataCCell[j];
			preComCell = preCell.getComCell();
			curComCell = curCell.getComCell();
			
			strPreTexing = preComCell.getTexing();
			strCurTexing = curComCell.getTexing();
			if (strPreTexing.IsEmpty() || strCurTexing.IsEmpty())//如果有空表示数据不会进入配对
			{
				continue;
			}					
			strPreQuxiang = preComCell.getQuxiang();
			strCurQuxiang = curComCell.getQuxiang();
			if ((!strPreQuxiang.IsEmpty()) && (!strCurQuxiang.IsEmpty()))//如果都不为空，说明已经找到去向，不用继续寻找
			{
				continue;
			}

			strPreSheetName = preCell.getSheetName();
			strCurSheetName = curCell.getSheetName();
			if (strPreSheetName.CompareNoCase(strCurSheetName) != 0)
			{	
				strPreTexing = preComCell.getRealTexing();
				strCurTexing = curComCell.getRealTexing();

				if(strPreTexing.CompareNoCase(strCurTexing) == 0)
				{
					if (strPreQuxiang.IsEmpty())//如果为空
					{
						//strPreQuxiang = curCell.getSheet() + curCell.getNameCell().getRealName() + _T(":") + curComCell.getDuanzi();
						strPreQuxiang = curComCell.getTempQuxiang();
						preComCell.setQuxiang(strPreQuxiang);

						//修改数据
						map<CString, CSingleSheetData>::iterator MIr = m_data.find(preCell.getSheetName());
						if (MIr == m_data.end())
						{
							continue;
						}
						CSingleSheetData mSingleData = MIr->second;
						map<CNameCell, vector<CCommonCell> > mMap = mSingleData.getMapInfo();
						CNameCell mNameCell = preCell.getNameCell();
						map<CNameCell, vector<CCommonCell> >::iterator MItr = mMap.find(mNameCell);
						if (MItr == mMap.end())
						{
							continue;
						}
						vector<CCommonCell> mCom = MItr->second;
						mCom.at(preCell.getIndex()) = preComCell;
						MItr->second = mCom;
						mSingleData.setMapInfo(mMap);
						m_data[preCell.getSheetName()] = mSingleData;


					}
					if (strCurQuxiang.IsEmpty())//如果没有数据
					{
						//strCurQuxiang = preCell.getSheet() + preCell.getNameCell().getRealName() + _T(":") + preComCell.getDuanzi();
						strCurQuxiang = preComCell.getTempQuxiang();
						curComCell.setQuxiang(strCurQuxiang);

						//修改数据
						map<CString, CSingleSheetData>::iterator MIr = m_data.find(curCell.getSheetName());
						if (MIr == m_data.end())
						{
							continue;
						}
						CSingleSheetData mSingleData = MIr->second;
						map<CNameCell, vector<CCommonCell> > mMap = mSingleData.getMapInfo();
						CNameCell mNameCell = curCell.getNameCell();
						map<CNameCell, vector<CCommonCell> >::iterator MItr = mMap.find(mNameCell);
						if (MItr == mMap.end())
						{
							continue;
						}
						vector<CCommonCell> mCom = MItr->second;
						mCom.at(curCell.getIndex()) = curComCell;
						MItr->second = mCom;
						mSingleData.setMapInfo(mMap);
						m_data[curCell.getSheetName()] = mSingleData;
					}
				}
			}
		}
	}
	return true;
}

bool CDataAnalysis::doRData()
{
	//将数据分类
	map<int, vector<CDataCell> > tmpMap;
	CString strPreName, strCurName, strPrefix, strNum;
	int nlen = 0;

	for (vector<pair<int, CDataCell> >::iterator iter = m_dataRCell.begin(); iter != m_dataRCell.end(); ++iter)
	{
		vector<CDataCell> tmpDataCell;
		tmpDataCell.push_back(iter->second);
		nlen = iter->first;

		pair<map<int, vector<CDataCell> >::iterator, bool> ret = tmpMap.insert(make_pair(nlen, tmpDataCell));
		if (!ret.second)
		{
			ret.first->second.push_back(iter->second);
		}
	}

	if (tmpMap.size() < 1)
	{
		return true;
	}
	for (map<int, vector<CDataCell> >::iterator Itr = tmpMap.begin(); Itr != tmpMap.end(); ++Itr)
	{	
		vector<CDataCell> preVec, curVec;
		preVec.clear();
		curVec.clear();

		preVec = Itr->second;
		
		//得到当前名称
		nlen = Itr->first;
		nlen++;
		map<int, vector<CDataCell> >::iterator IIR = tmpMap.find(nlen);
		if (IIR == tmpMap.end())
		{
			continue;
		}
		else
		{
			curVec = IIR->second;
			
			//////////////////////////////////////////////////////////////////////////
			//处理数据
			for (vector<CDataCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
			{
				CDataCell pSData = *preIter;
				CString strSTexing = pSData.getComCell().getRealTexing();
				if (strSTexing.IsEmpty())
				{
					continue;
				}
				for (vector<CDataCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
				{
					CDataCell pMData = *curIter;
					CString strMTexing = pMData.getComCell().getRealTexing();
					if (strMTexing.IsEmpty())
					{
						continue;
					}
					CString strSSheetName = pSData.getSheetName();
					CString strMSheetName = pMData.getSheetName();
					if (strSSheetName.CompareNoCase(strMSheetName) != 0)
					{
						if (strMTexing.CompareNoCase(strSTexing) == 0)
						{	
							//修改S对应的去向
							map<CString, CSingleSheetData>::iterator SIr = m_data.find(strSSheetName);
							if (SIr == m_data.end())
							{
								continue;
							}
							CSingleSheetData sSingleData = SIr->second;
							map<CNameCell, vector<CCommonCell> > sMap = sSingleData.getMapInfo();
							CNameCell sNameCell = pSData.getNameCell();
							map<CNameCell, vector<CCommonCell> >::iterator SItr = sMap.find(sNameCell);
							if (SItr == sMap.end())
							{
								continue;
							}

							vector<CCommonCell> sCom = SItr->second;
							CCommonCell sComCell = sCom.at(pSData.getIndex());
							CString strQuxiang;
							//strQuxiang = pMData.getSheet() + pMData.getNameCell().getRealName() + _T(":") + pMData.getComCell().getDuanzi();
							strQuxiang = pMData.getComCell().getTempQuxiang();
							sComCell.setQuxiang(strQuxiang);//先设置为端子
							sCom.at(pSData.getIndex()) = sComCell;
							SItr->second = sCom;
							sSingleData.setMapInfo(sMap);
							m_data[strSSheetName] = sSingleData;
						}
					}
				}
			}
		}
	}

	//处理R尾端到R始端
	map<int, vector<CDataCell> >::reverse_iterator bg = tmpMap.rbegin();
	map<int, vector<CDataCell> >::iterator iter = tmpMap.begin();

	vector<CDataCell> preVec, curVec;
	preVec.clear();
	curVec.clear();

	int nPre = bg->first;
	preVec = bg->second;
	curVec = iter->second;

	//////////////////////////////////////////////////////////////////////////
	//处理数据
	for (vector<CDataCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
	{
		CDataCell pSData = *preIter;
		CString strSTexing = pSData.getComCell().getRealTexing();
		if (strSTexing.IsEmpty())
		{
			continue;
		}
		for (vector<CDataCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
		{
			CDataCell pMData = *curIter;
			CString strMTexing = pMData.getComCell().getRealTexing();
			CString strSSheetName = pSData.getSheetName();
			CString strMSheetName = pMData.getSheetName();
			if (strSSheetName.CompareNoCase(strMSheetName) != 0)
			{
				if (strMTexing.CompareNoCase(strSTexing) == 0)
				{	
					//修改S对应的去向
					map<CString, CSingleSheetData>::iterator SIr = m_data.find(strSSheetName);
					if (SIr == m_data.end())
					{
						continue;
					}
					CSingleSheetData sSingleData = SIr->second;
					map<CNameCell, vector<CCommonCell> > sMap = sSingleData.getMapInfo();
					CNameCell sNameCell = pSData.getNameCell();
					map<CNameCell, vector<CCommonCell> >::iterator SItr = sMap.find(sNameCell);
					if (SItr == sMap.end())
					{
						continue;
					}

					vector<CCommonCell> sCom = SItr->second;
					CCommonCell sComCell = sCom.at(pSData.getIndex());
					CString strQuxiang;
					//strQuxiang = pMData.getSheet() + pMData.getNameCell().getRealName() + _T(":") + pMData.getComCell().getDuanzi();
					strQuxiang = pMData.getComCell().getTempQuxiang();
					sComCell.setQuxiang(strQuxiang);//先设置为端子
					sCom.at(pSData.getIndex()) = sComCell;
					SItr->second = sCom;
					sSingleData.setMapInfo(sMap);
					m_data[strSSheetName] = sSingleData;
				}
			}
		}
	}
	return true;
}

bool CDataAnalysis::doRFData()
{
	int i, j, temp;
	int nSize = m_dataRFCell.size();
	CDataCell preCell, curCell;
	CCommonCell preComCell, curComCell;
	CString strPreTexing,strCurTexing, strPreQuxiang, strCurQuxiang, strPreSheetName, strCurSheetName;
	for (i = 0; i < nSize - 1; i++)
	{
		for (j = i+1; j < nSize; j++)
		{
			preCell = m_dataRFCell[i];
			curCell = m_dataRFCell[j];
			preComCell = preCell.getComCell();
			curComCell = curCell.getComCell();

			strPreTexing = preComCell.getRealTexing();
			strCurTexing = curComCell.getRealTexing();

			if (strPreTexing.IsEmpty() || strCurTexing.IsEmpty())//如果有空表示数据不会进行匹配
			{
				continue;
			}
			strPreSheetName = preCell.getSheetName();
			strCurSheetName = curCell.getSheetName();
			//if (strPreSheetName.CompareNoCase(strCurSheetName) != 0)
			{
				if(strPreTexing.CompareNoCase(strCurTexing) == 0)
				{
					strPreQuxiang = preComCell.getQuxiang();
					strCurQuxiang = curComCell.getQuxiang();
					if ((!strPreQuxiang.IsEmpty()) && (!strCurQuxiang.IsEmpty()))//如果都不为空，说明已经找到去向，不用继续寻找
					{
						continue;
					}
					if (strPreQuxiang.IsEmpty())//如果为空
					{
						/*if ((curCell.getNameCell().getRealName().CompareNoCase(_T("其它")) == 0) ||
							(curCell.getNameCell().getRealName().CompareNoCase(_T("其他")) == 0))
						{
							strPreQuxiang = curComCell.getDuanzi() + _T(":1");
						}
						else	
						{
							strPreQuxiang = curCell.getSheet() + curCell.getNameCell().getRealName() + _T(":") + curComCell.getDuanzi();
						}*/
						strPreQuxiang = curComCell.getTempQuxiang();
						preComCell.setQuxiang(strPreQuxiang);

						//修改数据
						map<CString, CSingleSheetData>::iterator MIr = m_data.find(preCell.getSheetName());
						if (MIr == m_data.end())
						{
							continue;
						}
						CSingleSheetData mSingleData = MIr->second;
						map<CNameCell, vector<CCommonCell> > mMap = mSingleData.getMapInfo();
						CNameCell mNameCell = preCell.getNameCell();
						map<CNameCell, vector<CCommonCell> >::iterator MItr = mMap.find(mNameCell);
						if (MItr == mMap.end())
						{
							continue;
						}
						vector<CCommonCell> mCom = MItr->second;
						mCom.at(preCell.getIndex()) = preComCell;
						MItr->second = mCom;
						mSingleData.setMapInfo(mMap);
						m_data[preCell.getSheetName()] = mSingleData;


					}
					if (strCurQuxiang.IsEmpty())//如果没有数据
					{
						/*if ((preCell.getNameCell().getRealName().CompareNoCase(_T("其它")) == 0) ||
							(preCell.getNameCell().getRealName().CompareNoCase(_T("其他")) == 0))
						{
							strCurQuxiang = preComCell.getDuanzi() + _T(":1");
						}
						else
						{
							strCurQuxiang = preCell.getSheet() + preCell.getNameCell().getRealName() + _T(":") + preComCell.getDuanzi();
						}
						*/
						strCurQuxiang = preComCell.getTempQuxiang();
						curComCell.setQuxiang(strCurQuxiang);

						//修改数据
						map<CString, CSingleSheetData>::iterator MIr = m_data.find(curCell.getSheetName());
						if (MIr == m_data.end())
						{
							continue;
						}
						CSingleSheetData mSingleData = MIr->second;
						map<CNameCell, vector<CCommonCell> > mMap = mSingleData.getMapInfo();
						CNameCell mNameCell = curCell.getNameCell();
						map<CNameCell, vector<CCommonCell> >::iterator MItr = mMap.find(mNameCell);
						if (MItr == mMap.end())
						{
							continue;
						}
						vector<CCommonCell> mCom = MItr->second;
						mCom.at(curCell.getIndex()) = curComCell;
						MItr->second = mCom;
						mSingleData.setMapInfo(mMap);
						m_data[curCell.getSheetName()] = mSingleData;
					}
				}
			}
		}
	}
	return true;
}

bool CDataAnalysis::doDPData()
{
	int i, j, temp;
	int nSize = m_dataDPCell.size();
	CDataCell preCell, curCell;
	CCommonCell preComCell, curComCell;
	CString strPreTexing,strCurTexing, strPreQuxiang, strCurQuxiang, strPreSheetName, strCurSheetName;
	for (i = 0; i < nSize - 1; i++)
	{
		for (j = i+1; j < nSize; j++)
		{
			preCell = m_dataDPCell[i];
			curCell = m_dataDPCell[j];
			preComCell = preCell.getComCell();
			curComCell = curCell.getComCell();

			strPreTexing = preComCell.getRealTexing();
			strCurTexing = curComCell.getRealTexing();

			if (strPreTexing.IsEmpty() || strCurTexing.IsEmpty())//如果有空表示数据不会进行匹配
			{
				continue;
			}
			strPreSheetName = preCell.getSheetName();
			strCurSheetName = curCell.getSheetName();
			//if (strPreSheetName.CompareNoCase(strCurSheetName) != 0)
			{
				if(strPreTexing.CompareNoCase(strCurTexing) == 0)
				{
					strPreQuxiang = preComCell.getQuxiang();
					strCurQuxiang = curComCell.getQuxiang();
					if ((!strPreQuxiang.IsEmpty()) && (!strCurQuxiang.IsEmpty()))//如果都不为空，说明已经找到去向，不用继续寻找
					{
						continue;
					}
					if (strPreQuxiang.IsEmpty())//如果为空
					{
						//strPreQuxiang = curCell.getSheet() + curCell.getNameCell().getRealName() + _T(":") + curComCell.getDuanzi();
						strPreQuxiang = curComCell.getTempQuxiang();
						preComCell.setQuxiang(strPreQuxiang);

						//修改数据
						map<CString, CSingleSheetData>::iterator MIr = m_data.find(preCell.getSheetName());
						if (MIr == m_data.end())
						{
							continue;
						}
						CSingleSheetData mSingleData = MIr->second;
						map<CNameCell, vector<CCommonCell> > mMap = mSingleData.getMapInfo();
						CNameCell mNameCell = preCell.getNameCell();
						map<CNameCell, vector<CCommonCell> >::iterator MItr = mMap.find(mNameCell);
						if (MItr == mMap.end())
						{
							continue;
						}
						vector<CCommonCell> mCom = MItr->second;
						mCom.at(preCell.getIndex()) = preComCell;
						MItr->second = mCom;
						mSingleData.setMapInfo(mMap);
						m_data[preCell.getSheetName()] = mSingleData;


					}
					if (strCurQuxiang.IsEmpty())//如果没有数据
					{
						//strCurQuxiang = preCell.getSheet() + preCell.getNameCell().getRealName() + _T(":") + preComCell.getDuanzi();
						strCurQuxiang = preComCell.getTempQuxiang();
						curComCell.setQuxiang(strCurQuxiang);

						//修改数据
						map<CString, CSingleSheetData>::iterator MIr = m_data.find(curCell.getSheetName());
						if (MIr == m_data.end())
						{
							continue;
						}
						CSingleSheetData mSingleData = MIr->second;
						map<CNameCell, vector<CCommonCell> > mMap = mSingleData.getMapInfo();
						CNameCell mNameCell = curCell.getNameCell();
						map<CNameCell, vector<CCommonCell> >::iterator MItr = mMap.find(mNameCell);
						if (MItr == mMap.end())
						{
							continue;
						}
						vector<CCommonCell> mCom = MItr->second;
						mCom.at(curCell.getIndex()) = curComCell;
						MItr->second = mCom;
						mSingleData.setMapInfo(mMap);
						m_data[curCell.getSheetName()] = mSingleData;
					}
				}
			}
		}
	}
	return true;
}
