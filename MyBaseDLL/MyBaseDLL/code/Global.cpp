// Global.cpp: implementation of the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "inifile.h"
#include "global.h"

CIniFile m_IniTool;


double Myttof(CString strInput)
{
	double dReturn = 0.0;
#ifdef UNICODE
	dReturn=_tstof(strInput);
#else  
	dReturn=atof(strInput);
#endif
	return dReturn;
}

static int g_nColorTable[] =
	{
		RGB(  0,   0,   0),     // color:  0
		RGB(255,   0,   0),     // color:  1
		RGB(255, 255,   0),     // color:  2
		RGB(  0, 255,   0),     // color:  3
		RGB(  0, 255, 255),     // color:  4
		RGB(  0,   0, 255),     // color:  5
		RGB(255,   0, 255),     // color:  6
		RGB(255, 255, 255),     // color:  7
		RGB(128, 128, 128),     // color:  8
		RGB(192, 192, 192),     // color:  9
		RGB(255,   0,   0),     // color: 10
		RGB(255, 127, 127),     // color: 11
		RGB(204,   0,   0),     // color: 12
		RGB(204, 102, 102),     // color: 13
		RGB(153,   0,   0),     // color: 14
		RGB(153,  76,  76),     // color: 15
		RGB(127,   0,   0),     // color: 16
		RGB(127,  63,  63),     // color: 17
		RGB( 76,   0,   0),     // color: 18
		RGB( 76,  38,  38),     // color: 19
		RGB(255,  63,   0),     // color: 20
		RGB(255, 159, 127),     // color: 21
		RGB(204,  51,   0),     // color: 22
		RGB(204, 127, 102),     // color: 23
		RGB(153,  38,   0),     // color: 24
		RGB(153,  95,  76),     // color: 25
		RGB(127,  31,   0),     // color: 26
		RGB(127,  79,  63),     // color: 27
		RGB( 76,  19,   0),     // color: 28
		RGB( 76,  47,  38),     // color: 29
		RGB(255, 127,   0),     // color: 30
		RGB(255, 191, 127),     // color: 31
		RGB(204, 102,   0),     // color: 32
		RGB(204, 153, 102),     // color: 33
		RGB(153,  76,   0),     // color: 34
		RGB(153, 114,  76),     // color: 35
		RGB(127,  63,   0),     // color: 36
		RGB(127,  95,  63),     // color: 37
		RGB( 76,  38,   0),     // color: 38
		RGB( 76,  57,  38),     // color: 39
		RGB(255, 191,   0),     // color: 40
		RGB(255, 223, 127),     // color: 41
		RGB(204, 153,   0),     // color: 42
		RGB(204, 178, 102),     // color: 43
		RGB(153, 114,   0),     // color: 44
		RGB(153, 133,  76),     // color: 45
		RGB(127,  95,   0),     // color: 46
		RGB(127, 111,  63),     // color: 47
		RGB( 76,  57,   0),     // color: 48
		RGB( 76,  66,  38),     // color: 49
		RGB(255, 255,   0),     // color: 50
		RGB(255, 255, 127),     // color: 51
		RGB(204, 204,   0),     // color: 52
		RGB(204, 204, 102),     // color: 53
		RGB(153, 153,   0),     // color: 54
		RGB(153, 153,  76),     // color: 55
		RGB(127, 127,   0),     // color: 56
		RGB(127, 127,  63),     // color: 57
		RGB( 76,  76,   0),     // color: 58
		RGB( 76,  76,  38),     // color: 59
		RGB(191, 255,   0),     // color: 60
		RGB(223, 255, 127),     // color: 61
		RGB(153, 204,   0),     // color: 62
		RGB(178, 204, 102),     // color: 63
		RGB(114, 153,   0),     // color: 64
		RGB(133, 153,  76),     // color: 65
		RGB( 95, 127,   0),     // color: 66
		RGB(111, 127,  63),     // color: 67
		RGB( 57,  76,   0),     // color: 68
		RGB( 66,  76,  38),     // color: 69
		RGB(127, 255,   0),     // color: 70
		RGB(191, 255, 127),     // color: 71
		RGB(102, 204,   0),     // color: 72
		RGB(153, 204, 102),     // color: 73
		RGB( 76, 153,   0),     // color: 74
		RGB(114, 153,  76),     // color: 75
		RGB( 63, 127,   0),     // color: 76
		RGB( 95, 127,  63),     // color: 77
		RGB( 38,  76,   0),     // color: 78
		RGB( 57,  76,  38),     // color: 79
		RGB( 63, 255,   0),     // color: 80
		RGB(159, 255, 127),     // color: 81
		RGB( 51, 204,   0),     // color: 82
		RGB(127, 204, 102),     // color: 83
		RGB( 38, 153,   0),     // color: 84
		RGB( 95, 153,  76),     // color: 85
		RGB( 31, 127,   0),     // color: 86
		RGB( 79, 127,  63),     // color: 87
		RGB( 19,  76,   0),     // color: 88
		RGB( 47,  76,  38),     // color: 89
		RGB(  0, 255,   0),     // color: 90
		RGB(127, 255, 127),     // color: 91
		RGB(  0, 204,   0),     // color: 92
		RGB(102, 204, 102),     // color: 93
		RGB(  0, 153,   0),     // color: 94
		RGB( 76, 153,  76),     // color: 95
		RGB(  0, 127,   0),     // color: 96
		RGB( 63, 127,  63),     // color: 97
		RGB(  0,  76,   0),     // color: 98
		RGB( 38,  76,  38),     // color: 99
		RGB(  0, 255,  63),     // color:100
		RGB(127, 255, 159),     // color:101
		RGB(  0, 204,  51),     // color:102
		RGB(102, 204, 127),     // color:103
		RGB(  0, 153,  38),     // color:104
		RGB( 76, 153,  95),     // color:105
		RGB(  0, 127,  31),     // color:106
		RGB( 63, 127,  79),     // color:107
		RGB(  0,  76,  19),     // color:108
		RGB( 38,  76,  47),     // color:109
		RGB(  0, 255, 127),     // color:110
		RGB(127, 255, 191),     // color:111
		RGB(  0, 204, 102),     // color:112
		RGB(102, 204, 153),     // color:113
		RGB(  0, 153,  76),     // color:114
		RGB( 76, 153, 114),     // color:115
		RGB(  0, 127,  63),     // color:116
		RGB( 63, 127,  95),     // color:117
		RGB(  0,  76,  38),     // color:118
		RGB( 38,  76,  57),     // color:119
		RGB(  0, 255, 191),     // color:120
		RGB(127, 255, 223),     // color:121
		RGB(  0, 204, 153),     // color:122
		RGB(102, 204, 178),     // color:123
		RGB(  0, 153, 114),     // color:124
		RGB( 76, 153, 133),     // color:125
		RGB(  0, 127,  95),     // color:126
		RGB( 63, 127, 111),     // color:127
		RGB(  0,  76,  57),     // color:128
		RGB( 38,  76,  66),     // color:129
		RGB(  0, 255, 255),     // color:130
		RGB(127, 255, 255),     // color:131
		RGB(  0, 204, 204),     // color:132
		RGB(102, 204, 204),     // color:133
		RGB(  0, 153, 153),     // color:134
		RGB( 76, 153, 153),     // color:135
		RGB(  0, 127, 127),     // color:136
		RGB( 63, 127, 127),     // color:137
		RGB(  0,  76,  76),     // color:138
		RGB( 38,  76,  76),     // color:139
		RGB(  0, 191, 255),     // color:140
		RGB(127, 223, 255),     // color:141
		RGB(  0, 153, 204),     // color:142
		RGB(102, 178, 204),     // color:143
		RGB(  0, 114, 153),     // color:144
		RGB( 76, 133, 153),     // color:145
		RGB(  0,  95, 127),     // color:146
		RGB( 63, 111, 127),     // color:147
		RGB(  0,  57,  76),     // color:148
		RGB( 38,  66,  76),     // color:149
		RGB(  0, 127, 255),     // color:150
		RGB(127, 191, 255),     // color:151
		RGB(  0, 102, 204),     // color:152
		RGB(102, 153, 204),     // color:153
		RGB(  0,  76, 153),     // color:154
		RGB( 76, 114, 153),     // color:155
		RGB(  0,  63, 127),     // color:156
		RGB( 63,  95, 127),     // color:157
		RGB(  0,  38,  76),     // color:158
		RGB( 38,  57,  76),     // color:159
		RGB(  0,  63, 255),     // color:160
		RGB(127, 159, 255),     // color:161
		RGB(  0,  51, 204),     // color:162
		RGB(102, 127, 204),     // color:163
		RGB(  0,  38, 153),     // color:164
		RGB( 76,  95, 153),     // color:165
		RGB(  0,  31, 127),     // color:166
		RGB( 63,  79, 127),     // color:167
		RGB(  0,  19,  76),     // color:168
		RGB( 38,  47,  76),     // color:169
		RGB(  0,   0, 255),     // color:170
		RGB(127, 127, 255),     // color:171
		RGB(  0,   0, 204),     // color:172
		RGB(102, 102, 204),     // color:173
		RGB(  0,   0, 153),     // color:174
		RGB( 76,  76, 153),     // color:175
		RGB(  0,   0, 127),     // color:176
		RGB( 63,  63, 127),     // color:177
		RGB(  0,   0,  76),     // color:178
		RGB( 38,  38,  76),     // color:179
		RGB( 63,   0, 255),     // color:180
		RGB(159, 127, 255),     // color:181
		RGB( 51,   0, 204),     // color:182
		RGB(127, 102, 204),     // color:183
		RGB( 38,   0, 153),     // color:184
		RGB( 95,  76, 153),     // color:185
		RGB( 31,   0, 127),     // color:186
		RGB( 79,  63, 127),     // color:187
		RGB( 19,   0,  76),     // color:188
		RGB( 47,  38,  76),     // color:189
		RGB(127,   0, 255),     // color:190
		RGB(191, 127, 255),     // color:191
		RGB(102,   0, 204),     // color:192
		RGB(153, 102, 204),     // color:193
		RGB( 76,   0, 153),     // color:194
		RGB(114,  76, 153),     // color:195
		RGB( 63,   0, 127),     // color:196
		RGB( 95,  63, 127),     // color:197
		RGB( 38,   0,  76),     // color:198
		RGB( 57,  38,  76),     // color:199
		RGB(191,   0, 255),     // color:200
		RGB(223, 127, 255),     // color:201
		RGB(153,   0, 204),     // color:202
		RGB(178, 102, 204),     // color:203
		RGB(114,   0, 153),     // color:204
		RGB(133,  76, 153),     // color:205
		RGB( 95,   0, 127),     // color:206
		RGB(111,  63, 127),     // color:207
		RGB( 57,   0,  76),     // color:208
		RGB( 66,  38,  76),     // color:209
		RGB(255,   0, 255),     // color:210
		RGB(255, 127, 255),     // color:211
		RGB(204,   0, 204),     // color:212
		RGB(204, 102, 204),     // color:213
		RGB(153,   0, 153),     // color:214
		RGB(153,  76, 153),     // color:215
		RGB(127,   0, 127),     // color:216
		RGB(127,  63, 127),     // color:217
		RGB( 76,   0,  76),     // color:218
		RGB( 76,  38,  76),     // color:219
		RGB(255,   0, 191),     // color:220
		RGB(255, 127, 223),     // color:221
		RGB(204,   0, 153),     // color:222
		RGB(204, 102, 178),     // color:223
		RGB(153,   0, 114),     // color:224
		RGB(153,  76, 133),     // color:225
		RGB(127,   0,  95),     // color:226
		RGB(127,  63, 111),     // color:227
		RGB( 76,   0,  57),     // color:228
		RGB( 76,  38,  66),     // color:229
		RGB(255,   0, 127),     // color:230
		RGB(255, 127, 191),     // color:231
		RGB(204,   0, 102),     // color:232
		RGB(204, 102, 153),     // color:233
		RGB(153,   0,  76),     // color:234
		RGB(153,  76, 114),     // color:235
		RGB(127,   0,  63),     // color:236
		RGB(127,  63,  95),     // color:237
		RGB( 76,   0,  38),     // color:238
		RGB( 76,  38,  57),     // color:239
		RGB(255,   0,  63),     // color:240
		RGB(255, 127, 159),     // color:241
		RGB(204,   0,  51),     // color:242
		RGB(204, 102, 127),     // color:243
		RGB(153,   0,  38),     // color:244
		RGB(153,  76,  95),     // color:245
		RGB(127,   0,  31),     // color:246
		RGB(127,  63,  79),     // color:247
		RGB( 76,   0,  19),     // color:248
		RGB( 76,  38,  47),     // color:249
		RGB( 51,  51,  51),     // color:250
		RGB( 91,  91,  91),     // color:251
		RGB(132, 132, 132),     // color:252
		RGB(173, 173, 173),     // color:253
		RGB(214, 214, 214),     // color:254
		RGB(255, 255, 255)      // color:255
	};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGlobal::CGlobal()
{
}

