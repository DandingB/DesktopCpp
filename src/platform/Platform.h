#pragma once
#include <cstdarg>
#include <iostream>
#include "../cx/Types.h"


#ifdef __APPLE__

#define CX_FUNC_MAIN int main(int argc, const char * argv[])
#define SYSTEM_FONT L"Helvetica"

#elif _WIN32

#include <Windows.h>
#define CX_FUNC_MAIN int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#define SYSTEM_FONT L"Segoe UI"

#endif


void cxInitApp();
void cxRunApp();
void cxQuitApp(int exitCode = 0);

void cxMessageBox(std::wstring text);
void cxLog(std::wstring str, ...);

void cxGetMousePosition(float& x, float& y);

void cxShowCursor(bool show = true);
void cxSetCursor(cxCursorType type);

void cxRegisterFontFile(std::wstring file);