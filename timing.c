#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

/* getrusage() provides CPU time used down to the microsecond.
 * clock_gettime() is more precise on Linux, but is not available
 * on Mac. For simplicity, use getrusage() for now. */

static struct rusage started, stopped;

void start_timing(void)
{
    getrusage(RUSAGE_SELF, &started);
}

void stop_timing(void)
{
    getrusage(RUSAGE_SELF, &stopped);
}

double user_cpu_seconds(void)
{
    return difftime(stopped.ru_utime.tv_sec, started.ru_utime.tv_sec)
         + ((double)stopped.ru_utime.tv_usec - (double)started.ru_utime.tv_usec) / 1000000.0;
}

double system_cpu_seconds(void)
{
    return difftime(stopped.ru_stime.tv_sec, started.ru_stime.tv_sec)
         + ((double)stopped.ru_stime.tv_usec - (double)started.ru_stime.tv_usec) / 1000000.0;
}

double cpu_seconds(void)
{
    return difftime(stopped.ru_utime.tv_sec, started.ru_utime.tv_sec)
         + difftime(stopped.ru_stime.tv_sec, started.ru_stime.tv_sec)
         + ((double)stopped.ru_utime.tv_usec - (double)started.ru_utime.tv_usec
           +(double)stopped.ru_stime.tv_usec - (double)started.ru_stime.tv_usec) / 1000000.0;
}
