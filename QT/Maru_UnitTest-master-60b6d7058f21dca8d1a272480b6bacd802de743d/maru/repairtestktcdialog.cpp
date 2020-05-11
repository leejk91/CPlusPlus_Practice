#include "repairtestktcdialog.h"
#include "ui_repairtestktcdialog.h"

#include "maruserial.h"
#include "typedef.h"
#include "app_config.h"
#include "lib_koces.h"
#include "lib_ubcvan.h"

#define DBG_KTC 1

#if (DBG_KTC == 1)
# define	PRINTF(fmt,...) 	Dbg_Printf(fmt, ##__VA_ARGS__)
# define	PRINTHEX(fmt,...)  	PrintHex(fmt, ##__VA_ARGS__)
# define	HEXDUMP(fmt,...)  	HEX_Dump(fmt, ##__VA_ARGS__)
#else
# define    PRINTF(...)    (void)(0)
# define    PRINTHEX(...)  (void)(0)
# define    HEXDUMP(...)   (void)(0)
#endif

RepairTestKTCDialog::RepairTestKTCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestKTCDialog)
{
    ui->setupUi(this);

    MaruSerial &maru = MaruSerial::instance();

    connect(
                &maru
                , SIGNAL(ControllerMessageReceived(const QByteArray&))
                , this
                , SLOT(controllerMessageReceived(const QByteArray&))
                , Qt::QueuedConnection
                );

    connect(
                &maru
                , SIGNAL(debugMessage(QString, int))
                , this
                , SLOT(on_debugMessageReceived(QString, int))
                , Qt::QueuedConnection
               );

    if(maru.ControllerConnect()){
        ui->textEdit->append("[CTRL] connect");
        ui->textEdit->moveCursor(QTextCursor::End);
    }

    QFont font;
    font.setPointSize(11);
    ui->textEdit->setFont(font);

    Debug_Open();

    VM_GetConfigRead();
    koces_init();
}

RepairTestKTCDialog::~RepairTestKTCDialog()
{
    delete ui;
}

void RepairTestKTCDialog::SendResponse(quint8 cmd, quint8 res, const char *data, quint16 len)
{
    QByteArray ba;

    ba.append(static_cast<char>(res));
    ba.append(QByteArray::fromRawData(data, len));

    MaruSerial::sendToController(cmd, ba);
}

void RepairTestKTCDialog::on_pushButton_3_clicked()
{
    close();
    emit RepairTestKTCDialogClosed();
}

void RepairTestKTCDialog::on_debugMessageReceived(QString str, int color)
{
    switch(color){
    default:
        ui->textEdit->setTextColor(QColor(0,0,0));
        ui->textEdit->append(str);
        break;
    }
    ui->textEdit->moveCursor(QTextCursor::End);
}

void RepairTestKTCDialog::controllerMessageReceived(const QByteArray &message)
{
    quint8 stx,seq,cmd;
    quint16 sender,receiver, len;
    QByteArray data;
    QDataStream istream(message);
    quint16 result;
    int res, length;

#if 1
    QString str = "[CTRL] << " + message.toHex();
    ui->textEdit->append(str);
    ui->textEdit->moveCursor(QTextCursor::End);
#endif

    istream.setByteOrder(QDataStream::BigEndian);
    istream >> stx >> seq >> sender >> receiver >> cmd >> len;

    switch(cmd){
    case MaruSerial::SET_TID:
        istream.readRawData(reinterpret_cast<char*>(rcvBuff), len);
        VM_SetConfigTID(rcvBuff, len);
        PRINTF(const_cast<char*>("\n\r"));
        HEXDUMP(rcvBuff, len, const_cast<char*>("TID"));
        SendResponse(MaruSerial::SET_TID, 0x01, static_cast<const char*>("OK"), 2);
        break;

    case MaruSerial::GET_TID:
        result = static_cast<quint16>(VM_GetConfigTID(sndBuff));
        if(result != 0){
            SendResponse(MaruSerial::GET_TID, 0x01, reinterpret_cast<const char*>(sndBuff), result);
        }
        else {
            SendResponse(MaruSerial::GET_TID, 0xFF, nullptr, 0);
        }
        break;

    case MaruSerial::SET_CID:
        istream.readRawData(reinterpret_cast<char*>(rcvBuff), len);
        VM_SetConfigCatID(rcvBuff, len);
        PRINTF(const_cast<char*>("\n\r"));
        HEXDUMP(rcvBuff, len, const_cast<char*>("CATID"));
        SendResponse(MaruSerial::SET_TID, 0x01, static_cast<const char*>("OK"), 2);
        break;

    case MaruSerial::GET_CID:
        result = static_cast<quint16>(VM_GetConfigCatID(sndBuff));
        if(result != 0){
            SendResponse(MaruSerial::GET_CID, 0x01, reinterpret_cast<const char*>(sndBuff), result);
        }
        else {
            SendResponse(MaruSerial::SET_TID, 0xFF, nullptr, 0);
        }
        break;

    case MaruSerial::SET_HDATA:
        istream.readRawData(reinterpret_cast<char*>(rcvBuff), len);
        PRINTF(const_cast<char*>("\n\r"));
        HEXDUMP(rcvBuff, len, const_cast<char*>("HDATA"));

        res = koces_init_dupkt_injection(rcvBuff, len);
        if(res == RC_DATA){
            SendResponse(MaruSerial::SET_HDATA, 0x01, static_cast<const char*>("OK"), 2);
        }
        else{
            SendResponse(MaruSerial::SET_HDATA, 0xFF, nullptr, 0);
        }
        break;


    case MaruSerial::CMD_BINGO_TRANSACTION4_KTC_KEY_RENEWAL:
        //키갱신
        koces_dupkt_request();
        length = UBC_ReadVanFlash(sndBuff);
        if(length > 0){
            SendResponse(MaruSerial::CMD_BINGO_TRANSACTION4_KTC_KEY_RENEWAL, 0xED, reinterpret_cast<const char*>(sndBuff), static_cast<quint16>(length));
        }
        else{
            SendResponse(MaruSerial::CMD_BINGO_TRANSACTION4_KTC_KEY_RENEWAL, 0xFF, nullptr, 0);
        }
        break;

    case MaruSerial::CMD_BINGO_TRANSACTION4_RES:
        //응답전문
        length = 0;
        istream.readRawData(reinterpret_cast<char*>(rcvBuff), len);
        UBC_RcvVan(rcvBuff, len);
        sndBuff[length++] = 0x0;
        UBC_GetResCode(reinterpret_cast<char*>(&sndBuff[length]));
        length += 4;
        SendResponse(MaruSerial::CMD_BINGO_TRANSACTION4_RES, 0x01, reinterpret_cast<const char*>(sndBuff), static_cast<quint16>(length));
        break;

    case MaruSerial::CMD_BINGO_TRANSACTION4:
        //결재요청
        break;

    default :
        break;
    }


#if 0
    QString str = "[CTRL] << " + message.toHex();
    ui->textEdit->append(str);
    ui->textEdit->moveCursor(QTextCursor::End);
#endif
}

void RepairTestKTCDialog::on_pushButton_clicked()
{
    ui->textEdit->clear();
}
