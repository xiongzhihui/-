#pragma once

class CDirTreeCtrl : public CTreeCtrl
{
public:
	CDirTreeCtrl(void);
	~CDirTreeCtrl(void);

public:
	BOOL SetSelPath(LPCTSTR strPath);
	CString GetFullPath(HTREEITEM hItem);  //获取全文件名
	CString GetPath(HTREEITEM hItem);
	LPCTSTR GetSubPath(LPCTSTR strPath);       //获取子目录
	BOOL DisPlayTree(LPCTSTR strRoot, BOOL bFile);   //显示数
	BOOL IsValidPath(LPCTSTR strPath);
	void ExpandItem(HTREEITEM hItem, UINT nCode);   //展开项
	
	HTREEITEM SearchSiblingItem(HTREEITEM hItem, LPCTSTR strText);
	BOOL FindSubDir(LPCTSTR strPath);     //查找子目录
	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR strPath); //添加项目
	void DisplayPath(HTREEITEM hParent, LPCTSTR strPath);  //展开目录
	BOOL GetSysImgList();   //获取系统图标
	BOOL DisplayDrives();                //显示磁盘
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);

	void SetParentPath(LPCTSTR strPath);//设置夫路径
	void SetContent(LPCTSTR strContent);
	bool readContent(LPCTSTR strFileName);
	//删除文件夹
	bool DelFolder(CString strPath);
	bool RenameFolder(CString strFromPath, CString strToPath);
	DECLARE_MESSAGE_MAP();


private:
	CImageList m_ImageList;
	CString m_strRoot;       
	BOOL m_bFile;         //文件或目录标志
	CString m_strError;              //错误信息
	CString m_strParent;
	bool m_bFlag;
	CString m_strContent;//文件内容
};
