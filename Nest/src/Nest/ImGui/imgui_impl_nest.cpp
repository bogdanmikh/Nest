#include "imgui_impl_nest.hpp"

#include "Nest/Application/Application.hpp"
#include "Nest/Events/WindowEvents.hpp"
#include "Nest/Events/KeyEvents.hpp"
#include "Nest/Events/MouseEvents.hpp"

struct ImGui_NestPlatformData {
    Nest::Cursor MouseCursors[ImGuiMouseCursor_COUNT];
    bool modCtrl;
    bool modShift;
    bool modAlt;
    bool modSuper;
};

static ImGui_NestPlatformData *ImGui_ImplGlfw_GetBackendData() {
    return ImGui::GetCurrentContext()
               ? (ImGui_NestPlatformData *)ImGui::GetIO().BackendPlatformUserData
               : nullptr;
}

static ImGuiKey ImGui_ImplNest_KeyCodeToImGuiKey(Nest::Key key) {
    using namespace Nest;
    // clang-format off
    switch (key) {
        case Key::SPACE: return ImGuiKey_Space;
        case Key::APOSTROPHE: return ImGuiKey_Apostrophe;
        case Key::COMMA: return ImGuiKey_Comma;
        case Key::MINUS: return ImGuiKey_Minus;
        case Key::PERIOD: return ImGuiKey_Period;
        case Key::SLASH: return ImGuiKey_Slash;
        case Key::KEY_0: return ImGuiKey_0;
        case Key::KEY_1: return ImGuiKey_1;
        case Key::KEY_2: return ImGuiKey_2;
        case Key::KEY_3: return ImGuiKey_3;
        case Key::KEY_4: return ImGuiKey_4;
        case Key::KEY_5: return ImGuiKey_5;
        case Key::KEY_6: return ImGuiKey_6;
        case Key::KEY_7: return ImGuiKey_7;
        case Key::KEY_8: return ImGuiKey_8;
        case Key::KEY_9: return ImGuiKey_9;
        case Key::SEMICOLON: return ImGuiKey_Semicolon;
        case Key::EQUAL: return ImGuiKey_Equal;
        case Key::A: return ImGuiKey_A;
        case Key::B: return ImGuiKey_B;
        case Key::C: return ImGuiKey_C;
        case Key::D: return ImGuiKey_D;
        case Key::E: return ImGuiKey_E;
        case Key::F: return ImGuiKey_F;
        case Key::G: return ImGuiKey_G;
        case Key::H: return ImGuiKey_H;
        case Key::I: return ImGuiKey_I;
        case Key::J: return ImGuiKey_J;
        case Key::K: return ImGuiKey_K;
        case Key::L: return ImGuiKey_L;
        case Key::M: return ImGuiKey_M;
        case Key::N: return ImGuiKey_N;
        case Key::O: return ImGuiKey_O;
        case Key::P: return ImGuiKey_P;
        case Key::Q: return ImGuiKey_Q;
        case Key::R: return ImGuiKey_R;
        case Key::S: return ImGuiKey_S;
        case Key::T: return ImGuiKey_T;
        case Key::U: return ImGuiKey_U;
        case Key::V: return ImGuiKey_V;
        case Key::W: return ImGuiKey_W;
        case Key::X: return ImGuiKey_X;
        case Key::Y: return ImGuiKey_Y;
        case Key::Z: return ImGuiKey_Z;
        case Key::LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case Key::BACKSLASH: return ImGuiKey_Backslash;
        case Key::RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case Key::GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case Key::WORLD_1: return ImGuiKey_None;
        case Key::WORLD_2: return ImGuiKey_None;
        case Key::ESCAPE: return ImGuiKey_Escape;
        case Key::ENTER: return ImGuiKey_Enter;
        case Key::TAB: return ImGuiKey_Tab;
        case Key::BACKSPACE: return ImGuiKey_Backspace;
        case Key::INSERT: return ImGuiKey_Insert;
        case Key::DELETE: return ImGuiKey_Delete;
        case Key::RIGHT: return ImGuiKey_RightArrow;
        case Key::LEFT: return ImGuiKey_LeftArrow;
        case Key::DOWN: return ImGuiKey_DownArrow;
        case Key::UP: return ImGuiKey_UpArrow;
        case Key::PAGE_UP: return ImGuiKey_PageUp;
        case Key::PAGE_DOWN: return ImGuiKey_PageDown;
        case Key::HOME: return ImGuiKey_Home;
        case Key::END: return ImGuiKey_End;
        case Key::CAPS_LOCK: return ImGuiKey_CapsLock;
        case Key::SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case Key::NUM_LOCK: return ImGuiKey_NumLock;
        case Key::PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case Key::PAUSE: return ImGuiKey_Pause;
        case Key::F1: return ImGuiKey_F1;
        case Key::F2: return ImGuiKey_F2;
        case Key::F3: return ImGuiKey_F3;
        case Key::F4: return ImGuiKey_F4;
        case Key::F5: return ImGuiKey_F5;
        case Key::F6: return ImGuiKey_F6;
        case Key::F7: return ImGuiKey_F7;
        case Key::F8: return ImGuiKey_F8;
        case Key::F9: return ImGuiKey_F9;
        case Key::F10: return ImGuiKey_F10;
        case Key::F11: return ImGuiKey_F11;
        case Key::F12: return ImGuiKey_F12;
        case Key::F13: return ImGuiKey_None;
        case Key::F14: return ImGuiKey_None;
        case Key::F15: return ImGuiKey_None;
        case Key::F16: return ImGuiKey_None;
        case Key::F17: return ImGuiKey_None;
        case Key::F18: return ImGuiKey_None;
        case Key::F19: return ImGuiKey_None;
        case Key::F20: return ImGuiKey_None;
        case Key::F21: return ImGuiKey_None;
        case Key::F22: return ImGuiKey_None;
        case Key::F23: return ImGuiKey_None;
        case Key::F24: return ImGuiKey_None;
        case Key::F25: return ImGuiKey_None;
        case Key::KP_0: return ImGuiKey_Keypad0;
        case Key::KP_1: return ImGuiKey_Keypad1;
        case Key::KP_2: return ImGuiKey_Keypad2;
        case Key::KP_3: return ImGuiKey_Keypad3;
        case Key::KP_4: return ImGuiKey_Keypad4;
        case Key::KP_5: return ImGuiKey_Keypad5;
        case Key::KP_6: return ImGuiKey_Keypad6;
        case Key::KP_7: return ImGuiKey_Keypad7;
        case Key::KP_8: return ImGuiKey_Keypad8;
        case Key::KP_9: return ImGuiKey_Keypad9;
        case Key::KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case Key::KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case Key::KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case Key::KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case Key::KP_ADD: return ImGuiKey_KeypadAdd;
        case Key::KP_ENTER: return ImGuiKey_KeypadEnter;
        case Key::KP_EQUAL: return ImGuiKey_KeypadEqual;
        case Key::LEFT_SHIFT: return ImGuiKey_LeftShift;
        case Key::LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case Key::LEFT_ALT: return ImGuiKey_LeftAlt;
        case Key::LEFT_SUPER: return ImGuiKey_LeftSuper;
        case Key::RIGHT_SHIFT: return ImGuiKey_RightShift;
        case Key::RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case Key::RIGHT_ALT: return ImGuiKey_RightAlt;
        case Key::RIGHT_SUPER: return ImGuiKey_RightSuper;
        case Key::MENU: return ImGuiKey_Menu;
        case Key::MOD_SHIFT: return ImGuiKey_ModShift;
        case Key::MOD_CONTROL: return ImGuiKey_ModCtrl;
        case Key::MOD_ALT: return ImGuiKey_ModAlt;
        case Key::MOD_SUPER: return ImGuiKey_ModSuper;
        default: return ImGuiKey_None;
    }
    // clang-format on
}

