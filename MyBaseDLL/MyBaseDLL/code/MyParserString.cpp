#include "StdAfx.h"
#include "MyParserString.h"
#include "MyBaseUtils.h"

MyParserString::MyParserString(void)
{
}

MyParserString::~MyParserString(void)
{
}


int MyParserString::GetCount(CString strPile)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	for (int i=nCount-1; i>=0; i--)
	{
		if(!isdigit(strPile[i]))//主序号是数字的情况
			break;
		++nNum;
	}
	CString strLast;
	strLast = strPile.Right(nNum);
	nlen = _tstoi(strLast);

	return nlen;
}

int MyParserString::GetPileLength(CString strPile, CString& strPrefix)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	for (int i=nCount-1; i>=0; i--)
	{
		if(!isdigit(strPile[i]))//主序号是数字的情况
			break;
		++nNum;
	}
	CString strLast;
	strLast = strPile.Right(nNum);
#ifdef UNICODE
	nlen = _tstoi(strLast);
#else
	nlen = atoi(strLast);
#endif
	strPrefix = strPile.Left(nCount-nNum);
	return nlen;
}


CString MyParserString::GetNameAndStandard(CString strPile, CString& strPrefix)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	for (int i=0; i<nCount; i++)
	{
		if (strPile[i] < 255)
		{
			break;
		}
		++nNum;
	}
	CString strLast;
	strPrefix = strPile.Left(nNum);

	strLast = strPile.Right(nCount-nNum);
	strLast.Trim();
	return strLast;
}

int MyParserString::GetPileLengthEx(CString strPile, CString& strPrefix, CString& strNext)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	int nNumJ = 0;
	for (int i=nCount-1; i>=0; i--)
	{
		if(!isdigit(strPile[i]))//主序号是数字的情况
		{
			if (nNum>0)
			{
				break;
			}
			++nNumJ;
			//break;
		}
		else
		{
			if (nNumJ>0)
			{
				break;
			}
			++nNum;
		}
	}
	strNext = strPile.Right(nNumJ);

	if (nNumJ>0)
	{
		strPile = strPile.Left(nCount - nNumJ);
		int ntmpCount = strPile.GetLength();
		for (int i=ntmpCount-1; i>=0; i--)
		{
			if(!isdigit(strPile[i]))//主序号是数字的情况
				break;
			++nNum;
		}
	}

	CString strLast;
	strLast = strPile.Right(nNum);
#ifdef UNICODE
	nlen = _tstoi(strLast);
#else
	nlen = atoi(strLast);
#endif
	strPrefix = strPile.Left(nCount-nNum-nNumJ);
	return nlen;
}

int MyParserString::GetPrePileLengthEx(CString strPile, CString& strPrefix, CString& strNext)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	int nNumJ = 0;
	for (int i=0; i<nCount; i++)
	{
		if(!isdigit(strPile[i]))//主序号是数字的情况
		{
			if (nNum>0)
			{
				break;
			}
			++nNumJ;
			//break;
		}
		else
		{
			if (nNumJ>0)
			{
				break;
			}
			++nNum;
		}
	}
	strPrefix = strPile.Left(nNumJ);

	if (nNumJ>0)
	{
		strPile = strPile.Right(nCount - nNumJ);
		int ntmpCount = strPile.GetLength();
		for (int i=0; i<ntmpCount; i++)
		{
			if(!isdigit(strPile[i]))//主序号是数字的情况
				break;
			++nNum;
		}
	}

	CString strLast;
	strLast = strPile.Left(nNum);
#ifdef UNICODE
	nlen = _tstoi(strLast);
#else
	nlen = atoi(strLast);
#endif
	strNext = strPile.Right(nCount-nNum-nNumJ);
	return nlen;
}

