#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment(lib,"opengl32.lib");
#pragma comment(lib,"glu32.lib");

//define macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

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
GLfloat angleCube= 0.0f;
//decleration of matrices
GLfloat myIdentityMatrix[16]; 
GLfloat myScaleMatrix[16];
GLfloat myTranslationmatrix[16];
//rotation matrix
GLfloat myXRotationMatrix[16];
GLfloat myYRotationMatrix[16];
GLfloat myZRotationMatrix[16];


void updateAngle()
{
	angleCube = angleCube + 0.01f;
	if (angleCube == 360)
	{
		angleCube = 0.1f;
	}
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
	
	glClearDepth(1.0f);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//initialize matrix
	myIdentityMatrix[0] = 1.0f; 
	myIdentityMatrix[1] = 0.0f;
	myIdentityMatrix[2] = 0.0f;
	myIdentityMatrix[3] = 0.0f;

	myIdentityMatrix[4] = 0.0f;
	myIdentityMatrix[5] = 1.0f;
	myIdentityMatrix[6] = 0.0f;
	myIdentityMatrix[7] = 0.0f;

	myIdentityMatrix[8] = 0.0f;
	myIdentityMatrix[9] = 0.0f;
	myIdentityMatrix[10] = 1.0f;
	myIdentityMatrix[11] = 0.0f;

	myIdentityMatrix[12] = 0.0f;
	myIdentityMatrix[13] = 0.0f;
	myIdentityMatrix[14] = 0.0f;
	myIdentityMatrix[15] = 1.0f;

	//scale matrix 

	myScaleMatrix[0] = 0.75f;
	myScaleMatrix[1] = 0.0f;
	myScaleMatrix[2] = 0.0f;
	myScaleMatrix[3] = 0.0f;

	myScaleMatrix[4] = 0.0f;
	myScaleMatrix[5] = 0.75f;
	myScaleMatrix[6] = 0.0f;
	myScaleMatrix[7] = 0.0f;

	myScaleMatrix[8] = 0.0f;
	myScaleMatrix[9] = 0.0f;
	myScaleMatrix[10] = 0.75f;
	myScaleMatrix[11] = 0.0f;

	myScaleMatrix[12] = 0.0f;
	myScaleMatrix[13] = 0.0f;
	myScaleMatrix[14] = 0.0f;
	myScaleMatrix[15] = 1.0f;


	//Translation matrix
	myTranslationmatrix[0] = 1.0f;
	myTranslationmatrix[1] = 0.0f;
	myTranslationmatrix[2] = 0.0f;
	myTranslationmatrix[3] = 0.0f;

	myTranslationmatrix[4] = 0.0f;
	myTranslationmatrix[5] = 1.0f;
	myTranslationmatrix[6] = 0.0f;
	myTranslationmatrix[7] = 0.0f;

	myTranslationmatrix[8] = 0.0f;
	myTranslationmatrix[9] = 0.0f;
	myTranslationmatrix[10] = 1.0f;
	myTranslationmatrix[11] = 0.0f;

	myTranslationmatrix[12] = 0.0f;
	myTranslationmatrix[13] = 0.0f;
	myTranslationmatrix[14] = -6.0f;
	myTranslationmatrix[15] = 1.0f;
	resize(GLfloat(WIN_WIDTH), GLfloat(WIN_HEIGHT));

	//resize(WIN_WIDTH,WIN_HEIGHT);
}
void DrawFrontFace()
{
	//1st 
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

}
void DrawRightFace()
{
	//2nd 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
}
void DrawBackFace()
{
	//3rd 
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
}
void DrawLeftFace()
{
	//4th 
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
}
void DrawTopFace()
{
	//4th 
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
}
void DrawBottomFace()
{
	//4th 
	glColor3f(1.0f, 0.0f, 1.0f);
	
	glVertex3f(1.0f, -1.0f, -1.0f);

	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	
}

void ModifyRotationMatrix()
{
	//XRotation matrix
	myXRotationMatrix[0] = 1.0f;
	myXRotationMatrix[1] = 0.0f;
	myXRotationMatrix[2] = 0.0f;
	myXRotationMatrix[3] = 0.0f;

	myXRotationMatrix[4] = 0.0f;
	myXRotationMatrix[5] = cos(angleCube);
	myXRotationMatrix[6] = -sin(angleCube);
	myXRotationMatrix[7] = 0.0f;

	myXRotationMatrix[8] = 0.0f;
	myXRotationMatrix[9] = sin(angleCube);
	myXRotationMatrix[10] = cos(angleCube);
	myXRotationMatrix[11] = 0.0f;

	myXRotationMatrix[12] = 0.0f;
	myXRotationMatrix[13] = 0.0f;
	myXRotationMatrix[14] = 0.0f;
	myXRotationMatrix[15] = 1.0f;

	//YRotationMatrix
	
	myYRotationMatrix[0] = cos(angleCube);
	myYRotationMatrix[1] = 0.0f;
	myYRotationMatrix[2] = sin(angleCube);
	myYRotationMatrix[3] = 0.0f;

	myYRotationMatrix[4] = 0.0f;
	myYRotationMatrix[5] = 1.0f;
	myYRotationMatrix[6] = 0.0f;
	myYRotationMatrix[7] = 0.0f;

	myYRotationMatrix[8] = -sin(angleCube);
	myYRotationMatrix[9] = 0.0f;
	myYRotationMatrix[10] = cos(angleCube);
	myYRotationMatrix[11] = 0.0f;

	myYRotationMatrix[12] = 0.0f;
	myYRotationMatrix[13] = 0.0f;
	myYRotationMatrix[14] = 0.0f;
	myYRotationMatrix[15] = 1.0f;

	//ZRotation
	myZRotationMatrix[0] = cos(angleCube);
	myZRotationMatrix[1] = -sin(angleCube);
	myZRotationMatrix[2] = 0.0f;
	myZRotationMatrix[3] = 0.0f;

	myZRotationMatrix[4] = sin(angleCube);
	myZRotationMatrix[5] = cos(angleCube);
	myZRotationMatrix[6] = 0.0f;
	myZRotationMatrix[7] = 0.0f;

	myZRotationMatrix[8] = 0.0f;
	myZRotationMatrix[9] = 0.0f;
	myZRotationMatrix[10] = 1.0f;
	myZRotationMatrix[11] = 0.0f;

	myZRotationMatrix[12] = 0.0f;
	myZRotationMatrix[13] = 0.0f;
	myZRotationMatrix[14] = 0.0f;
	myZRotationMatrix[15] = 1.0f;

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//glFlush(); Step4. Remove the flush call and call swapbuffer instead of it
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glLoadMatrixf(myIdentityMatrix);
	glMultMatrixf(myTranslationmatrix);
	//glTranslatef(0.0f, 0.0f, -6.0f);

	//scale by 0.75
	glMultMatrixf(myScaleMatrix);
	//glRotatef(angleCube,1.0f,1.0f,1.0f);
	ModifyRotationMatrix();
	glMultMatrixf(myXRotationMatrix);
	glMultMatrixf(myYRotationMatrix);
	glMultMatrixf(myZRotationMatrix);

	glBegin(GL_QUADS);


	DrawFrontFace();
	DrawRightFace();
	DrawBackFace();
	DrawLeftFace();
	DrawTopFace();
	DrawBottomFace();
	glEnd();

	SwapBuffers(ghdc);
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glLoadMatrixf(myIdentityMatrix);
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height,0.1f,100.0f);

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
	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(gHwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(gHwnd);
	gHwnd = NULL;

}