#pragma once

class CSortStringArray : public CStringArray
{
public:
	CSortStringArray(void);
	~CSortStringArray(void);

	void Sort();
private:
	BOOL CompareAndSwap(int Pos);
};
