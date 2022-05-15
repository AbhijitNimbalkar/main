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
GLfloat light_XPos = 0.0f;
GLfloat light_YPos = 0.0f;
GLfloat light_ZPos = 0.0f;

bool bRotateAboutX = false;
bool bRotateAboutY = false;
bool bRotateAboutZ = false;

GLfloat light_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light_position[] = { 0.0f,0.0f,0.0f,0.0f };

///////////////////////////////

GLfloat material_ambient[24][4] = 
{ 
0.0215f,	0.1745f,	0.0215f,	1.0f,
0.329412,	0.223529,	0.027451,	1.0f,
0.0,0.0,0.0,1.0f,
0.02,0.02,0.02,1.0f,//completed first row
0.135,0.2225,0.1575,1.0f,
0.2125,0.1275,0.054,1.0f,
0.0,0.1,0.06,1.0f,
0.0,0.05,0.05,1.0f,//2nd row completed
0.05375,0.05,0.06625,1.0f,
0.25,0.25,0.25,1.0f,
0.0,0.0,0.0,1.0f,
0.0,0.05,0.0,1.0f,//3rd row completed
0.25,0.20725,0.20725,1.0f,
0.19125,0.0735,0.0225,1.0f,
0.0,0.0,0.0,1.0f,
0.05,0.0,0.0,1.0f,//4th row completed
0.1745,0.01175,0.01175,1.0f,
0.24725,0.1995,0.0745,1.0f,
0.0,0.0,0.0,1.0f,
0.05,0.05,0.05,1.0f,//5th row completed
0.1,0.18725,0.1745,1.0f,
0.19225,0.19225,0.19225,1.0f,
0.0,0.0,0.0,1.0f,
0.05,0.05,0.0,1.0f,//6th row completed
};
GLfloat material_diffuse[24][4] = 
{
0.07568f,0.61424f ,0.07568f ,1.0f,
0.780392,0.568627,0.113725,1.0f, 
0.01,0.01,0.01,1.0f,
0.01,0.01,0.01,1.0f,//completed first row
0.54,0.89,0.63,1.0f,
0.714,0.4284,0.18144,1.0f,
0.0,0.50980392,0.50980392,1.0f,
0.4,0.5,0.5,1.0f,//2nd row completed
0.18275,0.17,0.22525,1.0f,
0.4,0.4,0.4,1.0f,
0.1,0.35,0.1,1.0f,
0.4,0.5,0.4,1.0f,//3rd row completed
1.0,0.829,0.829,1.0f,
0.7038,0.27048,0.0828,1.0f,
0.5,0.0,0.0,1.0f,
0.5,0.4,0.4,1.0f,//4th row completed
0.61424,0.04136,0.04136,1.0f,
0.75164,0.60648,0.22648,1.0f,
0.55,0.55,0.55,1.0f,
0.5,0.5,0.5,1.0f,//5th row completed
0.396,0.74151,0.69102,1.0f,
0.50754,0.50754,0.50754,1.0f,
0.5,0.5,0.0,1.0f,
0.5,0.5,0.4,1.0f,// 6th row completed
};
GLfloat material_specular[24][4] = 
{ 
0.633 ,0.727811 ,0.633 ,1.0f,
0.992157,0.941176,0.807843,1.0f,
0.50,0.50,0.50,1.0f,
0.4,0.4,0.4,1.0f,//completed first row
0.316228,0.316228,0.316228,1.0f,//[2,1]
0.393548,0.271906,0.166721,1.0f,
0.50196078,0.50196078,0.50196078,1.0f,
0.04,0.7,0.7,1.0f,//2nd row completed
0.332741,0.328634,0.346435,1.0f,
0.774597,0.774597,0.774597,1.0f,
0.45,0.55,0.45,1.0f,
0.04,0.7,0.04,1.0f,//3rd row completed
0.296648,0.296648,0.296648,1.0f,
0.256777,0.137622,0.086014,1.0f,
0.7,0.6,0.6,1.0f,
0.7,0.04,0.04,1.0f,//4th row completed
0.727811,0.626959,0.626959,1.0f,
0.628281,0.555802,0.366065,1.0f,
0.70,0.70,0.70,1.0f,
0.7,0.7,0.7,1.0f,//5th row completed
0.297254,0.30829,0.306678,1.0f,
0.508273,0.508273,0.508273,1.0f,
0.60,0.60,0.50,1.0f,
0.7,0.7,0.04,1.0f,//6th row completed
};
GLfloat material_shininess[24] = 
{ 
0.6 * 128,
0.21794872 * 128,
0.25 * 128,
0.078125 * 128,//completed first row
0.1 * 128,
0.2 * 128,
0.25 * 128,
0.078125 * 128,//2nd row completed
0.3 * 128,
0.6 * 128,
0.25 * 128,
0.078125 * 128,//3rd row completed
0.088 * 128,
0.1 * 128,
0.25 * 128,
0.078125 * 128,//4th row completed
0.6 * 128,
0.4 * 128,
0.25 * 128,
0.078125 * 128,//5th row completed
0.1 * 128,
0.4 * 128,
0.25 * 128,
0.078125 * 128,//6th row complted
};

