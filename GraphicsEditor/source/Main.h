#pragma once
#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include "../../Exceptions/source/Exceptions.h"
#include "shapes/DefaultShapes.h"
#include "../../PluginSupporter/source/PluginSupporter.h"
#include "core/FileManager.h"
#include "core/PluginManager.h"
#include "core/UserShapeManager.h"

#define WND_CLASS		L"MainWindow"
#define WND_NAME		L"Graphical Editor"
#define WND_WIDTH		1280
#define WND_HEIGHT		720

#define BID_SAVE		0x0007
#define BID_LOAD		0x0008
#define BID_DELETE		0x0009
#define BID_EDIT		0x000A
#define BID_SAVESHAPE	0x000B
#define BID_USERSHAPE	0x000C
#define REG_INDEX_START	0x00A0

#define BC_SAVE			L"Save"
#define BC_LOAD			L"Load"
#define BC_DELETE		L"Delete"
#define BC_EDIT			L"Edit"
#define BC_SAVESHAPE	L"Save as Shape"

#define B_WIDTH			140
#define B_HEIGHT		30

enum ShapeType
{
	CORE,
	PLUGIN,
	USER
};

typedef struct _ShapeRegStruct
{
	std::wstring name;
	ShapeID id;
	ShapeType type;
} ShapeRegStruct;

void RegisterWindowClass();
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitUI(HWND);
void RegisterCoreShapes(WORD& regIndex, ShapesFactory* sf);
void RegisterPluginShapes(WORD& regIndex, const PluginManager* pm, ShapesFactory* sf);
void RegisterUserShapes(WORD& regIndex, const UserShapeManager* usm, ShapesFactory* sf);
void DrawButton(HWND, const wchar_t*, int, int, int, int, WORD);
HWND DrawInput(HWND, int, int, int, int, WORD);
void OnPaint(HWND hWnd);
void AddShape(HWND hWnd, const ShapesFactory* sf);
void AddStretchShape(HWND hWnd, const ShapesFactory* sf);
void SelectNextShape(HWND hWnd);
void DeselectShape(HWND hWnd);