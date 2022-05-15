#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

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
void LoadMeshData();
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
GLfloat light_ambient[] = { 0.3f,0.7f,0.1f,1.0f };
GLfloat light_diffuse[] = { 0.3f,0.7f,0.1f,1.0f };
GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light_position[] = { 0.0f,1.0f,1.0f,0.0f };

GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shinyness[] = { 50.0f };
GLfloat angle1 = 0.0f;

FILE *g_fp_meshfile = NULL;
FILE *g_fp_logfile = NULL;

#define NR_POINT_COORDS 3
#define NR_TEXTURE_COORDS 2
#define NR_NORMAL_COORDS 3
#define NR_FACE_TOKENS 3
#define BUFFER_SIZE 256
char line[BUFFER_SIZE] = { 0 };

#define START_ANGLE_POS 0.0f
#define END_ANGLE_POS 360.0f
#define MONKEYHEAD_ANGLE_INCREMENT 1.0f

GLfloat g_rotate = 0.0f;
std::vector<std::vector<int>> g_face_tri, g_face_texture, g_face_normals;
std::vector<std::vector<float>> g_vertices;

std::vector<std::vector<float>> g_texture;

std::vector<std::vector<float>> g_normals;

#define MONKEYHEAED_X_TRANSLATE 0.0f
#define MONKEYHEAED_Y_TRANSLATE 0.0f
#define MONKEYHEAED_Z_TRANSLATE -70.0f


#define MONKEYHEAED_X_SCALE_FACTOR 1.5f
#define MONKEYHEAED_Y_SCALE_FACTOR 1.5f
#define MONKEYHEAED_Z_SCALE_FACTOR 1.5f



void updateAngle()
{
	g_rotate = g_rotate + MONKEYHEAD_ANGLE_INCREMENT;

	
	if (g_rotate >= END_ANGLE_POS)
	{
		g_rotate = START_ANGLE_POS;
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

	/*g_fp_logfile = fopen_s("MONKEYHEAD.LOG", "W");
	if (!g_fp_logfile)
		uninitialize();*/
		 
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
	LoadMeshData();
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	glMaterialfv(GL_FRONT,GL_SPECULAR,material_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,material_shinyness);


	glEnable(GL_LIGHT0);


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
	glTranslatef(MONKEYHEAED_X_TRANSLATE, MONKEYHEAED_Y_TRANSLATE, MONKEYHEAED_Z_TRANSLATE);
	glRotatef(g_rotate, 0.0f, 1.0f, 0.0f);
	glScalef(MONKEYHEAED_X_SCALE_FACTOR, MONKEYHEAED_Y_SCALE_FACTOR, MONKEYHEAED_Z_SCALE_FACTOR);

	glFrontFace(GL_CCW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < g_face_tri.size(); i++)
	{
		glBegin(GL_TRIANGLES);
		for (int j =0; j <g_face_tri[i].size();j++)
		{
			int vi = g_face_tri[i][j] -1;
			glVertex3f(g_vertices[vi][0], g_vertices[vi][1], g_vertices[vi][2]);
		}
		glEnd();
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

void LoadMeshData()
{
	fopen_s(&g_fp_meshfile,"Scarecrow.OBJ", "r");
	if (!g_fp_meshfile)
		uninitialize();
	char* sep_space = " ";
	char* sep_fslash = "/";
	char* first_token = NULL;
	char* token = NULL;
	char* face_tokens[NR_FACE_TOKENS];
	long nr_tokens = 0;
	char * token_vertex_index = NULL;
	char * token_texture_index = NULL;
	char * token_normal_index = NULL;
	while (fgets(line,BUFFER_SIZE,g_fp_meshfile)!= NULL)
	{
		char *next_token;
		first_token = strtok_s(line, sep_space, &next_token);
		if (strcmp(first_token,"v") == 0)
		{
			std::vector<float> vec_point_coord(NR_POINT_COORDS);
			for (int i = 0 ;i != NR_POINT_COORDS;i++)
			{
				vec_point_coord[i] = atof(strtok_s(NULL,sep_space, &next_token));
			}
			g_vertices.push_back(vec_point_coord);
		}
		else if (strcmp(first_token,"vt") ==  0)
		{
			std::vector<float> vec_texture_coord(NR_TEXTURE_COORDS);

			for (int i = 0; i != NR_TEXTURE_COORDS; i++)
			{
				vec_texture_coord[i] = atof(strtok_s(NULL,sep_space, &next_token));
			}
			g_texture.push_back(vec_texture_coord);
		}
		else if (strcmp(first_token,"vn") == 0)
		{
			std::vector<float> vec_normal_coord(NR_NORMAL_COORDS);
			for (int i=0; i != NR_NORMAL_COORDS; i++)
			{
				vec_normal_coord[i] = atof(strtok_s(NULL,sep_space,&next_token));
			}
			g_normals.push_back(vec_normal_coord);
		}
		else if (strcmp(first_token,"f") == 0)
		{
			std::vector<int> triangle_vertex_indices(3), texture_vertex_indices(3), normal_vertex_indices(3);
			memset((void*)face_tokens,0,NR_FACE_TOKENS);
			nr_tokens = 0;

			while (token = strtok_s(NULL,sep_space, &next_token))
			{
				if (strlen(token) < 3)
				{
					break;
				}
				face_tokens[nr_tokens] = token;
				nr_tokens++;
			}
			for (int i = 0; i != NR_FACE_TOKENS; i++)
			{
				token_vertex_index = strtok_s(face_tokens[i],sep_fslash, &next_token);
				token_texture_index = strtok_s(NULL,sep_fslash,&next_token);
				token_normal_index = strtok_s(NULL, sep_fslash, &next_token);
				triangle_vertex_indices[i] = atoi(token_vertex_index);
				if(token_normal_index != 0)
					normal_vertex_indices[i] = atoi(token_normal_index);
				if (token_normal_index != 0)
					texture_vertex_indices[i] = atoi(token_texture_index);
				
			}
			g_face_tri.push_back(triangle_vertex_indices);
			if(texture_vertex_indices.size()>0)
				g_face_texture.push_back(texture_vertex_indices);
			if (g_face_normals.size()>0)
				g_face_normals.push_back(normal_vertex_indices);
		}
		memset((void*)line, (int)'\0', BUFFER_SIZE);
	}
	fclose(g_fp_meshfile);
	g_fp_meshfile = NULL;
	//fprintf(g_fp_logfile,"g_vertices:%llu g_texture:llu g_normals:%llu  g_face_tri:%llu\n",g_vertices.size(),g_texture.size(),g_face_tri.size());
}