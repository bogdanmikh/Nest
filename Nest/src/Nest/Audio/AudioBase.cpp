//
// Created by Bogdan on 25.11.2024.
//
#include "Nest/Audio/AudioBase.hpp"
#include "Nest/Audio/MiniAudio.hpp"

#include "Foundation/Logger.hpp"

namespace Nest {

AudioEngine *AudioEngine::s_instance = nullptr;

void *AudioEngine::get() {
    if (s_instance == nullptr) {
        s_instance = new MiniAudioEngine;
        s_instance->init();
    }
    return s_instance;
}

Audio::Audio()
    : m_audioEngine((AudioEngine *)AudioEngine::get())
    , m_id(AUDIO_INVALID_UUID) {}

Audio::~Audio() {
    erase();
}

void Audio::erase() {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
    m_audioEngine->eraseAudio(m_id);
}

void Audio::load(const Nest::Path &filePath) {
    m_id = m_audioEngine->addAudio(filePath);
}

void Audio::play(float sec) {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
    m_audioEngine->playAudio(m_id, sec);
}

void Audio::stop() {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
    m_audioEngine->stopAudio(m_id);
}

bool Audio::isPlaying() const {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return false;
    }
    return m_audioEngine->isPlaying(m_id);
}

void Audio::setVolume(float volume) {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
    m_audioEngine->setVolume(m_id, volume);
}

void Audio::setPan(float pan) {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
}

void Audio::setEcho(float delay, float decay) {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
    m_audioEngine->setEcho(m_id, delay, decay);
}

void Audio::setReverb(float roomSize, float damping) {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
}

void Audio::setPlaybackSpeed(float speed) {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
    m_audioEngine->setPlaybackSpeed(m_id, speed);
}

void Audio::clearEffects() {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
}

} // namespace Nest