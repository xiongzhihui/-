#pragma once

#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

//所有与符号表操作相关的
class SERVERDLL_API MySymble
{
public:
	MySymble(void);
	~MySymble(void);

	//@param strlayer为图层名
	//@return 通过图层名找图层Id
	static AcDbObjectId GetLayer(CString strlayer);
	//@param strLayer为图层名
	//@param nColor为索引色
	//@param bIsModColor为表示是否修改存在的strLayer的颜色信息
	//@param strLineTypeName图层的线型信息默认为Continouse
	//@return 返回图层Id
	static AcDbObjectId CreateNewLayer(CString strLayer,
								int nColor = 7, 
								BOOL bIsModColor = FALSE, 
								CString strLineTypeName = _T("Continuous"),
								AcDb::LineWeight Lweight = AcDb::kLnWtByLwDefault);

	//@param stylename为字体名称
	//@param strFont为小字体名
	//@param strBigFont为大字体名称
	//@param dTextS为字体比例
	//@param dTextSize为字体高度
	//@param bIsModified为是否修改
	//@return 返回字体Id
	static AcDbObjectId CreateTextStyle(CString stylename,
									CString strFont,
									CString strBigfont,
									double dTextS = 1.0, 
									double dTextSize = 3.0, 
									BOOL bIsModified = FALSE);

	//@param ObjId为字体Id
	//@param dTextScale为返回的该字体的比例
	//@param dTextSize为返回的该字体的高度
	//@return 返回函数状态
	static Acad::ErrorStatus GetTextStyleInfo(AcDbObjectId ObjId, double& dTextScale, double& dTextSize);

	//@param strLtName为线型名
	//@param bIsModified为是否修改
	//@return为返回线型Id
	static AcDbObjectId CreateLineType(CString strLtName, BOOL bIsModified = FALSE);

	//@return 返回所有字体样式名
	static void GetAllTextStyleName(CStringArray& StrArr);


	/////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ACAD SYMBOL TABLE UTILITIES //////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	//@param symbolId为符号表ID
	//@param 返回symbol name
	static Acad::ErrorStatus    symbolIdToName(const AcDbObjectId& symbolId, CString& name);
	//@param mlineStyleId为多线的id
	//@param name为输出值，为多线样式名
	static Acad::ErrorStatus    mlineStyleIdToName(const AcDbObjectId& mlineStyleId, CString& name);
	static Acad::ErrorStatus    nameToSymbolId(AcRxClass* symTblClass, LPCTSTR name,
		AcDbObjectId& symbolId, AcDbDatabase* db);

	static bool                 symbolExists(AcRxClass* symTblClass, LPCTSTR name, AcDbDatabase* db);
	static AcDbSymbolTable*     openSymbolTable(AcRxClass* classType, AcDb::OpenMode mode, AcDbDatabase* db);
	static Acad::ErrorStatus    collectSymbolIds(AcRxClass* symTblClass, AcDbObjectIdArray& objIds, AcDbDatabase* db);
	//static Acad::ErrorStatus	collectBlockIds(SdStrObjIdList& list, bool excludeMsPs,
	//	bool excludeXref,
	//	bool excludeAnonymous, AcDbDatabase* db);

	static Acad::ErrorStatus    addNewSymbolAndClose(AcDbSymbolTable* symTbl, AcDbSymbolTableRecord* newRecord);

	static Acad::ErrorStatus    addNewLayer(LPCTSTR layerName, AcDbDatabase* db);

	static Acad::ErrorStatus    defineNewBlock(LPCTSTR blkName, AcDbBlockTableRecord*& newBlkRec,
		AcDbObjectId& newBlkRecId, AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewBlock(LPCTSTR blkName, AcDbBlockTableRecord*& newBlkRec,
		AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewBlock(LPCTSTR blkName, AcDbObjectId& newBlkRecId,
		AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewAnonymousBlock(AcDbBlockTableRecord*& newBlkRec,
		AcDbObjectId& newBlkRecId, AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewAnonymousBlock(AcDbBlockTableRecord*& newBlkRec,
		AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewAnonymousBlock(AcDbObjectId& newBlkRecId,
		AcDbDatabase* db);
	//@Description 通过块定义创建块表记录
	static Acad::ErrorStatus        openBlockDef(LPCTSTR blkName, AcDbBlockTableRecord*& blkRec,
		AcDb::OpenMode mode, AcDbDatabase* db);
	//
	static AcDbObjectId CreateDimstyle(CString strDimName, double dheight = 2.5, double dDimlfac = 1.0,
		bool bDimTih = false, bool bDimToh = false);

};
