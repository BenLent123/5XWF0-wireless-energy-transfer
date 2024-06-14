/*
 * maincontroller.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Ben
 */

#include "PWM.h"
#include "main.h"
int32_t PWM_Freq_DCAC = 80000;
float PWM_DutyC_DCAC = 50;


void setup(){
SetBaseFreq(PWM_Freq_DCAC);
SetBaseDutyC(PWM_DutyC_DCAC);
StartPWM();
}


