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
    : m_audioEngine((AudioEngine*)AudioEngine::get()) {
}

Audio::~Audio() {
    erase();
}

void Audio::erase() {
    m_audioEngine->eraseAudio(m_id);
}

void Audio::load(const Nest::Path &filePath) {
   m_id = m_audioEngine->addAudio(filePath);
}

void Audio::play() {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
    m_audioEngine->playAudio(m_id);
}

void Audio::stop() {
    if (m_id == AUDIO_INVALID_UUID) {
        LOG_ERROR("Audio not loaded!");
        return;
    }
}

void Audio::pause() {}

void Audio::resume() {}

bool Audio::isPlaying() const {
    return true;
}

void Audio::setVolume(float volume) {}

void Audio::setPan(float pan) {}

void Audio::setEcho(float delay, float decay) {}

void Audio::setReverb(float roomSize, float damping) {}

void Audio::setPlaybackSpeed(float speed) {}

void Audio::clearEffects() {}

}