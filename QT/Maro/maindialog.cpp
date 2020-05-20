#include "maindialog.h"
#include "ui_maindialog.h"

#include "password.h"

static int ad_count = 0 ;
static int test_stacked = 0;

#define image_path "/mnt/hgfs/Practice/Practice/QT/Maro/images/"
#define adimage_path "/mnt/hgfs/Practice/Practice/QT/Maro/images/ad/"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    qDebug() << "TestStart" << adimage_path;
    ui->stackedWidget->setCurrentIndex(0);

#if 1 // 프레임 스타일 시트 설정
    this->setStyleSheet("QFrame {border-style: none; background-color : white; border: 2px; border-radius: 35px;}");
#endif

#if 1 // 점포 라벨 설정
    QString store_name;
    store_name = "UBCN 1호점";
    ui->store_name->setText(store_name);
    ui->store_name->setStyleSheet("#store_name{font:bold 20px; color:blue}");
#endif

#if 1 // 로고 설정
    ui->logo->setStyleSheet("#logo {border-image: url(:/new/adimage/images/logo.jpg);}");
#endif

#if 1 // 공지사항 라벨 설정
    QString announce_text;
    announce_text = "UBCN 빙고마루 테스트 중입니다";
    ui->announce->setText(announce_text);
    ui->announce->setStyleSheet("#announce {font:bold 20px;}");

    announce_move = new QTimer;
    connect(announce_move,SIGNAL(timeout()),this,SLOT(announce_move_timeout()));
    announce_move->start(20);
#endif



#if 1 // 윈도우 설정
    setGeometry(geometry());

    mainscene = new QGraphicsScene(); // 아이템 들을 저장하는 객체
    mainview = new QGraphicsView(); // 내가 보는 화면
    mainproxy = mainscene->addWidget(this); // 지우면 화면 분리.
    mainview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 세로 스크롤바 제거
    mainview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 가로 스크롤바 제거
    mainview->setStyleSheet("QGraphicsView { border-style: none;}"); // 테두리 없음
    //subview->setWindowFlags(Qt::FramelessWindowHint); // 윈도 바

    mainview->setScene(mainscene);
    mainview->setGeometry(geometry()); //  actual Display size
    mainview->resize(1280,800); // 화면 크기
    mainview->rotate(90); // 화면 회전
    mainview->show();

#endif

#if 1 // 광고 이미지 슬라이드 시작

    adimageslide = new QTimer;
    connect(adimageslide,SIGNAL(timeout()),this,SLOT(adimageslide_timeout()));
    adimageslide->start(4000);

#endif

#if 1 // 접근 페이지 설정
    ui->detected_image->setStyleSheet("#detected_image {border-image: url(:/new/adimage/images/insert.png);}");
    ui->detected_text->setStyleSheet("#detected_text {font: bold 25px;}");
#endif

#if 1 // 상품 트리위젯 설정
    ui->treeWidget->setColumnCount(3); // 항목 개수 설정
    ui->treeWidget->setHeaderLabels(QStringList() << "제품" << "가격" << "상태"); // 항목 이름 설정
    ui->treeWidget->setColumnWidth(0,450);
    ui->treeWidget->setColumnWidth(1,150);
    ui->treeWidget->setColumnWidth(2,100);

    ui->treeWidget->setStyleSheet("QTreeWidget {color:blue;} QHeaderView {font-size:30pt; color:red"); //header font size


    //QFont fnt;
    //fnt.setPixelSize(20);
    //ui->treeWidget->setFont(fnt);
    //ui->treeWidget->setForeground(0,b);


#endif

#if 1 // Test 출력
    list_insert("Test",10,100);
#endif

}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::announce_move_timeout()
{

    int x = ui->announce->geometry().x();
    int y = ui->announce->geometry().y();

    if(x>-800)
        ui->announce->move(x-1,y);
    else
        ui->announce->move(810,y);

}

void MainDialog::adimageslide_timeout() // 광고 출력
{
    QDir directory(adimage_path);
    images = directory.entryList(QStringList() << "*.jpg" << "*.png" << "*.gif",QDir::Files);
    length = images.length() - 1; // 이미지 파일의 갯수

    QString str = QString("#ad {border-image: url(%1%2);}").arg(adimage_path).arg(images.value(ad_count));

    qDebug() << "삽입되는 Stylesheet 내용 :"<< str;

    ui->ad->setStyleSheet(str);

    if(ad_count==length){
        ad_count = 0;
    }
    else {
        ad_count++;

    }
}


void MainDialog::ShowSubDialog(QWidget *widget)
{
    subscene = new QGraphicsScene();
    subview = new QGraphicsView();

    subproxy = subscene->addWidget(widget);
    subview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 세로스크롤바 제거
    subview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 가로스크롤바 제거
    subview->setStyleSheet("QGraphicsView { border-style: none;}"); // 테두리 없음
    //subview->setWindowFlags(Qt::FramelessWindowHint); //윈도 바 제거

    subview->setScene(subscene);
    subview->setGeometry(geometry()); // actual Display size
    subview->resize(1280,800); // 화면 크기
    subview->rotate(90); // 화면 회전
    subview->show();


}

void MainDialog::on_subDialog_closed()
{
    subview->close();
}

void MainDialog::on_config_clicked() // 관리자 메뉴 클릭
{
    Password *password_page = new Password();

    ShowSubDialog(password_page);

}

void MainDialog::on_pushButton_clicked() // stacked 테스트
{
    if(test_stacked==0)
    {
        ui->stackedWidget->setCurrentIndex(1);
        test_stacked++;
    }
    else if(test_stacked==1)
    {
        ui->stackedWidget->setCurrentIndex(2);
        test_stacked++;
    }
    else if(test_stacked==2)
    {
        ui->stackedWidget->setCurrentIndex(0);
        test_stacked=0;
    }
}

void MainDialog::list_insert(QString A,int B,int C) // 리스트 입력
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);


    treeItem->setText(0, A);

    QString value;
    value.setNum(B);
    treeItem->setText(1, value);

    QString value2;
    value2.setNum(C);
    treeItem->setText(2, value2);
}
