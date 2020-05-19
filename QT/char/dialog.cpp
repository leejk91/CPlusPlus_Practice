#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString str;

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("EUC-JP"));

    str[0]= QChar(0xD1);
    qDebug() << str;

    QChar K = 'A';

    if(K.isDigit()||K.isUpper())
        qDebug() << K ;

    QString str1 = tr("Letter");

    QString HostName = "LEE";

    qDebug() << tr("Host %1 found").arg(HostName);

    static const char * const flowers[] = {
      QT_TR_NOOP("Roses"),
        QT_TR_NOOP("Orchid"),0
    };

    qApp->setStyleSheet("QLabel {background-image: url(:/new/prefix1/aqua_button.jpg}");

    //qApp->setStyleSheet("QPushButton {background-color: qlineargradient(x1: 0, y1:0, x2: 1, y2:1, stop:0 white,stop:0.4 gray, stop:1 green)}");
    qApp->setStyleSheet("QPushButton {border-image: url(:/new/prefix1/aqua_button.jpg)}");
    qDebug() << flowers[0];
    qDebug() << flowers[1];


}

Dialog::~Dialog()
{
    delete ui;
}
