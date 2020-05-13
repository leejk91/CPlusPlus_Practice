#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinepen(Qt::black);
    outlinepen.setWidth(2);

    rectangle = scene->addRect(100,0,80,100,outlinepen,blueBrush);

    ellipse = scene->addEllipse(0, -100, 300, 60, outlinepen,greenBrush);

    text = scene->addText("AAAAAAAA",QFont("Arial",20));

    text->setFlag(QGraphicsItem::ItemIsMovable);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

}
