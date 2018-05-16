#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <stdint.h>
#include <unistd.h>

#include <QTime>

#ifdef Q_OS_WIN
#include <WinSock2.h>
#include <time.h>
#endif

#ifdef Q_OS_LINUX
#include <sys/times.h>
#endif

#ifdef Q_OS_WIN
#define CPU_CLK_TCK CLOCKS_PER_SEC
#endif

#ifdef Q_OS_LINUX
#define CPU_CLK_TCK sysconf(_SC_CLK_TCK)
#endif

using namespace std;

// 获得小端值
uint16_t getLittleValue(uint16_t bigValue);
// 获得大端值
uint16_t getBigValue(uint16_t littleValue);

clock_t GetCurrentClock(void);
bool IsTimeout(clock_t starttime,uint32_t ms);

#endif // UTIL_H