// X11 does not include current pressed/released modifier key in 'mods' flags submitted by GLFW
// See https://github.com/ocornut/imgui/issues/6034 and https://github.com/glfw/glfw/issues/1630
static void ImGui_ImplNest_UpdateKeyModifiers() {
    ImGuiIO &io = ImGui::GetIO();
    ImGui_NestPlatformData *pd = ImGui_ImplGlfw_GetBackendData();
    io.AddKeyEvent(ImGuiMod_Ctrl, pd->modCtrl);
    io.AddKeyEvent(ImGuiMod_Shift, pd->modShift);
    io.AddKeyEvent(ImGuiMod_Alt, pd->modAlt);
    io.AddKeyEvent(ImGuiMod_Super, pd->modSuper);
}

IMGUI_IMPL_API void ImGui_ImplNest_HandleEvent(Nest::Event *event) {
    using namespace Nest;

    ImGuiIO &io = ImGui::GetIO();
    ImGui_NestPlatformData *pd = ImGui_ImplGlfw_GetBackendData();

    switch (event->type) {
        case EventType::None:
            break;
        case EventType::WindowClose:
            break;
        case EventType::WindowResize: {
            const WindowResizeEvent *ev = static_cast<const WindowResizeEvent *>(event);
            float w = ev->getWidth();
            float h = ev->getHeight();
            io.DisplaySize = ImVec2((float)w, (float)h);
            // if (w > 0 && h > 0) {
            //     io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);
            // }
            break;
        }
        case EventType::KeyPressed: {
            ImGui_ImplNest_UpdateKeyModifiers();
            const KeyPressedEvent *ev = static_cast<const KeyPressedEvent *>(event);
            ImGuiKey key = ImGui_ImplNest_KeyCodeToImGuiKey(ev->key);
            io.AddKeyEvent(key, true);
            pd->modShift = pd->modShift || ev->key == Nest::Key::LEFT_SHIFT ||
                           ev->key == Nest::Key::RIGHT_SHIFT;
            pd->modCtrl = pd->modCtrl || ev->key == Nest::Key::LEFT_CONTROL ||
                          ev->key == Nest::Key::RIGHT_CONTROL;
            pd->modAlt =
                pd->modAlt || ev->key == Nest::Key::LEFT_ALT || ev->key == Nest::Key::RIGHT_ALT;
            pd->modSuper = pd->modSuper || ev->key == Nest::Key::LEFT_SUPER ||
                           ev->key == Nest::Key::RIGHT_SUPER;
            event->isHandled = io.WantCaptureKeyboard;
            break;
        }
        case EventType::InputCharacter: {
            const CharacterInputEvent *ev = static_cast<const CharacterInputEvent *>(event);
            io.AddInputCharacter(ev->c);
            break;
        }
        case EventType::KeyReleased: {
            const KeyReleasedEvent *ev = static_cast<const KeyReleasedEvent *>(event);
            ImGuiKey key = ImGui_ImplNest_KeyCodeToImGuiKey(ev->key);
            io.AddKeyEvent(key, false);
            event->isHandled = io.WantCaptureKeyboard;
            if (ev->key == Nest::Key::LEFT_SHIFT || ev->key == Nest::Key::RIGHT_SHIFT) {
                pd->modShift = false;
            }
            if (ev->key == Nest::Key::LEFT_CONTROL || ev->key == Nest::Key::RIGHT_CONTROL) {
                pd->modCtrl = false;
            }
            if (ev->key == Nest::Key::LEFT_ALT || ev->key == Nest::Key::RIGHT_ALT) {
                pd->modAlt = false;
            }
            if (ev->key == Nest::Key::LEFT_SUPER || ev->key == Nest::Key::RIGHT_SUPER) {
                pd->modSuper = false;
            }
            break;
        }
        case EventType::MouseMoved: {
            const MouseMovedEvent *ev = static_cast<const MouseMovedEvent *>(event);
            io.AddMousePosEvent((float)ev->x, (float)ev->y);
            event->isHandled = io.WantCaptureMouse;
            break;
        }
        case EventType::MouseScrolled: {
            const MouseScrolledEvent *ev = static_cast<const MouseScrolledEvent *>(event);
            io.AddMouseWheelEvent(ev->xoffset, ev->yoffset);
            event->isHandled = io.WantCaptureMouse;
            break;
        }
        case EventType::MouseButtonPressed: {
            ImGui_ImplNest_UpdateKeyModifiers();
            const MouseKeyEvent *ev = static_cast<const MouseKeyEvent *>(event);
            int button = static_cast<int>(ev->button);
            if (button >= 0 && button < ImGuiMouseButton_COUNT) {
                io.AddMouseButtonEvent(button, true);
            }
            event->isHandled = io.WantCaptureMouse;
            break;
        }
        case EventType::MouseButtonReleased: {
            const MouseKeyEvent *ev = static_cast<const MouseKeyEvent *>(event);
            int button = static_cast<int>(ev->button);
            if (button >= 0 && button < ImGuiMouseButton_COUNT) {
                io.AddMouseButtonEvent(button, false);
            }
            event->isHandled = io.WantCaptureMouse;
            break;
        }
    }
}

