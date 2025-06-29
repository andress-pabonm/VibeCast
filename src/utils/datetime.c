#include <utils/datetime.h>
#include <time.h>

#define TIME_FMT "%T"
#define DATE_FMT "%F"
#define DATETIME_FMT DATE_FMT " " TIME_FMT

datetime_t now()
{
    time_t now = time(NULL);
    return localtime(&now);
}

void getTime(time_buf_t buf, datetime_t dt)
{
    if (buf && dt)
        strftime(buf, TIME_BUFSIZ,
                 TIME_FMT,
                 dt);
}

void getDate(date_buf_t buf, datetime_t dt)
{
    if (buf && dt)
        strftime(buf, DATE_BUFSIZ,
                 DATE_FMT,
                 dt);
}

void getDateTime(datetime_buf_t buf, datetime_t dt)
{
    if (buf && dt)
        strftime(buf, DATETIME_BUFSIZ,
                 DATETIME_FMT,
                 dt);
}
