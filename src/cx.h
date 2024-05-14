#pragma once
#include <cstdarg>
#include <iostream>

#include "cx/WindowBase.h"

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

#define CX_FUNC_MAIN int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)

#endif

void cxInitApp();
void cxRunApp();
void cxQuitApp(int exitCode = 0);

void cxMessageBox(std::wstring text);

void cxLog(std::wstring str, ...);