/*
 * pwm.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Ben
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_


void SetBaseFreq(int PWM_Freq_DCAC);

void SetBaseDutyC(float PWM_DutyC_DCAC);

void StartPWM();




#endif /* SRC_PWM_H_ */
