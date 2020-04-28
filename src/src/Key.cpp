#include "stdafx.h"

bool pushKeyState(int state, int DIK)
{
	static int old_diks[KEYMAX] = { OFF };

	if (GetKeyState(state) & 0x8000)
	{
		if (old_diks[DIK] == OFF)
		{
			old_diks[DIK] = ON;
			return true;
		}
	}
	else
	{
		old_diks[DIK] = OFF;
	}
	
	return false;
}
