#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

#if 0
    QGraphicsProxyWidget *w;
    QGraphicsScene *scene;

    scene = new QGraphicsScene();
    w = scene->addWidget(ui->pushButton_4);

    w->setPos(50, 50);
    w->setRotation(45);
    ui->graphicsView->setScene(scene);
#endif

#if 1
    mainscene = new QGraphicsScene(); // 아이템 들을 저장하는 객체
    mainview = new QGraphicsView(); // 내가 보는 화면
    mainproxy = mainscene->addWidget(this); // 지우면 화면 틀과 화면이 분리됨. mainscene으로 설정하는 듯함.

    mainview->setScene(mainscene); // 지우면 틀만 나옴.
    mainview->setGeometry(geometry()); // 위치
    mainview->resize(700,400); // 크기조절
    mainview->show();
#endif

    //qApp->setStyleSheet("QLabel{background-color: yellow}"); // 프로그램 전체 스타일 시트
    this->setStyleSheet("QLabel,QLineEdit{background-color: yellow}"); // 특정 스타일 시트
    //this->setStyleSheet("QCheckBox{background-color: yellow}"); // 특정 스타일 시트
    // color: rgb(0, 88, 152) , rgba(97%, 80%, 9%, 50%)


    //label->setStyleSheet("QLabel{font:bold 30px; color:cyan; background-color:#444444}");

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_2_clicked()
{
    newpage = new New;
    newpage->show();
}

void Dialog::on_pushButton_clicked()
{
    New *dlg = new New();

    connect(dlg,SIGNAL(DialogClosed()), this, SLOT(on_subDialog_closed()));
    connect(dlg,SIGNAL(DialogRotate()), this, SLOT(rotate()));

    ShowSubDialog(dlg);
}

void Dialog::ShowSubDialog(QDialog *widget)
{
    subscene = new QGraphicsScene();
    subview = new QGraphicsView();

    subproxy = subscene->addWidget(widget);

    subview->setScene(subscene);
    subview->setGeometry(geometry()); //  actual Display size
    subview->resize(600,800);


    subview->show();
}

void Dialog::on_subDialog_closed()
{
    subview->close();
}

void Dialog::rotate()
{
    subview->rotate(90);
}

void Dialog::on_pushButton_3_clicked()
{
    mainview->rotate(90);
}
