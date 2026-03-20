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
* 文件名称          zf_driver_adc
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

#ifndef _zf_driver_adc_h
#define _zf_driver_adc_h

#include "zf_common_typedef.h"

typedef enum    // 枚举ADC通道
{
    // ADC0可选引脚
    ADC0_CH5_A5 = 0*16 + 5,
    ADC0_CH6_A6,
    ADC0_CH7_A7,
    ADC0_CH8_A8,
    ADC0_CH9_A9,
    ADC0_CH10_A10,
    ADC0_CH11_A11,

    // ADC1可选引脚
    ADC1_CH0_A12  = 1*16 + 0,
    ADC1_CH1_A13,
    ADC1_CH2_A14,
    ADC1_CH3_A15,
    ADC1_CH4_A16,
    ADC1_CH5_A17,
    ADC1_CH6_A18,
}adc_channel_enum;

// 此枚举定义不允许用户修改
typedef enum        // 枚举ADC通道
{
    ADC_8BIT,       // 8位分辨率
    ADC_10BIT,      // 10位分辨率
    ADC_12BIT,      // 12位分辨率
}adc_resolution_enum;

//====================================================ADC 通用操作====================================================
uint16  adc_convert             (adc_channel_enum vadc_chn);                                    // ADC转换一次
uint16  adc_mean_filter_convert (adc_channel_enum vadc_chn, uint8 count);                       // ADC均值滤波
void    adc_init                (adc_channel_enum vadc_chn, adc_resolution_enum resolution);    // ADC初始化
//====================================================ADC 通用操作====================================================


#endif
