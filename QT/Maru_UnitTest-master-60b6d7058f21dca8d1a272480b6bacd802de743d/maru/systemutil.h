#ifndef SYSTEMUTIL_H
#define SYSTEMUTIL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

//GPIO
#define GPIO_OUTPUT 0
#define GPIO_INPUT  1

#define GPIO_HIGH   1
#define GPIO_LOW    0

#define GPIO_NONE       "none"
#define GPIO_FALLING    "falling"
#define GPIO_RISING     "rising"
#define GPIO_BOTH       "both"

#define SYS_GPIO_DIR "/sys/class/gpio"

#define MAX_BUF 64

//BACKLIGHT
#define SYS_BACKLIGHT_DIR "/sys/class/backlight/backlight-display"


class SystemUtil
{
public:
    SystemUtil();
    //gpio
#if 1
    int gpio_export(unsigned int gpio);
    //int gpio_unexport(unsigned int gpio);
    //int gpio_get_dir(unsigned int gpio, unsigned int *dir);
    int gpio_set_dir(unsigned int gpio, unsigned int dir, unsigned int val);
    int gpio_get_val(unsigned int gpio, unsigned int *val);
    int gpio_set_val(unsigned int gpio, unsigned int val);
    //int gpio_set_edge(unsigned int gpio, char *edge);
#if 0
    int gpio_open(unsigned int gpio);
#else
    int gpio_open(unsigned int gpio, unsigned int dir, unsigned int flag);
#endif
    //int gpio_close(int fd);
    //int gpio_read(int fd, unsigned int *val);
#endif

    //backlight
    int gpio_init_backlight (void);
    int sys_backlight_set(int bright);
    int sys_backlight_get(int *value);

};

#endif // SYSTEMUTIL_H
