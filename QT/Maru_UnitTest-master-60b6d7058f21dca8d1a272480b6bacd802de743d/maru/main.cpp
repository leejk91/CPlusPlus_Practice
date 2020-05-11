#include "maindialog.h"
#include <QApplication>

#include "Settings.h"
#include "SoundUtil.h"
#include "marudoorlock.h"
#include "maruserial.h"


class Application : public QApplication
{
public:
    inline Application(int argc, char *argv[])
        : QApplication(argc, argv)
        /* inline
         * 함수 호출이 함수 자체의 내용 복사본으로 대체되어 함수 오버헤드가 제거됨.
         * 작은 함수의 경우 함수 호출에 필요한 시간이 실제 함수 코드 실행보다 많은데 이를 위해 사용함.
         */
    {
        SoundUtil::init(this);
        /*this 용도
         * 맴버변수를 나타낸다 or 객체 자신의 주소를 리턴할때
         */
        MaruDoorlock::init();
        MaruSerial::instance().init();
    }
    inline ~Application()
    {
        SoundUtil::destroy();
    }
};

int main(int argc, char *argv[])
{
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    Application a(argc, argv);
    MainDialog w;
    w.show();

    return a.exec();
}
