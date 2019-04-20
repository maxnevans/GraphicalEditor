#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <sstream>
#include "Exception.h"
#include "WinException.h"
#include "ShapesFactory.h"
#include "ListShapes.h"
#include "FileManager.h"

#define WND_CLASS		L"MainWindow"
#define WND_NAME		L"Graphical Editor"
#define WND_WIDTH		1280
#define WND_HEIGHT		720

#define BID_LINE		0x0001
#define BID_TRIANGLE	0x0004
#define BID_RECTANGLE	0x0002
#define BID_ELLIPSE		0x0003
#define BID_CIRCLE		0x0005
#define BID_SQUARE		0x0006
#define BID_SAVE		0x0007
#define BID_LOAD		0x0008
#define BID_DELETE		0x0009
#define BID_EDIT		0x000A

#define BC_LINE			L"Line"
#define BC_TRIANGLE		L"Triangle"
#define BC_RECTANGLE	L"Rectangle"
#define BC_ELLIPSE		L"Ellipse"
#define BC_CIRCLE		L"Circle"
#define BC_SQUARE		L"Square"
#define BC_SAVE			L"Save"
#define BC_LOAD			L"Load"
#define BC_DELETE		L"Delete"
#define BC_EDIT			L"Edit"

#define B_WIDTH			140
#define B_HEIGHT		30

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitUI(HWND);
void DrawButton(HWND, const wchar_t*, int, int, int, int, WORD);
HWND DrawInput(HWND, int, int, int, int, WORD);
void OnPaint(HWND hWnd);
void AddShape(HWND hWnd);
void AddStretchShape(HWND hWnd);

