/*********************************************************************************************************************
* TC212 Opensourec Library 即（TC212 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC212 开源库的一部分
*
* TC212 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          zf_driver_pwm
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC212
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-10-15       pudding            first version
********************************************************************************************************************/

#ifndef _zf_driver_pwm_h_
#define _zf_driver_pwm_h_

#include "zf_common_typedef.h"

#define PWM_DUTY_MAX     10000                 // PWM最大占空比  最大占空比越大占空比的步进值越小

// 此枚举定义不允许用户修改
typedef enum // 枚举PWM引脚
{
    TOM0_CH0_P15_5,  TOM0_CH0_P33_10,
    TOM0_CH1_P33_5,  TOM0_CH1_P33_9,
    TOM0_CH2_P10_5,  TOM0_CH2_P33_6,
    TOM0_CH3_P10_6,  TOM0_CH3_P14_0,   TOM0_CH3_P33_7,
    TOM0_CH4_P00_0,  TOM0_CH4_P02_0,   TOM0_CH4_P02_1,   TOM0_CH4_P02_8,  TOM0_CH4_P11_2,  TOM0_CH4_P14_1, TOM0_CH4_P15_1,  TOM0_CH4_P15_2,  TOM0_CH4_P20_8,  TOM0_CH4_P20_9,  TOM0_CH4_P21_6, TOM0_CH4_P33_8,
    TOM0_CH5_P02_3,  TOM0_CH5_P02_2,   TOM0_CH5_P11_3,   TOM0_CH5_P11_6,  TOM0_CH5_P15_3,  TOM0_CH5_P15_5, TOM0_CH5_P20_11, TOM0_CH5_P21_7,  TOM0_CH5_P33_5,  TOM0_CH5_P33_6,
    TOM0_CH6_P02_4,  TOM0_CH6_P02_5,   TOM0_CH6_P11_9,   TOM0_CH6_P11_10, TOM0_CH6_P14_0,  TOM0_CH6_P14_3, TOM0_CH6_P20_12, TOM0_CH6_P20_13, TOM0_CH6_P23_1,  TOM0_CH6_P33_7,  TOM0_CH6_P33_8,
    TOM0_CH7_P02_6,  TOM0_CH7_P02_7,   TOM0_CH7_P11_11,  TOM0_CH7_P11_12, TOM0_CH7_P14_1,  TOM0_CH7_P14_4, TOM0_CH7_P15_0,  TOM0_CH7_P20_8,  TOM0_CH7_P20_14, TOM0_CH7_P33_9,  TOM0_CH7_P33_10,
    TOM0_CH8_P00_0,  TOM0_CH8_P02_0,   TOM0_CH8_P02_8,   TOM0_CH8_P11_2,  TOM0_CH8_P20_12,
    TOM0_CH9_P02_1,  TOM0_CH9_P20_13,
    TOM0_CH10_P02_2, TOM0_CH10_P11_3,  TOM0_CH10_P20_14,
    TOM0_CH11_P02_3, TOM0_CH11_P11_6,  TOM0_CH11_P15_0,
    TOM0_CH12_P02_4, TOM0_CH12_P11_9,  TOM0_CH12_P15_1,
    TOM0_CH13_P02_5, TOM0_CH13_P11_10, TOM0_CH13_P15_2,  TOM0_CH13_P20_9,
    TOM0_CH14_P02_6, TOM0_CH14_P11_11, TOM0_CH14_P15_3,
    TOM0_CH15_P02_7, TOM0_CH15_P11_12, TOM0_CH15_P20_11, TOM0_CH15_P23_1,

    TOM1_CH0_P00_0,  TOM1_CH0_P02_8,   TOM1_CH0_P15_5,   TOM1_CH0_P20_12, TOM1_CH0_P33_10,
    TOM1_CH1_P11_2,  TOM1_CH1_P20_13,  TOM1_CH1_P33_5,   TOM1_CH1_P33_9,
    TOM1_CH2_P11_3,  TOM1_CH2_P20_14,  TOM1_CH2_P33_6,
    TOM1_CH3_P11_6,  TOM1_CH3_P14_0,   TOM1_CH3_P15_0,   TOM1_CH3_P33_7,
    TOM1_CH4_P00_0,  TOM1_CH4_P02_0,   TOM1_CH4_P02_1,   TOM1_CH4_P02_8,  TOM1_CH4_P11_2,  TOM1_CH4_P11_9, TOM1_CH4_P14_1,  TOM1_CH4_P15_1,  TOM1_CH4_P15_2,  TOM1_CH4_P20_8,  TOM1_CH4_P20_9, TOM1_CH4_P21_6, TOM1_CH4_P33_8,
    TOM1_CH5_P02_2,  TOM1_CH5_P02_3,   TOM1_CH5_P11_3,   TOM1_CH5_P11_6,  TOM1_CH5_P11_10, TOM1_CH5_P15_2, TOM1_CH5_P15_3,  TOM1_CH5_P15_5,  TOM1_CH5_P20_11, TOM1_CH5_P21_7,  TOM1_CH5_P33_5, TOM1_CH5_P33_6,
    TOM1_CH6_P02_4,  TOM1_CH6_P02_5,   TOM1_CH6_P11_9,   TOM1_CH6_P11_10, TOM1_CH6_P11_11, TOM1_CH6_P14_0, TOM1_CH6_P14_3,  TOM1_CH6_P15_3,  TOM1_CH6_P20_12, TOM1_CH6_P20_13, TOM1_CH6_P33_7, TOM1_CH6_P33_8,
    TOM1_CH7_P02_6,  TOM1_CH7_P02_7,   TOM1_CH7_P11_11,  TOM1_CH7_P11_12, TOM1_CH7_P14_1,  TOM1_CH7_P14_4, TOM1_CH7_P15_0,  TOM1_CH7_P20_8,  TOM1_CH7_P20_14, TOM1_CH7_P33_9,  TOM1_CH7_P33_10,
    TOM1_CH8_P02_0,
    TOM1_CH9_P02_1,
    TOM1_CH10_P02_2, TOM1_CH10_P10_5,
    TOM1_CH11_P02_3, TOM1_CH11_P10_6,
    TOM1_CH12_P02_4,
    TOM1_CH13_P02_5, TOM1_CH13_P20_9,
    TOM1_CH14_P02_6,
    TOM1_CH15_P02_7, TOM1_CH15_P20_11,
}pwm_channel_enum;

void pwm_all_channel_close      (void);
void pwm_init                   (pwm_channel_enum pwmch, uint32 freq, uint32 duty);
void pwm_set_duty               (pwm_channel_enum pwmch, uint32 duty);

#endif
