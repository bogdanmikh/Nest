//
// Created by Bogdan on 25.11.2024.
//

#pragma once

#include "AudioBase.hpp"

class ma_engine;
class ma_sound;

namespace Nest {

class MiniAudioEngine : public AudioEngine {
public:
    MiniAudioEngine();
    ~MiniAudioEngine() override;
    void init() override;

private:
    UUID addAudio(const Path &filePath) override;
    void playAudio(const UUID &uuid) override;
    void eraseAudio(const UUID &uuid) override;

    size_t findAudio(const UUID &uuid);

    ma_engine* m_engine;
    bool m_engineInit;

    std::vector<std::pair<UUID, ma_sound>> m_listAudio;
};

}