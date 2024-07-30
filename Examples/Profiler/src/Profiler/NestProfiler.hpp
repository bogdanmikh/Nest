#pragma once

#include "AppInfo.hpp"

#include <string>
#include <chrono>
#include <thread>
#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

#define CURRENT_FILE __FILE__
#define CURRENT_FUNCTION __func__
#define CURRENT_LINE __LINE__

namespace NestProfiler {

class Instrumentor {
public:
    inline static Instrumentor* get() {
        return s_instance;
    }
    void addFrame(const FrameInfo& frameInfo);
    void addFunc(const FuncInfo& funcInfo);
    AppInfo& getResult();
private:
    Instrumentor() : currentFrame(-1) {}
    static Instrumentor* s_instance;
    int currentFrame;
    AppInfo m_appInfo;
};

uint64_t getTimeMs() {
    auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    uint64_t milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
    return milliseconds;
}

struct FuncProfiler {
    FuncProfiler (const std::string& nameFile, const std::string& nameFunc, int line)
        : m_nameFile(nameFile)
        , m_nameFunc(nameFunc)
        , m_line(line)
        , m_startTime(getTimeMs()) {}

    ~FuncProfiler() {
        FuncInfo funcInfo;
        funcInfo.nameFile = m_nameFile;
        funcInfo.nameFunc = m_nameFunc;
        funcInfo.line = m_line;
        funcInfo.startTime = m_startTime;
        funcInfo.endTime = getTimeMs();
        Instrumentor::get()->addFunc(funcInfo);
    }
    std::string m_nameFunc;
    std::string m_nameFile;
    int m_line;
    uint64_t m_startTime;
};

struct FrameProfiler {
    FrameProfiler (const std::string &nameThread)
        : m_nameThread(nameThread)
        , m_startTime(getTimeMs()) {}

    ~FrameProfiler() {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        int threadId = (int)std::stoull(ss.str());
        FrameInfo frameInfo;
        frameInfo.nameThread = m_nameThread;
        frameInfo.startTime = m_startTime;
        frameInfo.endTime = getTimeMs();
        frameInfo.threadId = threadId;
        Instrumentor::get()->addFrame(frameInfo);
    }
    uint64_t m_startTime;
    std::string m_nameThread;
};

}

#define NEST_FRAME(nameThread) NestProfiler::FrameProfiler frameProfiler(nameThread)
#define NEST_FUNC() NestProfiler::FuncProfiler funcProfiler(CURRENT_FILE, CURRENT_FUNCTION, CURRENT_LINE)