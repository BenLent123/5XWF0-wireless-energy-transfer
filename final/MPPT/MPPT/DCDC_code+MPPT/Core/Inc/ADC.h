/*
 * ADC.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Ben
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

void calibration();

void initializeADC();

int voltageAndCurrentLimit();

void HAL_ADC_ConvCpltCallback();

#endif /* INC_ADC_H_ */
