#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "../DLLSupport.h"

class __dll IDrawable
{
public:
	virtual void Redraw(Gdiplus::Graphics* graphics) = 0;
};

