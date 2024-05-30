/*
 * LED.c
 *
 *  Created on: May 30, 2024
 *      Author: Ben
 */

#include "LED.h"
#include "main.h"

void LEDtoggle(){
	HAL_GPIO_TogglePin(LED_onboard_GPIO_Port, LED_Pin);
}

