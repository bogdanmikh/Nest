#include "imgui_impl_bird.hpp"

#include <Bird/Bird.hpp>
#include <Nest/Assets/AssetLoader.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

bool deviceObjectsCreated = false;
Bird::ProgramHandle shader;
Bird::VertexLayoutHandle vertexLayout;
Bird::TextureHandle fontTexture;
glm::mat4 projMat(1.f);

IMGUI_IMPL_API bool ImGui_ImplBird_Init() {
    ImGuiIO &io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_Bird";
    // io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
    return true;
}

IMGUI_IMPL_API void ImGui_ImplBird_Shutdown() {
    ImGui_ImplBird_DestroyDeviceObjects();
}

IMGUI_IMPL_API void ImGui_ImplBird_NewFrame() {
    if (!deviceObjectsCreated) {
        ImGui_ImplBird_CreateDeviceObjects();
        deviceObjectsCreated = true;
    }
}

static void ImGui_ImplBird_SetProjMat(ImDrawData *draw_data, int fb_width, int fb_height) {
    using namespace Bird;

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    projMat = glm::ortho(L, R, B, T);
    int textureIndex = 0;
    Bird::setUniform(shader, "Texture", &textureIndex, UniformType::Sampler);
    Bird::setUniform(shader, "ProjMtx", &projMat[0], UniformType::Mat4);
}

IMGUI_IMPL_API void ImGui_ImplBird_RenderDrawData(ImDrawData *draw_data) {
    using namespace Bird;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates !=
    // framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0) {
        return;
    }

    ImGui_ImplBird_SetProjMat(draw_data, fb_width, fb_height);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];

        // Vertex/index buffers data
        Bird::TransientVertexBuffer tvb;
        Bird::allocTransientVertexBuffer(&tvb, cmd_list->VtxBuffer.size_in_bytes());
        memcpy(tvb.data, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.size_in_bytes());

        Bird::TransientIndexBuffer tib;
        Bird::allocTransientIndexBuffer(
            &tib, cmd_list->IdxBuffer.size(), Bird::BufferElementType::UnsignedShort
        );
        memcpy(tib.data, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.size_in_bytes());

        // Will project scissor/clipping rectangles into framebuffer space
        ImVec2 clip_off = draw_data->DisplayPos; // (0,0) unless using multi-viewports
        ImVec2 clip_scale =
            draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

        const ImDrawCmd *cmd = cmd_list->CmdBuffer.begin();
        const ImDrawCmd *cmdEnd = cmd_list->CmdBuffer.end();
        for (; cmd != cmdEnd; ++cmd) {
            if (cmd->UserCallback != NULL) {
                cmd->UserCallback(cmd_list, cmd);
            } else {
                ImVec2 clip_min(
                    (cmd->ClipRect.x - clip_off.x) * clip_scale.x,
                    (cmd->ClipRect.y - clip_off.y) * clip_scale.y
                );
                ImVec2 clip_max(
                    (cmd->ClipRect.z - clip_off.x) * clip_scale.x,
                    (cmd->ClipRect.w - clip_off.y) * clip_scale.y
                );
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) {
                    continue;
                }
                Rect scissorRect = Rect(
                    clip_min.x,
                    ((float)fb_height - clip_max.y),
                    (clip_max.x - clip_min.x),
                    (clip_max.y - clip_min.y)
                );
                Bird::setScissorRect(scissorRect);
                Bird::setState(0);
                Bird::setShader(shader);
                TextureHandle texture = TextureHandle((intptr_t)cmd->GetTexID());
                Bird::setTexture(texture, 0);
                Bird::setVertexBuffer(tvb.handle, tvb.startVertex);
                Bird::setVertexLayout(vertexLayout);
                uint32_t offset = cmd->IdxOffset * sizeof(ImDrawIdx);
                Bird::setIndexBuffer(tib.handle, tib.startIndex + offset, cmd->ElemCount);
                Bird::submit(0);
            }
        }
    }
}

IMGUI_IMPL_API bool ImGui_ImplBird_CreateFontsTexture() {
    // Build texture atlas
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    uint32_t textureSize = width * height * 4;
    LOG_INFO("FONT TEXTURE SIZE: {}, {}", width, height);
    void *texture = malloc(textureSize);
    memcpy(texture, pixels, textureSize);

    using namespace Bird;
    TextureCreate create;
    create.m_data =
        Foundation::Memory(texture, nullptr, [](void *ptr, void *userData) { free(ptr); });
    create.m_format = TextureFormat::RGBA8;
    create.m_width = width;
    create.m_height = height;
    create.m_numMips = 0;
    create.m_magFiltering = NEAREST;
    create.m_minFiltering = NEAREST;
    fontTexture = createTexture(create);
    io.Fonts->SetTexID((ImTextureID)(intptr_t)fontTexture.id);
    return true;
}

IMGUI_IMPL_API void ImGui_ImplBird_DestroyFontsTexture() {
    Bird::deleteTexture(fontTexture);
}

IMGUI_IMPL_API bool ImGui_ImplBird_CreateDeviceObjects() {
    using namespace Bird;
    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
        "default-shaders/imgui/imgui_vertex.glsl", "default-shaders/imgui/imgui_fragment.glsl"
    );
    shader = Bird::createProgram(programAsset.getBirdProgramCreate());

    VertexBufferLayoutData layoutData;
    layoutData.pushVec2();
    layoutData.pushVec2();
    layoutData.push8BitRGBAColor();
    vertexLayout = Bird::createVertexLayout(layoutData);
    ImGui_ImplBird_CreateFontsTexture();
    return true;
}

IMGUI_IMPL_API void ImGui_ImplBird_DestroyDeviceObjects() {
    using namespace Bird;
    deleteProgram(shader);
    ImGui_ImplBird_DestroyFontsTexture();
    deviceObjectsCreated = false;
}
