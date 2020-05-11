#include "maruserial.h"
#include "Checksum.h"

#include <QDebug>
#include <QDataStream>
#include <QDateTime>

static quint8 sequence;
static QSerialPort *debugger, *controller, *reader;

MaruSerial MaruSerial::_instance;

MaruSerial::MaruSerial(QObject *parent) : QObject(parent)
{

}

MaruSerial::~MaruSerial()
{
    //SerialDisconnect();
}

void MaruSerial::init()
{
    debugger = new QSerialPort(this);
    controller = new QSerialPort(this);
    reader = new QSerialPort(this);

    //connect(debugger, SIGNAL(readyRead()), this, SLOT(debuggerReceived()));
    connect(controller, SIGNAL(readyRead()), this, SLOT(controllerReceived()));
    connect(reader, SIGNAL(readyRead()), this, SLOT(readerReceived()));
}

#ifdef USE_SERIAL_DEBUG
bool MaruSerial::DebugConnect()
{
#if 0
    debugger = new QSerialPort(this);
    //connect(debugger, SIGNAL(readyRead()), this, SLOT(debuggerReceived()));
#endif
    if(!debugger->isOpen()){
        debugger->setPortName("/dev/ttymxc0");
        debugger->setBaudRate(QSerialPort::Baud115200);
        debugger->setDataBits(QSerialPort::Data8);
        debugger->setParity(QSerialPort::NoParity);
        debugger->setStopBits(QSerialPort::OneStop);
        debugger->setFlowControl(QSerialPort::NoFlowControl);

        if(debugger->open(QIODevice::ReadWrite)){
        //if(debugger->open(QIODevice::WriteOnly)){
            qDebug() << "Debugger Connected";
            return true;
        }
        else{
            qDebug() << "Debugger Connect Fail";
            return false;
        }
    }
    else{
        qDebug() << "Debugger Aleady Opend";
    }

    return false;
}
#endif

bool MaruSerial::ControllerConnect()
{
#if 0
    controller = new QSerialPort(this);
    connect(controller, SIGNAL(readyRead()), this, SLOT(controllerReceived()));
#endif
    if(!controller->isOpen()){
        controller->setPortName("/dev/ttymxc3");
        controller->setBaudRate(QSerialPort::Baud115200);
        controller->setDataBits(QSerialPort::Data8);
        controller->setParity(QSerialPort::NoParity);
        controller->setStopBits(QSerialPort::OneStop);
        controller->setFlowControl(QSerialPort::NoFlowControl);

        if(controller->open(QIODevice::ReadWrite)){
            qDebug() << "Controller Connected";
            return true;
        }
        else{
            qDebug() << "Controller Connect Fail";
            return false;
        }
    }
    else{
        qDebug() << "Controller Aleady Opend";
    }

    return false;
}

bool MaruSerial::ReaderConnect()
{
#if 0
    reader = new QSerialPort(this);
    connect(reader, SIGNAL(readyRead()), this, SLOT(readerReceived()));
#endif
    if(!reader->isOpen()){
        reader->setPortName("/dev/ttymxc1");
        reader->setBaudRate(QSerialPort::Baud115200);
        reader->setDataBits(QSerialPort::Data8);
        reader->setParity(QSerialPort::NoParity);
        reader->setStopBits(QSerialPort::OneStop);
        reader->setFlowControl(QSerialPort::NoFlowControl);

        if(reader->open(QIODevice::ReadWrite)){
            qDebug() << "Reader Connected";
            sequence = 1;
            return true;
        }
        else{
            qDebug() << "Reader Connect Fail";
            return false;
        }
    }
    else{
        qDebug() << "Reader Aleady Opend";
    }

    return false;
}

bool MaruSerial::ControllerDisconnect()
{
    if(controller->isOpen()){
        controller->close();
        return true;
    }

    return false;
}

bool MaruSerial::ReaderDisconnect()
{
    if(reader->isOpen()){
        reader->close();
        return true;
    }
    return false;
}

