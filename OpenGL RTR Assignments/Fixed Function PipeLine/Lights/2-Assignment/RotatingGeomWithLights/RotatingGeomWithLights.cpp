#include <windows.h>
#include <gl/GL.h>
#include <GL/GLU.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 400

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

//callback functions -we dont call these functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct MyWindowParams
{
	HWND _hwnd;
	HDC _hDC;
	HGLRC _hrc;
	bool _bFullScreen;
	bool _bEscapeKeyIsPressed;
	bool _bActiveWindow;
	DWORD _dwStyle;
	WINDOWPLACEMENT _wpPrev;
};

struct MyObjectAngles
{
	GLfloat _anglePyramid;
	GLfloat _angleCube;
};
MyWindowParams gWinParams;
MyObjectAngles gObjectAngles;

void initialize();
void update();
void display();
void uninitialize();
void resize(GLfloat width, GLfloat height);
void ToggleFullScreen(void);
bool bLighting = false;

GLfloat light_ambient[] = { 0.5f,0.5f,0.5f,1.0f };
GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light_position[] = { 0.0f,0.0f,1.0f,0.0f };

void update()
{
	gObjectAngles._anglePyramid = gObjectAngles._anglePyramid + 0.03f;

	if (gObjectAngles._anglePyramid >= 360.0f)
	{
		gObjectAngles._anglePyramid = gObjectAngles._anglePyramid - 360.0f;
	}
	gObjectAngles._angleCube = gObjectAngles._angleCube - 0.03f;
	if (gObjectAngles._angleCube < 0.0f)
	{
		gObjectAngles._angleCube = gObjectAngles._angleCube + 360.0f;
	}
}

void initializeData()
{
	// window params initialization
	gWinParams._hDC = NULL;
	gWinParams._hrc = NULL;
	gWinParams._hwnd = NULL;
	gWinParams._dwStyle = 0;
	gWinParams._wpPrev = { sizeof(WINDOWPLACEMENT) };
	gWinParams._bFullScreen = gWinParams._bEscapeKeyIsPressed = gWinParams._bActiveWindow = FALSE;

	gObjectAngles._angleCube = 0.0f;
	gObjectAngles._anglePyramid = 0.0f;
}

void ToggleFullScreen(void)
{
	MONITORINFO mi;
	if (false == gWinParams._bFullScreen)
	{
		gWinParams._dwStyle = GetWindowLong(gWinParams._hwnd, GWL_STYLE);
		if (gWinParams._dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(gWinParams._hwnd, &gWinParams._wpPrev) && GetMonitorInfo(MonitorFromWindow(gWinParams._hwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(gWinParams._hwnd, GWL_STYLE, gWinParams._dwStyle& ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(gWinParams._hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
		}

	}
	else
	{
		SetWindowLong(gWinParams._hwnd, GWL_STYLE, gWinParams._dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gWinParams._hwnd, &gWinParams._wpPrev);
		SetWindowPos(gWinParams._hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndClassExObj;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("MYOGL_Lights");
	bool bDone = false;

	initializeData();// initialize structure members

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

	gWinParams._hwnd = hwnd;

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
			if (gWinParams._bActiveWindow = true)
			{
				if (gWinParams._bEscapeKeyIsPressed == true)
				{
					bDone = true;

				}
				update();
				display();//step2. Call display in game loop
			}
		}
	}

	uninitialize();
	return ((int)msg.wParam);

}

void resize(GLfloat width, GLfloat height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wparam) == 0)
		{

			gWinParams._bActiveWindow = true;
		}
		else
		{
			gWinParams._bActiveWindow = false;
		}
		break;
	case WM_SIZE:
		resize(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			gWinParams._bEscapeKeyIsPressed = true;
			break;
		case 0x4C:
			if (bLighting == false)
			{
				bLighting = true;
				glEnable(GL_LIGHTING);
			}
			else
			{
				glDisable(GL_LIGHTING);
				bLighting = false;
			}
			break;
		case 0x46:
			if (false == gWinParams._bFullScreen)
			{
				ToggleFullScreen();
				gWinParams._bFullScreen = true;
			}
			else
			{
				ToggleFullScreen();
				gWinParams._bFullScreen = false;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		uninitialize();
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
void initialize()
{
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
	pfd.cDepthBits = 24;
	gWinParams._hDC = GetDC(gWinParams._hwnd);
	iPixelFormatIndex = ChoosePixelFormat(gWinParams._hDC, &pfd);
	if (0 == iPixelFormatIndex)
	{
		ReleaseDC(gWinParams._hwnd, gWinParams._hDC);
		gWinParams._hDC = NULL;
	}
	if (FALSE == SetPixelFormat(gWinParams._hDC, iPixelFormatIndex, &pfd))
	{
		ReleaseDC(gWinParams._hwnd, gWinParams._hDC);
		gWinParams._hDC = NULL;
	}
	gWinParams._hrc = wglCreateContext(gWinParams._hDC);
	if (NULL == gWinParams._hrc)
	{
		ReleaseDC(gWinParams._hwnd, gWinParams._hDC);
		gWinParams._hDC = NULL;
	}
	if (wglMakeCurrent(gWinParams._hDC, gWinParams._hrc) == FALSE)
	{
		wglDeleteContext(gWinParams._hrc);
		gWinParams._hrc = NULL;
		ReleaseDC(gWinParams._hwnd, gWinParams._hDC);
		gWinParams._hDC = NULL;
	}
	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LIGHT0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	resize(GLfloat(WIN_WIDTH), GLfloat(WIN_HEIGHT));
}
void uninitialize()
{
	if (true == gWinParams._bFullScreen)
	{
		gWinParams._dwStyle = GetWindowLong(gWinParams._hwnd, GWL_STYLE);
		SetWindowLong(gWinParams._hwnd, GWL_STYLE, gWinParams._dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gWinParams._hwnd, &gWinParams._wpPrev);
		SetWindowPos(gWinParams._hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(gWinParams._hrc);
	gWinParams._hrc = NULL;

	ReleaseDC(gWinParams._hwnd, gWinParams._hDC);
	gWinParams._hDC = NULL;

	DestroyWindow(gWinParams._hwnd);
	gWinParams._hwnd = NULL;

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//***********************************Pyramid**************************************
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glRotatef(gObjectAngles._anglePyramid, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	//Front face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	//right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	//back face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//left face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	//***************************************CUBE******************************************
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 0.0f, -6.0f);
	glScalef(0.75f, 0.75f, 0.75f);
	glRotatef(gObjectAngles._angleCube, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);

	//Front face
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	//Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	//Back face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	//Left face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//Top face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	//bottom face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();
	SwapBuffers(gWinParams._hDC);

}