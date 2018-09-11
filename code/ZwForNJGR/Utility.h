#pragma once
#include "CCommonCell.h"

class CUtility
{
public:
	CUtility(void);
	~CUtility(void);

	static AcDbObjectId getStyleIdByiniFile(CString strHead, CString strNo);
	static AcDbObjectId createBlkRef();
	static AcDbObjectId getContentBlkRef();
	static int getRowByPt(AcGePoint3d pt);
	static int getRowAndColByPt(AcGePoint3d pt, int& nRow, int& nCol);
	static double getAddData(AcGePoint3d pt);
	static CString GetLocalUserDocPath();
	static CString getFileNameByFullName(CString strFullName);	
	//excel及dwg文件路径
	static CString getFilePath();
	//log日志路径
	static CString getLogPath();
	static CString getBmPath();
	static CString getDate();
	static CString getExcelPos(int nRow, int nCol);

	static CString getFrameIniFile();

	static void UpdatePageNo(int nBegin);
	//解析：或:
	//************************************
	// Method:    ParserStringComma
	// FullName:  CUtility::ParserStringComma
	// Access:    public static 
	// Returns:   int 返回：的index
	// Qualifier:
	// Parameter: CString strInput 传入完整数据
	// Parameter: CString & strDuanzi传入端子数据
	// Parameter: CString & strTexing传入特性
	//************************************
	static int ParserStringComma(CString strInput, CString& strTexing);

	//************************************
	// Method:    IsIgnoreErrorFlag
	// FullName:  CUtility::IsIgnoreErrorFlag
	// Access:    public static 
	// Returns:   bool
	// Qualifier: 返回是否忽略数据
	// Parameter: CCommonCell cell
	//************************************
	static bool IsIgnoreErrorFlag(CCommonCell cell);

	//************************************
	// Method:    EditignoreError
	// FullName:  CUtility::EditignoreError
	// Access:    public static 
	// Returns:   void
	// Qualifier:修改需要编辑的忽略数据
	// Parameter: AcDbObjectIdArray objIdArr
	//************************************
	static void EditignoreError(AcDbObjectIdArray objIdArr);
	static void DwgZoomExtent( AcDbDatabase *pDb );
	static AcDbExtents GetModelSpaceExtent( AcDbDatabase *pDb );
	static AcDbObjectIdArray GetAllEntityIds( const TCHAR* layerName, AcDbDatabase *pDb );
	static void ZoomExtent();
	static void GetCurrentView( AcDbViewTableRecord &view );
	static AcGePoint3d WcsToDcsPoint( const AcGePoint3d &point );
	static CString RemoveString(CString strInput, CString strTemp);
	static BOOL CreateDirectoryWithUserFullControlACL(LPCTSTR lpPath);

	//************************************
	// Method:    getIniFromFile
	// FullName:  CUtility::getIniFromFile
	// Access:    public static 
	// Returns:   bool
	// Qualifier:获取图框设置信息的信息
	// Parameter: CString strProperty
	// Parameter: double & dX
	// Parameter: double & dY
	// Parameter: double & dSize
	//************************************
	static void getIniFromFile(CString strProperty, double& dX, double& dY, double& dSize);

	//************************************
	// Method:    ParserStringAndGetStartAndEnd
	// FullName:  CUtility::ParserStringAndGetStartAndEnd
	// Access:    public static 
	// Returns:   void
	// Qualifier:传入字符串，返回起始和终止
	// Parameter: CString strInput
	// Parameter: int & nStart
	// Parameter: int & nEnd
	//************************************
	static bool ParserStringAndGetStartAndEnd(CString strInput, int& nStart, int& nEnd);

	static void doIt();
	static void preView();
	static void UpdatePageNo();
	//验证是否dp和rf的一一匹配关系
	static bool verifyOther();
};
