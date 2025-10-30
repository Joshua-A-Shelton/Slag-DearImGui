// Copyright (c) 2025 Joshua Shelton
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#include <imgui.h>
#include <iostream>
#include <backends/imgui_impl_sdl3.h>
#include <slagIMGUI/imgui_impl_slag.h>

class ImGuiFrameResources: public slag::FrameResources
{
public:
    slag::DescriptorPool* descriptorPool = nullptr;
    slag::CommandBuffer* commandBuffer = nullptr;
    slag::Semaphore* frameFinishedSemaphore = nullptr;
    ImGuiFrameResources()
    {
        descriptorPool = slag::DescriptorPool::newDescriptorPool();
        commandBuffer = slag::CommandBuffer::newCommandBuffer(slag::GPUQueue::QueueType::GRAPHICS);
        frameFinishedSemaphore = slag::Semaphore::newSemaphore(1);
    }
    ~ImGuiFrameResources()
    {
        frameFinishedSemaphore->waitForValue(1);
        delete frameFinishedSemaphore;
        delete descriptorPool;
        delete commandBuffer;
    }
};

slag::FrameResources* generateResources(uint8_t frame, slag::SwapChain* swapChain)
{
    return new ImGuiFrameResources();
}

void DebugHandler(const std::string& text, slag::SlagDebugLevel level, int32_t message)
{
    std::cout << text << std::endl;
}

void* extractNativeHandle(ImGuiViewport* fromViewport)
{
    auto sdlWindowID = fromViewport->PlatformHandle;
    auto window = SDL_GetWindowFromID((SDL_WindowID)sdlWindowID);
    auto properties = SDL_GetWindowProperties(window);
#ifdef _WIN32
    return SDL_GetPointerProperty(properties,SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#else
    const char* driverName = SDL_GetCurrentVideoDriver();
    if (strcmp(driverName, "wayland") == 0)
    {
        return SDL_GetPointerProperty(properties,SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
    }
    else
    {
        return reinterpret_cast<void*>(SDL_GetNumberProperty(properties,SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
    }
#endif
}
int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("Dear ImGui Slag Example", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);

    slag::initialize({.graphicsBackend = slag::GraphicsBackend::VULKAN_GRAPHICS_BACKEND,.slagDebugHandler = DebugHandler});

    slag::PlatformData pd{};

    auto properties = SDL_GetWindowProperties(window);
#ifdef _WIN32
    pd.platform = slag::Platform::WIN_32;
    pd.details.win32.hwnd = SDL_GetPointerProperty(properties,SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
    pd.details.win32.hinstance = SDL_GetPointerProperty(properties,SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER, nullptr);
#else
    const char* driverName = SDL_GetCurrentVideoDriver();
    if (strcmp(driverName, "wayland") == 0)
    {
        platformData.platform = slag::PlatformData::WAYLAND;
        pd.details.wayland.surface = SDL_GetPointerProperty(properties,SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
        pd.details.display = SDL_GetPointerProperty(properties,SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
    }
    else
    {
        platformData.platform = slag::PlatformData::X11;
        pd.details.x11.display = SDL_GetPointerProperty(properties,SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        pd.details.x11.window = reinterpret_cast<void*>(SDL_GetNumberProperty(properties,SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
    }
#endif

    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    auto backBufferFormat = slag::Pixels::Format::B8G8R8A8_UNORM;
    slag::SwapChainDetails swapChainDetails;
    swapChainDetails.createResourceFunction = generateResources;
    auto swapchain = slag::SwapChain::newSwapChain(pd,w,h,swapChainDetails);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui_ImplSDL3_InitForOther(window);
    auto defaultSampler = slag::Sampler::newSampler({});
    ImGui_ImplSlag_Init(swapchain,pd,extractNativeHandle,defaultSampler,backBufferFormat);

    bool keepAlive = true;
    while(keepAlive)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                keepAlive = false;
            }
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
            {
                keepAlive = false;
            }
        }
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        if (auto frame = swapchain->next())
        {
            auto frameResources = frame->frameResources<ImGuiFrameResources>();
            delete frameResources->frameFinishedSemaphore;
            frameResources->frameFinishedSemaphore = slag::Semaphore::newSemaphore(0);
            auto descriptorPool = frameResources->descriptorPool;
            descriptorPool->reset();
            ImGui_ImplSlag_NewFrame(frameResources->descriptorPool);
            auto commandBuffer = frameResources->commandBuffer;
            commandBuffer->begin();
            commandBuffer->bindDescriptorPool(descriptorPool);
            ImGui::ShowDemoWindow();
            ImGui::Render();
            ImDrawData* drawData = ImGui::GetDrawData();
            slag::Attachment colorAttachment{.texture = frame->backBuffer(),.autoClear = true,.clearValue{.color = {0,0,0,1}}};
            commandBuffer->beginRendering(&colorAttachment,1,nullptr,{{0,0},{frame->backBuffer()->width(),frame->backBuffer()->height()}});
            ImGui_ImplSlag_RenderDrawData(drawData,commandBuffer);
            commandBuffer->endRendering();
            commandBuffer->end();

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }

            slag::SemaphoreValue signal{.semaphore = frameResources->frameFinishedSemaphore,.value = 1};

            slag::QueueSubmissionBatch batch
            {
                .waitSemaphores = nullptr,
                .waitSemaphoreCount = 0,
                .commandBuffers = &commandBuffer,
                .commandBufferCount = 1,
                .signalSemaphores = &signal,
                .signalSemaphoreCount = 1,
            };
            slag::slagGraphicsCard()->graphicsQueue()->submit(&batch,1,frame);
        }

    }

    delete swapchain;
    delete defaultSampler;
    ImGui_ImplSlag_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    slag::cleanup();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}