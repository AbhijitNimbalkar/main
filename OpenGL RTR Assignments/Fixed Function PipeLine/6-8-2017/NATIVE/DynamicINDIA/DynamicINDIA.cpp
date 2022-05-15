#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

//define macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define _COLOR_ORANGE_	glColor3f(1.0f,0.50f,0.0f);
#define _COLOR_GREEN_	glColor3f(0.0f,1.0f,0.0f);
#define _COLOR_WHITE_	glColor3f(1.0f,1.0f,1.0f);
#define _Z_DEPTH_	-4.0f

//prototype's
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);
//function prototype
void initialize();
void uninitialize();
void display();
void resize(GLfloat, GLfloat);
void ToggleFullScreen(void);

//Global section
HWND gHwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPre = { sizeof(WINDOWPLACEMENT) };

// full screen variables
bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

int negativeXPointI = 3000;
int yPointN = 100;
int yPoinI = 300;
int positiveXPointA = 200;
int glColorOrangeDelta = 1;
int glColorGreenDelta = 1;
int glColorRedDelta = 1;
int A_Pt1XDelta = -350;
int gA_delta = -300;

bool bIsCompleteIX = false;
bool bIsCompleteIY = false;
bool bIsCompleteN = false;
bool bIsDComplete = false;
bool bIsAComplete = false;
GLfloat gxDeltaI = 0.0f;
GLfloat gxDeltaA = 0.0f;
GLfloat gyDeltaN = 0.0f;
GLfloat gyDeltaI = 0.0f;
GLfloat glColorOrangeVal = 0.0f;
GLfloat glColorGreenVal = 0.0f;
GLfloat  glColorRedVal   = 0.0f;
GLfloat glAPt1_Val = -3.5f;
GLfloat gA_Val = 0.0f;

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
	void resize(GLfloat, GLfloat);
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
void AnimateIoverXAxis(bool bOverXAxis)
{
	if(bOverXAxis)
	{
		if (negativeXPointI != 2500)
		{
			gxDeltaI = negativeXPointI = negativeXPointI - 1;
			gxDeltaI = gxDeltaI / 1000;
			bIsCompleteIX = false;
		}
		else
		{
			bIsCompleteIX = true;
		}
		
	}
	else
	{
		if (yPoinI != 0)
		{	
			gyDeltaI = yPoinI = yPoinI -1;
			gyDeltaI = gyDeltaI / 100;
			bIsCompleteIY = false;
		}
		else
		{
			bIsCompleteIY = true;
		}

	}
	
}
void DrawI(bool bOverXAxis)
{
	glLoadIdentity();
	if(bOverXAxis)
		glTranslatef(-gxDeltaI, 0.0f, _Z_DEPTH_);
	else
		glTranslatef(1.0f, -gyDeltaI, _Z_DEPTH_);
	glLineWidth(5.0f);

	glBegin(GL_LINES);
	_COLOR_ORANGE_
		glVertex3f(0.0f, 1.5f, 0.0f);
	_COLOR_GREEN_
		glVertex3f(0.0f, -1.5f, 0.0f);
	glEnd();
}
void AnimateN()
{
	if (yPointN != 0)
	{
		gyDeltaN = yPointN = yPointN - 1;
		gyDeltaN = gyDeltaN / 100;
		bIsCompleteN = false;
	}
	else
	{
		bIsCompleteN = true;
	}
}
void DrawN()
{
	// get back to original and translate again
	glLoadIdentity();
	glTranslatef(-2.0f, gyDeltaN, _Z_DEPTH_);
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

void updateColors()
{
	
	//update red color
	if (glColorRedDelta < 100)
	{
		glColorRedVal = glColorRedDelta = glColorRedDelta + 1;
		glColorRedVal = glColorRedVal / 100;
	}

	//update green color
	if (glColorGreenDelta < 100)
	{
		glColorGreenVal = glColorGreenDelta  = glColorGreenDelta + 1;
		glColorGreenVal = glColorGreenVal / 100;
	}
		
	//update orange color
	if (glColorOrangeDelta < 50)
	{
		glColorOrangeVal  = glColorOrangeDelta = glColorOrangeDelta + 1;
		glColorOrangeVal = glColorOrangeVal / 100;
	}
	if (glColorRedDelta == 100 && glColorGreenDelta == 100 && glColorOrangeDelta == 50)
	{
		bIsDComplete = true;
	}
}
void DrawD()
{
	glLoadIdentity();
	glTranslatef(-0.5f, 0.0f, _Z_DEPTH_);
	glBegin(GL_LINES);

	glColor3f(glColorRedVal, glColorOrangeVal , 0.0f);//orange
	glVertex3f(0.0f, 1.5f, 0.0f);

	glColor3f(0.0f, glColorGreenVal , 0.0f);//green
	glVertex3f(0.0f, -1.5f, 0.0f);

	glColor3f(glColorRedVal, glColorOrangeVal, 0.0f);//orange
	
	glVertex3f(-0.10f, 1.5f, 0.0f);
	glVertex3f(1.0f, 1.5f, 0.0f);
	glVertex3f(1.0f, 1.5f, 0.0f);
	
	glColor3f(0.0f, glColorGreenVal, 0.0f);//green
	
	glVertex3f(1.0f, -1.5f, 0.0f);
	glVertex3f(1.0f, -1.5f, 0.0f);
	glVertex3f(-0.10f, -1.5f, 0.0f);

	glEnd();

}

void AnimateA()
{
	if (positiveXPointA != 150)
	{
		gxDeltaA = positiveXPointA = positiveXPointA - 1;
		gxDeltaA = gxDeltaA / 100;	
	}
	else
	{
		bIsAComplete = true;
	}
}

void AnimateA_()
{
	if (gA_delta != 150)
	{
		gA_Val = gA_delta = gA_delta + 1;
		gA_Val = gA_Val / 100;
	}
	else
	{
		if (A_Pt1XDelta != 25)
		{
			glAPt1_Val = A_Pt1XDelta = A_Pt1XDelta + 1;
			glAPt1_Val = glAPt1_Val / 100;
		}
	}
	
}
void DrawA_()
{
	glLoadIdentity();
	glTranslatef(gA_Val, 0.0f, _Z_DEPTH_);
	
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	_COLOR_ORANGE_
	glVertex3f(glAPt1_Val, 0.01f, 0.0f);
	glVertex3f(0.75, 0.01f, 0.0f);

	_COLOR_WHITE_
	glVertex3f(glAPt1_Val, 0.0f, 0.0f);
	glVertex3f(0.75, 0.0f, 0.0f);

	_COLOR_GREEN_
	glVertex3f(glAPt1_Val, -0.01f, 0.0f);
	glVertex3f(0.75, -0.01f, 0.0f);
	glEnd();
}
void DrawA()
{
	glLoadIdentity();
	glTranslatef(gxDeltaA, 0.0f, _Z_DEPTH_);

	glBegin(GL_LINES);
	_COLOR_GREEN_
		glVertex3f(0.0f, -1.5f, 0.0f);
	_COLOR_ORANGE_
		glVertex3f(0.5f, 1.5f, 0.0f);

	glVertex3f(0.5f, 1.5f, 0.0f);
	_COLOR_GREEN_
		glVertex3f(1.0f, -1.5f, 0.0f);
	glEnd();
}
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	if (gbFullScreen)
	{
		AnimateIoverXAxis(true);
		DrawI(true);
		if (bIsCompleteIX)
		{
			AnimateN();
			DrawN();
			if (bIsCompleteN)
			{
				updateColors();
				DrawD();
				if (bIsDComplete)
				{
					AnimateIoverXAxis(false);
					DrawI(false);
					if (bIsCompleteIY)
					{
						AnimateA();
						DrawA();
						if (bIsAComplete)
						{
							AnimateA_();
							DrawA_();
						}
					}
				}
			}
		}
	}
	
	SwapBuffers(ghdc);
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
}