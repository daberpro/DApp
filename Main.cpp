#include "Headers/DApp.h"
// #include "Headers/DFont.h"
// #include <opencv2/opencv.hpp>
// #include "Image/AppIcon/include.h"


class App : public DApp{
public:
    App(): DApp(){}
private: 
    Texture img1;
    ImGuiWindowFlags HomePageFlags = ImGuiWindowFlags_NoDocking;
    int WinPosX,WinPosY;
    int svg1 = 0;
    
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
        // cv::Mat img = cv::imread(__current_path+"/../Image/a.png");
        // this->img1.Load((__current_path+"/../Image/a.png").c_str(),0);
    }

    void DButton(const char* label,const float& width, const float& height){
        ImGui::PushFont(this->MaterialDesignFontIcon);
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 defaultButtonTextAlign = style.ButtonTextAlign;
        ImVec2 fontSize = ImGui::CalcTextSize(label);
        style.ButtonTextAlign = ImVec2(0.5f,0.5f + (fontSize.y/height)/2.0f);
        ImGui::Button(label,{width,height});
        style.ButtonTextAlign = defaultButtonTextAlign;
        ImGui::PopFont();
    }

    void DHomePage(){
        ImGui::Begin("HomePage",nullptr,0);
        ImGui::PushFont(this->PoppinsFontText25Px);
        ImGui::Text("Testing Text");
        ImGui::PopFont();
        ImGui::End();
    }

    void DContent(const float& width, const float& height){
        const ImGuiViewport* viewport = ImGui::GetWindowViewport();
        auto [x,y] = viewport->WorkPos;
        ImGui::SetNextWindowPos({x+70.0f,y});
        ImGui::SetNextWindowSize({width,height});
        ImGui::Begin("MainContainer",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);      
        ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());
        ImGuiID dockspace_id = ImGui::GetID("MainContentDockSpace");
        ImGui::DockSpace(dockspace_id, {0.0f,0.0f},ImGuiDockNodeFlags_None);
        this->DHomePage();
        ImGui::End();
    }
    
    void RenderContent(ImGuiIO& io) override {
        glfwGetWindowPos(this->window,&this->WinPosX,&this->WinPosY);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        this->HomePageFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        this->HomePageFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // ImGui::SetNextWindowPos({(float)this->WinPosX,(float)this->WinPosY + this->titleBarHeight});
        // ImGui::SetNextWindowSize({io.DisplaySize.x,io.DisplaySize.y - titleBarHeight});
        ImGui::Begin("##",nullptr,this->HomePageFlags);

        ImVec2 parentSize = ImGui::GetItemRectSize();
        ImGui::Columns(2,nullptr,false);
        ImGui::SetColumnWidth(0,70.0f);

        ImGui::PushStyleColor(ImGuiCol_ButtonActive,{1.0f,1.0f,1.0f,0.3f});
        ImGui::PushStyleColor(ImGuiCol_Button,{1.0f,1.0f,1.0f,0.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,{1.0f,1.0f,1.0f,0.15f});
        this->DButton(ICON_MD_HOME,70.0f,70.0f);
        this->DButton(ICON_MD_SETTINGS,70.0f,70.0f);
        this->DButton(ICON_MD_INFO,70.0f,70.0f);
        this->DButton(ICON_MD_PERSON,70.0f,70.0f);
        ImGui::PopStyleColor(3);

        ImGui::NextColumn();
        ImGui::SetColumnWidth(1,parentSize.x - 70.0f);
        this->DContent(parentSize.x - 70.0f,viewport->WorkSize.y);

        ImGui::Columns(1);

        ImGui::End();
        ImGui::PopStyleVar(5);
    }
};

int main()
{
    // DFont f;

    App TestApp;
    TestApp.customTitleBar = true;
    TestApp.width = 600.0f;
    TestApp.height = 400.0f;
    TestApp.title = "Test App";
    TestApp.showDemoWindow = false;
    TestApp.Start();
    return EXIT_SUCCESS;
}