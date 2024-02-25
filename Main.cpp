#include <iostream>
#include <GL/glew.h>
#include "Headers/imgui.h"
#include "Headers/imgui_impl_glfw.h"
#include "Headers/imgui_impl_opengl3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include "Headers/FileReader.h"
#include "Headers/Shader.h"
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <dwmapi.h>
#include <windowsx.h>
#include "Headers/App.h"

static void OnResize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    InvalidateRect(hWnd, NULL, TRUE);
}

ImGuiMouseSource GetMouseSourceFromMessageExtraInfo()
{
    LPARAM extra_info = ::GetMessageExtraInfo();
    if ((extra_info & 0xFFFFFF80) == 0xFF515700)
        return ImGuiMouseSource_Pen;
    if ((extra_info & 0xFFFFFF80) == 0xFF515780)
        return ImGuiMouseSource_TouchScreen;
    return ImGuiMouseSource_Mouse;
}

LONG_PTR prevProc;

static LRESULT CALLBACK WindowProcedure(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_MOUSEMOVE: case WM_NCMOUSEMOVE:
    case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK: case WM_LBUTTONUP:
    case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK: case WM_RBUTTONUP:
    case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK: case WM_MBUTTONUP:
    case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK: case WM_XBUTTONUP:
        ImGui::GetIO().AddMouseSourceEvent(GetMouseSourceFromMessageExtraInfo());
        break;
    case WM_NCHITTEST:
    {
        // Expand the hit test area for resizing
        const int borderWidth = 11; // Adjust this value to control the hit test area size

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

        // if(clientMousePos.x >= windowRect.left 
        // && clientMousePos.x <= (windowRect.right - windowRect.left)
        // && clientMousePos.y >= windowRect.top
        // && clientMousePos.y <= 20){
        //     return HTCAPTION;
        // }

        return HTCLIENT;
    }
    case WM_NCPAINT:
    {
        // Prevent the non-client area from being painted
        return 0;
    }
    case WM_NCCALCSIZE:
    {
        // Remove the window's standard sizing border
        if (wParam == TRUE && lParam != FALSE)
        {
            NCCALCSIZE_PARAMS *pParams = reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
            pParams->rgrc[0].top += 0;
            pParams->rgrc[0].right -= 0;
            pParams->rgrc[0].bottom -= 0;
            pParams->rgrc[0].left += 0;
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
    }
    return ::CallWindowProc((WNDPROC) prevProc, Window, Message, wParam, lParam);
}

void disableTitlebar(GLFWwindow *window)
{
    HWND hWnd = glfwGetWin32Window(window);
    SetWindowLongPtr(hWnd, GWL_STYLE, 0);
    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);
    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;
    prevProc = GetWindowLongPtr(hWnd,GWLP_WNDPROC);
    (WNDPROC) SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcedure));
    SetWindowPos(hWnd, NULL, windowRect.left, windowRect.top, width, height, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

int main()
{

    GLFWwindow *app;

    if (!glfwInit())
    {
        std::cout << "Failed to init glfw\n";
        exit(EXIT_FAILURE);
    }

    app = glfwCreateWindow(400, 400, "OpenGL", nullptr, nullptr);
    disableTitlebar(app);
    glfwMakeContextCurrent(app);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 3.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(app, true);
    ImGui_ImplOpenGL3_Init();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to init glew\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << "\n";

    GLuint shader = createShader(
        __current_path + "/../Shader/StandardVertexShader.glsl",
        __current_path + "/../Shader/StandardFragmentShader.glsl");
    glUseProgram(shader);

    int display_w, display_h;
    glfwGetWindowPos(app, &display_w, &display_h);
    while (!glfwWindowShouldClose(app))
    {
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Rendering
        // Creating a titlebar
        {
            ImGui::Begin("Test",nullptr,ImGuiWindowFlags_NoTitleBar);
            ImGui::Text("Hello World\n");
            ImGui::End();
        }

        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(app);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}