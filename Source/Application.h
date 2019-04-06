#pragma once
#include "Exception.h"
#include "ShapesFactory.h"
#include "ListShapes.h"
#include "FileManager.h"
#include "ICursor.h"


class Application
{
private:
	typedef struct _Point
	{
		int X;
		int Y;
	} Point2D, *PPoint2D;
public:
	Application();
	~Application();
	void SetFirstPoint(int x, int y);
	void SetSecondPoint(int x, int y);
	void ClearSelection();
	void MoveSelection(int x, int y);
	void SelectTool(const wchar_t* toolName);
	void SaveProjectToText(const wchar_t* fileName);
	void LoadProjectFromText(const wchar_t* fileName);
	
private:
	ListShapes* shapes;
	const wchar_t* currentTool;
	ICursor* cursor;
	Point2D points[2];
};

