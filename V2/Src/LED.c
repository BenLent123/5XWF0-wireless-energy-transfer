/*
 * LED.c
 *
 *  Created on: May 30, 2024
 *      Author: Ben
 */

#include "LED.h"
#include "main.h"

void LEDon(){
if (HAL_GPIO_ReadPin(LED_onboard_GPIO_Port, LED_onboard_Pin) == 0){
	HAL_GPIO_TogglePin(LED_onboard_GPIO_Port, LED_onboard_Pin);
	}
}

void LEDoff(){
if (HAL_GPIO_ReadPin(LED_onboard_GPIO_Port, LED_onboard_Pin) == 1){
	HAL_GPIO_TogglePin(LED_onboard_GPIO_Port, LED_onboard_Pin);
	}
}
