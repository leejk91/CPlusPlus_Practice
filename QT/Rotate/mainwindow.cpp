#include "mainwindow.h"
#include "ui_mainwindow.h"

bool A = true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainscene = new QGraphicsScene(); // item들을 저장하는 객체
    mainview = new QGraphicsView(); // 보는 화면
    mainview->setScene(mainscene);

    mainproxy = mainscene->addWidget(this); //

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
    //mainview->show();
}

void MainWindow::on_actionNew_triggered()
{
    dia_log = new Dialog(this);
    ShowSubDialog(dia_log);
}

/*
void MainWindow::ShowSubDialog(QWidget *widget)
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
    subview->resize(300,400);
    subview->rotate(90);
    subview->show();
}
*/
