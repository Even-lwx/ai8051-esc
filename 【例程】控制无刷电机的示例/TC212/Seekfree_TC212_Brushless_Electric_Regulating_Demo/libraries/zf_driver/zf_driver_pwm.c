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

#include "IfxGtm_Tom_Pwm.h"
#include "ifxGtm_PinMap.h"
#include "zf_common_debug.h"
#include "zf_driver_pwm.h"

#define CMU_CLK_FREQ           3125000.0f                       //CMU时钟频率

//-------------------------------------------------------------------------------------------------------------------
//  函数简介     获取端口参数
//  返回参数     IfxGtm_Atom_ToutMap
//  使用示例     get_pwm_pin(ATOM0_CH0_P00_0);
//  备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static IfxGtm_Tom_ToutMap* get_pwm_pin (pwm_channel_enum atom_pin)
{
    IfxGtm_Tom_ToutMap* pwm_pwm_pin_config;

    switch(atom_pin)
    {
        case TOM0_CH0_P33_10:  pwm_pwm_pin_config = &IfxGtm_TOM0_0_TOUT32_P33_10_OUT;   break;
        case TOM0_CH0_P15_5:   pwm_pwm_pin_config = &IfxGtm_TOM0_0_TOUT76_P15_5_OUT;    break;

        case TOM0_CH1_P33_5:   pwm_pwm_pin_config = &IfxGtm_TOM0_1_TOUT27_P33_5_OUT;    break;
        case TOM0_CH1_P33_9:   pwm_pwm_pin_config = &IfxGtm_TOM0_1_TOUT31_P33_9_OUT;    break;

        case TOM0_CH2_P10_5:   pwm_pwm_pin_config = &IfxGtm_TOM0_2_TOUT107_P10_5_OUT;   break;
        case TOM0_CH2_P33_6:   pwm_pwm_pin_config = &IfxGtm_TOM0_2_TOUT28_P33_6_OUT;    break;

        case TOM0_CH3_P10_6:   pwm_pwm_pin_config = &IfxGtm_TOM0_3_TOUT108_P10_6_OUT;   break;
        case TOM0_CH3_P33_7:   pwm_pwm_pin_config = &IfxGtm_TOM0_3_TOUT29_P33_7_OUT;    break;
        case TOM0_CH3_P14_0:   pwm_pwm_pin_config = &IfxGtm_TOM0_3_TOUT80_P14_0_OUT;    break;

        case TOM0_CH4_P02_0:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT0_P02_0_OUT;     break;
        case TOM0_CH4_P02_1:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT1_P02_1_OUT;     break;
        case TOM0_CH4_P33_8:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT30_P33_8_OUT;    break;
        case TOM0_CH4_P21_6:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT57_P21_6_OUT;    break;
        case TOM0_CH4_P20_8:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT64_P20_8_OUT;    break;
        case TOM0_CH4_P20_9:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT65_P20_9_OUT;    break;
        case TOM0_CH4_P15_1:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT72_P15_1_OUT;    break;
        case TOM0_CH4_P15_2:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT73_P15_2_OUT;    break;
        case TOM0_CH4_P14_1:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT81_P14_1_OUT;    break;
        case TOM0_CH4_P02_8:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT8_P02_8_OUT;     break;
        case TOM0_CH4_P11_2:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT95_P11_2_OUT;    break;
        case TOM0_CH4_P00_0:   pwm_pwm_pin_config = &IfxGtm_TOM0_4_TOUT9_P00_0_OUT;     break;

        case TOM0_CH5_P33_5:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT27_P33_5_OUT;    break;
        case TOM0_CH5_P33_6:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT28_P33_6_OUT;    break;
        case TOM0_CH5_P02_2:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT2_P02_2_OUT;     break;
        case TOM0_CH5_P02_3:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT3_P02_3_OUT;     break;
        case TOM0_CH5_P21_7:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT58_P21_7_OUT;    break;
        case TOM0_CH5_P20_11:  pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT67_P20_11_OUT;   break;
        case TOM0_CH5_P15_3:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT74_P15_3_OUT;    break;
        case TOM0_CH5_P15_5:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT76_P15_5_OUT;    break;
        case TOM0_CH5_P11_3:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT96_P11_3_OUT;    break;
        case TOM0_CH5_P11_6:   pwm_pwm_pin_config = &IfxGtm_TOM0_5_TOUT97_P11_6_OUT;    break;

        case TOM0_CH6_P33_7:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT29_P33_7_OUT;    break;
        case TOM0_CH6_P33_8:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT30_P33_8_OUT;    break;
        case TOM0_CH6_P23_1:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT42_P23_1_OUT;    break;
        case TOM0_CH6_P02_4:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT4_P02_4_OUT;     break;
        case TOM0_CH6_P02_5:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT5_P02_5_OUT;     break;
        case TOM0_CH6_P20_12:  pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT68_P20_12_OUT;   break;
        case TOM0_CH6_P20_13:  pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT69_P20_13_OUT;   break;
        case TOM0_CH6_P14_0:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT80_P14_0_OUT;    break;
        case TOM0_CH6_P14_3:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT83_P14_3_OUT;    break;
        case TOM0_CH6_P11_9:   pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT98_P11_9_OUT;    break;
        case TOM0_CH6_P11_10:  pwm_pwm_pin_config = &IfxGtm_TOM0_6_TOUT99_P11_10_OUT;   break;

        case TOM0_CH7_P11_11:  pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT100_P11_11_OUT;  break;
        case TOM0_CH7_P11_12:  pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT101_P11_12_OUT;  break;
        case TOM0_CH7_P33_9:   pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT31_P33_9_OUT;    break;
        case TOM0_CH7_P33_10:  pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT32_P33_10_OUT;   break;
        case TOM0_CH7_P20_8:   pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT64_P20_8_OUT;    break;
        case TOM0_CH7_P02_6:   pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT6_P02_6_OUT;     break;
        case TOM0_CH7_P20_14:  pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT70_P20_14_OUT;   break;
        case TOM0_CH7_P15_0:   pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT71_P15_0_OUT;    break;
        case TOM0_CH7_P02_7:   pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT7_P02_7_OUT;     break;
        case TOM0_CH7_P14_1:   pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT81_P14_1_OUT;    break;
        case TOM0_CH7_P14_4:   pwm_pwm_pin_config = &IfxGtm_TOM0_7_TOUT84_P14_4_OUT;    break;

        case TOM0_CH8_P02_0:   pwm_pwm_pin_config = &IfxGtm_TOM0_8_TOUT0_P02_0_OUT;     break;
        case TOM0_CH8_P20_12:  pwm_pwm_pin_config = &IfxGtm_TOM0_8_TOUT68_P20_12_OUT;   break;
        case TOM0_CH8_P02_8:   pwm_pwm_pin_config = &IfxGtm_TOM0_8_TOUT8_P02_8_OUT;     break;
        case TOM0_CH8_P11_2:   pwm_pwm_pin_config = &IfxGtm_TOM0_8_TOUT95_P11_2_OUT;    break;
        case TOM0_CH8_P00_0:   pwm_pwm_pin_config = &IfxGtm_TOM0_8_TOUT9_P00_0_OUT;     break;

        case TOM0_CH9_P02_1:   pwm_pwm_pin_config = &IfxGtm_TOM0_9_TOUT1_P02_1_OUT;     break;
        case TOM0_CH9_P20_13:  pwm_pwm_pin_config = &IfxGtm_TOM0_9_TOUT69_P20_13_OUT;   break;

        case TOM0_CH10_P02_2:  pwm_pwm_pin_config = &IfxGtm_TOM0_10_TOUT2_P02_2_OUT;    break;
        case TOM0_CH10_P20_14: pwm_pwm_pin_config = &IfxGtm_TOM0_10_TOUT70_P20_14_OUT;  break;
        case TOM0_CH10_P11_3:  pwm_pwm_pin_config = &IfxGtm_TOM0_10_TOUT96_P11_3_OUT;   break;

        case TOM0_CH11_P02_3:  pwm_pwm_pin_config = &IfxGtm_TOM0_11_TOUT3_P02_3_OUT;    break;
        case TOM0_CH11_P15_0:  pwm_pwm_pin_config = &IfxGtm_TOM0_11_TOUT71_P15_0_OUT;   break;
        case TOM0_CH11_P11_6:  pwm_pwm_pin_config = &IfxGtm_TOM0_11_TOUT97_P11_6_OUT;   break;

        case TOM0_CH12_P02_4:  pwm_pwm_pin_config = &IfxGtm_TOM0_12_TOUT4_P02_4_OUT;    break;
        case TOM0_CH12_P15_1:  pwm_pwm_pin_config = &IfxGtm_TOM0_12_TOUT72_P15_1_OUT;   break;
        case TOM0_CH12_P11_9:  pwm_pwm_pin_config = &IfxGtm_TOM0_12_TOUT98_P11_9_OUT;   break;

        case TOM0_CH13_P02_5:  pwm_pwm_pin_config = &IfxGtm_TOM0_13_TOUT5_P02_5_OUT;    break;
        case TOM0_CH13_P15_2:  pwm_pwm_pin_config = &IfxGtm_TOM0_13_TOUT73_P15_2_OUT;   break;
        case TOM0_CH13_P20_9:  pwm_pwm_pin_config = &IfxGtm_TOM0_13_TOUT65_P20_9_OUT;   break;
        case TOM0_CH13_P11_10: pwm_pwm_pin_config = &IfxGtm_TOM0_13_TOUT99_P11_10_OUT;  break;

        case TOM0_CH14_P11_11: pwm_pwm_pin_config = &IfxGtm_TOM0_14_TOUT100_P11_11_OUT; break;
        case TOM0_CH14_P02_6:  pwm_pwm_pin_config = &IfxGtm_TOM0_14_TOUT6_P02_6_OUT;    break;
        case TOM0_CH14_P15_3:  pwm_pwm_pin_config = &IfxGtm_TOM0_14_TOUT74_P15_3_OUT;   break;

        case TOM0_CH15_P11_12: pwm_pwm_pin_config = &IfxGtm_TOM0_15_TOUT101_P11_12_OUT; break;
        case TOM0_CH15_P23_1:  pwm_pwm_pin_config = &IfxGtm_TOM0_15_TOUT42_P23_1_OUT;   break;
        case TOM0_CH15_P20_11: pwm_pwm_pin_config = &IfxGtm_TOM0_15_TOUT67_P20_11_OUT;  break;
        case TOM0_CH15_P02_7:  pwm_pwm_pin_config = &IfxGtm_TOM0_15_TOUT7_P02_7_OUT;    break;

        case TOM1_CH0_P33_10:  pwm_pwm_pin_config = &IfxGtm_TOM1_0_TOUT32_P33_10_OUT;   break;
        case TOM1_CH0_P20_12:  pwm_pwm_pin_config = &IfxGtm_TOM1_0_TOUT68_P20_12_OUT;   break;
        case TOM1_CH0_P15_5:   pwm_pwm_pin_config = &IfxGtm_TOM1_0_TOUT76_P15_5_OUT;    break;
        case TOM1_CH0_P02_8:   pwm_pwm_pin_config = &IfxGtm_TOM1_0_TOUT8_P02_8_OUT;     break;
        case TOM1_CH0_P00_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_0_TOUT9_P00_0_OUT;     break;

        case TOM1_CH1_P33_5:   pwm_pwm_pin_config = &IfxGtm_TOM1_1_TOUT27_P33_5_OUT;    break;
        case TOM1_CH1_P33_9:   pwm_pwm_pin_config = &IfxGtm_TOM1_1_TOUT31_P33_9_OUT;    break;
        case TOM1_CH1_P20_13:  pwm_pwm_pin_config = &IfxGtm_TOM1_1_TOUT69_P20_13_OUT;   break;
        case TOM1_CH1_P11_2:   pwm_pwm_pin_config = &IfxGtm_TOM1_1_TOUT95_P11_2_OUT;    break;

        case TOM1_CH2_P33_6:   pwm_pwm_pin_config = &IfxGtm_TOM1_2_TOUT28_P33_6_OUT;    break;
        case TOM1_CH2_P20_14:  pwm_pwm_pin_config = &IfxGtm_TOM1_2_TOUT70_P20_14_OUT;   break;
        case TOM1_CH2_P11_3:   pwm_pwm_pin_config = &IfxGtm_TOM1_2_TOUT96_P11_3_OUT;    break;

        case TOM1_CH3_P33_7:   pwm_pwm_pin_config = &IfxGtm_TOM1_3_TOUT29_P33_7_OUT;    break;
        case TOM1_CH3_P15_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_3_TOUT71_P15_0_OUT;    break;
        case TOM1_CH3_P14_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_3_TOUT80_P14_0_OUT;    break;
        case TOM1_CH3_P11_6:   pwm_pwm_pin_config = &IfxGtm_TOM1_3_TOUT97_P11_6_OUT;    break;

        case TOM1_CH4_P02_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT0_P02_0_OUT;     break;
        case TOM1_CH4_P02_1:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT1_P02_1_OUT;     break;
        case TOM1_CH4_P33_8:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT30_P33_8_OUT;    break;
        case TOM1_CH4_P21_6:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT57_P21_6_OUT;    break;
        case TOM1_CH4_P20_8:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT64_P20_8_OUT;    break;
        case TOM1_CH4_P20_9:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT65_P20_9_OUT;    break;
        case TOM1_CH4_P15_1:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT72_P15_1_OUT;    break;
        case TOM1_CH4_P15_2:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT73_P15_2_OUT;    break;
        case TOM1_CH4_P14_1:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT81_P14_1_OUT;    break;
        case TOM1_CH4_P02_8:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT8_P02_8_OUT;     break;
        case TOM1_CH4_P11_2:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT95_P11_2_OUT;    break;
        case TOM1_CH4_P11_9:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT98_P11_9_OUT;    break;
        case TOM1_CH4_P00_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_4_TOUT9_P00_0_OUT;     break;

        case TOM1_CH5_P33_5:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT27_P33_5_OUT;    break;
        case TOM1_CH5_P33_6:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT28_P33_6_OUT;    break;
        case TOM1_CH5_P02_2:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT2_P02_2_OUT;     break;
        case TOM1_CH5_P02_3:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT3_P02_3_OUT;     break;
        case TOM1_CH5_P21_7:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT58_P21_7_OUT;    break;
        case TOM1_CH5_P20_11:  pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT67_P20_11_OUT;   break;
        case TOM1_CH5_P15_2:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT73_P15_2_OUT;    break;
        case TOM1_CH5_P15_3:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT74_P15_3_OUT;    break;
        case TOM1_CH5_P15_5:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT76_P15_5_OUT;    break;
        case TOM1_CH5_P11_3:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT96_P11_3_OUT;    break;
        case TOM1_CH5_P11_6:   pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT97_P11_6_OUT;    break;
        case TOM1_CH5_P11_10:  pwm_pwm_pin_config = &IfxGtm_TOM1_5_TOUT99_P11_10_OUT;   break;

        case TOM1_CH6_P11_11:  pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT100_P11_11_OUT;  break;
        case TOM1_CH6_P33_7:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT29_P33_7_OUT;    break;
        case TOM1_CH6_P33_8:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT30_P33_8_OUT;    break;
        case TOM1_CH6_P02_4:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT4_P02_4_OUT;     break;
        case TOM1_CH6_P02_5:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT5_P02_5_OUT;     break;
        case TOM1_CH6_P20_12:  pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT68_P20_12_OUT;   break;
        case TOM1_CH6_P20_13:  pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT69_P20_13_OUT;   break;
        case TOM1_CH6_P15_3:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT74_P15_3_OUT;    break;
        case TOM1_CH6_P14_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT80_P14_0_OUT;    break;
        case TOM1_CH6_P14_3:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT83_P14_3_OUT;    break;
        case TOM1_CH6_P11_9:   pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT98_P11_9_OUT;    break;
        case TOM1_CH6_P11_10:  pwm_pwm_pin_config = &IfxGtm_TOM1_6_TOUT99_P11_10_OUT;   break;

        case TOM1_CH7_P11_11:  pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT100_P11_11_OUT;  break;
        case TOM1_CH7_P11_12:  pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT101_P11_12_OUT;  break;
        case TOM1_CH7_P33_9:   pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT31_P33_9_OUT;    break;
        case TOM1_CH7_P33_10:  pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT32_P33_10_OUT;   break;
        case TOM1_CH7_P20_8:   pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT64_P20_8_OUT;    break;
        case TOM1_CH7_P02_6:   pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT6_P02_6_OUT;     break;
        case TOM1_CH7_P20_14:  pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT70_P20_14_OUT;   break;
        case TOM1_CH7_P15_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT71_P15_0_OUT;    break;
        case TOM1_CH7_P02_7:   pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT7_P02_7_OUT;     break;
        case TOM1_CH7_P14_1:   pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT81_P14_1_OUT;    break;
        case TOM1_CH7_P14_4:   pwm_pwm_pin_config = &IfxGtm_TOM1_7_TOUT84_P14_4_OUT;    break;

        case TOM1_CH8_P02_0:   pwm_pwm_pin_config = &IfxGtm_TOM1_8_TOUT0_P02_0_OUT;     break;

        case TOM1_CH9_P02_1:   pwm_pwm_pin_config = &IfxGtm_TOM1_9_TOUT1_P02_1_OUT;     break;

        case TOM1_CH10_P10_5:  pwm_pwm_pin_config = &IfxGtm_TOM1_10_TOUT107_P10_5_OUT;  break;
        case TOM1_CH10_P02_2:  pwm_pwm_pin_config = &IfxGtm_TOM1_10_TOUT2_P02_2_OUT;    break;

        case TOM1_CH11_P10_6:  pwm_pwm_pin_config = &IfxGtm_TOM1_11_TOUT108_P10_6_OUT;  break;
        case TOM1_CH11_P02_3:  pwm_pwm_pin_config = &IfxGtm_TOM1_11_TOUT3_P02_3_OUT;    break;

        case TOM1_CH12_P02_4:  pwm_pwm_pin_config = &IfxGtm_TOM1_12_TOUT4_P02_4_OUT;    break;

        case TOM1_CH13_P02_5:  pwm_pwm_pin_config = &IfxGtm_TOM1_13_TOUT5_P02_5_OUT;    break;
        case TOM1_CH13_P20_9:  pwm_pwm_pin_config = &IfxGtm_TOM1_13_TOUT65_P20_9_OUT;   break;

        case TOM1_CH14_P02_6:  pwm_pwm_pin_config = &IfxGtm_TOM1_14_TOUT6_P02_6_OUT;    break;

        case TOM1_CH15_P20_11: pwm_pwm_pin_config = &IfxGtm_TOM1_15_TOUT67_P20_11_OUT;  break;
        case TOM1_CH15_P02_7:  pwm_pwm_pin_config = &IfxGtm_TOM1_15_TOUT7_P02_7_OUT;    break;

        default: zf_assert(FALSE); pwm_pwm_pin_config = NULL;
    }
    return pwm_pwm_pin_config;
}

