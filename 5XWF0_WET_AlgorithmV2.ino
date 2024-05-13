#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arduino.h"
//constants
  //timer
unsigned long dead_time_timer_1_milisec = 10; //dead time in miliseconds
unsigned long dead_time_timer_2_milisec = 15; //dead time in miliseconds 
  //pins
    //outputs
const int pin_dcdc_pwm_output = 1;    // pin for output
const int pin_dcac_pwm_output = 2;    // pin for output
const int pin_dcac_pwm_complement_output = 3;    // pin for output 
    //inputs
const int pin_dcdc_vo_measurement = 4; // pin for input
const int pin_dcac_vo_measurement = 5; // pin for input
const int pin_dcdc_vi_measurement = 4; // pin for input
  //values
const int freq_ac = 40000;
const int freq_dc = 40000;  //freq
const int resolution = 8;
const int channelDCtoDC = 0;
const int channelDCtoAC = 1;
const int channelDCtoAC2 = 1;
const int measurement_resistor_DCDC = 10000;
const int measurement_resistor_DCAC = 10000;
//variables
  //DCTODC
int dutycycle_DC_to_DC = 50;
float vo_dcdc_measurment = 0;
float vo_dcdc_constant = 20;
float i2 = 0;
float p1 = 0;
float v1 = 0; 
float i1 = 0;
float vd_dcdc_measurment = 0;
  //DCTOAC
int dutycycle_DC_to_AC=0.5;
float v_dcac_measurement = 0;
int DCtoAC_pwm_pin_state = LOW;
int DCtoAC_pwm_pin_complement_state = HIGH;
  //timers
unsigned long current_time_timer_1 = 0; 
unsigned long current_time_timer_2 = 0;   


void setup(){
  // set output mode
  pinMode(pin_dcac_pwm_complement_output, OUTPUT);
  pinMode(pin_dcdc_pwm_output, OUTPUT);
  pinMode(pin_dcac_pwm_output, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(channelDCtoDC, freq, resolution);
  ledcSetup(channelDCtoAC, freq, resolution);
  ledcSetup(channelDCtoAC2, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pin_dcdc_pwm_output, channelDCtoDC);
  ledcAttachPin(pin_dcac_pwm_output, channelDCtoAC);
  ledcAttachPin(pin_dcac_pwm_complement_output, channelDCtoAC2);
}

void measure() {
 
  //DC to DC measurements
  vd_dcdc_measurement = analogRead(pin_dcdc_vi_measurement);
  vo_dcdc_measurement = analogRead(pin_dcdc_vo_measurement); 
    // calculate i2 using v2
    i_dcdc_measurement = vo_dcdc_measurement*measurement_resistor_DCDC; 

  // DC to AC measurments
  v_acdc_measurement = analogRead(pin_dcac_vo_measurement);
    //
    i_dcac_measurement = v_dcac_measurment*measurement_resistor_DCAC;
}

int Voltage_stability(int d) {
  if (vo_dcdc_measurment>20)
  {
    d = d-0.1;
  }
  else if (vo_dcdc_measurment<20)
  {
    d = d+0.1;
  }
    return d;
}

int MPPT_AC(int d) {
  float v2 = v_dcac_measurment;
  float i2 = i_dcac_measurement; 
  float p2 = v2 * i2;
  if (p2 - p1 == 0) 
  {
    freq_ac=freq_ac+0.1;
  } 
  else if (p2 - p1 > 0) 
    {
      if (v2 - v1 > 0) {
        freq_ac=freq_ac+0.1;
      } 
      else {
        freq_ac=freq_ac-0.1;
      }
    } 
    else if (v2 - v1 > 0) {
      freq_ac=freq_ac-0.1;
    }
    else {
    freq_ac=freq_ac+0.1;
    } 
    v1 = v2;
    i1 = i2;
    return d;
}

void DCtoAC_PWM(int freq, int dutycycle){
  unsigned long frequency_of_timer_1 = freq;
  unsigned long time_in_miliseconds = (1/(frequency_of_timer_1))*1000+dead_time_timer_1_milisec;
 if (millis() - current_time_timer_1 > time_in_miliseconds*dutycycle) //check if time interval has passed 
  {
      current_time_timer_1 =  current_time_timer_1 + time_in_miliseconds;
      //change state
      if (DCtoAC_pwm_pin_state == LOW)
      { 
        DCtoAC_pwm_pin_state = HIGH;
      }
      else
      {
      DCtoAC_pwm_pin_state = LOW;
      }
      // write to the output pin of the DCtoAC pin
      digitalWrite(pin_dcac_pwm_output, DCtoAC_pwm_pin_state);
  }
}

void DCtoAC_PWM_Complement(int freq, int dutycycle){
  unsigned long frequency_of_timer_2 = freq;
  unsigned long time_in_miliseconds = (1/(frequency_of_timer_2))*1000-dead_time_timer_2_milisec; // converts frequency to the time period in miliseconds
 if (millis() - current_time_timer_2 > (time_in_miliseconds*(1-dutycycle))) //check if time interval has passed where time interval is time period * dutycycle 
  {
      current_time_timer_2 =  current_time_timer_2 + time_in_miliseconds;
      //change state --> state machine 
      if (DCtoAC_pwm_pin_complement_state == LOW)
      { 
        DCtoAC_pwm_pin_complement_state = HIGH;
      }
      else
      {
      DCtoAC_pwm_pin_complement_state = LOW;
      }
      // write to the output pin of the complement DCtoAC pin
      digitalWrite(pin_dcac_pwm_complement_output, DCtoAC_pwm_pin_complement_state);
  }
}

void pwmDCtoDC(int dutycycle) {
  ledcWrite(channelDCtoDC, dutycycle_DC_to_DC);
}

void loop() {
  /*
  //measurement function
  measure();

  //calculation functions
    dutycycle_DC_to_DC = Voltage_stability(dutycycle_DC_to_DC);
     dutycycle_DC_to_AC = MPPT_DC(dutycycle_DC_to_AC);  

  //pwm functions
  pwmDCtoDC(dutycycle_DC_to_DC);
  DCtoAC_PWM(freq, dutycycle_DC_to_AC);
  DCtoAC_PWM_Complement(freq, dutycycle_DC_to_AC);
  */
}