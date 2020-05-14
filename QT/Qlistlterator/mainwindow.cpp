#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() //List
{

    QList<int> List;

    for(int i=0; i<10; i++) List.append(i);

    QMutableListIterator<int> miter(List);

    QListIterator<int> iter(List);

    qDebug() <<"Forward";

    while(iter.hasNext())
    {
        qDebug() << iter.next();
    }

    qDebug() <<"Backward";
    while(iter.hasPrevious())
    {
        qDebug() << iter.previous();
    }
}

void MainWindow::on_pushButton_2_clicked() //List
{
    QList<int> List;

    for(int i=0; i<10; i++) List.append(i);

    QMutableListIterator<int> miter(List);



    while(miter.hasNext())
    {
        int val=miter.next();

        if(val % 2 ==1) miter.remove(); // 홀수면 삭제
        else if(val %2==0) miter.setValue(-val); // 짝수면 -1을 곱함

    }

    miter.toFront();

    qDebug() <<"Forward";

    while(miter.hasNext()) qDebug() << miter.next();
}

void MainWindow::on_pushButton_3_clicked() //LinkedList
{
    QLinkedList<QString> List;

    QString A = "B";

    List << "A" << A << "C";

    List.append("D"); // 리스트 마지막에 추가
    List.append("E");
    List.append("F");

    foreach(QString s, List) qDebug() << s;

}

void MainWindow::on_pushButton_4_clicked() //QMap
{
    QMap<int, QString> Map;  // QMap<key, value>

    Map.insert(0,"A");
    Map.insert(1,"B");
    Map[3] = "C";

    foreach(int i,Map.keys()) qDebug() << Map[i];


    QMapIterator<int, QString> iter(Map);

    while(iter.hasNext())
    {
        iter.next();
        qDebug() << iter.key() << " : " << iter.value();
    }

}


void MainWindow::on_pushButton_5_clicked() // Hash
{
    QHash<int, QString> Hash;

    Hash.insert(1,"A");
    Hash.insert(2,"B");
    Hash[3] = "C";

    foreach(int i, Hash.keys()) qDebug() << Hash[i];

    QHashIterator<int, QString> iter(Hash);

    while(iter.hasNext())
    {
        iter.next();
        qDebug () << iter.key() << " : " << iter.value();
    }

}


void MainWindow::on_pushButton_6_clicked()
{
    QString str ="A,B,C,D,E,C,G"; // 콤마를 포함한 문장
    qDebug() << "QString str = " <<str;

    QStringList List;
    List = str.split(","); // ,로 구분하여 리스트에 저장
    qDebug() << "List = " << List;

    foreach(QString item, List)
        qDebug() << "List item = " << item;

    List.replaceInStrings("C","CCC"); // 리스트의 C를 CCC로 변경
    qDebug() << "List = " << List;


    QString joinedString = List.join(","); // 리스트를 ,로 연결해서 QString으로 만듬
    qDebug() << "joined string = " << joinedString;


}

void MainWindow::on_pushButton_7_clicked() // TextStream
{
    write();
    read();

}

void MainWindow::write()
{
    QFile file(":/new/prefix1/12.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file);

        stream << "De\n";
        stream << "Ra\n";

        //file.flush();
        file.close();
        qDebug() << "W finished";

    }
    else {
        qDebug() << "fail";
    }
}

void MainWindow::read()
{
    QFile file(":/new/prefix1/12.txt");

    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream stream(&file);

        QString line;
        do{

            line = stream.readLine();
            qDebug() << line;

        }while(!line.isNull());

        file.close();
        qDebug() << "R finished";
    }
}