//-------------------------------------------------------------------------------------------------------------------
//  函数简介      关闭所有通道的PWM输出
//  返回参数      void
//  使用示例      pwm_all_channel_close();
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void pwm_all_channel_close (void)
{
    IfxGtm_Tom_Pwm_Config g_tomConfig;
    IfxGtm_Tom_Pwm_Driver g_tomDriver;
    int index,channel;

    IfxGtm_enable(&MODULE_GTM);

    if(!(MODULE_GTM.CMU.CLK_EN.U & 0x2))
    {
        IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, CMU_CLK_FREQ);
        IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);
    }
    IfxGtm_Tom_Pwm_initConfig(&g_tomConfig, &MODULE_GTM);

    for(index = 0; index < 2; index++)
    {
        for(channel = 0; channel < 16; channel++)
        {
            g_tomConfig.tom = index;
            g_tomConfig.tomChannel = channel;
            IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tomConfig);
            IfxGtm_Tom_Pwm_start(&g_tomDriver, TRUE);
        }
    }

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM占空比设置
// 参数说明     pin             选择 PWM 引脚
// 参数说明     duty            设置占空比
// 返回参数     void
// 使用示例     pwm_set_duty(ATOM0_CH7_P02_7, 5000); // 设置占空比为百分之5000/PWM_DUTY_MAX*100
// 备注信息     GTM_ATOM0_PWM_DUTY_MAX宏定义在zf_driver_pwm.h  默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_duty (pwm_channel_enum pwmch, uint32 duty)
{
    uint32 period;

    zf_assert(duty <= PWM_DUTY_MAX);    // 如果在这里出现了断言，那么说明你输入的占空比已经大于了最大占空比 PWM_DUTY_MAX 宏定义在zf_driver_pwm.h  默认为10000

    IfxGtm_Tom_ToutMap *tom_channel;
    tom_channel = get_pwm_pin(pwmch);

    period = IfxGtm_Tom_Ch_getCompareZero(&MODULE_GTM.TOM[tom_channel->tom], tom_channel->channel);

    switch(tom_channel->tom)
    {
        case 0: duty = (uint32)((uint64)duty * period / PWM_DUTY_MAX); break;
        case 1: duty = (uint32)((uint64)duty * period / PWM_DUTY_MAX); break;
        case 2: duty = (uint32)((uint64)duty * period / PWM_DUTY_MAX); break;
        case 3: duty = (uint32)((uint64)duty * period / PWM_DUTY_MAX); break;
    }
    IfxGtm_Tom_Ch_setCompareOneShadow(&MODULE_GTM.TOM[tom_channel->tom], tom_channel->channel, duty);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 初始化
// 参数说明     pin             选择 PWM 引脚
// 参数说明     freq            设置频率 同个模块只有最后一次设置生效
// 参数说明     duty            设置占空比
// 返回参数     void
// 使用示例     pwm_init(TOM0_CH0_P15_5, 50, 1000);   // ATOM 0模块的通道7 使用P02_7引脚输出PWM  PWM频率50HZ  占空比百分之1000/PWM_DUTY_MAX*100
// 备注信息     PWM_DUTY_MAX宏定义在zf_driver_pwm.h  默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_init (pwm_channel_enum pwmch, uint32 freq, uint32 duty)
{
    IfxGtm_Tom_Pwm_Config g_tomConfig;
    IfxGtm_Tom_Pwm_Driver g_tomDriver;

    IfxGtm_Tom_ToutMap *tom_channel;

    zf_assert(duty <= PWM_DUTY_MAX);    // 如果在这里出现了断言，那么说明你输入的占空比已经大于了最大占空比 PWM_DUTY_MAX 宏定义在zf_driver_pwm.h  默认为10000


    tom_channel = get_pwm_pin(pwmch);

    switch(tom_channel->tom)
    {
        case 0: IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, duty <= PWM_DUTY_MAX); break;
        case 1: IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, duty <= PWM_DUTY_MAX); break;
        case 2: IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, duty <= PWM_DUTY_MAX); break;
        case 3: IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, duty <= PWM_DUTY_MAX); break;
    }

    IfxGtm_enable(&MODULE_GTM);

    if(!(MODULE_GTM.CMU.CLK_EN.B.EN_FXCLK & 0x2))
    {
        IfxGtm_Cmu_setGclkFrequency(&MODULE_GTM, CMU_CLK_FREQ);
        IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, CMU_CLK_FREQ);
        IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);
    }

    IfxGtm_Tom_Pwm_initConfig(&g_tomConfig, &MODULE_GTM);

    g_tomConfig.tom = tom_channel->tom;
    g_tomConfig.tomChannel = tom_channel->channel;
    g_tomConfig.period = CMU_CLK_FREQ/freq;
    g_tomConfig.pin.outputPin = tom_channel;
    g_tomConfig.synchronousUpdateEnabled = TRUE;

    switch(tom_channel->tom)
    {
        case 0: g_tomConfig.dutyCycle = (uint32)((uint64)duty * g_tomConfig.period / PWM_DUTY_MAX); break;
        case 1: g_tomConfig.dutyCycle = (uint32)((uint64)duty * g_tomConfig.period / PWM_DUTY_MAX); break;
        case 2: g_tomConfig.dutyCycle = (uint32)((uint64)duty * g_tomConfig.period / PWM_DUTY_MAX); break;
        case 3: g_tomConfig.dutyCycle = (uint32)((uint64)duty * g_tomConfig.period / PWM_DUTY_MAX); break;
    }

    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tomConfig);
    IfxGtm_Tom_Pwm_start(&g_tomDriver, TRUE);
}


