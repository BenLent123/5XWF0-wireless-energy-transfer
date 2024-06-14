/*
 * PWM.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Ben
 */

#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "PWM.h"

TIM_HandleTypeDef htim3;

//variables
int32_t PWM_Period_DCDC;
int32_t PWM_Pulsewidth_DCDC;


void SetBaseTime(){
HAL_TIM_Base_Start(&htim3);
}

void SetBaseFreq(int PWM_Freq_DCDC){
	PWM_Period_DCDC = 64000000/(PWM_Freq_DCDC)-1;
	__HAL_TIM_SET_AUTORELOAD(&htim3, PWM_Period_DCDC);
}

void SetBaseDutyC(float PWM_DutyC_DCDC){

	  //----------------------DCDC PWM settings----------------------------------------

	  PWM_Pulsewidth_DCDC = (int)((PWM_Period_DCDC*PWM_DutyC_DCDC)/100);


	  //------------------------set the settings--------------------------------------


	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM_Pulsewidth_DCDC);
	  //---------------------START PWM----------------------------------------

}


void StartPWM(){
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
}

void PWMdutyCcontroller(float PWM_DutyC_DCDC){
	  //----------------------DCDC PWM settings----------------------------------------

	  PWM_Pulsewidth_DCDC = (int)((PWM_Period_DCDC*PWM_DutyC_DCDC)/100);

	  //------------------------set the settings--------------------------------------
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM_Pulsewidth_DCDC);
	  //---------------------START PWM----------------------------------------


}

void PWMoffSaftey(){
 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
}
