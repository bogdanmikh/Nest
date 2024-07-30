//
// Created by Bogdan on 30.07.2024.
//

#include "NestProfiler.hpp"

NestProfiler::Instrumentor *NestProfiler::Instrumentor::s_instance = new NestProfiler::Instrumentor;

void NestProfiler::Instrumentor::addFrame(const FrameInfo& frameInfo) {
    currentFrame++;
    m_appInfo.m_frameResults.emplace_back(frameInfo);
}

void NestProfiler::Instrumentor::addFunc(const FuncInfo &funcInfo) {
    m_appInfo.m_frameResults[currentFrame].funcInfo.emplace_back(funcInfo);
}

AppInfo& NestProfiler::Instrumentor::getResult() {
    return m_appInfo;
}