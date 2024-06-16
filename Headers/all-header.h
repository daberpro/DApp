#pragma once
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include <GL/glew.h>
#include "../Fonts/Poppins-Regular.h"
#include "../Fonts/MaterialIcons-Regular.h"
#include "../Fonts/IconsMaterialDesign.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include "FileReader.h"
#include "Shader.h"
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <windowsx.h>
#include <functional>
#include "Texture.h"