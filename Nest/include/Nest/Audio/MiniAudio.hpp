//
// Created by Bogdan on 25.11.2024.
//

#pragma once

#include "AudioBase.hpp"

#include <miniaudio.h>

namespace Nest {

class MiniAudioEngine : public AudioEngine {
public:
    MiniAudioEngine();
    ~MiniAudioEngine() override;
    void init() override;

private:
    UUID addAudio(const Path &filePath) override;
    void playAudio(const UUID &uuid, float sec) override;
    void eraseAudio(const UUID &uuid) override;
    void setVolume(const UUID &uuid, float volume) override;
    void stopAudio(const UUID &uuid) override;
    void setEcho(const UUID &uuid, float delay, float decay) override;
    void setPlaybackSpeed(const UUID &uuid, float speed) override;
    bool isPlaying(const UUID &uuid) override;

    void eraseAllAudio();
    size_t findAudio(const UUID &uuid);

    ma_engine *m_engine;
    bool m_engineInit;

    bool m_echo;

    ma_delay_node *m_delayNode;

    std::vector<std::pair<UUID, ma_sound>> m_listAudio;
};

} // namespace Nest