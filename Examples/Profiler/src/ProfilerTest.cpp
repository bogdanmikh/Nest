#include "ProfilerTest.hpp"
#include "Profiler/NestProfiler.hpp"
#include "Rain/Coders/JsonEncoder.hpp"

#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void ProfilerTest::onAttach() {}

void ProfilerTest::onDetach() {
    {
        std::ofstream file("config.json");
        if (file.is_open()) {
            Rain::Encoder *encoder = new Rain::JsonEncoder(true);
            encoder->encode(file, NestProfiler::Instrumentor::get()->getResult());
            delete encoder;
            file.close();
        } else {
            LOG_ERROR("JSON FILE NOT OPEN");
        }
    }
    delete NestProfiler::Instrumentor::get();
}

std::thread th1;
std::thread th2;

void func2() {
    NEST_FRAME("Func2");
    //    NEST_FUNC();
    for (int i = 0; i < 100000; ++i) {
        for (int j = 0; j < 1000; ++j) {}
    }
}

void ProfilerTest::onUpdate(double deltaTime) {
    NEST_FRAME("Update");
    if (th1.joinable()) {
        th1.detach();
    }

    th1 = std::thread(func2);
    th1.join();
    //    func2();
    auto &appInfo = NestProfiler::Instrumentor::get()->getResult().m_frameResults;
    ImGui::Begin("Loshara");
    ImGui::End();
}

ProfilerTest::~ProfilerTest() {}

void ProfilerTest::addEntity(Nest::Entity *entity) {}