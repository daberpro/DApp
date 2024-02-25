#include "App.h"

static void OnResize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    // ...

    InvalidateRect(hWnd, NULL, TRUE);
}

static LRESULT CALLBACK WindowProcedure(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_NCHITTEST:
    {
        // Expand the hit test area for resizing
        const int borderWidth = 8; // Adjust this value to control the hit test area size

        POINTS mousePos = MAKEPOINTS(lParam);
        POINT clientMousePos = {mousePos.x, mousePos.y};
        ScreenToClient(Window, &clientMousePos);

        RECT windowRect;
        GetClientRect(Window, &windowRect);

        if (clientMousePos.y >= windowRect.bottom - borderWidth)
        {
            if (clientMousePos.x <= borderWidth)
                return HTBOTTOMLEFT;
            else if (clientMousePos.x >= windowRect.right - borderWidth)
                return HTBOTTOMRIGHT;
            else
                return HTBOTTOM;
        }
        else if (clientMousePos.y <= borderWidth)
        {
            if (clientMousePos.x <= borderWidth)
                return HTTOPLEFT;
            else if (clientMousePos.x >= windowRect.right - borderWidth)
                return HTTOPRIGHT;
            else
                return HTTOP;
        }
        else if (clientMousePos.x <= borderWidth)
        {
            return HTLEFT;
        }
        else if (clientMousePos.x >= windowRect.right - borderWidth)
        {
            return HTRIGHT;
        }

        break;
    }
    case WM_NCCALCSIZE:
    {
        // Remove the window's standard sizing border
        if (wParam == TRUE && lParam != NULL)
        {
            NCCALCSIZE_PARAMS *pParams = reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
            pParams->rgrc[0].top += 0;
            pParams->rgrc[0].right -= 1;
            pParams->rgrc[0].bottom -= 1;
            pParams->rgrc[0].left += 1;
        }
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(Window);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        OnResize(Window, Message, wParam, lParam);
        break;
    default:
        return DefWindowProc(Window, Message, wParam, lParam);
        break;
    }
    return 0;
}

App::App(HINSTANCE Instance, int nCmdShow)
    : AppInstance(Instance), CLASS_NAME(_T("DAPP"))
{

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcedure;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = this->AppInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 0, 0)));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = this->CLASS_NAME;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
                   _T("Call to RegisterClassEx failed!"),
                   _T("Windows Desktop Guided Tour"),
                   0);
    }
    DWORD Style = WS_POPUP | WS_THICKFRAME;
    RECT rect;
    int width = 400, height = 400;
    rect.left = 200;
    rect.top = 200;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, Style, false);

    this->Window = CreateWindowEx(
        0,
        CLASS_NAME,
        _T("Test App"),
        Style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        this->AppInstance,
        NULL);

    SetWindowLong(this->Window, GWL_STYLE, 0);
    ShowWindow(this->Window, nCmdShow);
    UpdateWindow(this->Window);
}

App::~App()
{
    UnregisterClass(this->CLASS_NAME, this->AppInstance);
}

bool App::ProcessMessages(MSG &msg)
{
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}