/*
 * ADC.c
 *
 *  Created on: May 29, 2024
 *      Author: Ben
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "ADC.h"
#include "main.h"

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;


//buffers
uint32_t raw_adc1_values_buffer [2];
uint32_t raw_adc2_values_buffer [2];
//actual measurements
float voltage_meas_adc1 = 0;
float current_meas_adc1 = 0;
float voltage_meas_adc2 = 0;
float current_meas_adc2 = 0;
float old_current_meas = 0;
//buffer readout values
uint16_t voltage_raw_adc1;
uint16_t current_raw_adc1;
uint16_t voltage_raw_adc2;
uint16_t current_raw_adc2;

void calibration(){
	 HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
}

void initializeADC(){
	 HAL_ADC_Start_DMA(&hadc1,(uint32_t*)raw_adc1_values_buffer,2);
	 HAL_ADC_Start_DMA(&hadc1,(uint32_t*)raw_adc2_values_buffer,2);
}

int voltageAndCurrentLimit(){
	if (current_meas_adc1 > 10){
		return 1;
	}
	if (voltage_meas_adc1>30){
		return 1;
	}

	return 0;

}

int currentMeasFunc(float current_meas){
int deltaI = current_meas-old_current_meas;
int precentage_deltaI = (int)current_meas % (int)old_current_meas;
	if (deltaI > 0 && precentage_deltaI > 2){
		return 1 ;
	}
		else{
			return 0;
		}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

HAL_ADC_Start_DMA(&hadc1,(uint32_t*)raw_adc1_values_buffer,2);
voltage_raw_adc1 = raw_adc1_values_buffer[0];
current_raw_adc1 = raw_adc1_values_buffer[1];
voltage_meas_adc1 = (((float)voltage_raw_adc1) / 4095.0f) *3.3f;
current_meas_adc1 = (((float)current_raw_adc1) / 4095.0f) *3.3f;

HAL_ADC_Start_DMA(&hadc2,(uint32_t*)raw_adc2_values_buffer,2);
voltage_raw_adc2 = raw_adc2_values_buffer[0];
current_raw_adc2 = raw_adc2_values_buffer[1];
voltage_meas_adc2 = (((float)voltage_raw_adc2) / 4095.0f) *3.3f;
current_meas_adc2 = (((float)current_raw_adc2) / 4095.0f) *3.3f;
}



