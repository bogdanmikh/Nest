#pragma once

#include "AppInfo.hpp"

#include <Rain/Codable.hpp>
#include <Rain/Rain.hpp>
#include <chrono>
#include <string>
#include <thread>

#define CURRENT_FILE __FILE__
#define CURRENT_FUNCTION __func__
#define CURRENT_LINE __LINE__

namespace NestProfiler {

class Instrumentor {
public:
    inline static Instrumentor *get() {
        return s_instance;
    }
    void addFrame(const FrameInfo &frameInfo);
    void setEndTimeFrame(const uint64_t &endTime);
    void addFunc(const FuncInfo &funcInfo);
    AppInfo &getResult();

private:
    Instrumentor()
        : currentFrame(-1) {}
    static Instrumentor *s_instance;
    int currentFrame;
    AppInfo m_appInfo;
};

static uint64_t getTimeUs() {
    auto now =
        std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    uint64_t microseconds =
        std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
    return microseconds;
}

struct FuncProfiler {
    FuncProfiler(const std::string &nameFile, const std::string &nameFunc, int line)
        : m_nameFile(nameFile)
        , m_nameFunc(nameFunc)
        , m_line(line)
        , m_startTime(getTimeUs()) {}

    ~FuncProfiler() {
        FuncInfo funcInfo;
        funcInfo.nameFile = m_nameFile;
        funcInfo.nameFunc = m_nameFunc;
        funcInfo.line = m_line;
        funcInfo.startTime = m_startTime;
        funcInfo.endTime = getTimeUs();
        Instrumentor::get()->addFunc(funcInfo);
    }
    std::string m_nameFunc;
    std::string m_nameFile;
    int m_line;
    uint64_t m_startTime;
};

struct FrameProfiler {
    FrameProfiler(const std::string &nameThread) {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        int threadId = (int)std::stoull(ss.str());

        FrameInfo frameInfo;
        frameInfo.nameThread = nameThread;
        frameInfo.startTime = getTimeUs();
        frameInfo.threadId = threadId;
        Instrumentor::get()->addFrame(frameInfo);
    }

    ~FrameProfiler() {
        Instrumentor::get()->setEndTimeFrame(getTimeUs());
    }
};

} // namespace NestProfiler

#define NEST_FRAME(nameThread) NestProfiler::FrameProfiler frameProfiler(nameThread)
#define NEST_FUNC()                                                                                \
    NestProfiler::FuncProfiler funcProfiler(CURRENT_FILE, CURRENT_FUNCTION, CURRENT_LINE)