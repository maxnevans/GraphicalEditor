#include <windows.h>
#include <exception>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define WND_CLASS L"MainWindow"

#define BID_LINE		0x0001
#define BID_TRIANGLE	0x0004
#define BID_RECTANGLE	0x0002
#define BID_ELLIPSE		0x0003
#define BID_CIRCLE		0x0005
#define BID_LINEZ		0x0006

#define B_WIDTH			140
#define B_HEIGHT		30

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void goError(const wchar_t* description, DWORD code = ERROR_SUCCESS);
void InitUI(HWND);
void DrawButton(HWND, const wchar_t*, int, int, int, int, int);

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nShowCmd
)
{

	WNDCLASSEX wcex = {0};
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = WND_CLASS;

	DWORD retVal = RegisterClassEx(&wcex);
	if (!retVal) goError(L"register window");
	
	HWND hWnd = CreateWindowW(
		WND_CLASS,
		L"Window Title",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		0,0,
		1280, 720,
		NULL,
		NULL,
		hInstance,
		0
	);
	if (hWnd == NULL) goError(L"create window");

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;

}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
			InitUI(hWnd);
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
	DrawButton(hWnd, L"Line", 0, 0, B_WIDTH, B_HEIGHT, BID_LINE);
	DrawButton(hWnd, L"Rectangle", B_WIDTH, 0, B_WIDTH, B_HEIGHT, BID_RECTANGLE);
	DrawButton(hWnd, L"Ellipse", B_WIDTH*2, 0, B_WIDTH, B_HEIGHT, BID_ELLIPSE);
	DrawButton(hWnd, L"Triangle", B_WIDTH*3, 0, B_WIDTH, B_HEIGHT, BID_TRIANGLE);
	DrawButton(hWnd, L"Circle", B_WIDTH*4, 0, B_WIDTH, B_HEIGHT, BID_CIRCLE);
	DrawButton(hWnd, L"LineZ", B_WIDTH*5, 0, B_WIDTH, B_HEIGHT, BID_LINEZ);
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