#include "repairtestsounddialog.h"
#include "ui_repairtestsounddialog.h"

#include "Settings.h"
#include "SoundUtil.h"

#include <QProcess>

#if 0
#define VOLUME_STEP_0               0
#define VOLUME_STEP_1               6
#define VOLUME_STEP_2               11
#define VOLUME_STEP_3               16
#define VOLUME_STEP_4               21
#define VOLUME_STEP_5               26
#define VOLUME_STEP_6               31
#else
#define VOLUME_STEP_0               0
#define VOLUME_STEP_1               24
#define VOLUME_STEP_2               45
#define VOLUME_STEP_3               64
#define VOLUME_STEP_4               83
#define VOLUME_STEP_5               105
#define VOLUME_STEP_6               127
#endif

RepairTestSoundDialog::RepairTestSoundDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestSoundDialog)
{
    ui->setupUi(this);
    qDebug() << "<init> " << Settings::volumeLevel();

    switch (Settings::volumeLevel()) {
        case VOLUME_STEP_0:
            preVolume = currentVolume = 0;
            break;
        case VOLUME_STEP_1:
            preVolume = currentVolume = 1;
            break;
        case VOLUME_STEP_2:
            preVolume = currentVolume = 2;
            break;
        case VOLUME_STEP_3:
            preVolume = currentVolume = 3;
            break;
        case VOLUME_STEP_4:
            preVolume = currentVolume = 4;
            break;
        case VOLUME_STEP_5:
            preVolume = currentVolume = 5;
            break;
        case VOLUME_STEP_6:
            preVolume = currentVolume = 6;
            break;
       default:
            preVolume = currentVolume = 6;
            break;
        }

    if (currentVolume == 1)
    {
        ui->label_3->setText(QString("%1%2").arg(currentVolume).arg(QString("(min)")));
    }
    else if (currentVolume == 6)
    {
        ui->label_3->setText(QString("%1%2").arg(currentVolume).arg(QString("(max)")));
    }
    else{
        ui->label_3->setText(QString("%1").arg(currentVolume, 3));
    }

    if( Settings::volumeLevel() == VOLUME_STEP_0 )
    {
        ui->label_3->setText("음소거");
    }
}

RepairTestSoundDialog::~RepairTestSoundDialog()
{
    delete ui;
}

void RepairTestSoundDialog::on_pushButton_3_clicked()
{
    //close button
    preVolume = currentVolume;
    volumeCheck(currentVolume);
    SoundUtil::play("sound/ok.wav");

    close();
    emit RepairTestSoundDialogClosed();
}

void RepairTestSoundDialog::on_pushButton_clicked()
{
    //up
    //SoundUtil::play("sound/ok.wav");

#if 0
    if( SoundUtil::mute() )
    {
        SoundUtil::unmute();
    }
#endif

    if( currentVolume < 6 )
    {
        currentVolume++;
        volumeCheck(currentVolume);

        qDebug() << "<up> " << currentVolume;

        if (currentVolume == 1)
        {
            ui->label_3->setText(QString("%1%2").arg(currentVolume).arg(QString("(min)")));
        }
        else if (currentVolume == 6)
        {
            ui->label_3->setText(QString("%1%2").arg(currentVolume).arg(QString("(max)")));
        }
        else{
            ui->label_3->setText(QString("%1").arg(currentVolume, 3));
        }
    }

    SoundUtil::play("sound/ok.wav");
}

void RepairTestSoundDialog::on_pushButton_2_clicked()
{
    //down
    //SoundUtil::play("sound/ok.wav");

#if 0
    if( SoundUtil::mute() )
    {
        SoundUtil::unmute();
    }
#endif

    if( currentVolume > 1 )
    {
        currentVolume--;
        volumeCheck(currentVolume);

        qDebug() << "<down> " << currentVolume;

        if (currentVolume == 1)
        {
            ui->label_3->setText(QString("%1%2").arg(currentVolume).arg(QString("(min)")));
        }
        else if (currentVolume == 6)
        {
            ui->label_3->setText(QString("%1%2").arg(currentVolume).arg(QString("(max)")));
        }
        else{
            ui->label_3->setText(QString("%1").arg(currentVolume, 3));
        }
    }

    SoundUtil::play("sound/ok.wav");
}

void RepairTestSoundDialog::volumeCheck(qint8 res)
{
    switch (res) {
        case 0:
            Settings::setVolumeLevel(VOLUME_STEP_0);
            break;
        case 1:
            Settings::setVolumeLevel(VOLUME_STEP_1);
            break;
        case 2:
            Settings::setVolumeLevel(VOLUME_STEP_2);
            break;
        case 3:
            Settings::setVolumeLevel(VOLUME_STEP_3);
            break;
        case 4:
            Settings::setVolumeLevel(VOLUME_STEP_4);
            break;
        case 5:
            Settings::setVolumeLevel(VOLUME_STEP_5);
            break;
        case 6:
            Settings::setVolumeLevel(VOLUME_STEP_6);
            break;
    }

#if 0
    SoundUtil::setVolume(Settings::volumeLevel() * 100 / 31);
#else
    qDebug() << "<volumeCheck>" << Settings::volumeLevel();
    SoundUtil::setVolume(Settings::volumeLevel() * 100 / 127);
#endif
    //SoundUtil::play("sound/ok.wav");
}
