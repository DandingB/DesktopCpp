#include "platform/Platform.h"

#include "cx/WindowBase.h"
#include "cx/WindowContainer.h"

void cxInitApp();
void cxRunApp();
void cxQuitApp(int exitCode = 0);

void cxMessageBox(std::wstring text);

void cxLog(std::wstring str, ...);