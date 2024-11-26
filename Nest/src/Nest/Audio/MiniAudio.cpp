//
// Created by Bogdan on 25.11.2024.
//


#include "Nest/Audio/MiniAudio.hpp"
#include <Foundation/Logger.hpp>
#include <Foundation/Allocator.hpp>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace Nest {

MiniAudioEngine::MiniAudioEngine()
    : m_engineInit(false) {}

MiniAudioEngine::~MiniAudioEngine() {
    ma_engine_uninit(m_engine);
    DELETE(Foundation::getAllocator(), m_engine);
}

void MiniAudioEngine::init() {
    ma_result result;
    m_engine = NEW(Foundation::getAllocator(), ma_engine);

    result = ma_engine_init(nullptr, m_engine);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to initialize audio engine.");
        return;
    }
    m_engineInit = true;
}

UUID MiniAudioEngine::addAudio(const Nest::Path &filePath) {
    if (!m_engineInit) {
        LOG_ERROR("Audio engine not init!");
        return AUDIO_INVALID_UUID;
    }
    if (!std::filesystem::exists(filePath)) {
        LOG_ERROR("Audio: {} not exists", filePath.c_str());
        return AUDIO_INVALID_UUID;
    }
    
    ma_result result;
    m_listAudio.emplace_back();


    result = ma_sound_init_from_file(m_engine, filePath.c_str(), 0, NULL, NULL, &m_listAudio.back().second);

    if (result != MA_SUCCESS) {
        m_listAudio.pop_back();
        LOG_ERROR("Failed to initialize audio \"{}\".", filePath.c_str());
        return AUDIO_INVALID_UUID;
    }
    LOG_INFO("Loaded audio: {}", filePath.c_str());

    return m_listAudio.back().first;
}

void MiniAudioEngine::playAudio(const UUID &uuid) {
    if (!m_engineInit) {
        LOG_ERROR("Audio engine not init!");
        return;
    }

    size_t index = findAudio(uuid);
    if (index == -1) {
        LOG_WARN("Audio not found");
        return;
    }
    auto &sound = m_listAudio[index].second;

    ma_sound_start(&sound);
}

void MiniAudioEngine::eraseAudio(const UUID &uuid) {
    if (!m_engineInit) {
        LOG_ERROR("Audio engine not init!");
        return;
    }

    size_t index = findAudio(uuid);
    if (index == -1) {
        LOG_WARN("Audio not found");
        return;
    }
    auto &sound = m_listAudio[index].second;
    ma_sound_uninit(&sound);
    m_listAudio.erase(m_listAudio.begin() + index);
}

size_t MiniAudioEngine::findAudio(const UUID &uuid) {
    size_t index = 0;
    for (auto &[id, sound] : m_listAudio) {
        if (uuid == id) {
            return index;
        }
        index++;
    }
    return -1;
}

}
