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
extern float voltage_meas_adc1; //Vdcdc in
extern float current_meas_adc1; //Idcdc in
extern float voltage_meas_adc2; //Vdcdc out
extern float current_meas_adc2; //Idcdc out
int8_t LEDstate = 0;
int8_t LOADstate = 0;
int32_t PWM_Freq_DCAC = 80000;
int32_t PWM_Freq_DCDC = 40000;
float PWM_DutyC_DCAC = 50;
float PWM_DutyC_DCDC = 50;
int32_t over_voltage_and_current_true = 0;

void sysinit(){
	calibration();
}

void setup(){
	SetBaseTime();
	SetBaseFreq(PWM_Freq_DCAC,PWM_Freq_DCDC);
	SetBaseDutyC(PWM_DutyC_DCAC, PWM_DutyC_DCDC);
    initializeADC();
}

void loop(){
	over_voltage_and_current_true = voltageAndCurrentLimit();
	LOADstate = currentMeasFunc(current_meas_adc2);
	if (LOADstate == 1){
		LEDstate = LEDon();
	}else if (LOADstate == 0){
		LEDstate = LEDoff();
	}
	if (over_voltage_and_current_true == 1){
		PWMoffSaftey();
		over_voltage_and_current_true = 0;
	}
	else if (over_voltage_and_current_true == 0){
		PWM_DutyC_DCDC = INC_MPPT_Algorithm(voltage_meas_adc1,current_meas_adc1,PWM_DutyC_DCDC);
		PWMdutyCcontroller(PWM_DutyC_DCDC);
		StartPWM();
	}

}
