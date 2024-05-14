#pragma once
#include "window/WindowBase.h"

#ifdef __APPLE__
//#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

#define CX_FUNC_MAIN int main(int argc, const char * argv[])

#elif _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <gl/GL.h>

#define CX_FUNC_MAIN int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)

#endif

void cxRunApp();
void cxQuitApp(int exitCode = 0);

void cxMessageBox(std::wstring text);