/*
 * PWM.h
 *
 *  Created on: 29.05.2024
 *      Author: Ben
 */

#ifndef PWM_H_
#define PWM_H_
#define PWM_DCDC_TIM3CH1_Pin GPIO_PIN_6
#define PWM_DCDC_TIM3CH1_GPIO_Port GPIOA
#define PWM_DCAC_TIM1CH1N_Pin GPIO_PIN_7
#define PWM_DCAC_TIM1CH1N_GPIO_Port GPIOA
#define PWM_DCAC_TIM1CH1_Pin GPIO_PIN_8
#define PWM_DCAC_TIM1CH1_GPIO_Port GPIOA


void SetBaseTime();

void setBaseFreq(int PWM_Freq_DCAC, int PWM_Freq_DCDC);

void setBaseDutyC(float PWM_DutyC_DCAC, float PWM_DutyC_DCDC);

void freqtuning();

void PWMdutyCcontroller(float PWM_DutyC_DCDC);


#endif /* PWM_H_ */
