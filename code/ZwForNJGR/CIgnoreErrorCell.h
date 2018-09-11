#pragma once

//ºöÂÔ´íÎóÐÐÊý¾Ý
class CIgnoreErrorCell
{
public:
	CIgnoreErrorCell(void);
	~CIgnoreErrorCell(void);

	CString sheetName();
	CString row();
	CString col();
	CString texing();
	CString realTexing();
	CString prompt();

	void SetSheetName(CString strSheetName);
	void SetRow(CString strRow);
	void SetCol(CString strCol);
	void setTexing(CString strTexing);
	void setRealTexing(CString strRealTexing);
	void setPrompt(CString strPrompt);

	void set(CString strSheetName, CString strRow, CString strCol, CString strTexing, CString strRealTexing, CString strPrompt);

private:
	CString m_strSheetName;
	CString m_strRow;
	CString m_strCol;
	CString m_strTexing;
	CString m_strRealTexing;
	CString m_strPrompt;
};
