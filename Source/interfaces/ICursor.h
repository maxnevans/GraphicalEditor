#pragma once

class ICursor
{
public:
	virtual void SelectArea(int x1, int y1, int x2, int y2) = 0;
	virtual void SelectPoint(int x, int y) = 0;
};

