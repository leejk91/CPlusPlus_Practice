#include "marudoorlock.h"

#define GPIO_PORT_OUT 40
#define GPIO_PORT_IN  41

static SystemUtil gpio;

MaruDoorlock::MaruDoorlock()
{

}

void MaruDoorlock::init()
{
    //output port
 #if 0
    gpio.gpio_open(GPIO_PORT_OUT);
    gpio.gpio_export(GPIO_PORT_OUT);
    gpio.gpio_set_dir(GPIO_PORT_OUT, 0 , 1);

    //input port
    gpio.gpio_open(GPIO_PORT_IN);
    gpio.gpio_export(GPIO_PORT_IN);
    gpio.gpio_set_dir(GPIO_PORT_IN, 1 , 1);
#else
    gpio.gpio_open(GPIO_PORT_OUT, GPIO_OUTPUT, GPIO_LOW);
    gpio.gpio_open(GPIO_PORT_IN, GPIO_INPUT, GPIO_HIGH);
#endif
}

void MaruDoorlock::open()
{
    gpio.gpio_set_val(GPIO_PORT_OUT, 1);
    usleep(100000);
    gpio.gpio_set_val(GPIO_PORT_OUT, 0);
}

bool MaruDoorlock::status()
{
    unsigned int val = 0;

    gpio.gpio_get_val(GPIO_PORT_IN, &val);

    if(val == 1){
        // open
        return false;
    }
    else{
        // close
        return true;
    }
}


