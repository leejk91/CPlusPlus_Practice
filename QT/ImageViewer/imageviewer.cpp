#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    QImage image(":/new/prefix1/Dog3.png");
    ui->label->setPixmap(QPixmap::fromImage(image));
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
