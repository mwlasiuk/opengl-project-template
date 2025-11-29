#include <App/ImGUINodes.hpp>

#include <Core/SystemInfo.hpp>

#include <imgui.h>

namespace App
{
    namespace ImGUINodes
    {
        void DrawRAMInfoGUI()
        {
            if (ImGui::TreeNode("System RAM info"))
            {
                Core::RAMInfo system_info = Core::GetRAMInfo();

                ImGui::Text(" Total : %u MB", system_info.total_memory_mb);
                ImGui::Text(" Used  : %u MB", system_info.used_memory_mb);
                ImGui::Text(" Free  : %u MB", system_info.free_memory_mb);

                ImGui::TreePop();
            }
        }

        void DrawProcessRAMInfoGUI()
        {
            if (ImGui::TreeNode("Process RAM info"))
            {
                Core::ProcessRAMInfo process_info = Core::GetProcessRAMInfo();

                ImGui::Text("RAM usage : %u MB", static_cast<uint32_t>(process_info.memory_used_mb));
                ImGui::Text("Virtual   : %u MB", static_cast<uint32_t>(process_info.virtual_memory_mb));

                ImGui::TreePop();
            }
        }

        void DrawGPUInfoGUI()
        {
            if (ImGui::TreeNode("GPU info"))
            {
                Core::GPUInfo gpu = Core::GetGPUInfo();

                ImGui::Text("OpenGL version      : %s", gpu.gl_version);
                ImGui::Text("OpenGL vendor       : %s", gpu.gl_vendor);
                ImGui::Text("OpenGL renderer     : %s", gpu.gl_renderer);
                ImGui::Text("OpenGL GLSL version : %s", gpu.gl_shading_language_version);

                ImGui::TreePop();
            }
        }

        void DrawGPUMemoryInfoGUI()
        {
            if (ImGui::TreeNode("GPU memory info"))
            {
                Core::GPUMemoryInfo gpu = Core::GetGPUMemoryInfo();

                if (ImGui::TreeNode("Memory : GL_NVX_gpu_memory_info"))
                {
                    ImGui::TextColored(gpu.has_memory_info_nvx ? ImVec4{0.0f, 1.0f, 0.0f, 1.0f} : ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, "GL_NVX_gpu_memory_info : %s", gpu.has_memory_info_nvx ? "supported" : "sot supported");
                    if (gpu.has_memory_info_nvx)
                    {
                        ImGui::Text(" - Dedicated V-RAM   : %u MB", gpu.nvx_pool.dedicated_mb);
                        ImGui::Text(" - Total available   : %u MB", gpu.nvx_pool.total_mb);
                        ImGui::Text(" - Current available : %u MB", gpu.nvx_pool.available_mb);
                        ImGui::Text(" - Eviction count    : %u", gpu.nvx_pool.eviction_count);
                        ImGui::Text(" - Evicted memory    : %u MB", gpu.nvx_pool.evicted_mb);
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Memory : GL_ATI_meminfo"))
                {
                    ImGui::TextColored(gpu.has_memory_info_ati ? ImVec4{0.0f, 1.0f, 0.0f, 1.0f} : ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, "GL_ATI_meminfo : %s", gpu.has_memory_info_ati ? "supported" : "sot supported");

                    if (gpu.has_memory_info_ati)
                    {
                        if (ImGui::TreeNode("Textures"))
                        {
                            ImGui::Text(" - Total free memory      : %u MB", gpu.texture.free_mb);
                            ImGui::Text(" - Largest free block     : %u MB", gpu.texture.largest_free_mb);
                            ImGui::Text(" - Total aux free memory  : %u MB", gpu.texture.total_auxiliary_free_mb);
                            ImGui::Text(" - Largest aux free block : %u MB", gpu.texture.largest_auxiliary_free_mb);
                            ImGui::TreePop();
                        }

                        if (ImGui::TreeNode("VBOs"))
                        {
                            ImGui::Text(" - Total free memory      : %u MB", gpu.vbo.free_mb);
                            ImGui::Text(" - Largest free block     : %u MB", gpu.vbo.largest_free_mb);
                            ImGui::Text(" - Total aux free memory  : %u MB", gpu.vbo.total_auxiliary_free_mb);
                            ImGui::Text(" - Largest aux free block : %u MB", gpu.vbo.largest_auxiliary_free_mb);
                            ImGui::TreePop();
                        }

                        if (ImGui::TreeNode("RenderBuffers"))
                        {
                            ImGui::Text(" - Total free memory      : %u MB", gpu.renderbuffer.free_mb);
                            ImGui::Text(" - Largest free block     : %u MB", gpu.renderbuffer.largest_free_mb);
                            ImGui::Text(" - Total aux free memory  : %u MB", gpu.renderbuffer.total_auxiliary_free_mb);
                            ImGui::Text(" - Largest aux free block : %u MB", gpu.renderbuffer.largest_auxiliary_free_mb);
                            ImGui::TreePop();
                        }
                    }

                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
        }
    }
}
