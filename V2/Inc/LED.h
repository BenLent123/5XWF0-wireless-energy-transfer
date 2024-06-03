/*
 * LED.h
 *
 *  Created on: May 30, 2024
 *      Author: Ben
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#define LED_onboard_Pin GPIO_PIN_3
#define LED_onboard_GPIO_Port GPIOB

int LEDon();

int LEDoff();

#endif /* INC_LED_H_ */
