//
// Created by Michael Andreichev on 13.09.2023.
//

#pragma once

namespace Nest {

enum Cursor {
    ARROW = 0,
    IBEAM = 1,
    CROSSHAIR = 2,
    POINTING_HAND = 3,
    RESIZE_EW = 4,
    RESIZE_NS = 5,
    RESIZE_NESW = 6,
    RESIZE_NWSE = 7,
    RESIZE_ALL = 8,
    NOT_ALLOWED = 9,
    COUNT = 10
};

}