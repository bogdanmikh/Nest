#include "imgui_impl_nestren.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include <Nest/Assets/AssetLoader.hpp>
#include <NestRen/NestRen.hpp>

#include <glm/glm.hpp>

bool deviceObjectsCreated = false;
NestRen::ProgramHandle shader;
NestRen::VertexLayoutHandle vertexLayout;
NestRen::TextureHandle fontTexture;
glm::mat4 projMat(1.f);

IMGUI_IMPL_API bool ImGui_ImplNestRen_Init() {
    ImGuiIO &io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_NestRen";
    // io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
    return true;
}

IMGUI_IMPL_API void ImGui_ImplNestRen_Shutdown() {
    ImGui_ImplNestRen_DestroyDeviceObjects();
}

IMGUI_IMPL_API void ImGui_ImplNestRen_NewFrame() {
    if (!deviceObjectsCreated) {
        ImGui_ImplNestRen_CreateDeviceObjects();
        deviceObjectsCreated = true;
    }
}

static void ImGui_ImplNestRen_SetProjMat(ImDrawData *draw_data, int fb_width, int fb_height) {
    using namespace NestRen;

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    projMat = glm::ortho(L, R, B, T);
    static int textureIndex = 0;
    NestRen::setUniform(shader, "Texture", &textureIndex, UniformDataType::Int);
    NestRen::setUniform(shader, "ProjMtx", &projMat[0], UniformDataType::Mat4);
}

IMGUI_IMPL_API void ImGui_ImplNestRen_RenderDrawData(ImDrawData *draw_data) {
    using namespace NestRen;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates !=
    // framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0) {
        return;
    }

    ImGui_ImplNestRen_SetProjMat(draw_data, fb_width, fb_height);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];

        // Vertex/index buffers data
        NestRen::TransientVertexBuffer tvb;
        NestRen::allocTransientVertexBuffer(&tvb, cmd_list->VtxBuffer.size_in_bytes());
        memcpy(tvb.data, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.size_in_bytes());

        NestRen::TransientIndexBuffer tib;
        NestRen::allocTransientIndexBuffer(
            &tib, cmd_list->IdxBuffer.size(), NestRen::BufferElementType::UnsignedShort
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
                NestRen::setScissorRect(scissorRect);
                NestRen::setState(0);
                NestRen::setShader(shader);
                TextureHandle texture = TextureHandle((intptr_t)cmd->GetTexID());
                NestRen::setTexture(texture, 0);
                NestRen::setVertexBuffer(tvb.handle, tvb.startVertex);
                NestRen::setVertexLayout(vertexLayout);
                uint32_t offset = cmd->IdxOffset * sizeof(ImDrawIdx);
                NestRen::setIndexBuffer(tib.handle, tib.startIndex + offset, cmd->ElemCount);
                NestRen::submit(0);
            }
        }
    }
}

IMGUI_IMPL_API bool ImGui_ImplNestRen_CreateFontsTexture() {
    // Build texture atlas
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    uint32_t textureSize = width * height * 4;
    LOG_INFO("FONT TEXTURE SIZE: {}, {}", width, height);
    void *texture = malloc(textureSize);
    memcpy(texture, pixels, textureSize);

    using namespace NestRen;
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

IMGUI_IMPL_API void ImGui_ImplNestRen_DestroyFontsTexture() {
    NestRen::deleteTexture(fontTexture);
}

IMGUI_IMPL_API bool ImGui_ImplNestRen_CreateDeviceObjects() {
    using namespace NestRen;
    NestRen::ProgramAsset programAsset = NestRen::AssetLoader::loadProgram(
        "default-shaders/imgui/imgui_vertex.glsl", "default-shaders/imgui/imgui_fragment.glsl"
    );
    shader = NestRen::createProgram(programAsset.getNestRenProgramCreate());

    VertexBufferLayoutData layoutData;
    layoutData.pushVec2();
    layoutData.pushVec2();
    layoutData.push8BitRGBAColor();
    vertexLayout = NestRen::createVertexLayout(layoutData);
    ImGui_ImplNestRen_CreateFontsTexture();
    return true;
}

IMGUI_IMPL_API void ImGui_ImplNestRen_DestroyDeviceObjects() {
    using namespace NestRen;
    deleteProgram(shader);
    ImGui_ImplNestRen_DestroyFontsTexture();
    deviceObjectsCreated = false;
}
