#pragma once

#include <Rain/Codable.hpp>
#include <Rain/Rain.hpp>

struct FuncInfo : public Rain::Codable {
    std::string nameFile;
    std::string nameFunc;
    int line;
    uint64_t startTime;
    uint64_t endTime;

    RAIN_FIELDS_BEGIN(FuncInfo)
    RAIN_FIELD(nameFile)
    RAIN_FIELD(nameFunc)
    RAIN_FIELD(line)
    RAIN_FIELD(startTime)
    RAIN_FIELD(endTime)
    RAIN_FIELDS_END
};

struct FrameInfo : public Rain::Codable {
    std::vector<FuncInfo> funcInfo;
    uint64_t startTime;
    uint64_t endTime;
    std::string nameThread;
    int threadId;

    RAIN_FIELDS_BEGIN(FrameInfo)
    RAIN_FIELD(startTime)
    RAIN_FIELD(endTime)
    RAIN_FIELD(funcInfo)
    RAIN_FIELD(nameThread)
    RAIN_FIELD(threadId)
    RAIN_FIELDS_END
};

struct AppInfo : public Rain::Codable {
    std::vector<FrameInfo> m_frameResults;
    RAIN_FIELDS_BEGIN(AppInfo)
    RAIN_FIELD(m_frameResults)
    RAIN_FIELDS_END
};