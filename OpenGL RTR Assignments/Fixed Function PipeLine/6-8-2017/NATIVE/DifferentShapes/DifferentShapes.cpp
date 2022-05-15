#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib,"opengl32.lib")

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

int glWIDTH = 0;
int glHEIGHT = 0;
enum eViewPortPos
{
	eTOP_LEFT =0,
	eTOP_CENTER =1,
	eTOP_RIGHT =2,
	eBOTTOM_LEFT= 3,
	eBOTTOM_CENTER = 4,
	eBOTTOM_RIGHT =5
};

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Step6 - Remove resize call from initialize but it will require in directx hence commented code    
	resize(WIN_WIDTH, WIN_HEIGHT);
}
GLsizei vpHeight = 0.0f;
GLsizei vpWidth = 0.0f;
void SetViewPort(eViewPortPos ePos)
{
	GLfloat ptX = 0.0f;
	GLfloat ptY = 0.0f;

	switch (ePos)
	{
	case eTOP_LEFT:
		ptX = 0.0f;
		ptY = (GLfloat)(glHEIGHT / 2);
		vpHeight = (GLsizei)(glHEIGHT / 2);
		vpWidth = (GLfloat)(glWIDTH / 3);
		break;
	case eTOP_CENTER:
		ptX = (GLfloat)glWIDTH / 3;
		ptY = (GLfloat)(glHEIGHT / 2);
		vpHeight = (GLsizei)(glHEIGHT/ 2);
		vpWidth = (GLsizei)(glWIDTH / 3);
		break;
	case eTOP_RIGHT:
		ptX = (GLfloat)(2*(glWIDTH / 3));
		ptY = (GLfloat)(glHEIGHT / 2);
		vpHeight = (GLsizei)(glHEIGHT / 2);
		vpWidth = (GLsizei)(glWIDTH / 3);
		break;
	case eBOTTOM_LEFT:
		ptX = 0.0f;
		ptY = 0.0f;
		vpHeight = (GLsizei)(glHEIGHT/2);
		vpWidth = (GLsizei)(glWIDTH / 3);
		break;
	case eBOTTOM_CENTER:
		ptX = (GLfloat)glWIDTH / 3;
		ptY = 0.0f;
		vpHeight = (GLsizei)(glHEIGHT / 2);
		vpWidth = (GLsizei)(glWIDTH / 3);
		
		break;
	case eBOTTOM_RIGHT:
		ptX = (GLfloat)(2*(glWIDTH / 3));
		ptY = 0.0f;
		vpWidth = (GLsizei)(glWIDTH / 3);
		vpHeight = (GLsizei)(glHEIGHT / 2);
		break;
	}
	glViewport(ptX, ptY, vpWidth, vpHeight);
}
GLfloat gdelta = 0.2f;
GLfloat gY = -0.5f;
GLfloat gX = -0.5f;
void DrawRectangle1(eViewPortPos ePos)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	SetViewPort(ePos);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	gY = -0.5f;
	for (int j = 0; j < 4; j++)
	{
		gX = -0.5f;
		for (int i = 0; i < 4; i++)
		{
			glVertex3f(gX, gY, 0.0f);
			gX = gX + gdelta;
		}
		gY = gY + gdelta;
	}
	glEnd();
}
void DrawRectangle3(eViewPortPos ePos)
{
	gX = -0.5;
	gY = -0.5f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	SetViewPort(ePos);
	glPointSize(3.0f);
	glBegin(GL_LINES);
	//
	//glVertex3f(gX,gY,0.0f);
	//glVertex3f((0.2f*3)+gX, gY, 0.0f);

	//glVertex3f((0.2f * 3)+gX, gY, 0.0f);
	//glVertex3f((0.2f * 3) + gX, (0.2f * 3)+gY, 0.0f);

	//glVertex3f((0.2f * 3) + gX, (0.2f * 3) + gY, 0.0f);
	//glVertex3f(gX, (0.2f * 3)+gY, 0.0f);
	//
	//glVertex3f(gX, (0.2f * 3) + gY, 0.0f);
	//glVertex3f(gX, gY, 0.0f);
	//// lines in between

	//glVertex3f(gX+(0.2f*2),gY,0);
	//glVertex3f(gX + (0.2f * 2), gY+(0.2f * 3), 0);

	//glVertex3f(gX + (0.2f*1), gY, 0);
	//glVertex3f(gX + (0.2f*1), gY + (0.2f * 3), 0);

	//glVertex3f(gX,gY+ (0.2f * 1),0);
	//glVertex3f(gX+(0.2f *3), gY + (0.2f * 1), 0);

	//glVertex3f(gX, gY + (0.2f * 2), 0);
	//glVertex3f(gX + (0.2f * 3), gY + (0.2f * 2), 0);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0;j < 3; j++)
		{
			glVertex3f(gX,gY,0);
			gY += gdelta;
			glVertex3f(gX, gY, 0);
		}
		gY = -0.5f;
		gX += gdelta;
	}
	gY = -0.5f;
	gX = -0.5f;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0;j < 3; j++)
		{
			glVertex3f(gX, gY, 0);
			gX += gdelta;
			glVertex3f(gX, gY, 0);
		}
		gX = -0.5f;
		gY += gdelta;
	}
	glEnd();

}
void DrawRectangle2(eViewPortPos ePos)
{
	gX = -0.5;
	gY = 0.1f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	SetViewPort(ePos);
	glPointSize(3.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0;j < 3; j++)
		{
			glVertex3f(gX, gY, 0);
			gY -= gdelta;
			glVertex3f(gX, gY, 0);
		}
		gY = 0.1f;
		gX += gdelta;
	}
	gY = 0.1f;
	gX = 0.1f;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0;j < 3; j++)
		{
			glVertex3f(gX, gY, 0);
			gX -= gdelta;
			glVertex3f(gX, gY, 0);
		}
		gX = 0.1f;
		gY -= gdelta;
	}
	gY = 0.1f;
	gX = -0.5f;
	for (int k = 1; k < 4; k++)
	{
		gY -= (gdelta*k);
		glVertex3f(gX, gY, 0);
		gY = 0.1f;
		gX += (gdelta*k);
		glVertex3f(gX, gY, 0.0f);
		gX = -0.5f;	
	}

	gY = -0.5f;
	gX = 0.1f;
	for (int k = 1; k < 3; k++)
	{
		gX -= (gdelta*k);
		glVertex3f(gX, gY, 0);
		gX = 0.1f;
		gY += (gdelta*k);
		glVertex3f(gX, gY, 0.0f);
		gY = -0.5f;
	}

	glEnd();
}
void DrawRectangle5(eViewPortPos ePos)
{
	gX = -0.5;
	gY = -0.5f;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	SetViewPort(ePos);
	glPointSize(3.0f);
	glBegin(GL_LINES);

	glVertex3f(-0.5f, -0.5f, 0);
	glVertex3f(0.1f, -0.5f, 0);

	glVertex3f(0.1f, -0.5f, 0);
	glVertex3f(0.1f, 0.1f, 0);

	glVertex3f(0.1f, 0.1f, 0);
	glVertex3f(-0.5f, 0.1f, 0);

	glVertex3f(-0.5f, 0.1f, 0);
	glVertex3f(-0.5f, -0.5f, 0);
	

	glVertex3f(-0.5f, 0.1f, 0);
	glVertex3f(0.1f, -0.5f, 0);

	glVertex3f(-0.5f, 0.1f, 0);
	glVertex3f(0.1f, -0.3f, 0);

	glVertex3f(-0.5f, 0.1f, 0);
	glVertex3f(0.1f, -0.1f, 0);

	glVertex3f(-0.5f, 0.1f, 0);
	glVertex3f(-0.1f, -0.5f, 0);
	
	glVertex3f(-0.5f, 0.1f, 0);
	glVertex3f(-0.3f, -0.5f, 0);
	glEnd();

}
void DrawRectangle6(eViewPortPos ePos)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	SetViewPort(ePos);

	glBegin(GL_QUADS);	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f,-0.5f,0);
	glVertex3f(-0.5f, 0.1f, 0);
	glVertex3f(-0.3f, 0.1f, 0);
	glVertex3f(-0.3f, -0.5f, 0);
	glVertex3f(-0.5f, -0.5f, 0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.3f, -0.5f, 0);
	glVertex3f(-0.3f, 0.1f, 0);
	glVertex3f(-0.1f, 0.1f, 0);
	glVertex3f(-0.1f, -0.5f, 0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.1f, -0.5f, 0);
	glVertex3f(-0.1f, 0.1f, 0);
	glVertex3f(0.1f, 0.1f, 0);
	glVertex3f(0.1f, -0.5f, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-0.5f,-0.3f,0.0f);
	glVertex3f(0.1f, -0.3f, 0.0f);

	glVertex3f(-0.5f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);


	glVertex3f(-0.3f, 0.1f, 0.0f);
	glVertex3f(-0.3f, -0.5f, 0.0f);

	glVertex3f(-0.1f, 0.1f, 0.0f);
	glVertex3f(-0.1f, -0.5f, 0.0f);

	glEnd();

}
void DrawRectangle4(eViewPortPos ePos)
{
	gX = -0.5;
	gY = 0.1f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	SetViewPort(ePos);
	glPointSize(3.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0;j < 3; j++)
		{
			glVertex3f(gX, gY, 0);
			gY -= gdelta;
			glVertex3f(gX, gY, 0);
		}
		gY = 0.1f;
		gX += gdelta;
	}
	gY = 0.1f;
	gX = 0.1f;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0;j < 3; j++)
		{
			glVertex3f(gX, gY, 0);
			gX -= gdelta;
			glVertex3f(gX, gY, 0);
		}
		gX = 0.1f;
		gY -= gdelta;
	}
	gY = 0.1f;
	gX = -0.5f;
	for (int k = 1; k < 4; k++)
	{
		gY -= (gdelta*k);
		glVertex3f(gX, gY, 0);
		gY = 0.1f;
		gX += (gdelta*k);
		glVertex3f(gX, gY, 0.0f);
		gX = -0.5f;
	}

	gY = -0.5f;
	gX = 0.1f;
	for (int k = 1; k < 3; k++)
	{
		gX -= (gdelta*k);
		glVertex3f(gX, gY, 0);
		gX = 0.1f;
		gY += (gdelta*k);
		glVertex3f(gX, gY, 0.0f);
		gY = -0.5f;
	}

	glEnd();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawRectangle1(eViewPortPos::eTOP_LEFT);
	DrawRectangle2(eViewPortPos::eTOP_CENTER);
	DrawRectangle3(eViewPortPos::eTOP_RIGHT);
	DrawRectangle4(eViewPortPos::eBOTTOM_LEFT);
	DrawRectangle5(eViewPortPos::eBOTTOM_CENTER);
	DrawRectangle6(eViewPortPos::eBOTTOM_RIGHT);
	SwapBuffers(ghdc);
}
void resize(int width, int height)
{
	glWIDTH = width;
	glHEIGHT = height;
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
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