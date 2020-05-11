#ifndef MARUDOORLOCK_H
#define MARUDOORLOCK_H


#include "systemutil.h"

class MaruDoorlock
{
public:
    MaruDoorlock();
    static void init();
    static void open();
    static bool status();

};

#endif // MARUDOORLOCK_H
