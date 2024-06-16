#pragma once
#include <assert.h>
#include "imgui.h"
#include "DVector.h"
#include <string>
#include <map>

class DFont{
private:

    std::map<const char*, DVector<ImFont*>> Fonts;
    ImGuiIO& IO;

public:

    DFont(){}
    ~DFont(){}

    void AddFontFromMemory(const char* label = nullptr,uint8_t* fontData = nullptr, size_t size){
        assert(label == nullptr);
        this->Fonts[label] = DVector<ImFont*>();
    }

};
