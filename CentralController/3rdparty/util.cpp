#include "util.h"

// 获得小端值
uint16_t getLittleValue(uint16_t bigValue)
{
    uint16_t littleValue = 0;
    littleValue = (bigValue&0x00ff)<<8|(bigValue&0xff00)>>8;
    return littleValue;
}

//=========================================
// 获得大端值
uint16_t getBigValue(uint16_t littleValue)
{
    uint16_t bigValue = 0;
    bigValue = (littleValue&0x00ff)<<8|(littleValue&0xff00)>>8;
    return bigValue;
}


clock_t GetCurrentClock(void)
{
    clock_t now;

#ifdef Q_OS_WIN

#endif

#ifdef Q_OS_LINUX
    struct tms  tmsnow;
    now = times(&tmsnow);    
#else
    now = clock();
#endif

    return now;
}

bool IsTimeout(clock_t starttime,uint32_t ms)
{
    clock_t now = GetCurrentClock();

    double ts = (double)(now-starttime)/(double)CPU_CLK_TCK;

    if((ts*1000) > ms) {
        return true;
    } else {
        return false;
    }
}



