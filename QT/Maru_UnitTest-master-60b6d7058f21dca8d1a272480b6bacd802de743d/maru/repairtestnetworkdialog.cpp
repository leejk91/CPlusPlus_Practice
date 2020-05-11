#include "repairtestnetworkdialog.h"
#include "ui_repairtestnetworkdialog.h"

#include <QtNetwork>
#include <QDebug>

RepairTestNetworkDialog::RepairTestNetworkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestNetworkDialog)
{
    ui->setupUi(this);

    proc = new QProcess();

#if 1
    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr, addrlist){
        if ( 0 >= addr.toIPv4Address() ) continue;
        else {
            if (addr.toString().contains("127.0.0.1") ) continue;
            else {

                sourceIP = addr.toString();
                qDebug() << sourceIP;

                break;
            }
        }
    }

    ui->label_3->setText(sourceIP);
#endif

    connect(ui->listWidget->model(), SIGNAL(rowsInserted(QModelIndex,int,int)),
            ui->listWidget, SLOT(scrollToBottom()));
}

RepairTestNetworkDialog::~RepairTestNetworkDialog()
{
    delete ui;
}

void RepairTestNetworkDialog::on_pushButton_3_clicked()
{
    //close
    close();
    emit RepairTestNetworkDialogClosed();
}

void RepairTestNetworkDialog::on_pushButton_clicked()
{
    //start
    onPing();
}

void RepairTestNetworkDialog::onPingEnded()
{
#if 1
    //ui->listWidget->clear();

    proc->waitForFinished(-1);

    QByteArray output = proc->readAllStandardOutput();

    if(!output.isEmpty())
    {
        qDebug() << output;

        QString command(QString::fromLocal8Bit(output));

        ui->listWidget->addItem(command);

        if(-1 != QString(output).indexOf("ttl", 0 , Qt::CaseSensitive))
        {
            //ping success
            qDebug() << "PING SUCCESS!!";
            ui->listWidget->addItem("PING SUCCESS\n");
        }
        else
        {
            //ping fail
            qDebug() << "PING FAIL!!";
            ui->listWidget->addItem("PING FAIL\n");
        }

        //ui->listWidget->scrollToBottom();
    }
#endif
}

void RepairTestNetworkDialog::onPing()
{
#if 1
    QString cmd = "ping -c 3 ";
    destinationIP = ui->lineEdit->text();

    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)) , this , SLOT(onPingEnded()) );

    //proc->start("ping", QStringList() << " -c " << " 3 " << destinationIP);

    cmd += destinationIP;

    proc->start(cmd);

    ui->listWidget->addItem("<<PING START>>\n");
    //ui->listWidget->scrollToBottom();

#endif
}