AcDb::LineWeight MyParserString::StrToLineWeight(CString& str)
{
	AcDb::LineWeight type;
	double dLweight = 0.0;
#ifdef UNICODE
	dLweight = _tcstod(str, 0);
#else
	dLweight = atof(str);
#endif

	if ((dLweight>=0)&&(dLweight<0.05))
	{
		type = AcDb::kLnWt000;
	}
	else if ((dLweight>=0.05)&&(dLweight<0.09))
	{
		type = AcDb::kLnWt005;
	}
	else if ((dLweight>=0.09)&&(dLweight<0.13))
	{
		type = AcDb::kLnWt009;
	}
	else if ((dLweight>=0.13)&&(dLweight<0.15))
	{
		type = AcDb::kLnWt013;
	}
	else if ((dLweight>=0.15)&&(dLweight<0.18))
	{
		type = AcDb::kLnWt015;
	}
	else if ((dLweight>=0.18)&&(dLweight<0.20))
	{
		type = AcDb::kLnWt018;
	}
	else if ((dLweight>=0.20)&&(dLweight<0.25))
	{
		type = AcDb::kLnWt020;
	}
	else if ((dLweight>=0.25)&&(dLweight<0.30))
	{
		type = AcDb::kLnWt025;
	}
	else if ((dLweight>=0.30)&&(dLweight<0.35))
	{
		type = AcDb::kLnWt030;
	}
	else if ((dLweight>=0.35)&&(dLweight<0.40))
	{
		type = AcDb::kLnWt035;
	}
	else if ((dLweight>=0.40)&&(dLweight<0.50))
	{
		type = AcDb::kLnWt040;
	}
	else if ((dLweight>=0.50)&&(dLweight<0.53))
	{
		type = AcDb::kLnWt050;
	}
	else if ((dLweight>=0.53)&&(dLweight<0.60))
	{
		type = AcDb::kLnWt053;
	}
	else if ((dLweight>=0.60)&&(dLweight<0.70))
	{
		type = AcDb::kLnWt060;
	}
	else if ((dLweight>=0.70)&&(dLweight<0.80))
	{
		type = AcDb::kLnWt070;
	}
	else if ((dLweight>=0.80)&&(dLweight<0.90))
	{
		type = AcDb::kLnWt080;
	}
	else if ((dLweight>=0.90)&&(dLweight<1.00))
	{
		type = AcDb::kLnWt090;
	}
	else if ((dLweight>=1.00)&&(dLweight<1.06))
	{
		type = AcDb::kLnWt100;
	}
	else if ((dLweight>=1.06)&&(dLweight<1.20))
	{
		type = AcDb::kLnWt106;
	}
	else if ((dLweight>=1.20)&&(dLweight<1.40))
	{
		type = AcDb::kLnWt120;
	}
	else if ((dLweight>=1.40)&&(dLweight<1.58))
	{
		type = AcDb::kLnWt140;
	}
	else if ((dLweight>=1.58)&&(dLweight<2.00))
	{
		type = AcDb::kLnWt158;
	}
	else if ((dLweight>=2.00)&&(dLweight<2.11))
	{
		type = AcDb::kLnWt200;
	}
	else if (dLweight>=2.11)
	{
		type = AcDb::kLnWt211;
	}
	else
	{
		type = AcDb::kLnWtByLayer;
	}
	return type;
}

void MyParserString::GetCommaFromString(CString strInput, CStringArray& strResultArr)
{
	strResultArr.RemoveAll();
	int nCount = strInput.Find(_T("、"));
	int nLength;
	if (nCount==-1)
	{
		strResultArr.Add(strInput);
		return;
	}
	CString strPrefix, strNext;
	strNext = strInput;
	CString strTmp;
	while(nCount!=-1)
	{
		strPrefix = strNext.Left(nCount);
		strResultArr.Add(strPrefix);
		nLength = strNext.GetLength();
#ifdef UNICODE
		strTmp = strNext.Right(nLength - nCount - 1);
#else
		strTmp = strNext.Right(nLength - nCount - 2);
#endif
		strNext = strTmp;
		nCount = strNext.Find(_T("、"));
	}
	strResultArr.Add(strNext);
	return;
}

void MyParserString::GetPlusFromString(CString strInput, CStringArray& strResultArr)
{
	strResultArr.RemoveAll();
	int nCount = strInput.Find(_T("+"));
	int nLength;
	if (nCount==-1)
	{
		strResultArr.Add(strInput);
		return;
	}
	CString strPrefix, strNext;
	strNext = strInput;
	CString strTmp;
	while(nCount!=-1)
	{
		strPrefix = strNext.Left(nCount);
		strResultArr.Add(strPrefix);
		nLength = strNext.GetLength();
		strTmp = strNext.Right(nLength - nCount - 1);
		strNext = strTmp;
		nCount = strNext.Find(_T("+"));
	}
	strResultArr.Add(strNext);
	return;
}

void MyParserString::GetParenthesesFromString(CString strInput, CStringArray& strResultArr)
{
	strResultArr.RemoveAll();
	CString strOutput(_T(""));
	int nCountLeft = strInput.Find(_T("("));
	int nCountRight = strInput.Find(_T(")"));
	while(nCountRight>nCountLeft)
	{
		strOutput = strInput.Mid(nCountLeft+1, nCountRight - nCountLeft - 1);
		strResultArr.Add(strOutput);
		strInput = strInput.Mid(nCountRight+1);
		nCountLeft = strInput.Find(_T("("));
		nCountRight = strInput.Find(_T(")"));
	}	
	return;
}
CString MyParserString::GetParenthesesFromString(CString strInput)
{
	CString strOutput(_T(""));
	int nCountLeft = strInput.Find(_T("("));
	int nCountRight = strInput.Find(_T(")"));
	if (nCountRight<nCountLeft)
	{
		return strOutput;
	}
	strOutput = strInput.Mid(nCountLeft+1, nCountRight - nCountLeft - 1);
	return strOutput;
}
void MyParserString::GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, const CString strTypeName, bool bIsFullPath)
{
	CFileFind ff;
	BOOL b = FALSE;
	b = ff.FindFile(strPath);

	BOOL bFlgLast = FALSE; //标志着遍历的最后一个文件
	if(b)
	{
		bFlgLast = FALSE;
	}
	else
	{
		bFlgLast = TRUE;
	}

	while(b || !bFlgLast)
	{
		if (!b)
		{
			bFlgLast = TRUE;
		}	
		if(!ff.IsDirectory()&&!ff.IsDots())
		{
			CString strFilename;
			strFilename = ff.GetFilePath();
			strFilename.MakeUpper();
			if(strFilename.Find(strTypeName) != -1)
			{
				if (bIsFullPath)
				{
					strFileNameArr.Add(strFilename);
				}
				else
				{
					CString strtemp1;
					strtemp1 = strFilename.Right(strFilename.GetLength() - strFilename.ReverseFind('\\')-1);	
					CString strTemp2;
					strTemp2=strtemp1.Left(strtemp1.GetLength() - 4);
					strFileNameArr.Add(strTemp2);// strtemp1);
				}
			}			
		}		
		if(!bFlgLast)
		{
			b = ff.FindNextFile();
		}
	}
	ff.Close();
}