ListShapes* shapes;
UINT currentTool;
Gdiplus::Point points[2];
Custom::BaseShape* stretchShape;
Custom::BaseShape* selectedShape;
HWND hInputX, hInputY, hInputWidth, hInputHeight;

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nShowCmd
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
		if (!retVal) throw WinException(L"register window");

		RECT rDesk;
		GetWindowRect(GetDesktopWindow(), &rDesk);

		HWND hWnd = CreateWindowW(
			WND_CLASS,
			WND_NAME,
			WS_VISIBLE | WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX | WS_CLIPCHILDREN,
			(rDesk.right - WND_WIDTH) / 2, (rDesk.bottom - WND_HEIGHT) / 2,
			WND_WIDTH, WND_HEIGHT,
			NULL,
			NULL,
			hInstance,
			0
		);
		if (hWnd == NULL) throw WinException(L"create window");
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
	catch (Exception error)
	{
		MessageBox(NULL, error.what().c_str(), L"Fatal error", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	catch (...)
	{
		MessageBox(NULL, L"Undefined error!", L"Undefined error", MB_OK | MB_ICONERROR);
		exit(-1);
	}
}

int tab_pressed = 0;

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			InitUI(hWnd);
			shapes = new ListShapes;
			currentTool = BID_LINE;
			points[0].X = -1;
			points[0].Y = -1;
			points[1].X = -1;
			points[1].Y = -1;
			stretchShape = nullptr;
			selectedShape = nullptr;
			break;
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_ERASEBKGND:
			return TRUE;
		case WM_LBUTTONDOWN:
			points[0].X = GET_X_LPARAM(lParam);
			points[0].Y = GET_Y_LPARAM(lParam);
			AddStretchShape(hWnd);
			break;
		case WM_LBUTTONUP:
			points[1].X = GET_X_LPARAM(lParam);
			points[1].Y = GET_Y_LPARAM(lParam);
			AddShape(hWnd);
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
				case BID_LINE:
				case BID_RECTANGLE:
				case BID_SQUARE:
				case BID_ELLIPSE:
				case BID_CIRCLE:
				case BID_TRIANGLE:
					currentTool = static_cast<UINT>(wParam);
					SetFocus(hWnd);
					break;
				case BID_SAVE:
					FileManager::SaveText(shapes, L"test.txt");
					SetFocus(hWnd);
					break;
				case BID_LOAD:
					while (!shapes->IsEmpty()) shapes->Pop();
					FileManager::LoadText(shapes, L"test.txt");
					InvalidateRect(hWnd, NULL, FALSE);
					SetFocus(hWnd);
					break;
				case BID_DELETE:
					if (selectedShape)
					{
						delete selectedShape;
						selectedShape = nullptr;
						SetWindowText(hInputX, NULL);
						SetWindowText(hInputY, NULL);
						SetWindowText(hInputWidth, NULL);
						SetWindowText(hInputHeight, NULL);
						InvalidateRect(hWnd, NULL, FALSE);
					}
					SetFocus(hWnd);
					break;
				case BID_EDIT:
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
					selectedShape->SetPoints(x1, y1, x2, y2);
					InvalidateRect(hWnd, NULL, FALSE);
					SetFocus(hWnd);
					break;
			}
			break;
		case WM_KEYDOWN:
			if (wParam == VK_TAB)
			{
				if (!shapes->IsEmpty())
				{
					if (selectedShape)
					{
						selectedShape->SetColor(Gdiplus::Color::Red);
						shapes->Push(selectedShape);
					}
					selectedShape = shapes->Pop();
					selectedShape->SetColor(Gdiplus::Color::Green);
					InvalidateRect(hWnd, NULL, FALSE);

					wchar_t buffer[256];
					wsprintf(buffer, L"%d", selectedShape->GetX());
					SetWindowText(hInputX, buffer);
					wsprintf(buffer, L"%d", selectedShape->GetY());
					SetWindowText(hInputY, buffer);
					wsprintf(buffer, L"%d", selectedShape->GetWidth());
					SetWindowText(hInputWidth, buffer);
					wsprintf(buffer, L"%d", selectedShape->GetHeight());
					SetWindowText(hInputHeight, buffer);
				}
			}
			else if (wParam == VK_ESCAPE) 
			{
				if (selectedShape)
				{
					selectedShape->SetColor(Gdiplus::Color::Red);
					shapes->Push(selectedShape);
					InvalidateRect(hWnd, NULL, FALSE);
					selectedShape = nullptr;
					SetWindowText(hInputX, NULL);
					SetWindowText(hInputY, NULL);
					SetWindowText(hInputWidth, NULL);
					SetWindowText(hInputHeight, NULL);
				}
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

void InitUI(HWND hWnd)
{
	DrawButton(hWnd, BC_LINE, B_WIDTH * 0, 0, B_WIDTH, B_HEIGHT, BID_LINE);
	DrawButton(hWnd, BC_RECTANGLE, B_WIDTH * 1, 0, B_WIDTH, B_HEIGHT, BID_RECTANGLE);
	DrawButton(hWnd, BC_ELLIPSE, B_WIDTH * 2, 0, B_WIDTH, B_HEIGHT, BID_ELLIPSE);
	DrawButton(hWnd, BC_TRIANGLE, B_WIDTH * 3, 0, B_WIDTH, B_HEIGHT, BID_TRIANGLE);
	DrawButton(hWnd, BC_CIRCLE, B_WIDTH * 4, 0, B_WIDTH, B_HEIGHT, BID_CIRCLE);
	DrawButton(hWnd, BC_SQUARE, B_WIDTH * 5, 0, B_WIDTH, B_HEIGHT, BID_SQUARE);
	DrawButton(hWnd, BC_SAVE, B_WIDTH * 6, 0, B_WIDTH, B_HEIGHT, BID_SAVE);
	DrawButton(hWnd, BC_LOAD, B_WIDTH * 7, 0, B_WIDTH, B_HEIGHT, BID_LOAD);
	DrawButton(hWnd, BC_DELETE, B_WIDTH * 8, 0, B_WIDTH, B_HEIGHT, BID_DELETE);
	DrawButton(hWnd, BC_EDIT, B_WIDTH * 4, B_HEIGHT, B_WIDTH, B_HEIGHT, BID_EDIT);

	hInputX = DrawInput(hWnd, B_WIDTH * 0, B_HEIGHT, B_WIDTH, B_HEIGHT, NULL);
	hInputY = DrawInput(hWnd, B_WIDTH * 1, B_HEIGHT, B_WIDTH, B_HEIGHT, NULL);
	hInputWidth = DrawInput(hWnd, B_WIDTH * 2, B_HEIGHT, B_WIDTH, B_HEIGHT, NULL);
	hInputHeight = DrawInput(hWnd, B_WIDTH * 3, B_HEIGHT, B_WIDTH, B_HEIGHT, NULL);
}

void DrawButton(
	HWND hWnd, 
	const wchar_t* caption, 
	int x, int y, 
	int width, int height, 
	WORD id
)
{
	HWND hTest = CreateWindow(
		L"Button",
		caption,
		WS_CHILD | WS_VISIBLE,
		x, y,
		width, height,
		hWnd,
		reinterpret_cast<HMENU>(id),
		GetModuleHandle(NULL),
		NULL
	);
	if (!hTest) throw WinException(L"button");
}

HWND DrawInput(HWND hParent, int x, int y, int width, int height, WORD id)
{
	HWND hInput = CreateWindow
	(
		L"Edit",
		NULL, 
		WS_CHILD | WS_VISIBLE,
		x, y, 
		width, height,
		hParent,
		reinterpret_cast<HMENU>(id),
		GetModuleHandle(NULL),
		NULL
	);
	if (!hInput) throw WinException(L"input");

	LONG styles = GetWindowLong(hInput, GWL_STYLE);
	SetWindowLong(hInput, GWL_STYLE, styles | ES_NUMBER);

	SendMessage(hInput, EM_SETLIMITTEXT, 4, 0);

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

	SolidBrush sb(Color::White);
	graphics.FillRectangle(&sb, 0, 0, WND_WIDTH, WND_HEIGHT);

	ListShapes* temp = new ListShapes();
	while (!shapes->IsEmpty())
	{
		Custom::BaseShape* shape = shapes->Pop();
		shape->Redraw(&graphics);
		temp->Push(shape);
	}
	delete shapes;
	shapes = temp;

	if (stretchShape)
		stretchShape->Redraw(&graphics);
	if (selectedShape)
		selectedShape->Redraw(&graphics);

	BitBlt(hdc, 0, 0, WND_WIDTH, WND_HEIGHT, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);
	DeleteObject(hBmp);
	EndPaint(hWnd, &ps);
}

void AddShape(HWND hWnd)
{
	Custom::BaseShape* shape = nullptr;
	switch (currentTool)
	{
		case BID_LINE:
			shape = ShapesFactory::CreateShape(ShapesFactory::LINE);
			break;
		case BID_RECTANGLE:
			shape = ShapesFactory::CreateShape(ShapesFactory::RECTANGLE);
			break;
		case BID_SQUARE:
			shape = ShapesFactory::CreateShape(ShapesFactory::SQUARE);
			break;
		case BID_ELLIPSE:
			shape = ShapesFactory::CreateShape(ShapesFactory::ELLIPSE);
			break;
		case BID_CIRCLE:
			shape = ShapesFactory::CreateShape(ShapesFactory::CIRCLE);
			break;
		case BID_TRIANGLE:
			shape = ShapesFactory::CreateShape(ShapesFactory::TRIANGLE);
			break;
		default:
			throw DebugException(L"undefined shape");
	}
	shape->SetPoints(points[0].X, points[0].Y, points[1].X, points[1].Y);
	shape->SetColor(Gdiplus::Color(0xFFFF0000));
	shapes->Push(shape);
}

void AddStretchShape(HWND hWnd)
{
	switch (currentTool)
	{
	case BID_LINE:
		stretchShape = ShapesFactory::CreateShape(ShapesFactory::LINE);
		break;
	case BID_RECTANGLE:
		stretchShape = ShapesFactory::CreateShape(ShapesFactory::RECTANGLE);
		break;
	case BID_SQUARE:
		stretchShape = ShapesFactory::CreateShape(ShapesFactory::SQUARE);
		break;
	case BID_ELLIPSE:
		stretchShape = ShapesFactory::CreateShape(ShapesFactory::ELLIPSE);
		break;
	case BID_CIRCLE:
		stretchShape = ShapesFactory::CreateShape(ShapesFactory::CIRCLE);
		break;
	case BID_TRIANGLE:
		stretchShape = ShapesFactory::CreateShape(ShapesFactory::TRIANGLE);
		break;
	default:
		throw DebugException(L"undefined shape");
	}
	stretchShape->SetColor(Gdiplus::Color(0xFFFF0000));
}