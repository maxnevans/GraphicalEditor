#include "Main.h"

std::map<WORD, ShapeRegStruct> registeredShapes;
std::vector<BaseShape*> vecShapes;
size_t selectedShapeIndex;
ShapeID currentShapeID;
ShapesFactory sf;
PluginManager* pm;
UserShapeManager* usm;
Gdiplus::Point points[2];
BaseShape* stretchShape;
HWND hInputX1, hInputY1, hInputX2, hInputY2;

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
					usm = new UserShapeManager(&sf, L"user");
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
					case BID_SAVE:
						{
							DeselectShape(hWnd);
							std::vector<const BaseShape*> vecShapesConst(vecShapes.begin(), vecShapes.end());
							FileManager::SaveText(&vecShapesConst, L"test.txt");
							SetFocus(hWnd);
						}
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
					case BID_SAVESHAPE:
						{
							DeselectShape(hWnd);
							std::vector<const BaseShape*> vecShapesConst(vecShapes.begin(), vecShapes.end());
							usm->SaveUserShape(L"some", L"Super user shape", vecShapesConst);
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

	hInputX1 = DrawInput(hWnd, B_WIDTH * 0, 0, B_WIDTH, B_HEIGHT, NULL);
	hInputY1 = DrawInput(hWnd, B_WIDTH * 1, 0, B_WIDTH, B_HEIGHT, NULL);
	hInputX2 = DrawInput(hWnd, B_WIDTH * 2, 0, B_WIDTH, B_HEIGHT, NULL);
	hInputY2 = DrawInput(hWnd, B_WIDTH * 3, 0, B_WIDTH, B_HEIGHT, NULL);
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
		vecShapes[selectedShapeIndex]->SetColor(Gdiplus::Color::Red);

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