#include <Bird/PlatformData.hpp>
#include <Foundation/Thread.hpp>
#include <Nest/Events/EventQueue.hpp>
#include <Nest/Application/Application.hpp>

#include <jni.h>

#include <android_native_app_glue.h>
#include <android/looper.h>

extern "C" {
#include <android_native_app_glue.c>
}

using namespace Nest;

extern int startApp(int argc, char **argv);

int32_t mainF(Foundation::Thread *thread, void *userData) {
    return startApp(0, nullptr);
}

namespace Entry {

Foundation::Thread m_thread;

android_app *m_app;

EventQueue *m_eventQueue;

void onAppCmd(int cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            Bird::PlatformData::get()->nativeWindowHandle = m_app->window;
            Bird::PlatformData::get()->assetManager = m_app->activity->assetManager;
            if (!m_thread.isRunning()) {
                m_thread.init(mainF, nullptr, 0, "MainF");
            }
            break;
        case APP_CMD_TERM_WINDOW:
            break;
    }
}

static void onAppCmdCB(struct android_app *app, int32_t cmd) {
    onAppCmd(cmd);
}

int32_t onInputEvent(AInputEvent *event) {
    if (m_thread.isRunning()) {
        m_eventQueue = Nest::Application::get()->getEventQueue();
    } else {
        return 0;
    }
    const int32_t type = AInputEvent_getType(event);
    const int32_t source = AInputEvent_getSource(event);
    const int32_t actionBits = AMotionEvent_getAction(event);

    switch (type) {
        case AINPUT_EVENT_TYPE_MOTION: {
            if ((source & (AINPUT_SOURCE_GAMEPAD | AINPUT_SOURCE_JOYSTICK)) != 0) {
                //                for (uint32_t ii = 0; ii < BX_COUNTOF(s_translateAxis); ++ii) {
                //                    const float fval = AMotionEvent_getAxisValue(event,
                //                                                                 s_translateAxis[ii].m_event,
                //                                                                 0);
                //                    int32_t value = int32_t(
                //                            (s_translateAxis[ii].m_convert ? fval * 2.0f + 1.0f :
                //                            fval) * INT16_MAX);
                //                    GamepadAxis::Enum axis = s_translateAxis[ii].m_axis;
                //                    if (filter(axis, &value)) {
                //                        m_eventQueue.postAxisEvent(defaultWindow, handle, axis,
                //                        value);
                //                    }
                //                }

                return 1;
            } else {
                int32_t count = AMotionEvent_getPointerCount(event);
                int32_t action = AMotionEvent_getAction(event);
                int32_t actionBits = action & AMOTION_EVENT_ACTION_MASK;
                int32_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                                AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

                for (int32_t i = 0; i < count; ++i) {
                    Size touchLocation;
                    touchLocation.x = AMotionEvent_getX(event, i);
                    touchLocation.y = AMotionEvent_getY(event, i);
                    int32_t id = AMotionEvent_getPointerId(event, i);

                    switch (actionBits) {
                        case AMOTION_EVENT_ACTION_DOWN:
                        case AMOTION_EVENT_ACTION_POINTER_DOWN:
                            if (i == index) {
                                m_eventQueue->postTouchBeganEvent(
                                    id, touchLocation.x, touchLocation.y
                                );
                            }
                            break;
                        case AMOTION_EVENT_ACTION_UP:
                        case AMOTION_EVENT_ACTION_POINTER_UP:
                            if (i == index) {
                                m_eventQueue->postTouchEndedEvent(
                                    id, touchLocation.x, touchLocation.y
                                );
                            }
                            break;
                        case AMOTION_EVENT_ACTION_MOVE:
                            m_eventQueue->postTouchMovedEvent(id, touchLocation.x, touchLocation.y);
                            break;
                        case AMOTION_EVENT_ACTION_CANCEL:
                            m_eventQueue->postTouchEndedEvent(id, touchLocation.x, touchLocation.y);
                            break;
                        default:
                            break;
                    }
                }
            }
        } break;

        case AINPUT_EVENT_TYPE_KEY: {
            int32_t keyCode = AKeyEvent_getKeyCode(event);

            if (0 != (source & (AINPUT_SOURCE_GAMEPAD | AINPUT_SOURCE_JOYSTICK))) {
                //                for (uint32_t jj = 0; jj < BX_COUNTOF(s_gamepadRemap); ++jj) {
                //                    if (keyCode == s_gamepadRemap[jj].m_keyCode) {
                //                        m_eventQueue.postKeyEvent(defaultWindow,
                //                        s_gamepadRemap[jj].m_key, 0,
                //                                                  actionBits ==
                //                                                  AKEY_EVENT_ACTION_DOWN);
                //                        break;
                //                    }
                //                }
            }

            return 1;
        } break;

        default:
            break;
    }

    return 0;
}

static int32_t onInputEventCB(struct android_app *app, AInputEvent *event) {
    return onInputEvent(event);
}

void run(android_app *app) {
    m_app = app;
    m_app->onAppCmd = onAppCmdCB;
    m_app->onInputEvent = onInputEventCB;
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