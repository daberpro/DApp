#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>

class Texture{
public:
    GLuint ID;
    Texture(){}
    void Load(const char* path,GLuint unit){
        glActiveTexture(GL_TEXTURE0 + unit);
        glGenTextures(1,&this->ID);
        this->Bind();
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        if(!std::filesystem::exists(path)){
            std::cout << "Could not find file in path " << path << "\n";
        }

        // stbi_set_flip_vertically_on_load(true);
        int width,height,bits;
        uint8_t* image_data = stbi_load(
            path,
            &width,
            &height,
            &bits,
            STBI_rgb_alpha
        );

        if(image_data){
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                width,
                height,
                GL_FALSE,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                image_data
            );
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        this->UnBind();
        stbi_image_free(image_data);
    }

    template <GLenum wrap = GL_REPEAT>
    void Load(uint8_t* img,GLuint unit,int size){
        glActiveTexture(GL_TEXTURE0 + unit);
        glGenTextures(1,&this->ID);
        this->Bind();
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        int width, height, bits;
        uint8_t* image_data = stbi_load_from_memory(img,size,&width,&height,&bits,STBI_rgb_alpha);

        if(wrap == GL_CLAMP_TO_BORDER){
            float borderColor[] = {0.0f,0.0f,0.0f,0.0f};
            glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
        }

        if(img){
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                width,
                height,
                GL_FALSE,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                image_data
            );
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        this->UnBind();
    }

    ~Texture(){
        glDeleteTextures(1,&this->ID);
    }

    void Bind(){
        glBindTexture(GL_TEXTURE_2D,this->ID);
    }

    void UnBind(){
        glBindTexture(GL_TEXTURE_2D,0);
    }

};