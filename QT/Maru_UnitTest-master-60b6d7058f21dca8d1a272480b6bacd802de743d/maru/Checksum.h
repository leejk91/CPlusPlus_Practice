#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <QFile>

/**
 * @brief 체크썸
 */
class Checksum
{
public:
    static inline quint16 crc16(const QByteArray &data)
    {
        return crc16(data.constData(), data.size());
    }
    static quint16 crc16(const char *data, int len);

    static inline quint32 crc32(const QByteArray &data)
    {
        return crc32(data.constData(), data.size());
    }
    static quint32 crc32(const char *data, int len);
    static quint32 crc32(QFile &file);

    static inline QByteArray md5(const QByteArray data)
    {
        return md5(data.constData(), data.size());
    }
    static QByteArray md5(const char *data, int len);
    static QByteArray md5(QFile &file);

    static quint16 CRC16Calc(QByteArray &Buffer, int len);
    static void CRC16Append(QByteArray &Buffer, int len);
    static bool CRC16Compare(QByteArray &Buffer, int len);

private:
    Checksum();
};

#endif // __CHECKSUM_H__
