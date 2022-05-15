#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

//define macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
bool bLighting = false;
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
GLUquadric* quadric = NULL;


//red
GLfloat light0_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light0_diffuse[] = { 1.0f,0.0f,0.0f,0.0f };
GLfloat light0_specular[] = { 1.0f,0.0f,0.0f,0.0f };
GLfloat light0_position[] = { 0.0f,0.0f,0.0f,0.0f };

//Green
GLfloat light1_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light1_diffuse[] = { 0.0f,1.0f,0.0f,0.0f };
GLfloat light1_specular[] = { 0.0f,1.0f,0.0f,0.0f };
GLfloat light1_position[] = { 0.0f,0.0f,0.0f,0.0f };

// blue
GLfloat light2_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light2_diffuse[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light2_specular[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light2_position[] = { 0.0f,0.0f,0.0f,0.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_Shyniness[] = { 50 };

GLfloat angle_RED = 0.0f;
GLfloat angle_GREEN = 0.0f;
GLfloat angle_BLUE = 0.0f;


void updateAngle()
{
	if (angle_RED == 360.0f)
	{
		angle_RED = 0.0f;
	}
	angle_RED = angle_RED + 1.0f;

	if (angle_GREEN == 360.0f)
	{
		angle_GREEN = 0.0f;
	}
	angle_GREEN = angle_GREEN + 1.0f;

	if (angle_BLUE == 360.0f)
	{
		angle_BLUE = 0.0f;
	}
	angle_BLUE = angle_BLUE + 1.0f;
}
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
				updateAngle();
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
		// Step1:  Remove WM_PAINT call. As we will do the painting after game loop
		/*case WM_PAINT:
		display();
		break;*/
	case WM_SIZE:
		resize(LOWORD(lparam), HIWORD(lparam));
		break;
		// step5- remove erase background
		/*case WM_ERASEBKGND:
		return 0;*/
	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
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
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_Shyniness);


	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	resize(GLfloat(WIN_WIDTH), GLfloat(WIN_HEIGHT));

	//resize(WIN_WIDTH,WIN_HEIGHT);
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glFlush(); Step4. Remove the flush call and call swapbuffer instead of it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -2.0f);
	glPushMatrix();
	gluLookAt(0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(angle_RED, 1.0f, 0.0f, 0.0f);
	light0_position[1] = angle_RED;
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle_GREEN, 0.0f, 1.0f, 0.0f);
	light1_position[2] = angle_GREEN;
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle_BLUE, 0.0f, 0.0f, 1.0f);
	light2_position[0] = angle_BLUE;
	glLightfv(GL_LIGHT1, GL_POSITION, light2_position);

	glPopMatrix();

	glPushMatrix();
	//////
	glTranslatef(0.0f, 0.0f, -3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	quadric = gluNewQuadric();
	glColor3f(1.0f, 1.0f, 1.0f);

	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();
	glPopMatrix();
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
	//glFrustum(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
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
	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}
	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(gHwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(gHwnd);
	gHwnd = NULL;

}