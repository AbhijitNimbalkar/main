#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib,"opengl32.lib");
#pragma comment(lib,"glu32.lib");

//define macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 800

#define _COLOR_ORANGE_	glColor3f(1.0f,0.58f,0.0f);
#define _COLOR_GREEN_	glColor3f(0.0f,1.0f,0.0f);
#define _COLOR_WHITE_	glColor3f(1.0f,1.0f,1.0f);

//prototype's
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);
//function prototype
void initialize();
void uninitialize();
void display();
void resize(int, int);
void ToggleFullScreen(void);

//Global section
HWND gHwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPre = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

GLfloat deltaIfNotFullScreen = -3.0f;
GLfloat gfZDelta = -4.0f;

static int year = 0;
static int day = 0;

GLUquadric* quadric = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{

	MSG msg;
	WNDCLASSEX wndClassExObj;
	TCHAR szClassName[] = __TEXT("My Double Buffer Window");
	HBRUSH hbrush = NULL;
	HWND hwnd = NULL;
	bool bDone = false;

	//intialize wndclassex members
	wndClassExObj.cbSize = sizeof(wndClassExObj);
	wndClassExObj.lpszClassName = szClassName;
	wndClassExObj.hIcon = ::LoadIcon(hInstance, IDI_APPLICATION);
	wndClassExObj.hIconSm = ::LoadIcon(hInstance, IDI_APPLICATION);
	wndClassExObj.hInstance = hInstance;

	// create brush of blue color
	//hbrush = ::CreateSolidBrush(RGB(0,0,0));
	wndClassExObj.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClassExObj.lpfnWndProc = WndProc;
	wndClassExObj.cbClsExtra = 0;
	wndClassExObj.cbWndExtra = 0;
	wndClassExObj.hCursor = ::LoadCursor(hInstance, IDC_HAND);
	wndClassExObj.lpszMenuName = NULL;
	wndClassExObj.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	// Register class
	RegisterClassEx(&wndClassExObj);

	// Create window now
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		TEXT("My Application Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0, 0, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

	gHwnd = hwnd;

	//initialize
	initialize();

	ShowWindow(hwnd, SW_SHOW);
	//to come the window over the top
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	while (false == bDone)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow = true)
			{
				if (gbEscapeKeyIsPressed == true)
				{
					bDone = true;
				}
				display();//step2. Call display in game loop
			}
		}
	}

	uninitialize();
	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wparam) == 0)
		{
			gbActiveWindow = true;
		}
		else
		{
			gbActiveWindow = false;
		}
		break;
	case WM_SIZE:
		resize(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
			break;

		case 'F':
		case 'f':
			if (false == gbFullScreen)
			{
				ToggleFullScreen();
				gbFullScreen = true;
			}
			else
			{
				ToggleFullScreen();
				gbFullScreen = false;
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void ToggleFullScreen(void)
{
	MONITORINFO mi;
	if (false == gbFullScreen)
	{
		dwStyle = GetWindowLong(gHwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(gHwnd, &wpPre) && GetMonitorInfo(MonitorFromWindow(gHwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(gHwnd, GWL_STYLE, dwStyle& ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(gHwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
		}

	}
	else
	{
		SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gHwnd, &wpPre);
		SetWindowPos(gHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}
void initialize()
{
	void resize(int, int);
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;//Step3. Add double buffer flag
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	ghdc = GetDC(gHwnd);
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (0 == iPixelFormatIndex)
	{
		ReleaseDC(gHwnd, ghdc);
		ghdc = NULL;
	}
	if (FALSE == SetPixelFormat(ghdc, iPixelFormatIndex, &pfd))
	{
		ReleaseDC(gHwnd, ghdc);
		ghdc = NULL;
	}
	ghrc = wglCreateContext(ghdc);
	if (NULL == ghrc)
	{
		ReleaseDC(gHwnd, ghdc);
		ghdc = NULL;
	}
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(gHwnd, ghdc);
		ghdc = NULL;
	}

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	resize(GLfloat(WIN_WIDTH), GLfloat(WIN_HEIGHT));
}
void DrawI(GLfloat xTranslateDelta)
{
	glLoadIdentity();

	//translate z = -6, x = -4
	glTranslatef(-2.5f+ xTranslateDelta, 0.0f, gbFullScreen ? gfZDelta : gfZDelta + deltaIfNotFullScreen);
	//declare line width
	glLineWidth(5.0f);

	glBegin(GL_LINES);
	_COLOR_ORANGE_
	glVertex3f(0.0f,1.5f,0.0f);
	_COLOR_GREEN_
	glVertex3f(0.0f,-1.5f,0.0f);
	glEnd();
}
void DrawN()
{
	// get back to original and translate again
	glLoadIdentity();
	//translate z = -0.2, x = -4
	glTranslatef(-2.0f, 0.0f, gbFullScreen ? gfZDelta : gfZDelta + deltaIfNotFullScreen);
	glBegin(GL_LINES);
	_COLOR_GREEN_
		glVertex3f(0.0f, -1.5f, 0.0f);
	_COLOR_ORANGE_
		glVertex3f(0.0f, 1.5f, 0.0f);

	_COLOR_ORANGE_
		glVertex3f(0.0f, 1.5f, 0.0f);
	_COLOR_GREEN_
		glVertex3f(1.0f, -1.5f, 0.0f);

	_COLOR_GREEN_
		glVertex3f(1.0f, -1.5f, 0.0f);
	_COLOR_ORANGE_
		glVertex3f(1.0f, 1.5f, 0.0f);
	glEnd();
}
void DrawD()
{
	// get back to original and translate again
	glLoadIdentity();
	//translate z = -0.5, x = -4
	glTranslatef(-0.5f, 0.0f, gbFullScreen ? gfZDelta : gfZDelta + deltaIfNotFullScreen);
	glBegin(GL_LINES);
	_COLOR_ORANGE_
		glVertex3f(0.0f, 1.5f, 0.0f);
	_COLOR_GREEN_
		glVertex3f(0.0f, -1.5f, 0.0f);

	_COLOR_ORANGE_
		glVertex3f(-0.10f, 1.5f, 0.0f);
		glVertex3f(1.0f, 1.5f, 0.0f);

		glVertex3f(1.0f, 1.5f, 0.0f);
	_COLOR_GREEN_
		glVertex3f(1.0f, -1.5f, 0.0f);

		glVertex3f(1.0f, -1.5f, 0.0f);
		glVertex3f(-0.10f, -1.5f, 0.0f);

	glEnd();

}
void DrawA()
{
	glLoadIdentity();
	glTranslatef(1.5f, 0.0f, gbFullScreen? gfZDelta : gfZDelta+deltaIfNotFullScreen);

	glBegin(GL_LINES);
	_COLOR_GREEN_
		glVertex3f(0.0f,-1.5f,0.0f);
	_COLOR_ORANGE_
		glVertex3f(0.5f, 1.5f, 0.0f);
		
		glVertex3f(0.5f, 1.5f, 0.0f);
	_COLOR_GREEN_
		glVertex3f(1.0f, -1.5f, 0.0f);
		
	glLineWidth(1.0f);
	_COLOR_ORANGE_
	glVertex3f(0.25f, 0.01f, 0.0f);
	glVertex3f(0.75f, 0.01f, 0.0f);
	
	_COLOR_WHITE_
	glVertex3f(0.25f, 0.0f, 0.0f);
	glVertex3f(0.75f, 0.0f, 0.0f);

	_COLOR_GREEN_
	glVertex3f(0.25f, -0.01f, 0.0f);
	glVertex3f(0.75f, -0.01f, 0.0f);

	glEnd();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glFlush(); Step4. Remove the flush call and call swapbuffer instead of it
	glMatrixMode(GL_MODELVIEW);
	DrawI(0.0f);
	DrawN();
	DrawD();
	DrawI(3.5f);// as left starts from -2.5f, so added delta of 3.5 into 2.5 so that it translate to right by 1.0
	DrawA();
	SwapBuffers(ghdc);
}
void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}
void uninitialize()
{
	if (true == gbFullScreen)
	{
		dwStyle = GetWindowLong(gHwnd, GWL_STYLE);
		SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gHwnd, &wpPre);
		SetWindowPos(gHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(gHwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(gHwnd);
	gHwnd = NULL;
	gluDeleteQuadric(quadric);
}