/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		pit_timer
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2024-01-22
 ********************************************************************************************************************/
#include "zf_common_typedef.h"
#include "zf_common_clock.h"

#include "bldc_config.h"
#include "motor_control.h"
#include "pwm_out.h"
#include "signal_input.h"

#define PWMIN_PIN   P21

#define PWMIN_PERIOD_MIN_TICK       (2040u)
#define PWMIN_PERIOD_MAX_TICK       (25000u)
#define PWMIN_HIGH_MIN_TICK         (1000u)
#define PWMIN_HIGH_MAX_TICK         (2000u)
#define PWMIN_TIMEOUT_TICK          (1500u)

#define PWMB_SR1_CC5IF              (0x02u)
#define PWMB_SR1_CC6IF              (0x04u)
#define PWMB_SR2_CC5OF              (0x02u)
#define PWMB_SR2_CC6OF              (0x04u)

pwmin_struct pwmin;

static uint8 pwmin_period_valid = 0;
static vuint16 pwm_input_timeout_count = 0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMB输入捕获中断
//  @param      void                        
//  @return     void          
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void pwmb_isr()interrupt 27
{
    uint8 sr1 = PWMB_SR1;
    uint8 sr2 = PWMB_SR2;
    uint8 capture_high;
    uint8 frame_overcapture;
    uint16 temp;
    frame_overcapture = sr2 & (PWMB_SR2_CC5OF | PWMB_SR2_CC6OF);

    if(sr1 & PWMB_SR1_CC5IF)
    {
        // 必须先读取高字节，再单独读取低字节；读取低字节会清除 CC5 标志。
        capture_high = PWMB_CCR5H;
        pwmin.period = ((uint16)capture_high << 8);
        pwmin.period |= PWMB_CCR5L;

        pwmin_period_valid = 0;
        if((0 == frame_overcapture)
        && (PWMIN_PERIOD_MIN_TICK <= pwmin.period)
        && (PWMIN_PERIOD_MAX_TICK >= pwmin.period))
        {
            pwmin_period_valid = 1;
        }
    }

    if(sr1 & PWMB_SR1_CC6IF)
    {
        // 必须先读取高字节，再单独读取低字节；读取低字节会清除 CC6 标志。
        capture_high = PWMB_CCR6H;
        pwmin.high_value = ((uint16)capture_high << 8);
        pwmin.high_value |= PWMB_CCR6L;
        pwmin.high_time = pwmin.high_value;

        if((0 == frame_overcapture)
        && pwmin_period_valid
        && (PWMIN_HIGH_MIN_TICK <= pwmin.high_time)
        && (PWMIN_HIGH_MAX_TICK >= pwmin.high_time))
        {
            temp = pwmin.high_time - PWMIN_HIGH_MIN_TICK;
            if(temp < 50u)
            {
                temp = 0;
            }
            pwmin.throttle = temp;
            pwm_input_timeout_count = 0;

#if (BLDC_USR_DUTY == 0)
            motor.duty = (uint32)pwmin.throttle * BLDC_PWM_ARR_MAX / 1000u;
#endif
        }

        // 一个周期只消费一次周期捕获，下一帧必须重新取得有效 CC5。
        pwmin_period_valid = 0;
    }

#if (BLDC_USR_DUTY > 0)
    // 宏固定占空比：1~100 表示目标 1%~100%，不读外部捕获、不做无信号清零
    motor.duty = (uint32)BLDC_USR_DUTY * (uint32)BLDC_PWM_ARR_MAX / 100u;
    pwm_input_timeout_count = 0;
#endif
}

void pwm_input_timeout_tick(void)
{
#if (BLDC_USR_DUTY == 0)
    if(pwm_input_timeout_count < PWMIN_TIMEOUT_TICK)
    {
        pwm_input_timeout_count++;
    }

    if(pwm_input_timeout_count >= PWMIN_TIMEOUT_TICK)
    {
        pwmin.throttle = 0;
        motor.duty = 0;
    }
#else
    pwm_input_timeout_count = 0;
#endif
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMB输入捕获初始化
//  @param      void                        
//  @return     void          
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_init(void)
{
    gpio_init(IO_P21, GPI, 0, GPI_IMPEDANCE);
    gpio_init(IO_P23, GPI, 0, GPI_IMPEDANCE);
    
    PWMB_PS = 0x0A;		// 通道引脚切换
    PWMB_CCMR1 = 0x01;	// CC5为输入模式,且映射到TI5FP5上
	PWMB_CCMR2 = 0x02;	// CC6为输入模式,且映射到TI5FP6上
    
	// CC5E 开启输入捕获
	// CC5P 捕获发生在TI5F的上升沿
	// CC6E 开启输入捕获
	// CC6P 捕获发生在TI5F的下降沿
    PWMB_CCER1 = 0x31;
    
    PWMB_PSCRH = 0;		// 分频值
	PWMB_PSCRL = system_clock / 1000000 - 1;    // 分频值
    PWMB_ARRH = 0xff;
    PWMB_ARRL = 0xff;
    PWMB_SMCR = 0x54;	// TS=TI1FP1,SMS=TI1上升沿复位模式
	PWMB_CR1 = 0x04;	// URS=1，仅计数器溢出产生更新请求
    PWMB_EGR = 0x01;	// 装载预分频和自动重装值
	PWMB_IER = PWMB_SR1_CC5IF | PWMB_SR1_CC6IF;
	PWMB_CR1 |= 0x01;	// 启动PWMB，向上计数

    pwmin.period = 0;
    pwmin.high_value = 0;
    pwmin.high_time = 0;
    pwmin.throttle = 0;
    pwmin_period_valid = 0;
    pwm_input_timeout_count = 0;

#if (BLDC_USR_DUTY > 0)
    motor.duty = (uint32)BLDC_USR_DUTY * (uint32)BLDC_PWM_ARR_MAX / 100u;
#endif
}
