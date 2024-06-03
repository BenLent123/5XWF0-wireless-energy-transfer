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
//float power_old = 0;
float INC_MPPT_Algorithm(float voltage_meas,float current_meas,float dutycycle){

	//Incremental Conductance MPPT algorithm can be found and explained online
	// checks deltas of output Voltage and current as well as incremental changes DI/DV
	float deltaI = current_meas - current_old;
	float deltaV = voltage_meas - voltage_old;


	if (deltaV == 0){
		if (deltaI == 0){

		}

		if (deltaI>0){
			 dutycycle = dutycycle + 0.01;

		}
		dutycycle = dutycycle - 0.01;

	}

	if ((current_meas + (deltaI)/(deltaV)*voltage_meas) == 0){

	}

	if ((current_meas + (deltaI)/(deltaV)*voltage_meas) > 0){
		dutycycle = dutycycle-0.01;

		}
	else {
		dutycycle = dutycycle+0.01;

	}

	return dutycycle;
	current_old = current_meas;
	voltage_old = voltage_meas;

}

//float PnO_MPPT(float voltage_meas,float current_meas,float dutycycle){
// float power = voltage_meas * current_meas;
// if (power-power_old == 0){
//
// }
// if (power-power_old > 0){
//	 if (voltage_meas-voltage_old > 0){
//		 dutycycle = dutycycle + 0.01;
//
//	 }
//	 dutycycle = dutycycle - 0.01;
//
// }
// if (voltage_meas - voltage_old > 0){
//	 dutycycle = dutycycle - 0.01;
//
// }
// dutycycle = dutycycle - 0.01;
// power_old = power;
// voltage_old = voltage_meas;
// current_old = current_meas;
// return dutycycle;
//}


