#include "Main.h"

HWND hMainWnd;
const ConfigManager::Config* appConfig;
std::map<WORD, ShapeRegStruct> registeredShapes;
std::vector<BaseShape*> vecShapes;
size_t selectedShapeIndex;
ShapeID currentShapeID;
ShapesFactory sf;
ConfigManager* cm;
PluginManager* pm;
UserShapeManager* usm;
Gdiplus::Point points[2];
BaseShape* stretchShape;
HWND hInputX1, hInputY1, hInputX2, hInputY2, hInputShapeName, hInputFileName;

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

		RegisterMainWindowClass();
		RegisterUserShapeWindowClass();

		RECT rDesk;
		GetWindowRect(GetDesktopWindow(), &rDesk);

		hMainWnd = CreateWindowW(MAIN_WND_CLASS, MAIN_WND_NAME,
			WS_VISIBLE | WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX | WS_CLIPCHILDREN,
			(rDesk.right - MAIN_WND_WIDTH) / 2, (rDesk.bottom - MAIN_WND_HEIGHT) / 2,
			MAIN_WND_WIDTH, MAIN_WND_HEIGHT, NULL, NULL, hInstance, 0);
		if (hMainWnd == NULL)
			throw WinException(L"create window");

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

HWND CreateUserShapeSaveWindow(HWND hParent)
{
	RECT parentRect;
	GetWindowRect(hParent, &parentRect);

	HWND hWnd = CreateWindowW(US_WND_CLASS, US_WND_NAME,
		WS_VISIBLE | WS_SYSMENU & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX | WS_CLIPCHILDREN,
		parentRect.left + (parentRect.right - parentRect.left - US_WND_WIDTH) / 2, parentRect.top + (parentRect.bottom - parentRect.top - US_WND_HEIGHT) / 2,
		US_WND_WIDTH, US_WND_HEIGHT, NULL, NULL, GetModuleHandle(NULL), 0);

	if (hWnd == NULL)
		throw WinException(L"create UserShape window failed");

	DrawStatic(hWnd, US_SC_SHAPENAME, 20, 10, US_S_WIDTH, US_S_HEIGHT);
	DrawStatic(hWnd, US_SC_FILENAME, 20, 50, US_S_WIDTH, US_S_HEIGHT);
	hInputShapeName = DrawInput(hWnd, US_S_WIDTH + 30, 10, US_B_WIDTH, US_B_HEIGHT, NULL);
	hInputFileName = DrawInput(hWnd, US_S_WIDTH + 30, 50, US_B_WIDTH, US_B_HEIGHT, NULL);
	DrawButton(hWnd, US_BC_SAVE, 20, 90, US_B_WIDTH, US_B_HEIGHT, US_BID_SAVE);
	DrawButton(hWnd, US_BC_CANCEL, US_B_WIDTH + 30, 90, US_B_WIDTH, US_B_HEIGHT, US_BID_CANCEL);
	

	return hWnd;
}

void RegisterUserShapeWindowClass()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = UserShapeWndProc;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszClassName = US_WND_CLASS;
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND) + 1;

	DWORD retVal = RegisterClassEx(&wcex);
	if (!retVal)
		throw WinException(L"register UserShape window");
}

void RegisterMainWindowClass()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszClassName = MAIN_WND_CLASS;

	DWORD retVal = RegisterClassEx(&wcex);
	if (!retVal)
		throw WinException(L"register Main window");
}

