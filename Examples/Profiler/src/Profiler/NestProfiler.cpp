//
// Created by Bogdan on 30.07.2024.
//

#include "NestProfiler.hpp"

NestProfiler::Instrumentor *NestProfiler::Instrumentor::s_instance = new NestProfiler::Instrumentor;

void NestProfiler::Instrumentor::addFrame(const FrameInfo &frameInfo) {
    currentFrame++;
    m_appInfo.m_frameResults.emplace_back(frameInfo);
}

void NestProfiler::Instrumentor::setEndTimeFrame(const uint64_t &endTime) {
    m_appInfo.m_frameResults[currentFrame].endTime = endTime;
    uint64_t start = m_appInfo.m_frameResults[currentFrame].startTime,
             end = m_appInfo.m_frameResults[currentFrame].endTime;
    m_appInfo.m_frameResults[currentFrame].elapsedTime = end - start;
}

void NestProfiler::Instrumentor::addFunc(const FuncInfo &funcInfo) {
    m_appInfo.m_frameResults[currentFrame].funcInfo.emplace_back(funcInfo);
}

AppInfo &NestProfiler::Instrumentor::getResult() {
    return m_appInfo;
}