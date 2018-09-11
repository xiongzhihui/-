#pragma once
#include "CNameCell.h"
class CCommonCell;

typedef vector<CCommonCell> COMVEC;
typedef pair<CCommonCell, CCommonCell> COMPAIR;
typedef map<CString, vector<CCommonCell> > MDMap;
typedef map<pair<int, CString>, vector<CCommonCell> > MRMAP;
typedef map<int, pair<CCommonCell, CCommonCell> > MPMAP;
typedef map<CString, vector<pair<CCommonCell, CCommonCell> > > MVMAP;

class CCommonCell
{
public:
	CCommonCell();
	~CCommonCell();
	void setDuanzi(CString strDuanzi);
	void setTexing(CString strTexing);
	void setQuxiang(CString strQuxiang);
	void setRealTexing(CString strRealQuxiang);
	void setType(int strType);
	void setCount(CString nCount);
	void setModQuxiang(CString strModQuxiang);
	void setTempQuxiang(CString strTempQuxiang);
	void setPreQuxiang(CString strPreQuxiang);//上一个去向
	void setRow(int nRow);
	void setCol(int nCol);
	void setSheetName(CString strSheetName);
	//void setExcelRow(int nExcelRow);
	//void setExcelCol(int nExcelCol);
	//void setNickName(CString strNickName);
	void setName(CString strName);
	void setNameCell(CNameCell nameCell);
	CString getDuanzi() const;
	CString getTexing() const;
	CString getQuxiang() const;
	CString getRealTexing() const;
	CString getChineseRealTexing() const;
	CString getModQuxiang() const;
	CString getTempQuxiang() const;
	CString getPreQuxiang() const;
	int getType() const;
	CString getCount() const;
	int getRow() const;
	int getCol() const;
	CString getSheetName() const;
	//int getExcelRow() const;
	//int getExcelCol() const;
	//CString getNickName() const;
	CString getName() const;
	CNameCell getNameCell() const;

	CCommonCell& operator=(const CCommonCell& node);
	//判定两个comcell是否相同
	bool IsEqual(CCommonCell comCell);

	CString getShortTexing() const;
	CString getModName() const;
	CString getShortName() const;
	CString getWeihao() const;
	CString getRealSheetName() const;
	//获取层级数据
	int getRlevel() const;
	//获取M特性数据
	bool getMFlag() const;
	bool getRFFlag() const;
private:
	CString m_strDuanzi;
	CString m_strTexing;
	CString m_strRealTexing;
	CString m_strQuxiang;
	CString m_strModQuxiang;
	CString m_strTempQuxiang;
	//0为普通数据，1为Dp数据，2为RF数据，3为数字光数据，
	//4为模拟光数据，5为集束光数据，6为R层级数据，
	//7为S数据，8为M数据，9为R+S数据
	//10为R+M数据,11为单光纤数据,12为LRMV，-1为带[D]的数据
	int m_nType;
	
	CString m_strCount;//计数，如果是低频和高频，记住各自对应的数字
	int m_nRow;
	int m_nCol;
	CString m_strSheetName;
	//int m_nExcelRow;
	//int m_nExcelCol;
	//CString m_strNickName;//当有R层级和SM层级的时候
	CString m_strName;//该item对应的名称
	CString m_strPreQuxiang;
	CNameCell m_nameCell;
};