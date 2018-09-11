#include "StdAfx.h"
#include "CCommonCell.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CCommonCell::CCommonCell()
{
	m_strDuanzi = _T("");
	m_strTexing = _T("");
	m_strQuxiang = _T("");
	m_strModQuxiang = _T("");
	m_nType = -1;
	m_strCount = _T("");
	m_nRow = 0;
	m_nCol	= 0;
	m_strSheetName = _T("");
	m_strRealTexing = _T("");
	m_strPreQuxiang = _T("");
	m_strTempQuxiang = _T("");
}

CCommonCell::~CCommonCell()
{

}

void CCommonCell::setDuanzi(CString strDuanzi)
{
	m_strDuanzi = strDuanzi;
}

void CCommonCell::setTexing(CString strTexing)
{
	m_strTexing = strTexing;
}

void CCommonCell::setQuxiang(CString strQuxiang)
{
	m_strQuxiang = strQuxiang;
}

void CCommonCell::setRealTexing(CString strRealQuxiang)
{
	m_strRealTexing = strRealQuxiang;
}

void CCommonCell::setType(int strType)
{
	m_nType = strType;
}

void CCommonCell::setCount(CString nCount)
{
	m_strCount = nCount;
}

void CCommonCell::setModQuxiang(CString strModQuxiang)
{
	m_strModQuxiang = strModQuxiang;
}

void CCommonCell::setTempQuxiang(CString strTempQuxiang)
{
	m_strTempQuxiang = strTempQuxiang;
}

void CCommonCell::setPreQuxiang(CString strPreQuxiang)
{
	m_strPreQuxiang = strPreQuxiang;
}

void CCommonCell::setRow(int nRow)
{
	m_nRow = nRow;
}

void CCommonCell::setCol(int nCol)
{
	m_nCol = nCol;
}

void CCommonCell::setSheetName(CString strSheetName)
{
	m_strSheetName = strSheetName;
}



void CCommonCell::setName(CString strName)
{
	m_strName = strName;
}

void CCommonCell::setNameCell(CNameCell nameCell)
{
	m_nameCell = nameCell;
}

CString CCommonCell::getDuanzi() const
{
	return m_strDuanzi;
}

CString CCommonCell::getTexing() const
{
	return m_strTexing;
}

CString CCommonCell::getQuxiang() const
{
	return m_strQuxiang;
}

CString CCommonCell::getRealTexing() const
{
	return m_strRealTexing;
}

CString CCommonCell::getModQuxiang() const
{
	return m_strModQuxiang;
}

CString CCommonCell::getTempQuxiang() const
{
	return m_strTempQuxiang;
}

CString CCommonCell::getPreQuxiang() const
{	
	return m_strPreQuxiang;
}

int CCommonCell::getType() const
{
	return m_nType;
}

CString CCommonCell::getCount() const
{
	return m_strCount;
}

int CCommonCell::getRow() const
{
	return m_nRow;
}

int CCommonCell::getCol() const
{
	return m_nCol;
}

CString CCommonCell::getSheetName() const
{
	return m_strSheetName;
}

CString CCommonCell::getName() const
{
	return m_strName;
}

CNameCell CCommonCell::getNameCell() const
{
	return m_nameCell;
}

