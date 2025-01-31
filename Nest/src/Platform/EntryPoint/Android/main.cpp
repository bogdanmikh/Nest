#include <Bird/PlatformData.hpp>
#include <Foundation/Thread.hpp>

#include <jni.h>

#include <android_native_app_glue.h>

extern "C" {
#include <android_native_app_glue.c>
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

int32_t onInputEvent(AInputEvent *event) {
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