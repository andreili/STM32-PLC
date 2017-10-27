#ifndef PLC_PLATFORM_H
#define PLC_PLATFORM_H

#include <inttypes.h>

int32_t load_file(char* fn, uint8_t* buf);

#ifdef PLATFORM_STM32

#define PLATFORM_DELAY_MS(delay) STM32_SYSTICK::delay(delay)

#else

#include <inttypes.h>

#define PLATFORM_DELAY_MS(delay) rt_delay_ms(delay)

#define RT_INIT_THREAD() \
    struct sched_param param; \
    pthread_attr_t attr; \
    pthread_t thread; \
    int ret; \
 \
    /* Lock memory */ \
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1)  \
    { \
        printf("mlockall failed: %m\n"); \
        return; \
    } \
 \
    /* Initialize pthread attributes (default values) */ \
    ret = pthread_attr_init(&attr); \
    if (ret) { \
        printf("init pthread attributes failed\n"); \
        return; \
    } \
 \
    /* Set a specific stack size  */ \
    ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN); \
    if (ret)  \
    { \
        printf("pthread setstacksize failed\n"); \
        return; \
    } \
 \
    /* Set scheduler policy and priority of pthread */ \
    ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO); \
    if (ret)  \
    { \
        printf("pthread setschedpolicy failed\n"); \
        return; \
    } \
    param.sched_priority = 80; \
    ret = pthread_attr_setschedparam(&attr, &param); \
    if (ret)  \
    { \
        printf("pthread setschedparam failed\n"); \
        return; \
    } \
    /* Use scheduling parameters of attr */ \
    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED); \
    if (ret)  \
    { \
        printf("pthread setinheritsched failed\n"); \
        return; \
    } \
    /* Create a pthread with specified attributes */ \
    ret = pthread_create(&thread, &attr, thread_func, NULL); \
    if (ret)  \
    { \
        printf("create pthread failed\n"); \
        return; \
    } \
 \
    /* Join the thread and wait until it is done */ \
    ret = pthread_join(thread, NULL); \
    if (ret) \
        printf("join pthread failed: %m\n");

void rt_delay_ms(uint32_t ms);

#endif

#endif
