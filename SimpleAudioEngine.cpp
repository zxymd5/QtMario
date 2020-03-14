#include <iostream>

#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

struct SimpleAudioEngineLinux {
    SimpleAudioEngine * engine = nullptr;
    int musicid;
    float effectsvolume;
    std::string musicpath;
};

SimpleAudioEngineLinux * g_SimpleAudioEngineLinux = nullptr;

SimpleAudioEngine* SimpleAudioEngine::getInstance()
{
    if (!g_SimpleAudioEngineLinux) {
        g_SimpleAudioEngineLinux = new SimpleAudioEngineLinux();
        g_SimpleAudioEngineLinux->engine = new SimpleAudioEngine();
    }
    return g_SimpleAudioEngineLinux->engine;
}

void SimpleAudioEngine::end()
{
    if (g_SimpleAudioEngineLinux) {
        delete g_SimpleAudioEngineLinux->engine;
        delete g_SimpleAudioEngineLinux;
    }
    g_SimpleAudioEngineLinux = nullptr;
}

SimpleAudioEngine::SimpleAudioEngine()
{
    g_SimpleAudioEngineLinux->musicid = -1;
    g_SimpleAudioEngineLinux->effectsvolume = 1.0f;
}

SimpleAudioEngine::~SimpleAudioEngine()
{
}

void SimpleAudioEngine::preloadBackgroundMusic(const char* filePath)
{
    g_SimpleAudioEngineLinux->musicpath = filePath;
    AudioEngine::preload(filePath);
}

void SimpleAudioEngine::playBackgroundMusic(const char* filePath, bool loop)
{
    g_SimpleAudioEngineLinux->musicpath = filePath;
    g_SimpleAudioEngineLinux->musicid = AudioEngine::play2d(filePath, loop);
}

void SimpleAudioEngine::stopBackgroundMusic(bool releaseData)
{
    AudioEngine::stop(g_SimpleAudioEngineLinux->musicid);
    if (releaseData) {
        AudioEngine::uncache(g_SimpleAudioEngineLinux->musicpath.c_str());
    }
}

void SimpleAudioEngine::pauseBackgroundMusic()
{
    AudioEngine::pause(g_SimpleAudioEngineLinux->musicid);
}

void SimpleAudioEngine::resumeBackgroundMusic()
{
    AudioEngine::resume(g_SimpleAudioEngineLinux->musicid);
}

void SimpleAudioEngine::rewindBackgroundMusic()
{
    AudioEngine::setCurrentTime(g_SimpleAudioEngineLinux->musicid, 0);
}

bool SimpleAudioEngine::willPlayBackgroundMusic()
{
    return g_SimpleAudioEngineLinux->musicid != -1;
}

bool SimpleAudioEngine::isBackgroundMusicPlaying()
{
    return AudioEngine::getState(g_SimpleAudioEngineLinux->musicid) == AudioEngine::AudioState::PLAYING;
}

float SimpleAudioEngine::getBackgroundMusicVolume()
{
    return AudioEngine::getVolume(g_SimpleAudioEngineLinux->musicid);
}

void SimpleAudioEngine::setBackgroundMusicVolume(float volume)
{
    AudioEngine::setVolume(g_SimpleAudioEngineLinux->musicid, volume);
}

float SimpleAudioEngine::getEffectsVolume()
{
    return g_SimpleAudioEngineLinux->effectsvolume;
}

void SimpleAudioEngine::setEffectsVolume(float volume)
{
    g_SimpleAudioEngineLinux->effectsvolume = volume;
}

unsigned int SimpleAudioEngine::playEffect(const char* filePath, bool loop, float pitch, float pan, float gain)
{
    return AudioEngine::play2d(filePath, loop, gain);
}

void SimpleAudioEngine::pauseEffect(unsigned int soundId)
{
    AudioEngine::pause(soundId);
}

void SimpleAudioEngine::pauseAllEffects()
{
    AudioEngine::pauseAll();
}

void SimpleAudioEngine::resumeEffect(unsigned int soundId)
{
    AudioEngine::resume(soundId);
}

void SimpleAudioEngine::resumeAllEffects()
{
    AudioEngine::resumeAll();
}

void SimpleAudioEngine::stopEffect(unsigned int soundId)
{
    AudioEngine::stop(soundId);
}

void SimpleAudioEngine::stopAllEffects()
{
    AudioEngine::stopAll();
}

void SimpleAudioEngine::preloadEffect(const char* filePath)
{
    AudioEngine::preload(filePath);
}

void SimpleAudioEngine::unloadEffect(const char* filePath)
{
    AudioEngine::uncache(filePath);
}
