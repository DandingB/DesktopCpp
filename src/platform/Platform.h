#pragma once
#include <cstdarg>
#include <iostream>
#include "../cx/Types.h"


#ifdef __APPLE__

#define CX_FUNC_MAIN int main(int argc, const char * argv[])

#elif _WIN32

#include <Windows.h>
#define CX_FUNC_MAIN int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)

#endif


void cxInitApp();
void cxRunApp();
void cxQuitApp(int exitCode = 0);

void cxMessageBox(std::wstring text);
void cxLog(std::wstring str, ...);

void cxGetMousePosition(float& x, float& y);

void cxShowCursor(bool show = true);
void cxSetCursor(cxCursorType type);
