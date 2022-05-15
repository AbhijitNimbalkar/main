#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define checkImageWidth 64
#define checkImageHeight 64


#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "glu32.lib")


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

HWND ghwnd = NULL;

HDC gHdc = NULL;
HGLRC ghrc = NULL;

DWORD dwStyle = 0;

WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

GLubyte checkImage[checkImageHeight][checkImageWidth][4];
GLuint texName;

void initialize();
void display();
void uninitialize();
void ToggleFullScreen(void);
void MakeCheckImage(void);;
void LoadGLTexture();
void resize(int width, int height);
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd = NULL;

	MSG msg;
	TCHAR szClassName[] = TEXT("MyOGL_FFp");

	bool bDone = false;


	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hInstance = hinstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		szClassName,
		TEXT("CheckerBoard TExture"),
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,
		100,
		100,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hinstance,
		NULL);
	ghwnd = hwnd;
	initialize();
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true)
					bDone = true;
			}
		}
	}
	uninitialize();

	return (int (msg.wParam));
}
void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1f, 30.0f);

}

void MakeCheckImage()
{
	int i, j, c;
	for (	i = 0;i<checkImageHeight;i++)
	{
		for (j = 0;j < checkImageWidth;j++)
		{
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)c;
		}
	}
}
void initialize()
{
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL ;//Step3. Add double buffer flag
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;

	gHdc = GetDC(ghwnd);
	iPixelFormatIndex = ChoosePixelFormat(gHdc, &pfd);
	if (0 == iPixelFormatIndex)
	{
		ReleaseDC(ghwnd, gHdc);
		gHdc = NULL;
	}
	if (FALSE == SetPixelFormat(gHdc, iPixelFormatIndex, &pfd))
	{
		ReleaseDC(ghwnd, gHdc);
		gHdc = NULL;
	}
	ghrc = wglCreateContext(gHdc);
	if (NULL == ghrc)
	{
		ReleaseDC(ghwnd, gHdc);
		gHdc = NULL;
	}
	if (wglMakeCurrent(gHdc, ghrc) == FALSE)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, gHdc);
		gHdc = NULL;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glShadeModel(GL_FLAT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//1st Step Enable texture 
	glEnable(GL_TEXTURE_2D);
	LoadGLTexture();
	resize(GLfloat(WIN_WIDTH), GLfloat(WIN_HEIGHT));
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
	case WM_CLOSE:
		uninitialize();
		break;
	case WM_PAINT:
		display();
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
void ToggleFullScreen(void)
{
	MONITORINFO mi;
	if (false == gbFullScreen)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle& ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
		}

	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

void uninitialize()
{
	glEnable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texName);
	if (true == gbFullScreen)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(ghwnd, gHdc);
	gHdc = NULL;

	DestroyWindow(ghwnd);
	ghwnd = NULL;

}

void LoadGLTexture()
{

	MakeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f,- 3.6f);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, -1.0f, 0.0f);//right top first

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);



	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);//right top first

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.41421f, 1.0f, -1.41421f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.41421f, -1.0f, -1.41421f);

	glEnd();
	glFlush();
}