CGlobal::~CGlobal()
{

}
CGlobal& CGlobal::Get(){
	static CGlobal m_gGlobal;
	return m_gGlobal;
}

//第1种形式 只判断正整数
int CGlobal::Myatoi(CString inputStr)
{
	BOOL isNumeic=TRUE;
	TCHAR temp[256];
	int len=0;
	wsprintf(temp,_T("%s"),inputStr);
	len=_tcslen(temp);
	//1.判断是否纯数字 否的话返回-1
	for(int i=0;i<len;i++)
	{
		if (temp[i]<'0'||temp[i]>'9')
		{
			isNumeic=FALSE;
			break;
		}
	}
	
	//2.是就返回_ttoi
	if(!isNumeic)
		return -1;
	else
		return _ttoi(temp);
}

//第2种形式 可以判断负数
BOOL CGlobal::Myatoi(CString inputStr,int& iresult)
{
	BOOL isNumeic=TRUE;

	TCHAR temp[256];
	int len=0;
	wsprintf(temp,_T("%s"),inputStr);
	len=_tcslen(temp);
	//		int fusigncount=0;

	//1.判断是否纯数字 否的话返回-1
	for(int i=0;i<len;i++)
	{
		if(i==0)	//第1位可以是负号
		{
			if (temp[i]<'0'||temp[i]>'9')
			{
				if(temp[i]=='-')
					;
				else
				{
					isNumeic=FALSE;
					break;
				}
			}
		}
		else
		{
			if (temp[i]<'0'||temp[i]>'9')
			{
				isNumeic=FALSE;
				break;
			}
		}
	}
	
	//	//2.是就返回_ttoi
	//	if(!isNumeic)
	//		return -1;
	//	else
	//		return _ttoi(temp);
	iresult=_ttoi(temp);
	return isNumeic;
}




