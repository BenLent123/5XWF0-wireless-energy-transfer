/*
 * PWM.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Ben
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

void SetBaseTime();

void SetBaseFreq(int PWM_Freq_DCDC);

void SetBaseDutyC(float PWM_DutyC_DCDC);

void PWMoffSaftey();

void StartPWM();

void PWMdutyCcontroller(float PWM_DutyC_DCDC);


#endif /* INC_PWM_H_ */
