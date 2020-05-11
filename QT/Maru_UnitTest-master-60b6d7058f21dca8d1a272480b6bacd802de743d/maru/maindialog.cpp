#include "maindialog.h"
#include "ui_maindialog.h"

#include "repairtestkeybuttondialog.h"
#include "repairtestlcddialog.h"
#include "repairtestsounddialog.h"
#include "repairtestnetworkdialog.h"
#include "repairtesttofdialog.h"
#include "repairtestdoorlockdialog.h"
#include "repairtestimageshowdialog.h"
#include "repairtestserialdialog.h"
#include "repairtestktcdialog.h"
#include "testdialog.h"

#include <QDebug>

#include <SoundUtil.h>
#include <Settings.h>
#include <QProcess>
#include <QtNetwork>
#include <QList>

#include "lib_util.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    //setWindowFlag(Qt::FramelessWindowHint);
    setGeometry(geometry());

    mainscene = new QGraphicsScene();
    mainview = new QGraphicsView();
    mainproxy = mainscene->addWidget(this);
    mainview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainview->setStyleSheet("QGraphicsView { border-style: none;}");
    //mainview->setWindowFlags(Qt::FramelessWindowHint);

    mainview->setScene(mainscene);
    mainview->setGeometry(geometry());
    mainview->resize(1280,800);
    mainview->rotate(90);
    mainview->show();
    //mainview->showFullScreen();

#if 0
    int i =1000;
    QString test =  QString("%1").arg(i,6,10,QLatin1Char('0'));
    qDebug() << test;
#endif

#if 0
    char time[14];
    GetTimeString((time_t)NULL, time);
    qDebug() << time;
#endif

#if 0
    QString retn;
#if QT_VERSION >= 0x050000
       QString configBasePath =
       QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) +
           "/" + qApp->applicationName();
   #else
       QString configBasePath =
       QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) +
           "/" + qApp->applicationName();
   #endif

   retn = configBasePath + "/config.ini";

   qDebug() << retn;
#endif
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_pushButton_5_clicked()
{
    mainview->close();
    close();
}

void MainDialog::on_pushButton_clicked()
{
    //Key Button Test
    RepairTestKeyButtonDialog *dlg = new RepairTestKeyButtonDialog();
    connect(dlg,SIGNAL(RepairTestKeyButtonDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_subDialog_closed()
{
    subview->close();
}

void MainDialog::on_pushButton_3_clicked()
{
    //Backlight test
    RepairTestLcdDialog *dlg = new RepairTestLcdDialog();
    connect(dlg,SIGNAL(RepairTestLcdDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_pushButton_7_clicked()
{
    //Network
    RepairTestNetworkDialog *dlg = new RepairTestNetworkDialog();
    connect(dlg,SIGNAL(RepairTestNetworkDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_pushButton_2_clicked()
{
    //Sound
    RepairTestSoundDialog *dlg = new RepairTestSoundDialog();
    connect(dlg,SIGNAL(RepairTestSoundDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_pushButton_4_clicked()
{
    //Door lock
    RepairTestDoorlockDialog *dlg = new RepairTestDoorlockDialog();
    connect(dlg,SIGNAL(RepairTestDoorlockDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_pushButton_6_clicked()
{
    //Reader
    RepairTestSerialDialog *dlg = new RepairTestSerialDialog();
    connect(dlg,SIGNAL(RepairTestSerialDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_pushButton_8_clicked()
{
    //TOF
    RepairTestTOFDialog *dlg = new RepairTestTOFDialog();
    connect(dlg,SIGNAL(RepairTestTOFDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::ShowSubDialog(QWidget *widget)
{
    subscene = new QGraphicsScene();
    subview = new QGraphicsView();

    subproxy = subscene->addWidget(widget);
    subview->setStyleSheet("QGraphicsView { border-style: none;}");
    subview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    subview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    subview->setWindowFlags(Qt::FramelessWindowHint);
    subview->setScene(subscene);

    subview->setGeometry(geometry());
    subview->resize(1280,800);
    subview->rotate(90);
    subview->show();
}


void MainDialog::on_pushButton_9_clicked()
{
    //Image Show
    RepairTestImageShowDialog *dlg = new RepairTestImageShowDialog();
    connect(dlg,SIGNAL(RepairTestImageShowDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_pushButton_10_clicked()
{
    //KTC
    RepairTestKTCDialog *dlg = new RepairTestKTCDialog();
    connect(dlg,SIGNAL(RepairTestKTCDialogClosed()), this, SLOT(on_subDialog_closed()));

    ShowSubDialog(dlg);
}

void MainDialog::on_pushButton_11_clicked()
{
    TestDialog *dlg = new TestDialog();
    connect(dlg,SIGNAL(TestDialogClosed(QString)), this, SLOT(on_test(QString)));

    subscene = new QGraphicsScene();
    subview = new QGraphicsView();
    subproxy = subscene->addWidget(dlg);

    subview->setStyleSheet("QGraphicsView { border-style: none;}");
    subview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    subview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    subview->setWindowFlags(Qt::FramelessWindowHint);
    subview->setScene(subscene);

    subview->setWindowModality(Qt::ApplicationModal);

    //subview->setGeometry(geometry());
    subview->setGeometry(0, 200, 400, 300);
    subview->resize(400,300);
    subview->rotate(90);

    subview->show();
}

void MainDialog::on_test(QString msg)
{
    subview->close();
    qDebug() << msg;
}
