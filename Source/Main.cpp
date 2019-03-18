#include <windows.h>
#include <windowsx.h>
#include <exception>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <gdiplus.h>
#include "List.h"
#include "Square.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Polygon.h"

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

#define BC_LINE			L"Line"
#define BC_TRIANGLE		L"Triangle"
#define BC_RECTANGLE	L"Rectangle"
#define BC_ELLIPSE		L"Ellipse"
#define BC_CIRCLE		L"Circle"
#define BC_SQUARE		L"Square"

#define B_WIDTH			140
#define B_HEIGHT		30

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void goError(const wchar_t* description, DWORD code = ERROR_SUCCESS);
void InitUI(HWND);
void DrawButton(HWND, const wchar_t*, int, int, int, int, WORD);
void OnPaint(HWND hWnd);
void AddShape(HWND hWnd);
void AddStretchShape(HWND hWnd);
void HideTools();
void ShowTools();

HWND buttons[6];
List* shapes;
UINT currentTool;
Gdiplus::Point points[2];
Custom::BaseShape* stretchShape;

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nShowCmd
)
{
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	WNDCLASSEX wcex = {0};
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = hInstance;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszClassName = WND_CLASS;

	DWORD retVal = RegisterClassEx(&wcex);
	if (!retVal) goError(L"register window");

	RECT rDesk;
	GetWindowRect(GetDesktopWindow(), &rDesk);
	
	HWND hWnd = CreateWindowW(
		WND_CLASS,
		WND_NAME,
		WS_VISIBLE | WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX,
		(rDesk.right - WND_WIDTH) / 2, (rDesk.bottom - WND_HEIGHT) / 2,
		WND_WIDTH, WND_HEIGHT,
		NULL,
		NULL,
		hInstance,
		0
	);
	if (hWnd == NULL) goError(L"create window");
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

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			InitUI(hWnd);
			shapes = new List;
			currentTool = BID_LINE;
			points[0].X = -1;
			points[0].Y = -1;
			points[1].X = -1;
			points[1].Y = -1;
			stretchShape = nullptr;
			break;
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_ERASEBKGND:
			return TRUE;
		case WM_LBUTTONDOWN:
			HideTools();
			points[0].X = GET_X_LPARAM(lParam);
			points[0].Y = GET_Y_LPARAM(lParam);
			AddStretchShape(hWnd);
			break;
		case WM_LBUTTONUP:
			ShowTools();
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
				InvalidateRect(hWnd, NULL, TRUE);
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
					break;
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

void goError(const wchar_t* description, DWORD code)
{
	const int MAX_ERROR_SIZE_MESSAGE = 256;
	assert(description != NULL);
	assert(wcslen(description) < MAX_ERROR_SIZE_MESSAGE);

	if (code == ERROR_SUCCESS) code = GetLastError();

	wchar_t buf[MAX_ERROR_SIZE_MESSAGE];
	swprintf_s(buf, L"Error: %s [CODE:%d]", description, code);

	MessageBoxW(NULL, buf , L"Fatal Failure", MB_OK| MB_ICONERROR );
	exit(EXIT_FAILURE);
}

void InitUI(HWND hWnd)
{
	DrawButton(hWnd, BC_LINE, B_WIDTH * 0, 0, B_WIDTH, B_HEIGHT, BID_LINE);
	DrawButton(hWnd, BC_RECTANGLE, B_WIDTH * 1, 0, B_WIDTH, B_HEIGHT, BID_RECTANGLE);
	DrawButton(hWnd, BC_ELLIPSE, B_WIDTH * 2, 0, B_WIDTH, B_HEIGHT, BID_ELLIPSE);
	DrawButton(hWnd, BC_TRIANGLE, B_WIDTH * 3, 0, B_WIDTH, B_HEIGHT, BID_TRIANGLE);
	DrawButton(hWnd, BC_CIRCLE, B_WIDTH * 4, 0, B_WIDTH, B_HEIGHT, BID_CIRCLE);
	DrawButton(hWnd, BC_SQUARE, B_WIDTH * 5, 0, B_WIDTH, B_HEIGHT, BID_SQUARE);
}

void DrawButton(
	HWND hWnd, 
	const wchar_t* caption, 
	int x, int y, 
	int width, int height, 
	WORD id
)
{
	static int countButtons = 0;
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
	if (!hTest) goError(L"button");
	buttons[countButtons++] = hTest;

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

	List* temp = new List();
	while (!shapes->is_empty())
	{
		Custom::BaseShape* shape = static_cast<Custom::BaseShape*>(shapes->pop());
		shape->Redraw(&graphics);
		temp->push(shape);
	}
	delete shapes;
	shapes = temp;

	if (stretchShape)
		stretchShape->Redraw(&graphics);

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
			shape = new Custom::Line();
			break;
		case BID_RECTANGLE:
			shape = new Custom::Rectangle();
			break;
		case BID_SQUARE:
			shape = new Custom::Square();
			break;
		case BID_ELLIPSE:
			shape = new Custom::Ellipse();
			break;
		case BID_CIRCLE:
			shape = new Custom::Circle();
			break;
		case BID_TRIANGLE:
			shape = new Custom::Triangle();
			break;
		default:
			goError(L"undefined shape");
	}
	shape->SetPoints(points[0].X, points[0].Y, points[1].X, points[1].Y);
	shape->SetColor(Gdiplus::Color(0xFFFF0000));
	shapes->push(shape);
}

void AddStretchShape(HWND hWnd)
{
	switch (currentTool)
	{
	case BID_LINE:
		stretchShape = new Custom::Line();
		break;
	case BID_RECTANGLE:
		stretchShape = new Custom::Rectangle();
		break;
	case BID_SQUARE:
		stretchShape = new Custom::Square();
		break;
	case BID_ELLIPSE:
		stretchShape = new Custom::Ellipse();
		break;
	case BID_CIRCLE:
		stretchShape = new Custom::Circle();
		break;
	case BID_TRIANGLE:
		stretchShape = new Custom::Triangle();
		break;
	default:
		goError(L"undefined shape");
	}
	stretchShape->SetColor(Gdiplus::Color(0xFFFF0000));
}

void HideTools()
{
	for (int i = 0; i < 6; i++)
	{
		ShowWindow(buttons[i], SW_HIDE);
	}
}

void ShowTools()
{
	for (int i = 0; i < 6; i++)
	{
		ShowWindow(buttons[i], SW_SHOW);
	}
}