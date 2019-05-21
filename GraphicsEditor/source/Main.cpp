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

struct ShapeRegStruct 
{
	std::wstring name;
	ShapeID id;
	ShapeType type;
};

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

std::map<WORD, ShapeRegStruct> registeredShapes;
std::vector<BaseShape*> vecShapes;
size_t selectedShapeIndex;
ShapeID currentShapeID;
ShapesFactory sf;
PluginManager* pm;
UserShapeManager* usm;
Gdiplus::Point points[2];
BaseShape* stretchShape;
HWND hInputX, hInputY, hInputWidth, hInputHeight;

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	try
	{
		Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
		ULONG_PTR						gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(wcex);
		wcex.hInstance = hInstance;
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wcex.lpszClassName = WND_CLASS;

		DWORD retVal = RegisterClassEx(&wcex);
		if (!retVal) 
			throw WinException(L"register window");

		RECT rDesk;
		GetWindowRect(GetDesktopWindow(), &rDesk);

		HWND hWnd = CreateWindowW(WND_CLASS, WND_NAME, 
			WS_VISIBLE | WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX | WS_CLIPCHILDREN,
			(rDesk.right - WND_WIDTH) / 2, (rDesk.bottom - WND_HEIGHT) / 2,
			WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, 0);
		if (hWnd == NULL) 
			throw WinException(L"create window");
		UpdateWindow(hWnd);

		MSG msg = { 0 };

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Gdiplus::GdiplusShutdown(gdiplusToken);
		return static_cast<int>(msg.wParam);
	}
	catch (Exception& error)
	{
		MessageBox(NULL, error.what(), L"Fatal error", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	catch (...)
	{
		MessageBox(NULL, L"Undefined error!", L"Undefined error", MB_OK | MB_ICONERROR);
		exit(-1);
	}
}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch (uMsg)
		{
		case WM_CREATE:
			{
				WORD regIndex = REG_INDEX_START;
				RegisterCoreShapes(regIndex, &sf);
				try
				{
					pm = new PluginManager(L"plugins");
					RegisterPluginShapes(regIndex, pm, &sf);
				}
				catch (Exception&)
				{
					delete pm;
					pm = nullptr;
				}

				try
				{
					usm = new UserShapeManager(&sf, L"user");
					RegisterUserShapes(regIndex, usm, &sf);
				}
				catch (Exception&)
				{
					delete usm;
					usm = nullptr;
				}
				InitUI(hWnd);
				currentShapeID = registeredShapes[0xA0].id;
				points[0].X = -1;
				points[0].Y = -1;
				points[1].X = -1;
				points[1].Y = -1;
				stretchShape = nullptr;
				selectedShapeIndex = -1;
			}
			break;
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_ERASEBKGND:
			return TRUE;
		case WM_LBUTTONDOWN:
			points[0].X = GET_X_LPARAM(lParam);
			points[0].Y = GET_Y_LPARAM(lParam);
			AddStretchShape(hWnd, &sf);
			break;
		case WM_LBUTTONUP:
			points[1].X = GET_X_LPARAM(lParam);
			points[1].Y = GET_Y_LPARAM(lParam);
			AddShape(hWnd, &sf);
			InvalidateRect(hWnd, NULL, FALSE);

			points[0].X = -1;
			points[0].Y = -1;
			points[1].X = -1;
			points[1].Y = -1;
			delete stretchShape;
			stretchShape = nullptr;
			break;
		case WM_MOUSEMOVE:
			if (points[0].X != -1 && points[0].Y)
			{
				points[1].X = GET_X_LPARAM(lParam);
				points[1].Y = GET_Y_LPARAM(lParam);
				stretchShape->SetPoints(points[0].X, points[0].Y, points[1].X, points[1].Y);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case BID_SAVE:
				DeselectShape(hWnd);
				FileManager::SaveText(&vecShapes, L"test.txt");
				SetFocus(hWnd);
				break;
			case BID_LOAD:
				DeselectShape(hWnd);
				vecShapes.clear();
				FileManager::LoadText(&sf, &vecShapes, L"test.txt");
				InvalidateRect(hWnd, NULL, FALSE);
				SetFocus(hWnd);
				break;
			case BID_DELETE:
				if (selectedShapeIndex != -1)
				{
					size_t tempIndex = selectedShapeIndex;
					DeselectShape(hWnd);
					std::vector<BaseShape*> temp;
					for (unsigned int i = 0; i < vecShapes.size(); i++)
					{
						if (i == tempIndex) continue;
						temp.push_back(vecShapes[i]);
					}
					vecShapes = temp;
				}
				SetFocus(hWnd);
				break;
			case BID_EDIT:
				{
					wchar_t buffer[256];

					GetWindowText(hInputX, buffer, 256);
					int x = std::stoi(buffer);

					GetWindowText(hInputWidth, buffer, 256);
					int width = std::stoi(buffer);

					GetWindowText(hInputY, buffer, 256);
					int y = std::stoi(buffer);

					GetWindowText(hInputHeight, buffer, 256);
					int height = std::stoi(buffer);

					int x1 = x;
					int y1 = y;
					int x2 = x + width;
					int y2 = y + height;
					vecShapes[selectedShapeIndex]->SetPoints(x1, y1, x2, y2);
					InvalidateRect(hWnd, NULL, FALSE);
					SetFocus(hWnd);
				}
				break;
			case BID_SAVESHAPE:
				DeselectShape(hWnd);
				FileManager::SaveText(&vecShapes, L"user\\some.ushp");
				SetFocus(hWnd);
				break;
			default:
				currentShapeID = registeredShapes[LOWORD(wParam)].id;
			}
			break;
		case WM_KEYDOWN:
			if (wParam == VK_TAB)
			{
				SelectNextShape(hWnd);
			}
			else if (wParam == VK_ESCAPE)
			{
				DeselectShape(hWnd);
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	catch (Exception& error)
	{
		MessageBox(NULL, error.what(), L"Fatal error", MB_OK | MB_ICONERROR);
		exit(-1);
	}
}

void InitUI(HWND hWnd)
{
	HMENU mainMenu = CreateMenu();

	HMENU shapesMenu = CreateMenu();
	AppendMenu(mainMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(shapesMenu), L"Shapes");
	int countCore = 0;
	HMENU pluginsMenu = CreateMenu();
	AppendMenu(mainMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(pluginsMenu), L"Plugins");
	int countPlugins = 0;
	HMENU userShapesMenu = CreateMenu();
	AppendMenu(mainMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(userShapesMenu), L"User Shapes");
	int countUserShapes = 0;
	for (std::map<WORD, ShapeRegStruct>::iterator shape = registeredShapes.begin(); shape != registeredShapes.end(); shape++)
	{
		switch (shape->second.type)
		{
			case ShapeType::CORE:
				countCore++;
				AppendMenu(shapesMenu, MF_STRING, shape->first, shape->second.name.c_str());
				break;
			case ShapeType::PLUGIN:
				countPlugins++;
				AppendMenu(pluginsMenu, MF_STRING, shape->first, shape->second.name.c_str());
				break;
			case ShapeType::USER:
				countUserShapes++;
				AppendMenu(userShapesMenu, MF_STRING, shape->first, shape->second.name.c_str());
				break;
		}
	}
	if (!countCore)
		EnableMenuItem(mainMenu, 0, MF_BYPOSITION | MF_DISABLED);
	if (!countPlugins)
		EnableMenuItem(mainMenu, 1, MF_BYPOSITION | MF_DISABLED);
	if (!countUserShapes)
		EnableMenuItem(mainMenu, 2, MF_BYPOSITION | MF_DISABLED);

	SetMenu(hWnd, mainMenu);

	hInputX = DrawInput(hWnd, B_WIDTH * 0, 0, B_WIDTH, B_HEIGHT, NULL);
	hInputY = DrawInput(hWnd, B_WIDTH * 1, 0, B_WIDTH, B_HEIGHT, NULL);
	hInputWidth = DrawInput(hWnd, B_WIDTH * 2, 0, B_WIDTH, B_HEIGHT, NULL);
	hInputHeight = DrawInput(hWnd, B_WIDTH * 3, 0, B_WIDTH, B_HEIGHT, NULL);
	DrawButton(hWnd, BC_EDIT, B_WIDTH * 4, 0, B_WIDTH, B_HEIGHT, BID_EDIT);
	DrawButton(hWnd, BC_DELETE, B_WIDTH * 5, 0, B_WIDTH, B_HEIGHT, BID_DELETE);
	DrawButton(hWnd, BC_SAVE, B_WIDTH * 6, 0, B_WIDTH, B_HEIGHT, BID_SAVE);
	DrawButton(hWnd, BC_LOAD, B_WIDTH * 7, 0, B_WIDTH, B_HEIGHT, BID_LOAD);
	DrawButton(hWnd, BC_SAVESHAPE, B_WIDTH * 8, 0, B_WIDTH, B_HEIGHT, BID_SAVESHAPE);
}

void RegisterCoreShapes(WORD& regIndex, ShapesFactory* sf)
{
	registeredShapes[regIndex++] = { Custom::Line::NAME, 
		sf->RegisterShape(Custom::Line::NAME, Custom::Line::CreateFactoryFunctor()), ShapeType::CORE };
	registeredShapes[regIndex++] = { Custom::Rectangle::NAME, 
		sf->RegisterShape(Custom::Rectangle::NAME, Custom::Rectangle::CreateFactoryFunctor()), ShapeType::CORE };
	registeredShapes[regIndex++] = { Custom::Square::NAME, 
		sf->RegisterShape(Custom::Square::NAME, Custom::Square::CreateFactoryFunctor()), ShapeType::CORE };
	registeredShapes[regIndex++] = { Custom::Ellipse::NAME, 
		sf->RegisterShape(Custom::Ellipse::NAME, Custom::Ellipse::CreateFactoryFunctor()), ShapeType::CORE };
	registeredShapes[regIndex++] = { Custom::Circle::NAME, 
		sf->RegisterShape(Custom::Circle::NAME, Custom::Circle::CreateFactoryFunctor()), ShapeType::CORE };
	registeredShapes[regIndex++] = { Custom::Triangle::NAME, 
		sf->RegisterShape(Custom::Triangle::NAME, Custom::Triangle::CreateFactoryFunctor()), ShapeType::CORE };
}

void RegisterPluginShapes(WORD& regIndex, const PluginManager* pm, ShapesFactory* sf)
{
	std::vector<CustomPlugin> plugins = pm->GetPlugins();
	for (const CustomPlugin plugin : plugins)
		registeredShapes[regIndex++] = { plugin.name, 
			sf->RegisterShape(plugin.name.c_str(), plugin.factory), ShapeType::PLUGIN };
}

void RegisterUserShapes(WORD& regIndex, const UserShapeManager* usm, ShapesFactory* sf)
{
	std::vector<UserShape> userShapes = usm->GetUserShapes();
	for (const UserShape userShape : userShapes)
		registeredShapes[regIndex++] = {userShape.name, 
			sf->RegisterShape(userShape.name.c_str(), userShape.factoryFunctor), ShapeType::USER };
}

void DrawButton(HWND hWnd, const wchar_t* caption, int x, int y, int width, int height, WORD id)
{
	HWND hTest = CreateWindow(L"Button", caption, WS_CHILD | WS_VISIBLE, x, y, width, height,
		hWnd, reinterpret_cast<HMENU>(id), GetModuleHandle(NULL), NULL);
	if (!hTest)
		throw WinException(L"button");
}

HWND DrawInput(HWND hParent, int x, int y, int width, int height, WORD id)
{
	HWND hInput = CreateWindow(L"Edit",	NULL, WS_CHILD | WS_VISIBLE, x, y, width, height, 
		hParent, reinterpret_cast<HMENU>(id), GetModuleHandle(NULL), NULL);
	if (!hInput)
		throw WinException(L"input");

	LONG styles = GetWindowLong(hInput, GWL_STYLE);
	SetWindowLong(hInput, GWL_STYLE, styles | ES_NUMBER);
	SendMessage(hInput, EM_SETLIMITTEXT, 4, 0);
	EnableWindow(hInput, FALSE);

	return hInput;
}

void OnPaint(HWND hWnd)
{
	using namespace Gdiplus;

	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, WND_WIDTH, WND_HEIGHT);
	SelectObject(hdcMem, hBmp);
	Graphics graphics(hdcMem);

	SolidBrush sb(Color::LightGray);
	graphics.FillRectangle(&sb, 0, 0, WND_WIDTH, WND_HEIGHT);

	for(BaseShape* shape : vecShapes)
		shape->Redraw(&graphics);

	if (stretchShape)
		stretchShape->Redraw(&graphics);

	BitBlt(hdc, 0, 0, WND_WIDTH, WND_HEIGHT, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);
	DeleteObject(hBmp);
	EndPaint(hWnd, &ps);
}

void AddShape(HWND hWnd, const ShapesFactory* sf)
{
	BaseShape* shape = sf->CreateShape(currentShapeID);
	shape->SetPoints(points[0].X, points[0].Y, points[1].X, points[1].Y);
	shape->SetColor(0xFFFF0000);
	vecShapes.push_back(shape);
}

void AddStretchShape(HWND hWnd, const ShapesFactory* sf)
{
	stretchShape = sf->CreateShape(currentShapeID);
	stretchShape->SetColor(0xFFFF0000);
}

void SelectNextShape(HWND hWnd)
{
	if (!vecShapes.empty())
	{
		if (selectedShapeIndex != -1)
			vecShapes[selectedShapeIndex]->SetColor(Gdiplus::Color::Red);
		
		selectedShapeIndex = (selectedShapeIndex + 1) % vecShapes.size();
		vecShapes[selectedShapeIndex]->SetColor(Gdiplus::Color::Green);
		InvalidateRect(hWnd, NULL, FALSE);

		wchar_t buffer[256];
		wsprintf(buffer, L"%d", vecShapes[selectedShapeIndex]->GetX());
		SetWindowText(hInputX, buffer);
		wsprintf(buffer, L"%d", vecShapes[selectedShapeIndex]->GetY());
		SetWindowText(hInputY, buffer);
		wsprintf(buffer, L"%d", vecShapes[selectedShapeIndex]->GetWidth());
		SetWindowText(hInputWidth, buffer);
		wsprintf(buffer, L"%d", vecShapes[selectedShapeIndex]->GetHeight());
		SetWindowText(hInputHeight, buffer);

		EnableWindow(hInputX, TRUE);
		EnableWindow(hInputY, TRUE);
		EnableWindow(hInputWidth, TRUE);
		EnableWindow(hInputHeight, TRUE);
	}
}

void DeselectShape(HWND hWnd)
{
	if (selectedShapeIndex != -1)
	{
		vecShapes[selectedShapeIndex]->SetColor(Gdiplus::Color::Red);

		InvalidateRect(hWnd, NULL, FALSE);
		selectedShapeIndex = -1;

		SetWindowText(hInputX, NULL);
		SetWindowText(hInputY, NULL);
		SetWindowText(hInputWidth, NULL);
		SetWindowText(hInputHeight, NULL);

		EnableWindow(hInputX, FALSE);
		EnableWindow(hInputY, FALSE);
		EnableWindow(hInputWidth, FALSE);
		EnableWindow(hInputHeight, FALSE);
	}
}