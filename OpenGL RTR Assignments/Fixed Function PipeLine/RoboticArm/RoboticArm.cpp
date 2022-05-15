#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hIcon = ::LoadIcon(hInstance,IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor(hInstance,IDC_ARROW);
	wndClass.hIconSm = ::LoadIcon(hInstance, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = TEXT("Robotic Arm");
	wndClass.lpszMenuName = NULL;
	wndClass.style =  CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	

	CreateWindowEx(WS_OVERLAPPEDWINDOW, TEXT("RoboticArm"), TEXT("RoboticArm"),
	
	);

	RegisterClassEx(wndClass);
	
}