CString MyParserString::OutPutStringByNumericalDigit(CString strPile, int nWeishu, int nFlag)
{
	CString strPrefix, strNext, strNum;
	int nlen = MyParserString::GetPileLengthEx(strPile, strPrefix, strNext);
	if (nFlag == 0)
	{	
		nlen++;	
	}
	else if (nFlag == 1)
	{
		nlen--;
	}

	if (nWeishu == 1)
	{
		strNum.Format(_T("%d"), nlen);
	}
	else if (nWeishu == 2)
	{
		if ((nlen<100)&&(nlen>=10) )
		{
			strNum.Format(_T("%d"), nlen);
		}
		else
		{
			strNum.Format(_T("0%d"), nlen);
		}
	}
	else
	{
		if ((nlen<1000)&&(nlen>=100))
		{
			strNum.Format(_T("%d"), nlen);
		}
		else if ((nlen<100)&&(nlen>=10))
		{
			strNum.Format(_T("0%d"), nlen);
		}
		else
		{
			strNum.Format(_T("00%d"), nlen);
		}
	}
	strPile = strPrefix + strNum + strNext;
	return strPile;
}

bool MyParserString::JudgeStr(CString str)
{
	bool bIsShuzi = false;
	int nCount = str.GetLength();
	for (int i=nCount-1; i>=0; i--)
	{
		if(isdigit(str[i]))//主序号是数字的情况
		{
			bIsShuzi = true;
		}
		else
		{
			CString strTmp = str[i];
			if (strTmp.CompareNoCase(_T("."))==0)
			{
				bIsShuzi = true;
			}
			else
			{
				bIsShuzi = false;
				break;
			}
		}
	}
	return bIsShuzi;
}

CString MyParserString::SubString(CString string,CString Split,int index)
{
	int Num=string.Find(Split);
	if(Num==-1) return _T("");
	if(index==0)
		return string.Left(Num);
	int i=1;
	CString retStr=_T("");
	int nCount = 0;
#ifdef UNICODE
	nCount = 0;
#else
	nCount = 1;
#endif
	while(i<index)
	{
		Num=string.Find(Split,Num+1+nCount);
		if(Num==-1) return _T("");
		i++;
	}
	int Num2=string.Find(Split,Num+1+nCount);
	if(Num2==-1){
		retStr=string.Mid(Num+1+nCount);
	}
	else
	{
		retStr=string.Mid(Num+1+nCount,Num2-Num-1-nCount);

	}
	return retStr;
}

int MyParserString::ConvertLetterToNumble(CString s)
{
	int ans = 0;  
	if(s.GetLength() == 0) 
		return ans;  

	for(int i = 0; i < s.GetLength(); i++)  
	{  
		ans = (s[i] - 'A' + 1) + ans * 26;  
	}  

	return ans;  
}

CString MyParserString::RemoveSymbol(CString strSymbol, CString strInput)
{
	CString strOut;
	int nFind = strInput.Find(strSymbol);	
	int nLen = strInput.GetLength();

	while (nFind >= 0)
	{
		strInput = strInput.Right(nLen - nFind - 1);
		nFind = strInput.Find(strSymbol);	
		nLen = strInput.GetLength();
	}
	strOut = strInput;

	return strOut;
}

CString MyParserString::RemoveRightSymbol(TCHAR strSymbol, CString strInput)
{
	CString strOut;
	int nFind = strInput.ReverseFind(strSymbol);	
	int nLen = strInput.GetLength();

	while (nFind >= 0)
	{
		strInput = strInput.Left(nFind);
		nFind = strInput.ReverseFind(strSymbol);	
		nLen = strInput.GetLength();
	}
	strOut = strInput;

	return strOut;
}

CString MyParserString::GetSymbolString(CString strInput, CString strFirst, CString strEnd)
{
	CString strOut;
	int nFirst = strInput.Find(strFirst);
	int nEnd = strInput.Find(strEnd);
	int nLength = strInput.GetLength();
	if (nFirst > nEnd)
	{
		return strOut;
	}
	else
	{
		strOut = strInput.Mid(nFirst+1, nEnd - nFirst - 1);
		return strOut;
	}
}
