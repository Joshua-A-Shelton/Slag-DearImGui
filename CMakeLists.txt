cmake_minimum_required(VERSION 3.31)
project(Slag_DearImGUI)

set(CMAKE_CXX_STANDARD 20)

OPTION(SLAG_DEAR_IMGUI_ANDROID_BACKEND "Use Android backend for dear imgui windowing" OFF)
OPTION(SLAG_DEAR_IMGUI_GLFW_BACKEND "Use GLFW backend for dear imgui windowing" OFF)
OPTION(SLAG_DEAR_IMGUI_OSX_BACKEND "Use OSX backend for dear imgui windowing" OFF)
OPTION(SLAG_DEAR_IMGUI_SDL2_BACKEND "Use SDL2 backend for dear imgui windowing" OFF)
OPTION(SLAG_DEAR_IMGUI_SDL3_BACKEND "Use SDL3 backend for dear imgui windowing" OFF)
OPTION(SLAG_DEAR_IMGUI_WIN32_BACKEND "Use Win32 backend for dear imgui windowing" OFF)


include(FetchContent)
find_package(imgui QUIET)
if(NOT imgui_FOUND)
    fetchcontent_declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui
            GIT_TAG v1.92.3-docking
            GIT_SHALLOW TRUE
            OVERRIDE_FIND_PACKAGE
    )
    fetchcontent_makeavailable(imgui)
    find_package(imgui REQUIRED)
endif()

set(SLAG_IMGUI_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/slagIMGUI/imgui_impl_slag.cpp" "${imgui_SOURCE_DIR}/imgui.cpp" "${imgui_SOURCE_DIR}/imgui_demo.cpp" "${imgui_SOURCE_DIR}/imgui_draw.cpp" "${imgui_SOURCE_DIR}/imgui_tables.cpp" "${imgui_SOURCE_DIR}/imgui_widgets.cpp")

if(SLAG_DEAR_IMGUI_ANDROID_BACKEND)
    list(APPEND SLAG_IMGUI_SOURCE "${imgui_SOURCE_DIR}/backends/imgui_impl_android.cpp")
endif ()

if(SLAG_DEAR_IMGUI_GLFW_BACKEND)
    list(APPEND SLAG_IMGUI_SOURCE "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
endif ()

if(SLAG_DEAR_IMGUI_OSX_BACKEND)
    list(APPEND SLAG_IMGUI_SOURCE "${imgui_SOURCE_DIR}/backends/imgui_impl_osx.cpp")
endif ()

if(SLAG_DEAR_IMGUI_SDL2_BACKEND)
    list(APPEND SLAG_IMGUI_SOURCE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp")
endif ()

if(SLAG_DEAR_IMGUI_SDL3_BACKEND)
    list(APPEND SLAG_IMGUI_SOURCE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp")
endif ()

if(SLAG_DEAR_IMGUI_WIN32_BACKEND)
    list(APPEND SLAG_IMGUI_SOURCE "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp")
endif ()

set(SLAG_IMGUI_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${imgui_SOURCE_DIR}")


if(PROJECT_IS_TOP_LEVEL)

    find_package(Slag QUIET)
    if(NOT Slag_FOUND)
        fetchcontent_declare(
                Slag
                GIT_REPOSITORY https://github.com/Joshua-A-Shelton/Slag.git
                GIT_TAG origin/master
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE
        )
        fetchcontent_makeavailable(Slag)
        find_package(Slag REQUIRED)
    endif()

    find_package(SDL3 QUIET)
    if(NOT SDL3_FOUND)
        fetchcontent_declare(
                SDL3
                GIT_REPOSITORY https://github.com/libsdl-org/SDL
                GIT_TAG release-3.2.24
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE
        )
        fetchcontent_makeavailable(SDL3)
        find_package(SDL3 REQUIRED)
    endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
    add_executable(Slag_DearImGUI_Example
            example/SlagImGuiExample.cpp
            ${SLAG_IMGUI_SOURCE}
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    )
    target_include_directories(Slag_DearImGUI_Example PRIVATE ${SLAG_IMGUI_INCLUDE_DIRS})
    target_link_libraries(Slag_DearImGUI_Example Slag SDL3::SDL3)
endif ()
