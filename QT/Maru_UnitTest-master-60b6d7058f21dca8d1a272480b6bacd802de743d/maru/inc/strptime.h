#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
int strncasecmp(const char *s1, const char *s2, size_t n);
const char *strptime(const char *buf, const char *fmt, struct tm *tm);
#endif
time_t ctime_to_time(const char *ctime_str);

#ifdef __cplusplus
}
#endif
