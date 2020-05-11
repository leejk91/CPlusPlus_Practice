#include "repairtestserialdialog.h"
#include "ui_repairtestserialdialog.h"

#include <QProcess>
#include <QTimer>

#include "maruserial.h"
#include "typedef.h"
#include "lib_koces.h"
#include "lib_util.h"
#include "app_config.h"

#define DBG_SERIAL 1

#if (DBG_SERIAL == 1)
# define	PRINTF(fmt,...) 	Dbg_Printf(fmt, ##__VA_ARGS__)
# define	PRINTHEX(fmt,...)  	PrintHex(fmt, ##__VA_ARGS__)
# define	HEXDUMP(fmt,...)  	HEX_Dump(fmt, ##__VA_ARGS__)
#else
# define    PRINTF(...)    (void)(0)
# define    PRINTHEX(...)  (void)(0)
# define    HEXDUMP(...)   (void)(0)
#endif

RepairTestSerialDialog::RepairTestSerialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestSerialDialog)
{
    ui->setupUi(this);

    MaruSerial &maru = MaruSerial::instance();

    connect(
                &maru
                , SIGNAL(messageReceived(const QByteArray&))
                , this
                , SLOT(readerMessageReceived(const QByteArray&))
                , Qt::QueuedConnection
                );
    connect(
                &maru
                , SIGNAL(debugMessage(QString, int))
                , this
                , SLOT(debugMessageReceived(QString, int))
                , Qt::QueuedConnection
               );

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_14->setEnabled(false);
    ui->pushButton_15->setEnabled(false);

    QFont font;
    font.setPointSize(11);
    ui->textEdit->setFont(font);

#if 0
    int result;
    unsigned char hdata[1024+16];
    unsigned char serial_number[20];
    result = KOCES_DUKPT_ReGen_Hide_Data(hdata, 1024, serial_number, strlen(reinterpret_cast<char*>(serial_number)));
#endif

    Debug_Open();
}

RepairTestSerialDialog::~RepairTestSerialDialog()
{
    Debug_Close();
    MaruSerial::instance().SerialDisconnect();
    delete ui;
}


void RepairTestSerialDialog::on_pushButton_3_clicked()
{
    //Dialog close
    close();
    emit RepairTestSerialDialogClosed();
}

