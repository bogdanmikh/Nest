#include <Bird/PlatformData.hpp>
#include <Foundation/Thread.hpp>

#include <jni.h>

extern "C" {
#include <game-activity/native_app_glue/android_native_app_glue.c>
}

extern int startApp(int argc, char **argv);

int32_t mainF(Foundation::Thread *thread, void *userData) {
    return startApp(0, nullptr);
}

namespace Entry {

Foundation::Thread m_thread;

android_app *m_app;

void onAppCmd(int cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            Bird::PlatformData::get()->nativeWindowHandle = m_app->window;
            Bird::PlatformData::get()->assetManager = m_app->activity->assetManager;
            if (!m_thread.isRunning()) {
                m_thread.init(mainF, nullptr, 0, "main");
            }
            break;
        case APP_CMD_TERM_WINDOW:
            break;
    }
}

static void onAppCmdCB(struct android_app *app, int32_t cmd) {
    onAppCmd(cmd);
}

static int32_t onInputEventCB(struct android_app *_app, AInputEvent *_event) {
    //    WindowHandle defaultWindow = {0};
    //    GamepadHandle handle = {0};
    //    const int32_t type = AInputEvent_getType(_event);
    //    const int32_t source = AInputEvent_getSource(_event);
    //    const int32_t actionBits = AMotionEvent_getAction(_event);
    //
    //    switch (type) {
    //        case AINPUT_EVENT_TYPE_MOTION: {
    //            if (0 != (source & (AINPUT_SOURCE_GAMEPAD | AINPUT_SOURCE_JOYSTICK))) {
    //                for (uint32_t ii = 0; ii < BX_COUNTOF(s_translateAxis); ++ii) {
    //                    const float fval =
    //                        AMotionEvent_getAxisValue(_event, s_translateAxis[ii].m_event, 0);
    //                    int32_t value = int32_t(
    //                        (s_translateAxis[ii].m_convert ? fval * 2.0f + 1.0f : fval) *
    //                        INT16_MAX
    //                    );
    //                    GamepadAxis::Enum axis = s_translateAxis[ii].m_axis;
    //                    if (filter(axis, &value)) {
    //                        m_eventQueue.postAxisEvent(defaultWindow, handle, axis, value);
    //                    }
    //                }
    //
    //                return 1;
    //            } else {
    //                float mx = AMotionEvent_getX(_event, 0);
    //                float my = AMotionEvent_getY(_event, 0);
    //                int32_t count = AMotionEvent_getPointerCount(_event);
    //
    //                int32_t action = (actionBits & AMOTION_EVENT_ACTION_MASK);
    //                int32_t index = (actionBits & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
    //                                AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    //
    //                // Simulate left mouse click with 1st touch and right mouse click with 2nd
    //                touch.
    //                // ignore other touchs
    //                if (count < 2) {
    //                    switch (action) {
    //                        case AMOTION_EVENT_ACTION_DOWN:
    //                        case AMOTION_EVENT_ACTION_POINTER_DOWN:
    //                            m_eventQueue.postMouseEvent(
    //                                defaultWindow,
    //                                (int32_t)mx,
    //                                (int32_t)my,
    //                                0,
    //                                action == AMOTION_EVENT_ACTION_DOWN ? MouseButton::Left
    //                                                                    : MouseButton::Right,
    //                                true
    //                            );
    //                            break;
    //
    //                        case AMOTION_EVENT_ACTION_UP:
    //                        case AMOTION_EVENT_ACTION_POINTER_UP:
    //                            m_eventQueue.postMouseEvent(
    //                                defaultWindow,
    //                                (int32_t)mx,
    //                                (int32_t)my,
    //                                0,
    //                                action == AMOTION_EVENT_ACTION_UP ? MouseButton::Left
    //                                                                  : MouseButton::Right,
    //                                false
    //                            );
    //                            break;
    //
    //                        default:
    //                            break;
    //                    }
    //                }
    //
    //                switch (action) {
    //                    case AMOTION_EVENT_ACTION_MOVE:
    //                        if (0 == index) {
    //                            m_eventQueue.postMouseEvent(defaultWindow, (int32_t)mx,
    //                            (int32_t)my, 0);
    //                        }
    //                        break;
    //
    //                    default:
    //                        break;
    //                }
    //            }
    //        } break;
    //
    //        case AINPUT_EVENT_TYPE_KEY: {
    //            int32_t keyCode = AKeyEvent_getKeyCode(_event);
    //
    //            if (0 != (source & (AINPUT_SOURCE_GAMEPAD | AINPUT_SOURCE_JOYSTICK))) {
    //                for (uint32_t jj = 0; jj < BX_COUNTOF(s_gamepadRemap); ++jj) {
    //                    if (keyCode == s_gamepadRemap[jj].m_keyCode) {
    //                        m_eventQueue.postKeyEvent(
    //                            defaultWindow,
    //                            s_gamepadRemap[jj].m_key,
    //                            0,
    //                            actionBits == AKEY_EVENT_ACTION_DOWN
    //                        );
    //                        break;
    //                    }
    //                }
    //            }
    //
    //            return 1;
    //        } break;
    //
    //        default:
    //            DBG("type %d", type);
    //            break;
    //    }

    //    return 0;
}

void run(android_app *app) {
    m_app = app;
    m_app->onAppCmd = onAppCmdCB;
    //    m_app->onInputEvent = onInputEventCB;
    while (m_app->destroyRequested == 0) {
        int32_t num;
        android_poll_source *source;
        ALooper_pollOnce(-1, nullptr, &num, (void **)&source);

        if (source != nullptr) {
            source->process(m_app, source);
        }
    }
    m_thread.shutdown();
}

} // namespace Entry

extern "C" void android_main(struct android_app *app) {
    Entry::run(app);
}