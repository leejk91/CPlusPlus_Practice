#include "SoundUtil.h"
#include "Settings.h"

#include <QFile>
#include <QDebug>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <alsa/asoundlib.h>

#include <sys/time.h>

#include <QDebug>

//#define USE_QSOUND


QMutex SoundUtil::_mutex(QMutex::Recursive);
#ifdef USE_QSOUND
bool SoundUtil::_inited = false;
QObject *SoundUtil::_parent = nullptr;
QVector<QSound *> SoundUtil::_systemSounds;
QMap<QString, QSound *> SoundUtil::_userSounds;
#else
QProcess SoundUtil::process;
#endif


bool SoundUtil::init(QObject *parent)
{
    QMutexLocker locker(&_mutex);

#ifdef USE_QSOUND
    if (_inited)
    {
        qDebug().nospace()
                << "system sound init fail - aleady init."
            ;

        return false;
    }

    _systemSounds
            << new QSound(SYSTEM_SOUND_0, parent)
            << new QSound(SYSTEM_SOUND_1, parent)
            << new QSound(SYSTEM_SOUND_2, parent)            
        ;
    _parent = parent;
    _inited = true;

    #define OSS_MIXER_DEVICE "/dev/mixer"

    if (QFile(OSS_MIXER_DEVICE).exists())
    {
        int volume = 100 | (100 << 8);
        int fd = ::open(OSS_MIXER_DEVICE, O_RDONLY);

        if (fd >= 0)
        {
            ::ioctl(fd, SOUND_MIXER_WRITE_VOLUME, &volume);
            ::close(fd);
        }
    }
#else
    Q_UNUSED(parent)
#endif
    return true;
}

void SoundUtil::destroy()
{
#ifdef USE_QSOUND
    QMutexLocker locker(&_mutex);

    foreach (QSound *sound, _systemSounds)
    {
        if (!sound->isFinished()) sound->stop();

        delete sound;
    }

    _systemSounds.clear();

    foreach (QSound *sound, _userSounds.values())
    {
        if (!sound->isFinished()) sound->stop();

        delete sound;
    }

    _userSounds.clear();

    _inited = false;
#endif
}

bool SoundUtil::mute(const char *name, bool on)
{
    int r = false;

#ifdef USE_QSOUND
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";

    if (snd_mixer_open(&handle, 0))
    {
        qDebug().nospace() << CURRENT_DATETIME << "no sound: mixer open fail";

        return r;
    }

    if (snd_mixer_attach(handle, card)
            || snd_mixer_selem_register(handle, nullptr, nullptr)
            || snd_mixer_load(handle)
        )
    {
        qDebug().nospace() << CURRENT_DATETIME << "no sound: mixer fail";
    }
    else
    {
        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, name ? name : SPEAKER);

        snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

        if (!elem || !snd_mixer_selem_has_playback_switch(elem))
        {
            qDebug().nospace() << CURRENT_DATETIME << "no sound : device not found";
        }
        else if (snd_mixer_selem_set_playback_switch_all(elem, on ? 0 : 1))
        {
            qDebug().nospace() << CURRENT_DATETIME << "no sound: switch setting fail";
        }
        else
        {
            r = true;
        }
   }

    snd_mixer_close(handle);
#else
    Q_UNUSED(name)
    Q_UNUSED(on)
#endif

    return r;
}

#if 1
static long long get_cur_msec(void)
{
    struct      timeval mytime;
    long long   tm_sec;

    gettimeofday( &mytime, nullptr);
    tm_sec = static_cast<long long>(mytime.tv_sec);

    return ( tm_sec * 1000LL + mytime.tv_usec / 1000);
}
#endif

int SoundUtil::volume(const char *name)
{
    long level = -1;

#ifdef USE_QSOUND
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";

    //qDebug() << "get volume";

    if (snd_mixer_open(&handle, 0))
    {
        qDebug().nospace() << CURRENT_DATETIME << "get volume: mixer open fail";

        return static_cast<int>(level);
    }

    if (snd_mixer_attach(handle, card)
            || snd_mixer_selem_register(handle, nullptr, nullptr)
            || snd_mixer_load(handle)
        )
    {
        qDebug().nospace() << CURRENT_DATETIME << "get volume: mixer fail";
    }
    else
    {
        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, name ? name : SPEAKER);

        snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

        if (!elem || snd_mixer_selem_get_playback_volume_range(elem, &min, &max))
        {
            qDebug().nospace() << CURRENT_DATETIME << "get volume: device not found";
        }
        else if (snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &level))
        {
            qDebug().nospace() << CURRENT_DATETIME << "get volume: fail";
        }
        else
        {
            level = (level * 100) / max;
        }
    }

    snd_mixer_close(handle);
