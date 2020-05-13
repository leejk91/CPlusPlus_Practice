#include "mainwindow.h"
#include "ui_mainwindow.h"

bool A = true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainscene = new QGraphicsScene();
    mainview = new QGraphicsView();
    mainproxy = mainscene->addWidget(this);

    mainview->setScene(mainscene);
    mainview->setGeometry(geometry());
    mainview->resize(400,300);
    mainview->rotate(90);
    mainview->show();

    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(A_Time_out()));

    timer->start(20);

    ui->label_2->setText("테스트 테스트 테스트 테스트");
     ui->label_2->setStyleSheet("QLabel{font:bold 20px; color:Blue;}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::A_Time_out()
{
    int x = ui->label_2->geometry().x();
    int y = ui->label_2->geometry().y();

    if(x>-250)
        ui->label_2->move(x-1,y);
    else
        ui->label_2->move(350,y);

    ui->label_2->setText(ui->lineEdit->text());

}

void MainWindow::on_pushButton_clicked()
{
    mainview->rotate(90);
    if(A==true)
    {
        mainview->resize(300,400);
        A = false;
    }
    else {
        mainview->resize(400,300);
        A = true;

    }
    mainview->show();
}
