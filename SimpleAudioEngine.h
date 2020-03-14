#ifndef SIMPLEAUDIOENGINE_H
#define SIMPLEAUDIOENGINE_H


class SimpleAudioEngine
{
public:
    static SimpleAudioEngine* getInstance();
    static void end();
    virtual void preloadBackgroundMusic(const char* filePath);
    virtual void playBackgroundMusic(const char* filePath, bool loop = false);
    virtual void stopBackgroundMusic(bool releaseData = false);
    virtual void pauseBackgroundMusic();
    virtual void resumeBackgroundMusic();
    virtual void rewindBackgroundMusic();
    virtual bool willPlayBackgroundMusic();
    virtual bool isBackgroundMusicPlaying();
    virtual float getBackgroundMusicVolume();
    virtual void setBackgroundMusicVolume(float volume);
    virtual float getEffectsVolume();
    virtual void setEffectsVolume(float volume);
    virtual unsigned int playEffect(const char* filePath, bool loop = false,
                                        float pitch = 1.0f, float pan = 0.0f, float gain = 1.0f);
    virtual void pauseEffect(unsigned int soundId);
    virtual void pauseAllEffects();
    virtual void resumeEffect(unsigned int soundId);
    virtual void resumeAllEffects();
    virtual void stopEffect(unsigned int soundId);
    virtual void stopAllEffects();
    virtual void preloadEffect(const char* filePath);
    virtual void unloadEffect(const char* filePath);

protected:
    SimpleAudioEngine();
    virtual ~SimpleAudioEngine();
};

#endif // SIMPLEAUDIOENGINE_H