#else
    FILE   *fp;
    long long  prev;
    char buff[2024];

    Q_UNUSED(name)

    sprintf( buff, "amixer -c 0 sget 'Speaker' | grep \"Front Left:\" | awk {'print $4'}");
    fp = popen( buff, "r");
    if ( nullptr == fp){
        return 0;
    }

    prev = get_cur_msec();
    while( 1 ){
        if ( nullptr != fgets( buff, sizeof( buff), fp) )
            break;
        if ( 1000 < get_cur_msec() - prev){
            sprintf( buff, "0");
            break;
        }
    }
    pclose( fp);
    level = atoi( buff);
#endif

    return static_cast<int>(level);
}

bool SoundUtil::setVolume(const char *name, int level)
{
    bool r = false;

#ifdef USE_QSOUND
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";

    if (snd_mixer_open(&handle, 0))
    {
        qDebug().nospace() << CURRENT_DATETIME << "set volume: mixer open fail";

        return r;
    }

    if (snd_mixer_attach(handle, card)
            || snd_mixer_selem_register(handle, nullptr, nullptr)
            || snd_mixer_load(handle)
        )
    {
        qDebug().nospace() << CURRENT_DATETIME << "set volume: mixer fail";
    }
    else
    {
        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, name ? name : SPEAKER);

        snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

        if (!elem || snd_mixer_selem_get_playback_volume_range(elem, &min, &max))
        {
            qDebug().nospace()
                    << CURRENT_DATETIME
                    << "set volume: device not found, device name = [" << (name ? name : SPEAKER) << "]"
                ;
        }
        else if (snd_mixer_selem_set_playback_volume_all(elem, level * max / 100))
        {
            qDebug().nospace()
                    << CURRENT_DATETIME
                    << "set volume: fail, volume = [" << (level * max / 100) << "]"
                ;
        }
        else
        {
            r = true;
        }
    }

    snd_mixer_close(handle);
#else
    char buff[2048];

    Q_UNUSED(name)

    if ( 0 > level){
        level = 0;
    } else if ( 127 < level){
        level = 127;
    }

    sprintf( buff, "amixer -c 0 sset 'Speaker' %d%%", level);

    qDebug() << "SET >> " << buff;

    if ( 0 != system( buff)){
        return -1;
    }
    r = true;
#endif

    return r;
}

#if 0
/**
 * @brief 시스템 사운드 재생
 *
 * @param no 시스템 사운드 번호
 * @param loops 반복 회수
 * @param mix 이전 재생 중인 사운드와 혼합여부
 *
 * @return 성공여부
 */
bool SoundUtil::play(SystemSound no, int loops, bool mix)
{
    QMutexLocker locker(&_mutex);

    if (!_inited)
    {
        qDebug().nospace()
                << CURRENT_DATETIME
                << "system sound play fail - init first."
            ;

        return false;
    }

    if ((no < 0) || (no >= _systemSounds.size()))
    {
        qDebug().nospace()
                << CURRENT_DATETIME
                << "system sound play fail - unknown system sound number = " << no
            ;

        return false;
    }

    if (!mix) stop();

    _systemSounds[no]->setLoops(loops);
    _systemSounds[no]->play();

    return true;
}
#endif

bool SoundUtil::play(const QString &fileName, int loops, bool mix)
{
    if (!QFile(fileName).exists())
    {
        qDebug().nospace()
                << CURRENT_DATETIME
                << "sound play fail - file not found = "
                << qPrintable(fileName)
            ;

        return false;
    }

#ifdef USE_QSOUND
    QSound *sound;
    QMutexLocker locker(&_mutex);

    if (!_inited)
    {
        qDebug().nospace()
                << CURRENT_DATETIME
                << "sound play fail - init first."
            ;

        return false;
    }

    if (!mix) stop();

    if (!_userSounds.contains(fileName))
    {
        sound = new QSound(fileName, _parent);
        _userSounds.insert(fileName, sound);
    } else {
        sound = _userSounds[fileName];
    }

    sound->setLoops(loops);
    sound->play();

#else
    Q_UNUSED(mix)
    Q_UNUSED(loops)

    process.close();
    process.start("aplay " + fileName);
    //process.execute("aplay " + fileName);
#endif

    return true;
}

/**
 * @brief 재생 중인 모든 사운드 재생중지
 */
void SoundUtil::stop()
{
    QMutexLocker locker(&_mutex);

#ifdef USE_QSOUND
    if (!_inited)
    {
        qDebug().nospace()
                << CURRENT_DATETIME
                << "system sound play fail - init first."
            ;

        return;
    }

    foreach (QSound *sound, _systemSounds) if (!sound->isFinished()) sound->stop();
    foreach (QSound *sound, _userSounds) if (!sound->isFinished()) sound->stop();
#else
    process.close();
#endif

}
