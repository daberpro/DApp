#pragma once
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <dwmapi.h>
#include <tchar.h>
class App{
protected:

   HINSTANCE AppInstance;
   HWND Window;
   LPCSTR CLASS_NAME;

public:
    App(HINSTANCE Instance, int nCmdShow);
    ~App();
    bool ProcessMessages(MSG& msg);

    App(const App& app) = delete;
    void operator=(const App& app) = delete;
};
