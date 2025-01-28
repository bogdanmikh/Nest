//
// Created by Bogdan on 25.11.2024.
//

#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>

#include "Nest/Audio/MiniAudio.hpp"

namespace Nest {

MiniAudioEngine::MiniAudioEngine()
    : m_engineInit(false)
    , m_echo(false) {}

MiniAudioEngine::~MiniAudioEngine() {
    eraseAllAudio();
    ma_engine_uninit(m_engine);
    F_DELETE(Foundation::getAllocator(), m_engine);
}

void MiniAudioEngine::init() {
    ma_result result;
    m_engine = F_NEW(Foundation::getAllocator(), ma_engine);

    result = ma_engine_init(nullptr, m_engine);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to initialize audio engine.");
        F_DELETE(Foundation::getAllocator(), m_engine);
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

    result = ma_sound_init_from_file(
        m_engine, filePath.c_str(), 0, NULL, NULL, &m_listAudio.back().second
    );

    if (result != MA_SUCCESS) {
        m_listAudio.pop_back();
        LOG_ERROR("Failed to initialize audio \"{}\".", filePath.c_str());
        return AUDIO_INVALID_UUID;
    }
    LOG_INFO("Loaded audio: {}", filePath.c_str());
    return m_listAudio.back().first;
}

void MiniAudioEngine::playAudio(const UUID &uuid, float sec) {
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

    ma_sound_stop(&sound);

    ma_uint64 seekTimeInMilliseconds = (ma_uint64)(sec * 1000.0f);
    ma_sound_seek_to_pcm_frame(&sound, seekTimeInMilliseconds);
    ma_sound_start(&sound);
}

void MiniAudioEngine::stopAudio(const UUID &uuid) {
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

    ma_sound_stop(&sound);
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
    if (m_echo) {
        F_DELETE(Foundation::getAllocator(), m_delayNode);
        ma_delay_node_uninit(m_delayNode, NULL);
        m_echo = false;
    }
    m_listAudio.erase(m_listAudio.begin() + index);
}

bool MiniAudioEngine::isPlaying(const UUID &uuid) {
    if (!m_engineInit) {
        LOG_ERROR("Audio engine not init!");
        return false;
    }

    size_t index = findAudio(uuid);
    if (index == -1) {
        LOG_WARN("Audio not found");
        return false;
    }
    auto &sound = m_listAudio[index].second;
    return ma_sound_is_playing(&sound);
}

void MiniAudioEngine::setVolume(const UUID &uuid, float volume) {
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
    ma_sound_set_volume(&sound, volume);
}
// delay in seconds
void MiniAudioEngine::setEcho(const UUID &uuid, float delay, float decay) {
    if (!m_engineInit) {
        LOG_ERROR("Audio engine not init!");
        return;
    }

    size_t index = findAudio(uuid);
    if (index == -1) {
        LOG_WARN("Audio not found");
        return;
    }
    m_echo = true;

    m_delayNode = F_NEW(Foundation::getAllocator(), ma_delay_node);

    ma_delay_node_config delayNodeConfig;
    ma_uint32 channels;
    ma_uint32 sampleRate;

    channels = ma_engine_get_channels(m_engine);
    sampleRate = ma_engine_get_sample_rate(m_engine);

    delayNodeConfig =
        ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * delay), decay);

    ma_result result;
    result =
        ma_delay_node_init(ma_engine_get_node_graph(m_engine), &delayNodeConfig, NULL, m_delayNode);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to initialize delay node.");
        return;
    }
    ma_node_attach_output_bus(m_delayNode, 0, ma_engine_get_endpoint(m_engine), 0);

    auto &sound = m_listAudio[index].second;
    ma_node_attach_output_bus(&sound, 0, m_delayNode, 0);
}

void MiniAudioEngine::setPlaybackSpeed(const UUID &uuid, float speed) {
    if (!m_engineInit) {
        LOG_ERROR("Audio engine not init!");
        return;
    }

    size_t index = findAudio(uuid);
    if (index == -1) {
        LOG_WARN("Audio not found");
        return;
    }
    speed = std::max(0.0f, speed);
    auto &sound = m_listAudio[index].second;
    ma_sound_set_pitch(&sound, speed);
}

void MiniAudioEngine::eraseAllAudio() {
    for (auto &[id, sound] : m_listAudio) {
        ma_sound_uninit(&sound);
    }
    m_listAudio.clear();
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

} // namespace Nest
