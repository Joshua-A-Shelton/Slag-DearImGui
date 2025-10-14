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

#ifndef SLAG_DEARIMGUI_IMGUI_IMPL_SLAG_H
#define SLAG_DEARIMGUI_IMGUI_IMPL_SLAG_H
#include <slag/Slag.h>
#include <imgui.h>

struct ImGui_ImplSlag_Data
{
    slag::ShaderPipeline* shaderPipeline = nullptr;
    slag::Sampler* sampler = nullptr;
    slag::DescriptorPool* currentFrameDescriptorPool = nullptr;
    slag::PlatformData platformData;
    slag::Pixels::Format backBufferFormat = slag::Pixels::Format::UNDEFINED;
    slag::DescriptorIndex* textureIndex = nullptr;
    slag::DescriptorIndex* drawDataIndex = nullptr;
};

struct ImGui_ImplSlag_ViewportData
{
    ImGui_ImplSlag_ViewportData()=delete;
    ImGui_ImplSlag_ViewportData(slag::SwapChain* swap, bool managedOutside)
    {
        swapchain = swap;
        outsideManaged = managedOutside;
        drawDataArrays = std::vector<slag::Buffer*>(swapchain->frameCount(), nullptr);
        drawDataIndexArrays = std::vector<slag::Buffer*>(swapchain->frameCount(), nullptr);
    }
    slag::SwapChain* swapchain= nullptr;
    bool outsideManaged = false;
    std::vector<slag::Buffer*> drawDataArrays;
    std::vector<slag::Buffer*> drawDataIndexArrays;
    ~ImGui_ImplSlag_ViewportData()
    {
        if(swapchain && !outsideManaged)
        {
            delete swapchain;
        }
        for(auto i=0; i< drawDataArrays.size(); i++)
        {
            if(drawDataArrays[i])
            {
                delete drawDataArrays[i];
            }
            if(drawDataIndexArrays[i])
            {
                delete drawDataIndexArrays[i];
            }
        }
    }
};

struct ImGui_ImplSlag_RenderState
{
    slag::CommandBuffer* commandBuffer = nullptr;
    slag::ShaderPipeline* shader = nullptr;
};

// public facing functions
IMGUI_IMPL_API bool     ImGui_ImplSlag_Init(slag::SwapChain* mainSwapchain, slag::PlatformData platformData, void* (*extractNativeHandle)(ImGuiViewport* fromViewport), slag::Sampler* sampler, slag::Pixels::Format backBufferFormat);
IMGUI_IMPL_API void     ImGui_ImplSlag_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplSlag_NewFrame(slag::DescriptorPool* framePool);
IMGUI_IMPL_API void     ImGui_ImplSlag_RenderDrawData(ImDrawData* draw_data,slag::CommandBuffer* commandBuffer);
#endif //SLAG_DEARIMGUI_IMGUI_IMPL_SLAG_H