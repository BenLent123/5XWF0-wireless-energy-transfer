#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arduino.h"
//constants
//timer
unsigned long dead_time_timer_1_milisec = 10;  //dead time in miliseconds
unsigned long dead_time_timer_2_milisec = 15;  //dead time in miliseconds
  //pins
  //outputs
const int pin_dcac_pwm_output = 17;             // pin for output
const int pin_dcac_pwm_complement_output = 18;  // pin for output
const int freq_ac = 20000;
//variables
//DCTOAC
int dutycycle_DC_to_AC = 0.5;
int DCtoAC_pwm_pin_state = LOW;
int DCtoAC_pwm_pin_complement_state = HIGH;
//timers
unsigned long current_time_timer_1 = 0;
unsigned long current_time_timer_2 = 0;


void setup() {
  // set output mode
  pinMode(pin_dcac_pwm_complement_output, OUTPUT);
  pinMode(pin_dcac_pwm_output, OUTPUT);
}

void DCtoAC_PWM(int freq, int dutycycle) {
  unsigned long frequency_of_timer_1 = freq;
  unsigned long time_in_miliseconds = (1 / (frequency_of_timer_1)) * 1000 + dead_time_timer_1_milisec;
  if (millis() - current_time_timer_1 > time_in_miliseconds * dutycycle)  //check if time interval has passed
  {
    current_time_timer_1 = current_time_timer_1 + time_in_miliseconds;
    //change state
    if (DCtoAC_pwm_pin_state == LOW) {
      DCtoAC_pwm_pin_state = HIGH;
    } else {
      DCtoAC_pwm_pin_state = LOW;
    }
    // write to the output pin of the DCtoAC pin
    digitalWrite(pin_dcac_pwm_output, DCtoAC_pwm_pin_state);
  }
}

void DCtoAC_PWM_Complement(int freq, int dutycycle) {
  unsigned long frequency_of_timer_2 = freq;
  unsigned long time_in_miliseconds = (1 / (frequency_of_timer_2)) * 1000 - dead_time_timer_2_milisec;  // converts frequency to the time period in miliseconds
  if (millis() - current_time_timer_2 > (time_in_miliseconds * (1 - dutycycle)))                        //check if time interval has passed where time interval is time period * dutycycle
  {
    current_time_timer_2 = current_time_timer_2 + time_in_miliseconds;
    //change state --> state machine
    if (DCtoAC_pwm_pin_complement_state == LOW) {
      DCtoAC_pwm_pin_complement_state = HIGH;
    } else {
      DCtoAC_pwm_pin_complement_state = LOW;
    }
    // write to the output pin of the complement DCtoAC pin
    digitalWrite(pin_dcac_pwm_complement_output, DCtoAC_pwm_pin_complement_state);
  }
}


void loop() {
  DCtoAC_PWM(freq_ac, dutycycle_DC_to_AC);
  DCtoAC_PWM_Complement(freq_ac, dutycycle_DC_to_AC);
}