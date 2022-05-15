#pragma once

//header guard 
#ifndef __HEADAER_H
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib,"opengl32.lib");

//define macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//prototype's
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wparam,LPARAM lparam);
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
WINDOWPLACEMENT wpPre = {sizeof(WINDOWPLACEMENT)};

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;
enum eColor
{
	eRed = 1,
	eGreen = 2,
	eBlue =3,
	eGray = 4,
	eOrange= 5,
	eWhite = 6,
	eCyan =7,
	eMagenta=8,
	eYellow = 9,
	eBrown=10
};
#endif // __HEADAER_H
#pragma 
