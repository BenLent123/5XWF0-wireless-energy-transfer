/*
 * ADC.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Ben
 */


/*
 * ADC.c
 *
 *  Created on: May 29, 2024
 *      Author: Ben
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ADC.h"
#include "main.h"


ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;


//buffers
uint32_t raw_adc1_values_buffer [2];
uint32_t raw_adc2_values_buffer [1];
//actual measurements
float voltage_meas_adc1 = 0;
float current_meas_adc1 = 0;
float old_current_meas = 0;
//buffer readout values
uint16_t voltage_raw_adc1;
uint16_t current_raw_adc1;
uint16_t voltage_raw_adc2;


void calibration(){
	 HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
}

void initializeADC(){
	 HAL_ADC_Start_DMA(&hadc1,(uint32_t*)raw_adc1_values_buffer,2);
}

int voltageAndCurrentLimit(){
	if (current_meas_adc1 > 10){
		return 1;
	}
	if (voltage_meas_adc1>60){
		return 1;
	}

	return 0;

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

HAL_ADC_Start_DMA(&hadc1,(uint32_t*)raw_adc1_values_buffer,2);
voltage_raw_adc1 = raw_adc1_values_buffer[0];
current_raw_adc1 = raw_adc1_values_buffer[1];
voltage_meas_adc1 = (((float)voltage_raw_adc1) / 4095.0f) *3.3f;
current_meas_adc1 = (((float)current_raw_adc1) / 4095.0f) *3.3f;

}



