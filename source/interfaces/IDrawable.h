#pragma once
#include <windows.h>
#include <gdiplus.h>

class IDrawable
{
public:
	virtual void Redraw(Gdiplus::Graphics* graphics) = 0;
};

