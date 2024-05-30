/*
 * maincontroller.c
 *
 *  Created on: 30.05.2024
 *      Author: Ben
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "maincontroller.h"
#include "PWM.h"
#include "ADC.h"
#include "MPPT.h"
#include "main.h"
#include "LED.h"

////ADC measurements
extern float voltage_meas_adc1;
extern float current_meas_adc1;
extern float voltage_meas_adc2;
extern float current_meas_adc2;
int32_t PWM_Freq_DCAC = 40000;
int32_t PWM_Freq_DCDC = 40000;
float PWM_DutyC_DCAC = 50;
float PWM_DutyC_DCDC = 50;

void setup(){
	calibration();
	SetBaseTime();
	setBaseFreq(PWM_Freq_DCAC,PWM_Freq_DCDC);
	setBaseDutyC(PWM_DutyC_DCAC, PWM_DutyC_DCDC);
}

void loop(){
	PWM_DutyC_DCDC = INC_MPPT_Algorithm(voltage_meas_adc2,current_meas_adc2,PWM_DutyC_DCDC);
	PWMdutyCcontroller(PWM_DutyC_DCDC);
}
