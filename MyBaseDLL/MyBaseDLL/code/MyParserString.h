#pragma once
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

#include <vector>
using namespace std;
class SERVERDLL_API MyParserString 
{
public:
	MyParserString(void);
	~MyParserString(void);

	static int GetCount(CString strPile);
	
	//输入strPile 返回strPile前面的字符及字符后的int型大小
	static int GetPileLength(CString strPile, CString& strPrefix);

	static CString GetNameAndStandard(CString strPile, CString& strPrefix);
	//输入strPile,返回strPile前面的字符strPrefix及后面的字符strNext及两个字符中间的数字int型大小
	static int GetPileLengthEx(CString strPile, CString& strPrefix, CString& strNext);
	//根据位数返回字符串
	//nflag=0表示递增
	//1表示递减
	//2表示不变
	static int GetPrePileLengthEx(CString strPile, CString& strPrefix, CString& strNext);

	static CString OutPutStringByNumericalDigit(CString strPile, int nWeishu, int nFlag = 0);
	//将字符转为lineweight
	static AcDb::LineWeight StrToLineWeight(CString& str);

	//解析“、”的字符，将其解析存到一个cstringarray中
	static void GetCommaFromString(CString strInput, CStringArray& strResult);

	//从strPath中找出所有文件名并存到stringarray中
	static void GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, const CString strTypeName, bool bIsFullPath = false);
	//将“（）”里的字符解析出来
	static void GetParenthesesFromString(CString strInput, CStringArray& strResult);

	static CString GetParenthesesFromString(CString strInput);

	//解析“+”字符，将其解析存到一个cstringarray中
	static void GetPlusFromString(CString strInput, CStringArray& strResult);
	//判断字符是否为纯数字
	static bool JudgeStr(CString str);

	static CString SubString(CString string,CString Split,int index);

	static int ConvertLetterToNumble(CString strNumber);

	static CString RemoveSymbol(CString strSymbol, CString strInput);
	static CString RemoveRightSymbol(TCHAR strSymbol, CString strInput);

	static CString GetSymbolString(CString strInput, CString strFirst, CString strEnd);
};