LRESULT WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch (uMsg)
		{
			case WM_CREATE:
				{
					cm = new ConfigManager(L"./");
					appConfig = cm->GetConfig();
					WORD regIndex = MW_REG_INDEX_START;
					RegisterCoreShapes(regIndex, &sf);
					try
					{
						pm = new PluginManager(appConfig->directory.plugins);
						RegisterPluginShapes(regIndex, pm, &sf);
					}
					catch (Exception&)
					{
						delete pm;
						pm = nullptr;
					}
					usm = new UserShapeManager(&sf, appConfig->directory.userShapes);
					RegisterUserShapes(regIndex, usm, &sf);
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
					case MW_BID_SAVE:
						{
							DeselectShape(hWnd);
							std::vector<const BaseShape*> vecShapesConst(vecShapes.begin(), vecShapes.end());
							std::wstring outputFullPath = appConfig->directory.working + L"/" + L"test.txt";
							FileManager::SaveText(&vecShapesConst, outputFullPath);
							SetFocus(hWnd);
						}
						break;
					case MW_BID_LOAD:
						{
							DeselectShape(hWnd);
							vecShapes.clear();
							std::wstring loadFullPath = appConfig->directory.working + L"/" + L"test.txt";
							FileManager::LoadText(&sf, &vecShapes, loadFullPath);
							InvalidateRect(hWnd, NULL, FALSE);
							SetFocus(hWnd);
						}
						break;
					case MW_BID_DELETE:
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
					case MW_BID_EDIT:
						{
							wchar_t buffer[256];

							GetWindowText(hInputX1, buffer, 256);
							int x1 = std::stoi(buffer);

							GetWindowText(hInputX2, buffer, 256);
							int x2 = std::stoi(buffer);

							GetWindowText(hInputY1, buffer, 256);
							int y1 = std::stoi(buffer);

							GetWindowText(hInputY2, buffer, 256);
							int y2 = std::stoi(buffer);

							vecShapes[selectedShapeIndex]->SetPoints(x1, y1, x2, y2);
							InvalidateRect(hWnd, NULL, FALSE);
							SetFocus(hWnd);
						}
						break;
					case MW_BID_SAVESHAPE:
						CreateUserShapeSaveWindow(hWnd);
						EnableWindow(hWnd, FALSE);
						break;
					case MW_AID_SAVEUSHAPE:
						{
							wchar_t buffer[256];
							GetWindowText(hInputShapeName, buffer, 256);
							std::wstring shapeName(buffer);
							GetWindowText(hInputFileName, buffer, 256);
							std::wstring fileName(buffer);

							DeselectShape(hWnd);
							std::vector<const BaseShape*> vecShapesConst(vecShapes.begin(), vecShapes.end());
							usm->SaveUserShape(fileName, shapeName, vecShapesConst);
							SetFocus(hWnd);
						}
					case MW_AID_CANCELUSHAPE:
						{
							EnableWindow(hWnd, TRUE);
							SetFocus(hWnd);
						}
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
					SetFocus(hWnd);
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

LRESULT WINAPI UserShapeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case US_BID_SAVE:
					SendMessage(hMainWnd, WM_COMMAND, MW_AID_SAVEUSHAPE, reinterpret_cast<LPARAM>(hWnd));
					DestroyWindow(hWnd);
					return 0;
				case US_BID_CANCEL:
					SendMessage(hMainWnd, WM_COMMAND, MW_AID_CANCELUSHAPE, reinterpret_cast<LPARAM>(hWnd));
					DestroyWindow(hWnd);
					return 0;
			}
			break;
		case WM_CLOSE:
			SendMessage(GetParent(hWnd), WM_COMMAND, MW_AID_CANCELUSHAPE, reinterpret_cast<LPARAM>(hWnd));
			DestroyWindow(hWnd);
			return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
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

	hInputX1 = DrawInput(hWnd, MW_B_WIDTH * 0, 0, MW_B_WIDTH, MW_B_HEIGHT, NULL);
	hInputY1 = DrawInput(hWnd, MW_B_WIDTH * 1, 0, MW_B_WIDTH, MW_B_HEIGHT, NULL);
	hInputX2 = DrawInput(hWnd, MW_B_WIDTH * 2, 0, MW_B_WIDTH, MW_B_HEIGHT, NULL);
	hInputY2 = DrawInput(hWnd, MW_B_WIDTH * 3, 0, MW_B_WIDTH, MW_B_HEIGHT, NULL);
	EnableWindow(hInputX1, FALSE);
	EnableWindow(hInputX2, FALSE);
	EnableWindow(hInputY1, FALSE);
	EnableWindow(hInputY2, FALSE);
	SetInputFilterNumbersOnly(hInputX1, 4);
	SetInputFilterNumbersOnly(hInputX2, 4);
	SetInputFilterNumbersOnly(hInputY1, 4);
	SetInputFilterNumbersOnly(hInputY2, 4);
	DrawButton(hWnd, MW_BC_EDIT, MW_B_WIDTH * 4, 0, MW_B_WIDTH, MW_B_HEIGHT, MW_BID_EDIT);
	DrawButton(hWnd, MW_BC_DELETE, MW_B_WIDTH * 5, 0, MW_B_WIDTH, MW_B_HEIGHT, MW_BID_DELETE);
	DrawButton(hWnd, MW_BC_SAVE, MW_B_WIDTH * 6, 0, MW_B_WIDTH, MW_B_HEIGHT, MW_BID_SAVE);
	DrawButton(hWnd, MW_BC_LOAD, MW_B_WIDTH * 7, 0, MW_B_WIDTH, MW_B_HEIGHT, MW_BID_LOAD);
	DrawButton(hWnd, MW_BC_SAVESHAPE, MW_B_WIDTH * 8, 0, MW_B_WIDTH, MW_B_HEIGHT, MW_BID_SAVESHAPE);
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

	

	return hInput;
}

