// opengl-quick-ref.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<Windows.h>
#include "GL/glew.h"
#pragma comment (lib, "opengl32.lib")



#include "sample_1_0.h"
#include "sample_1_1.h"
#include "sample_1_2.h"
#include "sample_1_3.h"
#include "sample_1_4.h"


LRESULT CALLBACK WndProc(HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CREATE:
	{

	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

float change = 0;

void OpenGL_Draw()
{
	//设置单个顶点绘制三角形
	//sample_1_0();
	//着色器绘制三角形
	//sample_1_1();
	//着色器绘制矩形
	//sample_1_2();
	//着色器详细用法
	//sample_1_3(&change);
	//纹理贴图
	sample_1_4();
}

int main()
{
	int width = 1280;
	int height = 720;

	HWND        hWnd;
	HGLRC       hRC;
	HDC         hDC;

	HINSTANCE   hInstance;
	//第 1）步：获取HINSTANCE

	hInstance = GetModuleHandle(NULL);
	WNDCLASS    windClass;
	TCHAR szWindowName[50] = TEXT("OpenGL_Window");
	TCHAR szClassName[50] = TEXT("OpenGL_Class");

	//初始化窗口结构体
	windClass.lpszClassName = szClassName;
	windClass.lpfnWndProc = (WNDPROC)WndProc;
	windClass.hInstance = hInstance;
	windClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windClass.hbrBackground = NULL;
	windClass.lpszMenuName = NULL;
	windClass.style = 0;
	windClass.cbClsExtra = 0;
	windClass.cbWndExtra = 0;

	//注册窗口类
	if (!RegisterClass(&windClass)) return 1;

	hWnd = CreateWindowEx(0,     // Extended style
		szClassName,   // class name
		szWindowName,  // window name
		0,
		0,      //window position, x
		0,      //window position, y
		width,      //width
		height,      //height
		NULL,          // Parent window
		NULL,          // menu
		hInstance,     // instance
		NULL);         // pass this toWM_CREATE

	hDC = GetDC(hWnd);


	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, format, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	//隐藏标题栏
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_CAPTION);

	glewInit();

	glViewport(0, 0, width, height);

	BOOL quit = FALSE;


	//wglSwapLayerBuffers()


	//bef_effect_handle_t handle;
	//bef_effect_result_t ret = bef_effect_ai_create(&handle);


	MSG msg;
	float theta = 0.0f;
	// 主消息循环:
	//while (GetMessage(&msg, nullptr, 0, 0))
	//{
	//    TranslateMessage(&msg);
	//    DispatchMessage(&msg);
	//}
	unsigned char* buff = (unsigned char*)malloc(width * height * 4);
	// program main loop
	while (!quit)
	{
		// check for messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// handle or dispatch messages
			if (msg.message == WM_QUIT)
			{
				quit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// OpenGL animation code goes here		
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			OpenGL_Draw();
			SwapBuffers(hDC);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buff);
			int a = 10;
		}
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);

	return 0;
}