static const char *getClipboardText(void *userData) {
    Nest::Window *window = static_cast<Nest::Window *>(userData);
    return window->getClipboardText();
}

static void setClipboardText(void *userData, const char *text) {
    Nest::Window *window = static_cast<Nest::Window *>(userData);
    window->setClipboardText(text);
}

IMGUI_IMPL_API bool ImGui_ImplNest_Init() {
    ImGuiIO &io = ImGui::GetIO();
    io.BackendPlatformName = "imgui_impl_Nest";

    ImGui_NestPlatformData *bd = IM_NEW(ImGui_NestPlatformData)();
    io.BackendPlatformUserData = (void *)bd;

    bd->MouseCursors[ImGuiMouseCursor_Arrow] = Nest::Cursor::ARROW;
    bd->MouseCursors[ImGuiMouseCursor_TextInput] = Nest::Cursor::IBEAM;
    bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = Nest::Cursor::RESIZE_NS;
    bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = Nest::Cursor::RESIZE_EW;
    bd->MouseCursors[ImGuiMouseCursor_Hand] = Nest::Cursor::POINTING_HAND;
    bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = Nest::Cursor::RESIZE_ALL;
    bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = Nest::Cursor::RESIZE_NESW;
    bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = Nest::Cursor::RESIZE_NWSE;
    bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = Nest::Cursor::NOT_ALLOWED;

    bd->modCtrl = false;
    bd->modAlt = false;
    bd->modSuper = false;
    bd->modShift = false;

    // Setup backend capabilities flags
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values
    // (optional) io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor
    // io.WantSetMousePos requests (optional, rarely used) io.BackendFlags |=
    // ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform
    // side (optional) io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can set
    // io.MouseHoveredViewport correctly (optional, not easy)
    io.SetClipboardTextFn = setClipboardText;
    io.GetClipboardTextFn = getClipboardText;
    io.ClipboardUserData = Nest::Application::get()->getWindow();

    return true;
}

IMGUI_IMPL_API void ImGui_ImplNest_Shutdown() {
    ImGui_NestPlatformData *bd = ImGui_ImplGlfw_GetBackendData();
    IM_DELETE(bd);
    ImGuiIO &io = ImGui::GetIO();
    io.BackendPlatformUserData = nullptr;
}

IMGUI_IMPL_API void ImGui_ImplNest_NewFrame(double deltaTime) {
    ImGuiIO &io = ImGui::GetIO();
    ImGui_NestPlatformData *bd = ImGui_ImplGlfw_GetBackendData();
    using namespace Nest;
    Application *app = Application::get();
    Size dpi = app->getWindow()->getDpi();
    io.DisplayFramebufferScale = ImVec2(dpi.width, dpi.height);
    io.DisplaySize = ImVec2(app->getWindow()->getSize().width, app->getWindow()->getSize().height);
    io.DeltaTime = (float)deltaTime;
    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    app->getWindow()->setCursor(bd->MouseCursors[imgui_cursor]);
}
