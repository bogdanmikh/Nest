#pragma once

#include <iostream>
#include <chrono>

class Timer {
public:
    void start() {
        m_start = std::chrono::high_resolution_clock::now();
    }

    float end() {
        std::chrono::time_point<std::chrono::high_resolution_clock> m_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = m_end - m_start;
        return duration.count();
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};