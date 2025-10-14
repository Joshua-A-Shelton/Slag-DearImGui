# Slag-DearImGUI #
This is a simple library to add SLAG as a dear-imgui renderer backend. You may use it either by following the dear-imgui philosophy and just copying imgui_impl_slag.h and imgui_impl_slag.cpp into your project files along the other dear-imgui files, or you may include everything in one swell swoop using cmake

```cmake
include(FetchContent)

#enable one or more of these options

#OPTION(SLAG_DEAR_IMGUI_ANDROID_BACKEND "Use Android backend for dear imgui windowing" OFF)
#OPTION(SLAG_DEAR_IMGUI_GLFW_BACKEND "Use GLFW backend for dear imgui windowing" OFF)
#OPTION(SLAG_DEAR_IMGUI_OSX_BACKEND "Use OSX backend for dear imgui windowing" OFF)
#OPTION(SLAG_DEAR_IMGUI_SDL2_BACKEND "Use SDL2 backend for dear imgui windowing" OFF)
#OPTION(SLAG_DEAR_IMGUI_SDL3_BACKEND "Use SDL3 backend for dear imgui windowing" OFF)
#OPTION(SLAG_DEAR_IMGUI_WIN32_BACKEND "Use Win32 backend for dear imgui windowing" OFF)

fetchcontent_declare(
        Slag
        GIT_REPOSITORY https://github.com/Joshua-A-Shelton/Slag-DearImGui.git
        GIT_TAG #COMMIT TAG GOES HERE
        GIT_SHALLOW TRUE
)
fetchcontent_makeavailable(Slag)

add_executable(YOUR_EXECUTABLE ${SLAG_IMGUI_SOURCE} ...)
target_include_directories(YOUR_EXECUTABLE PRIVATE ${SLAG_IMGUI_INCLUDE_DIRS})
```

You will still need to link to whatever backends you choose (SDL, GLFW, etc), including SLAG, but you will not need to include dear-imgui or the imgui backend files

An example project has been provided to help get you set up actually using this library in your own projects