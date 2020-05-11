#ifndef __SETTINGS_H__
#define __SETTINGS_H__


#include <QTime>
#include <QDebug>
#include <QSettings>

/* 시리얼디버그 포트를 사용하여 디버깅 */
#define USE_SERIAL_DEBUG

/* 디버깅 메시지 출력에 사용하는 현재시간 매크로 */
#define CURRENT_DATETIME \
    qPrintable(QDateTime::currentDateTime().toString(Settings::LOG_DATETIME_FORMAT))

/**
 * @brief 시스템 전역설정
 */
class Settings
{
public:
       
    constexpr static const char * const LOG_DATETIME_FORMAT = "[yyyy-MM-dd hh:mm:ss.zzz] ";

private:
    static const int MIN_VOLUME_LEVEL = 0;
    static const int MAX_VOLUME_LEVEL = 127;
    static const int MIN_BACKLIGHT_LEVEL = 1;
    static const int MAX_BACKLIGHT_LEVEL = 10;
    
private:
    constexpr static const char * const ORGANIZATION = "Maru";

    constexpr static const char * const ADDRESS = "Address";
    constexpr static const char * const NETMASK = "Netmask";
    constexpr static const char * const GATEWAY = "Gateway";
    constexpr static const char * const SERVER_ADDRESS = "ServerAddress";
    constexpr static const char * const SERVER_PORT = "ServerPort";
    constexpr static const char * const VOLUME_LEVEL = "VolumeLevel";
    constexpr static const char * const BACKLIGHT_LEVEL = "BacklightLevel";

public:
    /* IP주소 */
    static inline QString address() { return value(ADDRESS).toString(); }
    static inline void setAddress(const QString &address) { setValue(ADDRESS, address); }
    /* NETMASK */
    static inline QString netmask() { return value(NETMASK).toString(); }
    static inline void setNetmask(const QString &netmask) { setValue(NETMASK, netmask); }
    /* GATEWAY */
    static inline QString gateway() { return value(GATEWAY).toString(); }
    static inline void setGateway(const QString &gateway) { setValue(GATEWAY, gateway); }
    /* 서버IP주소 */
    static inline QString serverAddress() { return value(SERVER_ADDRESS).toString(); }
    static inline void setServerAddress(const QString &address)
    {
        setValue(SERVER_ADDRESS, address);
    }
    /* 서버포트 - default:45056 */
    static inline quint16 serverPort() { return value(SERVER_PORT).toUInt(); }
    static inline void setServerPort(quint16 port) { setValue(SERVER_PORT, port); }
   

    /* 음량수준:0 ~ 31 */
    static inline int volumeLevel() { return value(VOLUME_LEVEL, MAX_VOLUME_LEVEL).toUInt(); }
    static inline void setVolumeLevel(int level)
    {
        setValue(
                  VOLUME_LEVEL
                , (
                          (level < MIN_VOLUME_LEVEL) ? MIN_VOLUME_LEVEL
                        : (level > MAX_VOLUME_LEVEL) ? MAX_VOLUME_LEVEL
                        : level
                    )
            );
    }
    /* LCD 백라이트 밝기수준:1 ~ 10 */
    static inline int backlightLevel()
    {
        return value(BACKLIGHT_LEVEL, MAX_BACKLIGHT_LEVEL).toUInt();
    }
    static inline void setBacklightLevel(int level)
    {
        setValue(
                  BACKLIGHT_LEVEL
                , (
                          (level < MIN_BACKLIGHT_LEVEL) ? MIN_BACKLIGHT_LEVEL
                        : (level > MAX_BACKLIGHT_LEVEL) ? MAX_BACKLIGHT_LEVEL
                        : level
                    )
            );
    }
    
private:
    Settings() {}

public:
#if 0
    static inline QVariant value(QString key, const QVariant &defaultValue = QVariant())
    {
        return QSettings(ORGANIZATION).value(key, defaultValue);
    }
    static inline void setValue(QString key, QVariant value)
    {
        QSettings(ORGANIZATION).setValue(key, value);
    }
#else
    static inline QVariant value(QString key, const QVariant &defaultValue = QVariant())
    {
        return QSettings("/app/conf/maru.ini",QSettings::IniFormat).value(key, defaultValue);
    }
    static inline void setValue(QString key, QVariant value)
    {
        QSettings("/app/conf/maru.ini",QSettings::IniFormat).setValue(key, value);
    }
#endif

private:
    static quint8 _orgNo;
    static quint16 _meansCode;
    static QString _stationName;
    static QString _equipmentId;
    static QTime _startTime;
    static QTime _endTime;

    static bool _dockingMode;
    static quint8 _operationStatus;

    static quint32 _psgCount;
    static quint8 _userMode;
    static bool _usbMemExist;
    static bool _printerOpened;
    static bool _printerNoPaper;

};

#endif // __SETTINGS_H__