void MaruSerial::SerialConnect()
{
#ifdef  USE_SERIAL_DEBUG
    DebugConnect();
#endif
    ControllerConnect();
    ReaderConnect();
}



void MaruSerial::SerialDisconnect()
{
#ifdef USE_SERIAL_DEBUG
    if(debugger->isOpen()){
        debugger->close();
    }
#endif

    if(controller->isOpen()){
        controller->close();
    }

    if(reader->isOpen()){
        reader->close();
    }
}

bool MaruSerial::sendToDebugger(QString message)
{
    if (debugger->write(message.toLocal8Bit()) < 0) {
        return false;
    }
    return true;
}

bool MaruSerial::sendToControllerTEST(const char *data, quint16 len)
{
    QByteArray message = QByteArray::fromRawData(data, len);

    if (controller->write(message) < 0) {
        return false;
    }

    return true;
}

bool MaruSerial::sendToController(quint8 cmd, const char *data, quint16 len)
{
    if (!data) len = 0;

    QByteArray message;
    QDataStream ostream(&message, QIODevice::Append);
    ostream.setByteOrder(QDataStream::BigEndian);

    quint8 stx = 0x02;

    if(sequence == 255){
        sequence = 1;
    }
    else {
        sequence++;
    }

    ostream << stx << sequence << SENDER_INDEX << RECEIVER_INDEX << cmd << len;

    if (data && len) ostream.writeRawData(data, len);
    Checksum::CRC16Append(message, message.length()-1);
    message.append(QByteArray::fromHex("03"));

    if (controller->write(message) < 0) {
        qDebug() << "[CTRL] 전송실패";
        return false;
    }

    QString str = "[CTRL] >> " + message.toHex();
    MaruSerial::instance().Display(str, 0);
    return true;
}




bool MaruSerial::sendMessage(quint8 cmd, const char *data, quint16 len)
{
    if (!data) len = 0;

    QByteArray message;
    QDataStream ostream(&message, QIODevice::Append);
    ostream.setByteOrder(QDataStream::BigEndian);

    quint8 stx = 0x02;
    //quint8 seq = 0x00;

    if(sequence == 255){
        sequence = 1;
    }
    else {
        sequence++;
    }

    ostream << stx << sequence << SENDER_INDEX << RECEIVER_INDEX << cmd << len;

    if (data && len) ostream.writeRawData(data, len);
    Checksum::CRC16Append(message, message.length()-1);
    message.append(QByteArray::fromHex("03"));

#if 0
    int color = 0;
    QString str;
    str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz] ")
            + " <= ["
            + QString::number(message.size())
            + "]["
            + qPrintable(message.toHex())
            + "]";
    //MaruConnection::instance().Display(str, color);
#endif

    if (reader->write(message) < 0) {
        qDebug() << "[Reader] 전송실패";
        return false;
    }

    QString str = "[READER] >> " + message.toHex();
    MaruSerial::instance().Display(str, 0);
    return true;
}


bool MaruSerial::sendByte(const char *data)
{

    if (reader->write(data, 1) < 0) {
        qDebug() << "[Reader] 전송실패";
        return false;
    }

    //QString str = "[READER] >> " + QByteArray::fromRawData(data,1).toHex();
    //MaruSerial::instance().Display(str, 0);
    return true;
}


void MaruSerial::Display(QString str, int color)
{
    emit debugMessage(str, color);
}

