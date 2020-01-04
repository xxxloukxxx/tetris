#ifndef _TIMER_H_
#define _TIMER_H_
#include <stdbool.h>

int t0;
int t1;

void init_timer(int *t);
bool pulse(int *t, int period);

#endif
