#include <windows.h>
#include <gl/GL.h>
#include <GL/GLU.h>
#include "resource.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 400

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

//callback functions -we dont call these functions
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

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
struct MyTextureData
{
	GLuint _texture_Stone;
	GLuint _texture_Kundali;
};

MyWindowParams gWinParams;
MyObjectAngles gObjectAngles;
MyTextureData gTexData;

void initialize();
void update();
void display();
void uninitialize();
void resize(int width, int height);
void ToggleFullScreen(void);
int LoadGLTexture(GLuint resID, GLuint* texture);

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
	gWinParams._bFullScreen  = gWinParams._bEscapeKeyIsPressed = gWinParams._bActiveWindow= FALSE;

	gObjectAngles._angleCube = 0.0f;
	gObjectAngles._anglePyramid = 0.0f;

	gTexData._texture_Kundali = 0;
	gTexData._texture_Stone = 0;
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

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
	WNDCLASSEX wndClassExObj;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("MYOGL_FFP");
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

	gWinParams._hwnd= hwnd;

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

void resize(int width, int height)
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//1st Step Enable texture 
	glEnable(GL_TEXTURE_2D);
	LoadGLTexture(IDB_BITMAP1, &gTexData._texture_Stone);
	LoadGLTexture(IDB_BITMAP2,&gTexData._texture_Kundali);
	resize(GLfloat(WIN_WIDTH), GLfloat(WIN_HEIGHT));
}
int LoadGLTexture(GLuint resID,GLuint* texture )
{
	int iStatus = FALSE;
	HBITMAP hBitmap;
	BITMAP bmp;

	//2nd Step -Get the image data
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(resID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		iStatus = TRUE;
		GetObject(hBitmap, sizeof(bmp), &bmp);
		//3rd step - Generate texture
		glGenTextures(1, texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		//step 4- Bind texture
		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		DeleteObject(hBitmap);
		hBitmap = NULL;
	}
	return iStatus;
}
void uninitialize()
{
	if (gTexData._texture_Kundali)
	{
		glDeleteTextures(1, &gTexData._texture_Kundali);
		gTexData._texture_Kundali = 0;
	}

	if (gTexData._texture_Stone)
	{
		glDeleteTextures(1, &gTexData._texture_Stone);
		gTexData._texture_Stone = 0;
	}

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
	glRotatef(gObjectAngles._anglePyramid,0.0f,1.0f,0.0f);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, gTexData._texture_Stone);
	glBegin(GL_TRIANGLES);
	//Front face
	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	//right face
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	//back face
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//left face
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	//***************************************CUBE******************************************
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f,0.0f,-6.0f);
	glScalef(0.75f,0.75f,0.75f);
	glRotatef(gObjectAngles._angleCube,1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,gTexData._texture_Kundali);
	glBegin(GL_QUADS);
	
	//Front face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);


	//Right face
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	//Back face
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);


	//Left face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);


	//Top face
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	//bottom face
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);




	glEnd();
	SwapBuffers(gWinParams._hDC);
	 
}