LPCTSTR CGlobal::GetIniValue(LPCTSTR Key,LPCTSTR name)
{
	return m_IniTool.GetValue(Key,name);
	
}
double CGlobal::GetIniValue(LPCTSTR Key,LPCTSTR name,double)
{
	return m_IniTool.GetValue(Key,name,0);
}

/*************函数定义****************
函数名   : CGlobal::SetIniPath
函数描述 :设置Ini文件路径,同时读取Ini文件 
返回值   : void 
参数1    : LPCTSTR path
最后修改日期：2003-5-15 11:43:02
**************************************/
void CGlobal::SetIni(CString path)
{
	m_IniTool.SetPath(path);
	m_IniTool.Reset();
	m_IniTool.ReadFile();
}
/*************函数定义****************
函数名   : CGlobal::CloseIniFile
函数描述 : 将内容写入文件
返回值   : void 
最后修改日期：2003-5-15 11:43:42
**************************************/
void CGlobal::WriteIniFile()
{
	//return;	//死掉算了!!!!!!!!!!!!!!!955
	 m_IniTool.WriteFile();
}

BOOL CGlobal::SetIniValue(LPCTSTR key, LPCTSTR Name, LPCTSTR value)
{
	return m_IniTool.SetValue(key,Name,value);
}
BOOL CGlobal::SetIniValue(LPCTSTR key, LPCTSTR Name, double value)
{
	return m_IniTool.SetValue(key,Name,value);
}

