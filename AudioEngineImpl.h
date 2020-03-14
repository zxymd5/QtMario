#ifndef AUDIOENGINEIMPL_H
#define AUDIOENGINEIMPL_H

#include <functional>
#include <iostream>
#include <map>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "AudioEngine.h"

#define MAX_AUDIOINSTANCES 32

class AudioEngineImpl
{
public:
    AudioEngineImpl();
    ~AudioEngineImpl();

    void initTimer();
    static void threadFunc(sigval_t sig);

    bool init();
    int play2d(const std::string &fileFullPath ,bool loop ,float volume);
    void setVolume(int audioID,float volume);
    void setLoop(int audioID, bool loop);
    bool pause(int audioID);
    bool resume(int audioID);
    bool stop(int audioID);
    void stopAll();
    float getDuration(int audioID);
    float getCurrentTime(int audioID);
    bool setCurrentTime(int audioID, float time);
    void setFinishCallback(int audioID, const std::function<void (int, const std::string &)> &callback);

    void uncache(const std::string& filePath);
    void uncacheAll();


    int preload(const std::string& filePath, std::function<void(bool isSuccess)> callback);

    void update(float dt);
    void onSoundFinished(FMOD::Channel * channel);

private:
    FMOD::Sound * findSound(const std::string &path);

    FMOD::Channel * getChannel(FMOD::Sound *);

    struct ChannelInfo{
        int id;
        std::string path;
        FMOD::Sound * sound;
        FMOD::Channel * channel;
        bool loop;
        float volume;
        std::function<void (int, const std::string &)> callback;
    };

    std::map<int, ChannelInfo> mapChannelInfo;

    std::map<std::string, int> mapId;

    std::map<std::string, FMOD::Sound *> mapSound;

    FMOD::System* pSystem;

    timer_t timer_id;
};

#endif // AUDIOENGINEIMPL_H
