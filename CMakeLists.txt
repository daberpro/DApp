cmake_minimum_required(VERSION 3.27.2)
project(Main VERSION 1.0.0)

add_definitions(-DNVG_NO_STB)

file(GLOB ImGui "Headers/*.cpp")
set(ICON "${CMAKE_SOURCE_DIR}/app.ico")
set(RC "${CMAKE_SOURCE_DIR}/app.rc")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "--static -static-libstdc++ -O3")
set(all_dependencies glfw3 GLEW glm)

# Generate a resource script file (.rc) containing the icon path
file(WRITE ${CMAKE_SOURCE_DIR}/app.rc "id ICON \"${ICON}\"")
add_custom_command(
    OUTPUT icon.o
    COMMAND windres -i ${RC} -J rc -O coff -o icon.o
    DEPENDS ${ICON}
)
add_executable(${PROJECT_NAME} Main.cpp ${ImGui} icon.o)

# if(WIN32)
#     set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")
# endif()
message("Check Dependencies")

foreach(package IN LISTS all_dependencies)
    find_package(${package} REQUIRED PATHS "${PROJECT_SOURCE_DIR}\\Dependencies")  
    if(${package}_FOUND)
        message("FOUND ${package} VERSION ${${package}_VERSION}")
    endif()
endforeach()

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    message("FOUND OpenGL VERSION ${${OpenGL}_VERSION}")
endif()

link_directories("${PROJECT_SOURCE_DIR}\\Dependencies\\include\\")
link_directories("${PROJECT_SOURCE_DIR}\\Dependencies\\x64\\mingw\\staticlib\\")
target_link_directories(${PROJECT_NAME} PUBLIC ${OpenGL_INCLUDE_DIR} "Headers\\")
target_link_libraries(${PROJECT_NAME} glfw GLEW::GLEW ${OPENGL_LIBRARY} user32 gdi32 Dwmapi glm::glm-header-only)
