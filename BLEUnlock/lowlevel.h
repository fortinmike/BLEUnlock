#ifndef lowlevel_h
#define lowlevel_h
#include <stdbool.h>

void sleepDisplay(void);
void wakeDisplay(void);
int SACLockScreenImmediate(void);
bool isDisplaySleepPrevented(void);

#endif /* lowlevel_h */
