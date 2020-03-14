#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

class AudioEngineImpl;

class AudioProfile
{
public:
    std::string name;
    unsigned int maxInstances;
    double minDelay;

    AudioProfile()
        : maxInstances(0)
        , minDelay(0.0)
    {

    }
};

class AudioEngine
{
public:
    enum class AudioState
    {
        ERROR  = -1,
        INITIALIZING,
        PLAYING,
        PAUSED
    };

    static const int INVALID_AUDIO_ID;
    static const float TIME_UNKNOWN;

    static bool lazyInit();
    static void end();
    static AudioProfile* getDefaultProfile();
    static int play2d(const std::string& filePath, bool loop = false, float volume = 1.0f, const AudioProfile *profile = nullptr);
    static void setLoop(int audioID, bool loop);
    static bool isLoop(int audioID);
    static void setVolume(int audioID, float volume);
    static float getVolume(int audioID);
    static void pause(int audioID);
    static void pauseAll();
    static void resume(int audioID);
    static void resumeAll();
    static void stop(int audioID);
    static void stopAll();
    static bool setCurrentTime(int audioID, float sec);
    static float getCurrentTime(int audioID);
    static float getDuration(int audioID);
    static AudioState getState(int audioID);
    static void setFinishCallback(int audioID, const std::function<void(int,const std::string&)>& callback);
    static int getMaxAudioInstance() {return _maxInstances;}
    static bool setMaxAudioInstance(int maxInstances);
    static void uncache(const std::string& filePath);
    static void uncacheAll();
    static AudioProfile* getProfile(int audioID);
    static AudioProfile* getProfile(const std::string &profileName);
    static void preload(const std::string& filePath) { preload(filePath, nullptr); }
    static void preload(const std::string& filePath, std::function<void(bool isSuccess)> callback);
    static int getPlayingAudioCount();
    static void setEnabled(bool isEnabled);
    static bool isEnabled();

protected:
    static void addTask(const std::function<void()>& task);
    static void remove(int audioID);

    struct ProfileHelper
    {
        AudioProfile profile;
        std::list<int> audioIDs;
        double lastPlayTime;

        ProfileHelper()
            : lastPlayTime(0.0)
        {

        }
    };

    struct AudioInfo
    {
        const std::string* filePath;
        ProfileHelper* profileHelper;

        float volume;
        bool loop;
        float duration;
        AudioState state;

        AudioInfo();
        ~AudioInfo();
    private:
        AudioInfo(const AudioInfo& info);
        AudioInfo(AudioInfo&& info);
        AudioInfo& operator=(const AudioInfo& info);
        AudioInfo& operator=(AudioInfo&& info);
    };

    static std::unordered_map<int, AudioInfo> _audioIDInfoMap;
    static std::unordered_map<std::string,std::list<int>> _audioPathIDMap;
    static std::unordered_map<std::string, ProfileHelper> _audioPathProfileHelperMap;
    static unsigned int _maxInstances;

    static ProfileHelper* _defaultProfileHelper;

    static AudioEngineImpl* _audioEngineImpl;

    class AudioEngineThreadPool;
    static AudioEngineThreadPool* s_threadPool;

    static bool _isEnabled;

    friend class AudioEngineImpl;
};

#endif // AUDIOENGINE_H
