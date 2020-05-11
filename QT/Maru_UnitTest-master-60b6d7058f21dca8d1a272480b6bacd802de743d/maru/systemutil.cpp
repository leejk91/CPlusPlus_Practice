#include "systemutil.h"

SystemUtil::SystemUtil()
{

}

#if 0
int SystemUtil::gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYS_GPIO_DIR "/export", O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Can't export GPIO %d pin: %s\n", gpio, strerror(errno));
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, static_cast<size_t>(len));
    close(fd);

    return 0;
}

#if 0
int SystemUtil::gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYS_GPIO_DIR "/unexport", O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Can't unexport GPIO %d pin: %s\n", gpio, strerror(errno));
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, static_cast<size_t>(len));
    close(fd);

    return 0;
}

int SystemUtil::gpio_get_dir(unsigned int gpio, unsigned int *dir)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYS_GPIO_DIR "/gpio%d/direction", gpio);

    fd = open(buf, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "Can't get GPIO %d pin direction: %s\n", gpio, strerror(errno));
        return fd;
    }

    read(fd, &buf, MAX_BUF);
    close(fd);

    if (strcmp(buf, "in") == 0)
        *dir = GPIO_INPUT;
    else
        *dir = GPIO_OUTPUT;

    return 0;
}
#endif

int SystemUtil::gpio_set_dir(unsigned int gpio, unsigned int dir, unsigned int val)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYS_GPIO_DIR  "/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Can't set GPIO %d pin direction: %s\n", gpio, strerror(errno));
        return fd;
    }

    if (dir == GPIO_OUTPUT) {
        if (val == GPIO_HIGH)
            write(fd, "high", 5);
        else
            write(fd, "out", 4);
    } else {
        write(fd, "in", 3);
    }

    close(fd);

    return 0;
}

int SystemUtil::gpio_get_val(unsigned int gpio, unsigned int *val)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "Can't get GPIO %d pin value: %s\n", gpio, strerror(errno));
        return fd;
    }

    read(fd, buf, 1);
    close(fd);

    if (*buf != '0')
        *val = GPIO_HIGH;
    else
        *val = GPIO_LOW;

    return 0;
}

int SystemUtil::gpio_set_val(unsigned int gpio, unsigned int val)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Can't set GPIO %d pin value: %s\n", gpio, strerror(errno));
        return fd;
    }

    if (val == GPIO_HIGH)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);

    return 0;
}

#if 0
int SystemUtil::gpio_set_edge(unsigned int gpio, char *edge)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYS_GPIO_DIR "/gpio%d/edge", gpio);

    fd = open(buf, O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Can't set GPIO %d pin edge: %s\n", gpio, strerror(errno));
        return fd;
    }

    write(fd, edge, strlen(edge)+1);
    close(fd);

    return 0;
}
#endif

int SystemUtil::gpio_open(unsigned int gpio)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY|O_NONBLOCK);

    if (fd < 0)
        fprintf(stderr, "Can't open GPIO %d pin: %s\n", gpio, strerror(errno));

    return fd;
}

#if 0
int SystemUtil::gpio_close(int fd)
{
    return close(fd);
}

int SystemUtil::gpio_read(int fd, unsigned int *val)
{
    int ret;
    char ch;

    lseek(fd, 0, SEEK_SET);

    ret = read(fd, &ch, 1);

    if (ret != 1) {
        //fprintf(stderr, "Can't read GPIO %d pin: %s\n", gpio, strerror(errno));
        return ret;
    }

    if (ch != '0')
        *val = GPIO_HIGH;
    else
        *val = GPIO_LOW;

    return 0;
}
#endif

#else
int SystemUtil::gpio_export(unsigned int gpio)
{
    char buff[256];
    int ret;
    FILE *fp;

    snprintf(buff, sizeof(buff), "%s/export", SYS_GPIO_DIR);
    fp = fopen(buff, "w");

    if (fp == nullptr ) {
        fprintf(stderr, "Can't export GPIO %d pin: %s\n", gpio, strerror(errno));
            return -1;
    }

    ret = fprintf(fp, "%d\n", gpio);

    if( ret < 0 )
        return -1;

    fclose(fp);

    return 0;
}

int SystemUtil::gpio_set_dir(unsigned int gpio, unsigned int dir, unsigned int val)
{
    char buff[256];
    int ret;
    FILE *fp;

    snprintf(buff, sizeof(buff), "%s/gpio%d/direction", SYS_GPIO_DIR, gpio);
    fp = fopen(buff, "w");

    if (dir == GPIO_OUTPUT) {
        if( val == GPIO_HIGH ) {
            ret = fprintf(fp, "high\n");    // "high"
        } else {
            ret = fprintf(fp, "low\n");     // "out" or "low"
        }
    }
    else{
        ret = fprintf(fp, "in\n");
    }

    if( ret < 0 )
        return -1;

    fclose(fp);

    return 0;
}

int SystemUtil::gpio_get_val(unsigned int gpio, unsigned int *val)
{
    char buff[256];
    unsigned int value;
    FILE *fp;

    snprintf(buff, sizeof(buff), "%s/gpio%d/value", SYS_GPIO_DIR, gpio);
    fp = fopen(buff, "rb");

    fscanf(fp, "%d", &value);
    fclose(fp);

    *val = value;

    return 0;
}

int SystemUtil::gpio_set_val(unsigned int gpio, unsigned int val)
{
    char buff[256];
    int ret;
    FILE *fp;

    snprintf(buff, sizeof(buff), "%s/gpio%d/value", SYS_GPIO_DIR, gpio);
    fp = fopen(buff, "w");
    setvbuf(fp, nullptr, _IONBF, 0);

    ret = fprintf(fp, "%d\n", val);
    if( ret < 0 )
        return -1;

    fclose(fp);

    return 0;
}

int SystemUtil::gpio_open(unsigned int gpio, unsigned int dir, unsigned int flag)
{
    int ret;

    ret = gpio_export(gpio);
    if( ret < 0 )
        return -1;

    ret = gpio_set_dir(gpio, dir, flag);
    if( ret < 0 )
        return -1;

    return 0;
}
#endif

//Backlight
int SystemUtil::gpio_init_backlight (void)
{
    char buff[256];
    FILE *fp;

    snprintf(buff, sizeof(buff), "%s/brightness", SYS_BACKLIGHT_DIR);
    fp = fopen(buff, "w");
    if ( fp == nullptr ){

        printf("file open failed\n");
        return -1;
    }

    fclose(fp); // file exist // open enable

    return 0;
}

int SystemUtil::sys_backlight_set(int bright)
{
    char    buff[256];
    int     ret;
    FILE   *fp;

    snprintf(buff, sizeof(buff), "%s/brightness", SYS_BACKLIGHT_DIR);

    fp = fopen( buff, "w");
    ret = setvbuf(fp, nullptr, _IONBF, 0);
    if ( 0 > ret ) {
        return -1;
    }

    ret = fprintf(fp, "%d\n", bright);
    if ( 0 > ret ) {
        return -1;
    }

    fclose(fp);

    return 0;
}

int SystemUtil::sys_backlight_get(int *value)
{
    char buff[256];
    FILE *fp;
    int val     = 0;                                                // 밝기 값을 구하지 못 했다면 0으로 반환

    snprintf(buff, sizeof(buff), "%s/brightness", SYS_BACKLIGHT_DIR);
    fp = fopen(buff, "r");
    if (nullptr == fp) {
        return -1;
    }
    fscanf(fp, "%d", &val);
    fclose(fp);
    if ( 0 > val){
        return -1;
    }

    *value = val;

    return 0;
}
