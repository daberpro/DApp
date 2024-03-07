#include "Headers/DApp.h"
#include <memory>

Texture img1;
class App : public DApp{
public:
    App(): DApp(){}
private: 

    void OnSetup(ImGuiIO& io) override {
        // set theme for dear imgui
        ImGui::StyleColorsDark();
        img1.Load((__current_path+"/../Image/a.png").c_str(),0);
    }

    void RenderContent() override {
        ImGui::Begin("Test");
        ImGui::Image((void*)(intptr_t)(img1.ID),ImVec2(200.0f,200.0f));
        ImGui::End();
    }
};

int main()
{

    App TestApp;
    TestApp.customTitleBar = true;
    TestApp.width = 400.0f;
    TestApp.height = 400.0f;
    TestApp.title = "TestApp App";
    TestApp.Start();
    return EXIT_SUCCESS;
}