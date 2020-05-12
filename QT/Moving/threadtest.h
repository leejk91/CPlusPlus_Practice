#ifndef THREADTEST_H
#define THREADTEST_H

#include <QObject>
#include <QThread>

class ThreadTest : public QThread
{
    Q_OBJECT
public:
    explicit ThreadTest();
};

#endif // THREADTEST_H
