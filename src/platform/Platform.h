#pragma once
#include <cstdarg>
#include <iostream>


#ifdef __APPLE__
//#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

#define CX_FUNC_MAIN int main(int argc, const char * argv[])

#elif _WIN32

#define WINVER 0x0605
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "wgl.h"

#define CX_FUNC_MAIN int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)

#endif


void cxInitApp();
void cxRunApp();
void cxQuitApp(int exitCode = 0);

void cxMessageBox(std::wstring text);
void cxLog(std::wstring str, ...);

void cxGetMousePosition(int& x, int& y);