CString CGlobal::SubString(CString string, CString Split, int index)
{
	return m_IniTool.SubString(string, Split,index);
}

BOOL CGlobal::IsFileExist(CString strFileName)
{
	CFileFind  findFile;
	return findFile.FindFile(strFileName,0);
}


//老的找ini路径的方法
CString CGlobal::GetIniPath()
{

	CString path;
	TCHAR buf[1000];
#ifdef ARX
#ifdef ARX2014
	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("MyBaseDLL.arx")),buf,1000);
#else
	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("MyBaseDLL.dll")),buf,1000);
#endif
#else
	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("MyBaseDll.zrx")),buf,1000);
#endif
	if (pathLength)
	{
		path = buf;
		path = path.Left(path.ReverseFind('\\'));
		path+=_T("\\config.ini");
	}
	else
	{
		path =_T("");
	}

	return path;	
}


/*************函数定义****************
函数名   : CGlobal::GetIniValueNum
函数描述 : 得到Ini文件中key的value数值
返回值   : int 
参数1    : CString key
最后修改日期：2003-5-22 9:49:38
**************************************/
int CGlobal::GetIniValueNum(CString key)
{
	return m_IniTool.GetNumValues(key);
}

/*************函数定义****************
函数名   : CGlobal::GetDicValueNum
函数描述 : 得到字典中key的value数值
返回值   : int 
参数1    : CString Key
最后修改日期：2003-5-22 9:50:29
**************************************/
//int CGlobal::GetDicValueNum(CString Key)
//{
//	return m_DicTool.GetNumValues(Key);
//}
