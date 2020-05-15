#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    mainview = new QGraphicsView();
    //ui->graphicsView->setScene(scene);
    mainproxy = scene->addWidget(this);
    mainview->setScene(scene);

     QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinepen(Qt::black);

    outlinepen.setWidth(2);

    rectangle = scene->addRect(100,0,80,100,outlinepen,blueBrush);

    ellipse = scene->addEllipse(0, -100, 300, 60, outlinepen,greenBrush);

    text = scene->addText("AAAAAAAA",QFont("Arial",20));


   // ui->pushButton->
    text->setFlag(QGraphicsItem::ItemIsMovable);
    ellipse->setFlag(QGraphicsItem::ItemIsMovable);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable);


    mainview->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ellipse->setRotation(45);
    ellipse->setPos(50,50);
}
