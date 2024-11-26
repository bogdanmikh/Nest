//
// Created by Bogdan on 25.11.2024.
//

#pragma once

#include "Nest/Base/Base.hpp"
#include "Nest/Base/UUID.hpp"
#include <string>

#define AUDIO_INVALID_UUID -1

class Audio;

namespace Nest {

class AudioEngine {
public:
    virtual ~AudioEngine() = default;
    virtual void init() = 0;
    static void *get();

private:
    static AudioEngine *s_instance;
protected:
    virtual UUID addAudio(const Path &filePath) = 0;
    virtual void playAudio(const UUID &uuid) = 0;
    virtual void eraseAudio(const UUID &uuid) = 0;

    friend class Audio;
};

class Audio {
public:
    Audio();
    ~Audio();

    void erase();

    // Загружает музыкальный файл
    void load(const Path &filePath);

    // Воспроизводит музыку
    void play();

    // Останавливает музыку
    void stop();

    // Приостанавливает музыку
    void pause();

    // Возобновляет воспроизведение музыки
    void resume();

    // Проверяет, воспроизводится ли музыка
    bool isPlaying() const;

    // Устанавливает громкость музыки (0.0 - 1.0)
    void setVolume(float volume);

    // Устанавливает панораму (лево-право) (-1.0 - 1.0)
    void setPan(float pan);

    // Настройка эффекта эха
    void setEcho(float delay, float decay);

    // Применяет эффект реверберации
    void setReverb(float roomSize, float damping);

    // Устанавливает скорость воспроизведения (1.0 - нормальная скорость)
    void setPlaybackSpeed(float speed);

    // Отключает все эффекты
    void clearEffects();

private:
    UUID m_id;

    AudioEngine *m_audioEngine;
};

}