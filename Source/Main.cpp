#include <windows.h>
#include <exception>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <gdiplus.h>
#include "../Include/List.h"
#include "../Include/Line.h"

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
void DrawButton(HWND, const wchar_t*, int, int, int, int, int);
void OnPaint(HWND hWnd);

List* shapes;

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
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	return (int) msg.wParam;

}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			InitUI(hWnd);
			shapes = new List;
			break;
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case BID_LINE:
				{
					Line* line = new Line();
					line->SetPoints(100, 100, 200, 200);
					line->SetColor(Gdiplus::Color(0xFFFF0000));
					shapes->push(line);
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				}
				case BID_RECTANGLE:

					break;
				case BID_SQUARE:

					break;
				case BID_ELLIPSE:

					break;
				case BID_CIRCLE:

					break;
				case BID_TRIANGLE:

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
	int id
)
{
	HWND hTest = CreateWindow(
		L"Button",
		caption,
		WS_CHILD | WS_VISIBLE,
		x, y,
		width, height,
		hWnd,
		(HMENU)id,
		GetModuleHandle(NULL),
		NULL
	);
	if (!hTest) goError(L"line button");
}

void OnPaint(HWND hWnd)
{
	using namespace Gdiplus;

	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);

	List* temp = new List();
	while (!shapes->is_empty())
	{
		BaseShape* shape = (BaseShape*)shapes->pop();
		shape->Redraw(&graphics);
		temp->push(shape);
	}
	delete shapes;
	shapes = temp;

	EndPaint(hWnd, &ps);
}