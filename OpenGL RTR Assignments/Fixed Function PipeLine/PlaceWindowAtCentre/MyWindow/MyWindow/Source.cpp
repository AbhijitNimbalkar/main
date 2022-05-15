#include <Windows.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600


HWND ghwnd = NULL;
bool gbEscapeKeyIsPressed = false;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool gbFullscreen = false;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
DWORD gdwStyle;

//function declerations 
void ToggleFullscreen(void);
int giWidthOfmonitor = 0; 
int giHeightOfmonitor = 0; 
int gxPoint = 0;
int gyPoint = 0;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{

	WNDCLASS wndclass;
	TCHAR szClassName[] = TEXT("WindowAtCentre");
	HWND hwnd = NULL;
	MSG msg;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = ::LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = ::LoadCursor(NULL,IDC_HAND);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;
	wndclass.lpfnWndProc = WndProc;

	//register class 
	RegisterClass(&wndclass);

	//call create window
	hwnd = CreateWindow(
		szClassName,
		TEXT("Window At the Centre"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	
	ghwnd = hwnd;

	
	 giWidthOfmonitor = GetSystemMetrics(SM_CXSCREEN);
	 giHeightOfmonitor = GetSystemMetrics(SM_CYSCREEN);

	gxPoint = ((giWidthOfmonitor / 2) - (WIN_WIDTH / 2));
	gyPoint = ((giHeightOfmonitor / 2) - (WIN_HEIGHT / 2));

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = giWidthOfmonitor;
	rect.bottom = giHeightOfmonitor;
	SystemParametersInfo(SPI_SETWORKAREA,0, &rect,0);

	SetWindowPos(ghwnd, HWND_TOP, gxPoint, gyPoint, WIN_WIDTH, WIN_HEIGHT,SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
			break;
		case 0x46: //for 'f' or 'F'
			if (gbFullscreen == false)
			{
				ToggleFullscreen();
				gbFullscreen = true;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = false;
			}
			break;
		default:
			break;
		}
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	MONITORINFO mi;
	//int iWidthOfmonitor = GetSystemMetrics(SM_CXSCREEN);
	//int iHeightOfmonitor = GetSystemMetrics(SM_CYSCREEN);

	//int xPoint = ((giWidthOfmonitor / 2) - (WIN_WIDTH / 2));
	//int yPoint = ((giHeightOfmonitor / 2) - (WIN_HEIGHT / 2));
	if (gbFullscreen == false)
	{
		gdwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (gdwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, gdwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}

	else
	{

		SetWindowLong(ghwnd, GWL_STYLE, gdwStyle | WS_OVERLAPPEDWINDOW);
		
		SetWindowPos(ghwnd, HWND_TOP, 0,0, 0, 0, /*SWP_NOMOVE | */SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		SetWindowPlacement(ghwnd, &wpPrev);
		ShowCursor(TRUE);
	}
}
