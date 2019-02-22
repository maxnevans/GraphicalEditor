#include <windows.h>
#include <exception>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define WND_CLASS L"MainWindow"

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void goError(const wchar_t* description, DWORD code = ERROR_SUCCESS);

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