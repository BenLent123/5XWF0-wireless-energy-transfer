/*
 * PWM.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Ben
 */


/*
 * PWM.c
 *
 *  Created on: 29.05.2024
 *      Author: Ben
 */
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "PWM.h"

TIM_HandleTypeDef htim1;


//variables
int32_t PWM_Period_DCAC;
int32_t PWM_Pulsewidth_DCAC;


void SetBaseFreq(int PWM_Freq_DCAC){
	PWM_Period_DCAC = 64000000/(PWM_Freq_DCAC)-1;
	__HAL_TIM_SET_AUTORELOAD(&htim1, PWM_Period_DCAC);
}

void SetBaseDutyC(float PWM_DutyC_DCAC){
	 //----------------------DCAC PWM settings----------------------------------------

	  PWM_Pulsewidth_DCAC = (int)((PWM_Period_DCAC*PWM_DutyC_DCAC)/100);

	  //------------------------set the settings--------------------------------------

	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM_Pulsewidth_DCAC);


}

void StartPWM(){
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
}




