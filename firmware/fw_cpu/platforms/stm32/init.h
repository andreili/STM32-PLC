#ifndef __INIT__
#define __INIT__

#define TimerTick	F_CPU/1000-1

void RCCInit(void);
void SystemInit(void);
void SystemInit_ExtMemCtl(void);

#endif // __INIT__