void MaruSerial::controllerReceived()
{
    QByteArray check = controller->peek(1);
    quint8 ch = static_cast<quint8>(check.at(0));
    if(ch != 0x02){
        controller->clear(controller->AllDirections);
        return;
    }

    /* 전문 최소길이(12) 이상 수신되지 않은 경우 */
    while ((controller->bytesAvailable() >= HEADER_SIZE + 3))
    {
        quint8 stx, cmd, seq;
        quint16 len, sender, receiver, checksum;
        QByteArray header = controller->peek(HEADER_SIZE);
        {
            QDataStream istream(header);
            istream.setByteOrder(QDataStream::BigEndian);
            istream >> stx >> seq >> sender >> receiver >> cmd  >> len;

            if(stx != 0x02){
                qDebug() << "STX";
                emit messageReceived(header);
                controller->clear(reader->AllDirections);
                return;
            }

            /* 전문이 모두 수신되지 않은 경우 */
            if (controller->bytesAvailable() < (len + HEADER_SIZE + 3)) return;
        }

        QByteArray message = controller->read(len + HEADER_SIZE + 3);

        if (false == Checksum::CRC16Compare(message, message.length()-2))
        {
            checksum = static_cast<quint16>((static_cast<quint8>(message.at(len + 9)) << 8) |  static_cast<quint8>(message.at(len + 10)));
            qDebug() << " CRC Error " << QString::number(checksum, 16);
            emit ControllerMessageReceived(message);
            return;
        }

        emit ControllerMessageReceived(message);
    }
}


#if 0
void MaruSerial::readerReceived()
{
    /* 전문 최소길이(12) 이상 수신되지 않은 경우 */
    while ((reader->bytesAvailable() >= HEADER_SIZE + 3))
    {
        quint8 stx, cmd, seq;
        quint16 len, sender, receiver, checksum;
        QByteArray header = reader->peek(HEADER_SIZE);
        {
            QDataStream istream(header);
            istream.setByteOrder(QDataStream::BigEndian);
            istream >> stx >> seq >> sender >> receiver >> cmd  >> len;

            if(stx != 0x02){
                qDebug() << "STX";
                emit messageReceived(header);
                reader->clear(reader->AllDirections);
                return;
            }

            /* 전문이 모두 수신되지 않은 경우 */
            if (reader->bytesAvailable() < (len + HEADER_SIZE + 3)) return;
        }

        QByteArray message = reader->read(len + HEADER_SIZE + 3);

        if (false == Checksum::CRC16Compare(message, message.length()-2))
        {
            checksum = static_cast<quint16>((static_cast<quint8>(message.at(len + 9)) << 8) |  static_cast<quint8>(message.at(len + 10)));
            emit messageReceived(message);
            //qDebug() << " CRC Error " << QString::number(checksum, 16);
            return;
        }

        emit messageReceived(message);
    }
}
#else
void MaruSerial::readerReceived()
{
    QByteArray check = reader->peek(1);
    quint8 ch = static_cast<quint8>(check.at(0));
    if(ch != 0x02){
        reader->clear(reader->AllDirections);
        return;
    }

    /* 전문 최소길이(12) 이상 수신되지 않은 경우 */
    while ((reader->bytesAvailable() >= HEADER_SIZE + 3))
    {
        quint8 stx, cmd, seq;
        quint16 len, sender, receiver, checksum;
        QByteArray header = reader->peek(HEADER_SIZE);
        {
            QDataStream istream(header);
            istream.setByteOrder(QDataStream::BigEndian);
            istream >> stx >> seq >> sender >> receiver >> cmd  >> len;

            if(stx != 0x02){
                qDebug() << "STX";
                emit messageReceived(header);
                reader->clear(reader->AllDirections);
                return;
            }

            /* 전문이 모두 수신되지 않은 경우 */
            if (reader->bytesAvailable() < (len + HEADER_SIZE + 3)) return;
        }

        QByteArray message = reader->read(len + HEADER_SIZE + 3);

        if (false == Checksum::CRC16Compare(message, message.length()-2))
        {
            checksum = static_cast<quint16>((static_cast<quint8>(message.at(len + 9)) << 8) |  static_cast<quint8>(message.at(len + 10)));
            qDebug() << " CRC Error " << QString::number(checksum, 16);
            emit messageReceived(message);
            return;
        }

        emit messageReceived(message);
    }
}
#endif