void RepairTestSerialDialog::on_pushButton_clicked()
{
    // Debug connect
    MaruSerial &maru = MaruSerial::instance();
    if(maru.DebugConnect()){
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
        ui->textEdit->append("[DEBUG] connect");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void RepairTestSerialDialog::on_pushButton_2_clicked()
{
    //Debug send
    QString str = ui->lineEdit->text();

    ui->textEdit->append("[DBG] >> " + str);
    ui->textEdit->moveCursor(QTextCursor::End);

    MaruSerial::sendToDebugger(str);
}

void RepairTestSerialDialog::on_pushButton_4_clicked()
{
    //Controller connect
    MaruSerial &maru = MaruSerial::instance();
    if(maru.ControllerConnect()){
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(true);
        ui->textEdit->append("[CTRL] connect");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void RepairTestSerialDialog::on_pushButton_5_clicked()
{
    //Controller send
    QString str = ui->lineEdit_2->text();

    ui->textEdit->append("[CTRL] >> " + str);
    ui->textEdit->moveCursor(QTextCursor::End);

    MaruSerial::sendToControllerTEST(QByteArray::fromHex(str.toLatin1()));
}

void RepairTestSerialDialog::on_pushButton_12_clicked()
{
    //카드배출
    MaruSerial::sendMessage(MaruSerial::EJECT);
}

void RepairTestSerialDialog::on_pushButton_6_clicked()
{
    //Reader connect
    MaruSerial &maru = MaruSerial::instance();
    if(maru.ReaderConnect()){
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        //ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
        //ui->pushButton_13->setEnabled(true);
        ui->pushButton_14->setEnabled(true);
        //ui->pushButton_15->setEnabled(true);

        ui->textEdit->append("[READER] connect");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void RepairTestSerialDialog::debugMessageReceived(QString str, int color)
{
    switch(color){
#if 0
    case 0:
        ui->textEdit->setTextColor(QColor(0x22,0x22,0x22));
        break;
    case 1:
        ui->textEdit->setTextColor(QColor(0x00,0x00,0xDD));
        break;
    case 2:
        ui->textEdit->setTextColor(QColor(0xEE,0x00,0x00));
        break;
#else
    default:
        ui->textEdit->setTextColor(QColor(0,0,0));
        break;
#endif
    }

    ui->textEdit->append(str);
    ui->textEdit->moveCursor(QTextCursor::End);
}

void RepairTestSerialDialog::on_Step2()
{
    PRINTF("Download Step 2\n\r");
    system("sz --ymodem /app/reader/BingoMaruNFC.bin > /dev/ttymxc1 < /dev/ttymxc1");
    PRINTF("Download End\n\r");
}

void RepairTestSerialDialog::on_Step1()
{
    PRINTF("Download Step 1\n\r");

    for(int i = 0 ; i < 5 ;i++){
        MaruSerial::sendByte("1");
    }

    QTimer::singleShot(1500, this, SLOT(on_Step2()));
}

void RepairTestSerialDialog::readerMessageReceived(const QByteArray &message)
{
    quint8 stx,seq,cmd, res;
    quint16 sender,receiver, len;
    QByteArray data;
    QDataStream istream(message);
    //MaruSerial &maru = MaruSerial::instance();
    istream.setByteOrder(QDataStream::BigEndian);
    istream >> stx >> seq >> sender >> receiver >> cmd >> len;
    switch(cmd){
        case MaruSerial::IC_STATUS:
             break;
        case MaruSerial::CREDIT_REQUEST:
            unsigned char tmp[4];
            int kocesLength;
            istream >> res;
            if(res == 0x01){
                istream.readRawData(reinterpret_cast<char*>(tmp), 1);
                track2Length = tmp[0];
                istream.readRawData(reinterpret_cast<char*>(track2), track2Length);
                kocesLength = (len - 2) - track2Length;
                istream.readRawData(reinterpret_cast<char*>(koces), kocesLength);
                PRINTF(const_cast<char*>("\n\r"));
                PRINTHEX(track2, track2Length, const_cast<char*>("\n\rTRACK2:"));
                PRINTHEX(koces, static_cast<unsigned long>(kocesLength), const_cast<char*>("\n\rKOCES:"));
            }
            else{
                PRINTF(const_cast<char*>("CREDIT_REQ FAIL\n\r"));
            }
            break;

        case MaruSerial::UPDATE:
            istream >> res;
            if(res != 0xFF){
                PRINTF("Download Start \n\r");
                QTimer::singleShot(500, this, SLOT(on_Step1()));
            }
           break;
    }

    QString str = "[READER] << " + message.toHex();
    ui->textEdit->append(str);
    ui->textEdit->moveCursor(QTextCursor::End);
}

void RepairTestSerialDialog::on_pushButton_7_clicked()
{
    //Pollig
    MaruSerial::sendMessage(MaruSerial::POLLING);
}

void RepairTestSerialDialog::on_pushButton_8_clicked()
{
    //Reboot
    MaruSerial::sendMessage(MaruSerial::REBOOT);
}

void RepairTestSerialDialog::on_pushButton_9_clicked()
{
    //신용카드거래요청
    unsigned int _amount = 100;

    QByteArray time = QDateTime::currentDateTime().toString("yyyyMMddhhmmss").toUtf8();
    QByteArray amount = QString("%1").arg(_amount,12,10,QLatin1Char('0')).toUtf8();

    unsigned char temp[14], tmpTime[7], tmpAmount[6];
    memcpy(temp, time.constData(), 14);
    Char2BCD(temp, tmpTime, 14);

    memcpy(temp, amount.constData(), 12);
    Char2BCD(temp, tmpAmount, 12);

    QByteArray data;
    //data.append(QByteArray::fromRawData(reinterpret_cast<const char*>(temp2),7));

    QDataStream ostream(&data, QIODevice::Append);
    ostream.setByteOrder(QDataStream::BigEndian);
    ostream.writeRawData(reinterpret_cast<char*>(tmpTime), 7);
    ostream.writeRawData(reinterpret_cast<char*>(tmpAmount), 6);

    //ostream << _amount;
    MaruSerial::sendMessage(MaruSerial::CREDIT_REQUEST, data);
}

void RepairTestSerialDialog::on_pushButton_10_clicked()
{
    //신용카드거래결과전송
}

void RepairTestSerialDialog::on_pushButton_11_clicked()
{
    //시간세팅
    QString time = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    MaruSerial::sendMessage(MaruSerial::TIME_SETTING, time.toUtf8());
}

void RepairTestSerialDialog::on_pushButton_13_clicked()
{
    //모듈업데이트초기화
}

void RepairTestSerialDialog::on_pushButton_14_clicked()
{
    //모듈업데이트
    MaruSerial::sendMessage(MaruSerial::UPDATE);
}

void RepairTestSerialDialog::on_pushButton_15_clicked()
{
    //카드상태정보
}

void RepairTestSerialDialog::on_pushButton_16_clicked()
{
    ui->textEdit->clear();
}
