#ifndef  TIMING_H
#define  TIMING_H

void start_timing(void);
void stop_timing(void);

/* For last measured interval: */
double cpu_seconds(void);
double user_cpu_seconds(void);
double system_cpu_seconds(void);

#endif /* TIMING_H */

