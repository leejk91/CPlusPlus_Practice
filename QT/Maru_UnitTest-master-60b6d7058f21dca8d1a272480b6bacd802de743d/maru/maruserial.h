#ifndef MARUSERIAL_H
#define MARUSERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "Settings.h"

#define MARU_DEBUGGER_SERIAL_PORT   "/dev/ttymxc0"
#define MARU_READER_SERIAL_PORT     "/dev/ttymxc1"
#define MARU_CONTROLLR_SERIAL_PORT  "/dev/ttymxc3"

class MaruSerial : public QObject
{
    Q_OBJECT

public:
    enum COMMAND_CODE
    {
        POLLING = 0x10,             // 폴링
        REBOOT  = 0xB0 ,            // 모듈 재부팅
        CREDIT_REQUEST = 0xB1 ,     // 신용 카드 거래요청
        CREDIT_RESULT  = 0xB2 ,     // 거래 결과 전송
        TIME_SETTING   = 0xB4 ,     // 시간 설정
        EJECT   = 0xBD ,            // 카드 배출
        UPDATE_INIT = 0xBE ,        // 업데이트 초기화
        UPDATE = 0xBF ,             // 업데이트

        IC_STATUS = 0xC1 ,          // IC CARD 상태정보
        DISPLAY = 0xC2 ,            // 디스플레이 요청
    };

    enum CTRL_COMMAND_CODE
    {
        SET_CONFIG = 0x92,
        GET_CONFIG = 0x93,
        SET_TID	   = 0x94,
        GET_TID	   = 0x95,
        SET_CID	   = 0x57,
        GET_CID	   = 0x96,
        SET_HDATA  = 0x97,

        CMD_BINGO_TRANSACTION4 = 0xCB,
        CMD_BINGO_TRANSACTION4_RES =0xCC,
        CMD_BINGO_TRANSACTION4_KTC_KEY_RENEWAL = 0xD1,
    };

    static const quint16 SENDER_INDEX = 0x0E01;
    static const quint16 RECEIVER_INDEX = 0x0B01;
    static const quint16 MAX_DATA_SIZE = 65535;
    static const qint16 HEADER_SIZE = 9;                /* 전문 헤더크기 */

public:
    explicit MaruSerial(QObject *parent = nullptr);
    ~MaruSerial();

    void init();
#ifdef USE_SERIAL_DEBUG
    bool DebugConnect();
#endif
    bool ControllerConnect();
    bool ReaderConnect();

    bool ControllerDisconnect();
    bool ReaderDisconnect();

    void SerialConnect();
    void SerialDisconnect();

    static bool sendToDebugger(QString message);

    //
    static bool sendToControllerTEST(const char *data, quint16 len);
    static inline bool sendToControllerTEST(const QByteArray &data)
    {
        return sendToControllerTEST(data.constData(),  static_cast<quint16>(data.size()));
    }

    //
    static bool sendToController(quint8 cmd, const char *data, quint16 len);
    static inline bool sendToController(quint8 cmd, const QByteArray &data)
    {
        return sendToController(cmd, data.constData(), static_cast<quint16>(data.size()));
    }
    static inline bool sendToController(quint8 cmd)
    {
        return sendToController(cmd, nullptr, 0);
    }


    //
    static bool sendByte(const char *data);

    static bool sendMessage(
             quint8 cmd
            , const char *data
            , quint16 len
        );
    static inline bool sendMessage(quint8 cmd, const QByteArray &data)
    {
        return sendMessage(cmd, data.constData(),  static_cast<quint16>(data.size()));
    }
    static inline bool sendMessage(quint8 cmd)
    {
        return sendMessage(cmd, nullptr, 0);
    }

    static inline MaruSerial &instance() { return _instance; }

    void Display(QString str, int color);

signals:
    void ControllerMessageReceived(const QByteArray &message);
    void messageReceived(const QByteArray &message);
    void debugMessage(QString str, int color);

private:
    //QSerialPort *controller, *reader;
    static MaruSerial _instance;


private slots:
    void controllerReceived();
    void readerReceived();

};

#endif // MARUSERIAL_H
