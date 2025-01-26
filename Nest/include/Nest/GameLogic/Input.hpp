//
// Created by Admin on 05.02.2022.
//

#pragma once

#include "Bird/Base.hpp"
#include "Nest/Events/Event.hpp"
#include "Nest/Events/Key.hpp"
#include "Nest/Base/Base.hpp"

using namespace Bird;

namespace Nest {

class Window;

class Input {
public:
    struct Touch {
        int id;
        float x;
        float y;

        Touch(int id, float x, float y)
            : id(id)
            , x(x)
            , y(y) {}
    };

    // GET INPUT DATA
    static bool isKeyPressed(Key key);
    static bool isKeyJustPressed(Key key);
    static bool isMouseButtonPressed(MouseButton mouseButton);
    static bool isMouseButtonJustPressed(MouseButton mouseButton);
    static int touchCount();
    static Touch getTouch(int index);
    static double getMousePositionX();
    static double getMousePositionY();
    static double getMouseViewportPositionX();
    static double getMouseViewportPositionY();
    static double getMouseScrollX();
    static double getMouseScrollY();
    static Size getWindowSize();
    static void onEvent(Event *event);
    static void setViewportFrame(Rect frame);
    static void nextFrame();

private:
    // POST EVENTS
    static void postTouchBeganEvent(int id, float x, float y);
    static void postTouchMovedEvent(int id, float x, float y);
    static void postTouchEndedEvent(int id);
    static void setKeyPressed(Key key, bool state);
    static void setMouseButtonPressed(MouseButton mouseButton, bool state);
    static void postMouseChangedPosition(double x, double y);
    static void postScrollEvent(double x, double y);
    static void setWindowSize(Size size);

    static uint32_t frame;
    // Состояния клавиш
    static bool keys[1024];
    // Номера кадров при нажатии клавиш
    static uint32_t framesKeys[1024];
    // Состояния кнопок мыши
    static bool mouseButtons[8];
    // Номера кадров при нажатии мыши
    static uint32_t framesMouseButtons[4];
    static Size windowSize;
    static Rect viewportFrame;
    static double mousePositionX;
    static double mousePositionY;
    static double mouseScrollX;
    static double mouseScrollY;
    static std::vector<Touch> activeTouches;
};

} // namespace Nest
