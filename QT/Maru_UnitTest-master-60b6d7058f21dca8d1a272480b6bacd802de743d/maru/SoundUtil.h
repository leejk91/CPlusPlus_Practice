#ifndef __SOUND_UTIL_H__
#define __SOUND_UTIL_H__

#include <QObject>
#include <QMap>
#include <QSound>
#include <QMutex>
#include <QVector>
#include <QString>
#include <QProcess>

/**
 * @brief 사운드 도구
 */
class SoundUtil
{
private:
    constexpr static const char * const SYSTEM_SOUND_0 = "sound/ok.wav";
    constexpr static const char * const SYSTEM_SOUND_1 = "sound/pass.wav";
    constexpr static const char * const SYSTEM_SOUND_2 = "sound/buzzer.wav";
    //constexpr static const char * const SYSTEM_SOUND_3 = "sound/3.wav";
    //constexpr static const char * const SYSTEM_SOUND_4 = "sound/4.wav";
    //constexpr static const char * const SYSTEM_SOUND_5 = "sound/5.wav";
    //constexpr static const char * const SYSTEM_SOUND_6 = "sound/6.wav";
    //constexpr static const char * const SYSTEM_SOUND_7 = "sound/7.wav";

    /* 볼륨설정과 음소거 지원 - 기본장치 스피커 */
    constexpr static const char * const SPEAKER = "Speaker";
    constexpr static const char * const HEADPHONE = "Headphone";
    constexpr static const char * const AUXOUT = "Auxout";
    /* 볼륨설정만 지원 */
    constexpr static const char * const PCM = "PCM";
    constexpr static const char * const RECORDER = "Rec";

public:
    enum SystemSound
    {
        SOUND_NONE = -1,
        SOUND_CLICK,
        SOUND_PASS,
        SOUND_ERROR,
        SOUND_IN,
        SOUND_OUT,
        SOUND_STARTUP,
        SOUND_SHUTDOWN,
        SOUND_ALARM
    };

public:
    static bool init(QObject *parent = nullptr);
    static void destroy();

    static bool mute(const char *name, bool on = true);
    inline static bool mute(bool on = true) { return mute(nullptr, on); }
    inline static bool unmute(const char *name = nullptr) { return mute(name, false); }

    static int volume(const char *name = nullptr);
    static bool setVolume(const char *name, int level);
    inline static bool setVolume(int level) { return setVolume(nullptr, level); }

    //static bool play(SystemSound sound = SOUND_CLICK, int loops = 1, bool mix = false);
    //inline static bool play(SystemSound sound, bool mix) { return play(sound, 1, mix); }
    static bool play(const QString &fileName, int loops = 1, bool mix = false);
    inline static bool play(const QString &fileName, bool mix) { return play(fileName, 1, mix); }

    static void stop();

private:
    SoundUtil();

    static QMutex _mutex;

    static bool _inited;
    static QObject *_parent;
    static QVector<QSound *> _systemSounds;
    static QMap<QString, QSound *> _userSounds;

    static QProcess process;

};

#endif // __SOUND_UTIL_H__