void DrawStatic(HWND hParent, std::wstring caption, int x, int y, int width, int height)
{
	HWND hStatic = CreateWindow(L"Static", caption.c_str(), WS_CHILD | WS_VISIBLE, x, y, width, height,
		hParent, NULL, GetModuleHandle(NULL), NULL);

	if (!hStatic)
		throw WinException(L"static window failed to create");
}

void SetInputFilterNumbersOnly(HWND hInput, int amountAllowed)
{
	LONG styles = GetWindowLong(hInput, GWL_STYLE);
	SetWindowLong(hInput, GWL_STYLE, styles | ES_NUMBER);
	SendMessage(hInput, EM_SETLIMITTEXT, amountAllowed, 0);
}

void OnPaint(HWND hWnd)
{
	using namespace Gdiplus;

	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, MAIN_WND_WIDTH, MAIN_WND_HEIGHT);
	SelectObject(hdcMem, hBmp);
	Graphics graphics(hdcMem);

	SolidBrush sb(appConfig->color.background);
	graphics.FillRectangle(&sb, 0, 0, MAIN_WND_WIDTH, MAIN_WND_HEIGHT);

	for(BaseShape* shape : vecShapes)
		shape->Redraw(&graphics);

	if (stretchShape)
		stretchShape->Redraw(&graphics);

	BitBlt(hdc, 0, 0, MAIN_WND_WIDTH, MAIN_WND_HEIGHT, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);
	DeleteObject(hBmp);
	EndPaint(hWnd, &ps);
}

void AddShape(HWND hWnd, const ShapesFactory* sf)
{
	BaseShape* shape = sf->CreateShape(currentShapeID);
	shape->SetPoints(points[0].X, points[0].Y, points[1].X, points[1].Y);
	shape->SetColor(appConfig->color.shape);
	vecShapes.push_back(shape);
}

void AddStretchShape(HWND hWnd, const ShapesFactory* sf)
{
	stretchShape = sf->CreateShape(currentShapeID);
	stretchShape->SetColor(appConfig->color.shape);
}

void SelectNextShape(HWND hWnd)
{
	if (!vecShapes.empty())
	{
		if (selectedShapeIndex != -1)
			vecShapes[selectedShapeIndex]->SetColor(appConfig->color.shape);
		
		selectedShapeIndex = (selectedShapeIndex + 1) % vecShapes.size();
		vecShapes[selectedShapeIndex]->SetColor(appConfig->color.selection);
		InvalidateRect(hWnd, NULL, FALSE);

		int x1, y1, x2, y2;
		vecShapes[selectedShapeIndex]->GetPoints(x1, y1, x2, y2);
		SetWindowText(hInputX1, std::to_wstring(x1).c_str());
		SetWindowText(hInputY1, std::to_wstring(y1).c_str());
		SetWindowText(hInputX2, std::to_wstring(x2).c_str());
		SetWindowText(hInputY2, std::to_wstring(y2).c_str());

		EnableWindow(hInputX1, TRUE);
		EnableWindow(hInputY1, TRUE);
		EnableWindow(hInputX2, TRUE);
		EnableWindow(hInputY2, TRUE);
	}
}

void DeselectShape(HWND hWnd)
{
	if (selectedShapeIndex != -1)
	{
		vecShapes[selectedShapeIndex]->SetColor(appConfig->color.shape);

		InvalidateRect(hWnd, NULL, FALSE);
		selectedShapeIndex = -1;

		SetWindowText(hInputX1, NULL);
		SetWindowText(hInputY1, NULL);
		SetWindowText(hInputX2, NULL);
		SetWindowText(hInputY2, NULL);

		EnableWindow(hInputX1, FALSE);
		EnableWindow(hInputY1, FALSE);
		EnableWindow(hInputX2, FALSE);
		EnableWindow(hInputY2, FALSE);
	}
}