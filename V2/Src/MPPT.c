/*
 * MPPT.c
 *
 *  Created on: 29.05.2024
 *      Author: Ben
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MPPT.h"
#include "main.h"
//variables for MPPT
float current_old = 0;
float voltage_old = 0;
float INC_MPPT_Algorithm(float voltage_meas,float current_meas,float dutycycle){

	//Incremental Conductance MPPT algorithm can be found and explained online
	// checks deltas of output Voltage and current as well as incremental changes DI/DV
	float deltaI = current_meas - current_old;
	float deltaV = voltage_meas - voltage_old;


	if (deltaV == 0){
		if (deltaI == 0){
			return dutycycle;
		}

		if (deltaI>0){
			 dutycycle = dutycycle + 0.01;
			 return dutycycle;
		}
		dutycycle = dutycycle - 0.01;
		return dutycycle;
	}

	if ((current_meas + (deltaI)/(deltaV)*voltage_meas) == 0){
		return dutycycle;
	}

	if ((current_meas + (deltaI)/(deltaV)*voltage_meas) > 0){
		dutycycle = dutycycle-0.01;
		return dutycycle;
		}
	else {
		dutycycle = dutycycle+0.01;
		return dutycycle;
	}

	current_old = current_meas;
	voltage_old = voltage_meas;

}



