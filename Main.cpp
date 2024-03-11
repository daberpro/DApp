#include "Headers/DApp.h"
// #include "Image/AppIcon/include.h"

class App : public DApp{
public:
    App(): DApp(){}
private: 
    Texture img1;
    ImGuiWindowFlags HomePageFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    int WinPosX,WinPosY;

    void OnSetup(ImGuiIO& io) override {

        // // app icon
        // GLFWimage icon[TOTAL_APP_ICONS];
        // for(uint32_t i = 0; i < TOTAL_APP_ICONS; i++){
        //     icon[i].pixels = stbi_load_from_memory(
        //         AppIcon[i].data(),
        //         AppIcon[i].size(),
        //         &icon[i].width,
        //         &icon[i].height,
        //         nullptr,
        //         STBI_rgb_alpha
        //     );
        // }
        // glfwSetWindowIcon(this->window,TOTAL_APP_ICONS,icon);
        // for(uint32_t i = 0; i < TOTAL_APP_ICONS; i++){
        //     stbi_image_free(icon[i].pixels);
        // }

        // set theme for dear imgui
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        this->img1.Load((__current_path+"/../Image/a.png").c_str(),0);
    }
    
    void RenderContent(ImGuiIO& io) override {
        glfwGetWindowPos(this->window,&this->WinPosX,&this->WinPosY);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::SetNextWindowPos({(float)this->WinPosX,(float)this->WinPosY + this->titleBarHeight});
        ImGui::SetNextWindowSize({io.DisplaySize.x,io.DisplaySize.y - titleBarHeight});
        ImGui::Begin("HomePage",nullptr,this->HomePageFlags);
            
        ImGui::Columns(2,"panel",false);
        ImGui::SetColumnWidth(0,100.0f);

        ImGui::PushFont(this->MaterialDesignFontIcon);
        ImGui::Button(ICON_MD_HOME, ImVec2(100.0f, 100.0f));
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(ICON_MD_HOME).x) * 0.5f);
        ImGui::Text(ICON_MD_HOME);
        ImGui::PopFont();
        
        ImGui::NextColumn();
        ImGui::SetColumnWidth(1,io.DisplaySize.x - 100.0f);

        ImGui::Columns(1);

        ImGui::End();
        ImGui::PopStyleVar(5);
    }
};

int main()
{

    App TestApp;
    TestApp.customTitleBar = true;
    TestApp.width = 400.0f;
    TestApp.height = 400.0f;
    TestApp.title = "TestApp App";
    TestApp.showDemoWindow = false;
    TestApp.Start();
    return EXIT_SUCCESS;
}