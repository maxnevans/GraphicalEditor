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
#include "../../libs/pugixml/pugixml.hpp"

// MAIN WINDOW DEFS

#define MAIN_WND_CLASS		L"MainWindow"
#define MAIN_WND_NAME		L"Graphical Editor"
#define MAIN_WND_WIDTH		1280
#define MAIN_WND_HEIGHT		720

#define MW_BID_SAVE			0x0007
#define MW_BID_LOAD			0x0008
#define MW_BID_DELETE		0x0009
#define MW_BID_EDIT			0x000A
#define MW_BID_SAVESHAPE	0x000B
#define MW_BID_USERSHAPE	0x000C
#define MW_REG_INDEX_START	0x00A0

#define MW_AID_SAVEUSHAPE	0x00A1
#define MW_AID_CANCELUSHAPE	0x00A2

#define MW_BC_SAVE			L"Save"
#define MW_BC_LOAD			L"Load"
#define MW_BC_DELETE		L"Delete"
#define MW_BC_EDIT			L"Edit"
#define MW_BC_SAVESHAPE		L"Save as Shape"

#define MW_B_WIDTH			140
#define MW_B_HEIGHT			30


// USER SHAPE WINDOW DEFS

#define US_WND_CLASS		L"UserShapeWindow"
#define US_WND_NAME			L"Save User Shape"
#define US_WND_WIDTH		350
#define US_WND_HEIGHT		180

#define US_BID_SAVE			0x0001
#define US_BID_CANCEL		0x0002

#define US_BC_SAVE			L"Save"
#define US_BC_CANCEL		L"Cancel"

#define US_B_WIDTH			140
#define US_B_HEIGHT			30

#define US_S_WIDTH			140
#define US_S_HEIGHT			30

#define US_SC_SHAPENAME		L"Shape Name"
#define US_SC_FILENAME		L"File Name"


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

HWND CreateUserShapeSaveWindow(HWND hParent);
void RegisterMainWindowClass();
void RegisterUserShapeWindowClass();
LRESULT WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI UserShapeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitUI(HWND);
void RegisterCoreShapes(WORD& regIndex, ShapesFactory* sf);
void RegisterPluginShapes(WORD& regIndex, const PluginManager* pm, ShapesFactory* sf);
void RegisterUserShapes(WORD& regIndex, const UserShapeManager* usm, ShapesFactory* sf);
void DrawButton(HWND, const wchar_t*, int, int, int, int, WORD);
HWND DrawInput(HWND, int, int, int, int, WORD);
void DrawStatic(HWND, std::wstring, int, int, int, int);
void SetInputFilterNumbersOnly(HWND hInput, int amountAllowed);
void OnPaint(HWND hWnd);
void AddShape(HWND hWnd, const ShapesFactory* sf);
void AddStretchShape(HWND hWnd, const ShapesFactory* sf);
void SelectNextShape(HWND hWnd);
void DeselectShape(HWND hWnd);