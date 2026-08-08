#include "zf_common_typedef.h"

#define MDU32_SELFTEST_MAX_FILTER_SUM   (393210UL)

extern uint32 bldc_mdu32_div_u32(uint32 dividend, uint32 divisor);

vuint32 mdu32_selftest_input = 0;
vuint32 mdu32_selftest_expected = 0;
vuint32 mdu32_selftest_actual = 0;
vuint32 mdu32_selftest_failures = 0;

// 仅用于独立测试固件：断开电机输出并关闭中断后，从调试器调用本函数。
void mdu32_hardware_selftest(void)
{
    uint32 value;

    mdu32_selftest_failures = 0;
    for(value = 0; value <= MDU32_SELFTEST_MAX_FILTER_SUM; value++)
    {
        mdu32_selftest_input = value;
        mdu32_selftest_actual = bldc_mdu32_div_u32(value, 36UL);
        mdu32_selftest_expected = value / 36UL;

        if(mdu32_selftest_actual != mdu32_selftest_expected)
        {
            mdu32_selftest_failures++;
            break;
        }
    }
}