bool CCommonCell::IsEqual(CCommonCell comCell)
{
	if ((this->getSheetName().CompareNoCase(comCell.getSheetName()) == 0)
		&&(this->getRow() == comCell.getRow())
		&&(this->getCol() == comCell.getCol()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

CString CCommonCell::getShortTexing() const
{
	CString strTmp;
	int nFind = m_strTexing.ReverseFind(_T(']'));
	strTmp = m_strTexing.Right(m_strTexing.GetLength() - nFind - 1);
	return strTmp;
}

CString CCommonCell::getModName() const
{
	CString strPre,strFix, strNextFix, strEnd, strOut, strNum;

	int nFind = m_strSheetName.Find(_T("("));
	if (nFind>=0)
	{
		strPre = m_strSheetName.Left(nFind);
		strEnd = m_strSheetName.Right(m_strSheetName.GetLength() - nFind);
		if (!strPre.IsEmpty())
		{
			strEnd = strEnd.Mid(1, strEnd.GetLength() - 2);
		}
		else
		{
			return strPre;
		}
	}
	
	return strEnd;
}

CCommonCell& CCommonCell::operator=(const CCommonCell& node)
{
	m_strDuanzi = node.getDuanzi();
	m_strTexing = node.getTexing();
	m_strQuxiang = node.getQuxiang();
	m_strModQuxiang = node.getModQuxiang();
	m_nType = node.getType();
	m_strCount = node.getCount();
	m_nRow = node.getRow();
	m_nCol = node.getCol();
	m_strSheetName = node.getSheetName();
	m_strRealTexing = node.getRealTexing();
	m_strTempQuxiang = node.getTempQuxiang();
	m_strName = node.getName();
	m_strPreQuxiang = node.getPreQuxiang();

	m_nameCell = node.getNameCell();
	return *this;
}

CString CCommonCell::getShortName() const
{
	CString strTmp;
	int nFind = m_strName.ReverseFind(_T(']'));
	if (nFind >= 0)
	{
		strTmp = m_strName.Right(m_strName.GetLength() - nFind - 1);
	}
	else
	{
		strTmp = m_strName;
	}
	//再去除（数据
	nFind = strTmp.Find(_T('('));
	if (nFind >= 0)
	{
		strTmp = strTmp.Left(nFind);
	}
	return strTmp;
}

CString CCommonCell::getWeihao() const
{
	CString strOut;
	CString strTemp = getTempQuxiang();
	if (strTemp.IsEmpty())
	{
		return strTemp;
	}
	else
	{
		int nFind = strTemp.Find(_T(":"));
		if (nFind > 0)
		{
			strOut = strTemp.Left(nFind);
			return strOut;
		}
		else
		{
			return strTemp;
		}
	}	
}

CString CCommonCell::getRealSheetName() const
{
	CString strSheet;
	int nFind = m_strSheetName.Find(_T("("));
	if (nFind >=0)
	{
		strSheet = m_strSheetName.Left(nFind);
	}
	else
	{
		strSheet = m_strSheetName;
	}
	return strSheet;
}

int CCommonCell::getRlevel() const
{
	int nTmp1,nTmp2,nLen;
	CString strTmp;

	nTmp2 = m_strTexing.Find(ICD_PROPERTY_R);
	if (nTmp2 >= 0)
	{
		nTmp1 = m_strTexing.Find(_T("]"), nTmp2);
		strTmp = m_strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
		nLen = MyTransFunc::StringToInt(strTmp);
	}
	else
	{
		nLen = 0;
	}
	return nLen;
}

bool CCommonCell::getMFlag() const
{
	bool bIsMFlag = true;
	int nFind = 0;
	nFind = m_strTexing.Find(ICD_PROPERTY_M);
	if (nFind >= 0)
	{
		bIsMFlag = true;
	}
	else
	{
		bIsMFlag = false;
	}
	return bIsMFlag;
}

CString CCommonCell::getChineseRealTexing() const
{
	CString strOut;
	int nFind = m_strTexing.Find(_T("[CH="));
	if (nFind >= 0)
	{
		int nTemp = m_strTexing.Find(_T("]"), nFind);
		strOut = m_strTexing.Mid(nFind+4, nTemp - nFind - 4);
	}
	else
	{
		strOut = m_strRealTexing;
	}
	return strOut;
}

bool CCommonCell::getRFFlag() const
{
	bool bIsRFFlag = true;
	int nFind = 0;
	nFind = m_strTexing.Find(ICD_PROPERTY_RF);
	if (nFind >= 0)
	{
		bIsRFFlag = true;
	}
	else
	{
		bIsRFFlag = false;
	}
	return bIsRFFlag;
}
