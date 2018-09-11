#include "StdAfx.h"
#include "SortStringArray.h"

CSortStringArray::CSortStringArray(void)
{
}

CSortStringArray::~CSortStringArray(void)
{
}


void CSortStringArray::Sort()
{
	BOOL bNotDone = TRUE;
	while (bNotDone)
	{
		bNotDone = FALSE;
		for (int pos = 0; pos < GetUpperBound(); pos++)
		{
			bNotDone |= CompareAndSwap(pos);
		}
	}
}

BOOL CSortStringArray::CompareAndSwap(int Pos)
{
	CString temp;
	int posFirst = Pos;
	int posNext = Pos + 1;
	if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)
	{
		temp = GetAt(posFirst);
		SetAt(posFirst, GetAt(posNext));
		SetAt(posNext, temp);
		return TRUE;
	}
	return FALSE;
}

