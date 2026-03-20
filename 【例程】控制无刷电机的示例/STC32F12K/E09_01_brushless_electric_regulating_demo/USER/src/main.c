/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 					接线定义：
					------------------------------------
					模块管脚            单片机管脚
					// 硬件 SPI 引脚
					SCL/SPC             查看 SEEKFREE_IMU963RA.h 中 IMU963RA_SPC_PIN 宏定义
					SDA/DSI             查看 SEEKFREE_IMU963RA.h 中 IMU963RA_SDI_PIN 宏定义
					SA0/SDO             查看 SEEKFREE_IMU963RA.h 中 IMU963RA_SDO_PIN 宏定义
					CS                  查看 SEEKFREE_IMU963RA.h 中 IMU963RA_CS_PIN  宏定义
					VCC                 3.3V电源
					GND                 电源地
					其余引脚悬空
	
					// 软件 IIC 引脚
					SCL/SPC             查看 SEEKFREE_IMU963RA.h 中 IMU963RA_SCL_PIN 宏定义
					SDA/DSI             查看 SEEKFREE_IMU963RA.h 中 IMU963RA_SDA_PIN 宏定义
					VCC                 3.3V电源
					GND                 电源地
					其余引脚悬空
					------------------------------------
 ********************************************************************************************************************/
#include "headfile.h"



/*
 * 关于内核频率的设定，可以查看board.h文件
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */

// STC无刷电调仅支持3S锂电池，请按照规定使用电池
// STC无刷电调仅支持3S锂电池，请按照规定使用电池
// STC无刷电调仅支持3S锂电池，请按照规定使用电池
uint16 duty;
    	
// 电调控制是看高电平时间，范围： 1ms-2ms
// 1ms 为 0%
// 2ms 为 100%


void main()
{
	clock_init(SYSTEM_CLOCK_56M);	// 初始化系统频率,勿删除此句代码。
	board_init();			 		// 初始化寄存器,勿删除此句代码。
	
	// 此处编写用户代码(例如：外设初始化代码等)

	// pwm初始化频率为50hz 无刷设置0%占空比
	duty = 1.0 / 20 * 10000;		// (1ms/20ms * 10000)（10000是PWM的满占空比时候的值） 10000为PWM最大值
	pwm_init(PWMB_CH1_P00, 50, duty);
	
							
	pwm_init(PWMB_CH2_P01, 50, 600);	// (1.2ms/20ms * 10000)（10000是PWM的满占空比时候的值） 10000为PWM最大值
										// 固定20%转速

    while(1)
	{
		// 计算无刷电调转速   （1ms - 2ms）/20ms * 10000（10000是PWM的满占空比时候的值）
        // 无刷电调转速 0%   为 500
		// 无刷电调转速 20%  为 600
		// 无刷电调转速 40%  为 700
		// 无刷电调转速 60%  为 800
		// 无刷电调转速 80%  为 900
		// 无刷电调转速 100% 为 1000
		
		// 修改duty的值，可以修改无刷电调转速
		
        duty++;
        if(800 < duty) 
		{
			duty = 500;
		}
        
        // 控制无刷电调转速
        pwm_duty(PWMB_CH1_P00, duty);

		// 固定20%的占空比
		pwm_duty(PWMB_CH2_P01, 600);
		
		delay_ms(10);
		
    }
}
