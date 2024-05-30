/*
 * ADC.h
 *
 *  Created on: May 29, 2024
 *      Author: Ben
 */

#ifndef SRC_ADC_H_
#define SRC_ADC_H_

#define V_AnalogPin1_ADC1_IN1_Pin GPIO_PIN_0
#define V_AnalogPin1_ADC1_IN1_GPIO_Port GPIOA
#define I_AnalogPin2_ADC1_IN2_Pin GPIO_PIN_1
#define I_AnalogPin2_ADC1_IN2_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define V_AnalogPin3_ADC2_IN1_Pin GPIO_PIN_4
#define V_AnalogPin3_ADC2_IN1_GPIO_Port GPIOA
#define I_Analogpin4_ADC2_IN2_Pin GPIO_PIN_5
#define I_Analogpin4_ADC2_IN2_GPIO_Port GPIOA

void initializeADC();
void HAL_ADC_ConvCpltCallback();
void calibration();

#endif /* SRC_ADC_H_ */