#define ARR_ROW 6
#define ARR_COL 4
GLUquadric* arrObjetcs[ARR_ROW][ARR_COL];

void updateAngle()
{
	if (bRotateAboutX == true)
	{
		if (light_XPos == 360.0f)
		{
			light_XPos = 0.0f;
		}
		light_XPos = light_XPos + 1.0f;
	}
	else if (bRotateAboutY == true)
	{
		if (light_YPos == 360.0f)
		{
			light_YPos = 0.0f;
		}
		light_YPos = light_YPos + 1.0f;
	}
	else if (bRotateAboutZ == true)
	{
		if (light_ZPos == 360.0f)
		{
			light_ZPos = 0.0f;
		}
		light_ZPos = light_ZPos + 1.0f;
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
		case 0x58://x
			bRotateAboutX = true;
			bRotateAboutY = false;
			bRotateAboutZ = false;
			light_position[0] = 0.0;
			light_position[1] = 0.0;
			light_position[2] = 0.0;
			break;
		case 0x59:// y
			bRotateAboutX = false;
			bRotateAboutY = true;
			bRotateAboutZ = false;
			light_position[0] = 0.0;
			light_position[1] = 0.0;
			light_position[2] = 0.0;
			break;
		case 0x5A:// z
			bRotateAboutX = false;
			bRotateAboutY = false;
			bRotateAboutZ = true;
			light_position[0] = 0.0;
			light_position[1] = 0.0;
			light_position[2] = 0.0;
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
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);
	glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
	resize(GLfloat(WIN_WIDTH), GLfloat(WIN_HEIGHT));

	//resize(WIN_WIDTH,WIN_HEIGHT);
}
void drawObjects(GLfloat x,GLfloat y,int rIndex,int cIndex)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();//1

	if (bRotateAboutX)
	{
		//glTranslatef(0.0f,10.0f,-12.0f);
		//glPushMatrix();//1
		glRotatef(light_XPos, 1.0f, 0.0f, 0.0f);
		light_position[1] = light_XPos;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		//glPopMatrix();//1
	}
	else if(bRotateAboutY)
	{
		//glTranslatef(10.0f, 0.0f, -12.0f);
		//glPushMatrix();//2
		glRotatef(light_YPos,0.0f , 1.0f, 0.0f);
		light_position[2] = light_YPos;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		//glPopMatrix();//1
	}
	else if (bRotateAboutZ)
	{
		//glTranslatef(10.0f, 0.0f, -12.0f);
		//glPushMatrix();//2
		glRotatef(light_ZPos,  0.0f, 0.0f, 1.0f);
		light_position[0] = light_ZPos;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		//glPopMatrix();//1
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x, y, -14.0f);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	arrObjetcs[rIndex][cIndex] = gluNewQuadric();
	gluSphere(arrObjetcs[rIndex][cIndex], 0.75, 30, 30);
	glPopMatrix();

	glPopMatrix();//2
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glFlush(); Step4. Remove the flush call and call swapbuffer instead of it

	GLfloat yPos = 5.0f;
	int numOfMaterial = 0;
	for (int i = 0; i < ARR_ROW; i++)
	{
		GLfloat xPos = -3.0f;
		for (int j = 0; j < ARR_COL; j++)
		{
		
			glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient[numOfMaterial]);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse[numOfMaterial]);
			glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular[numOfMaterial]);
			glMaterialf(GL_FRONT, GL_SHININESS, material_shininess[numOfMaterial]);

			drawObjects(xPos, yPos, i,j);
			xPos = xPos + 2.0f;
			numOfMaterial++;
		}
		yPos = yPos - 2.0f;
	}
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

	for (int i = 0;  i < ARR_ROW; i++)
	{
		for (int j = 0;j <ARR_COL;j++)
		{
			if (arrObjetcs[i][j])
			{
				gluDeleteQuadric(arrObjetcs[i][j]);
				arrObjetcs[i][j] = NULL;
			}
		}
	}
	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(gHwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(gHwnd);
	gHwnd = NULL;

}