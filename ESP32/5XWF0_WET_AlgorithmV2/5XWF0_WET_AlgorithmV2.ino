#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arduino.h"
//constants
const int freq_ac = 40000;
const int freq_dc = 40000;  
const int resolution = 8;
const int channelDCtoDC = 0;
const float vo_dcdc_constant = 20;
  //pins
    //outputs
const int pin_dcdc_pwm_output = 1;    // pin for output
const int pin_dcac_pwm_output = 2;    // pin for output
const int pin_dcac_pwm_complement_output = 3;    // pin for output 
    //inputs
const int pin_dcdc_io_measurement = 4; // output current of DCDC
const int pin_dcac_io_measurement = 5; // output current of DCAC
const int pin_dcdc_vo_measurement = 6; // output voltage of DCDC
const int pin_dcac_vo_measurement = 7; // input voltage of DCDC
const int pin_dcdc_vi_measurement = 8; // output voltage of DCDC

//variables
  //Dutycycles
int dutycycle_DC_to_DC = 0.5;
int dutycycle_DC_to_AC = 0.5;
  // measurement variables
float vd_dcdc_measurement = 0;
float io_dcdc_measurement = 0;
float vo_dcdc_measurement = 0;
float vo_dcac_measurement = 0;
float io_dcac_measurement = 0;
  //mppt variables
float v1 = 0;
float i1 = 0;
float v2 = vo_dcac_measurement;
float i2 = io_dcac_measurement;
float p1 = v1*i1;
float p2 = vo_dcac_measurement*io_dcac_measurement;


//timer constants and variables 
unsigned long dead_time_timer_1_milisec = 10; //dead time in miliseconds
unsigned long dead_time_timer_2_milisec = 15; //dead time in miliseconds 
int DCtoAC_pwm_pin_state = LOW;
int DCtoAC_pwm_pin_complement_state = HIGH;
unsigned long current_time_timer_1 = 0; 
unsigned long current_time_timer_2 = 0; 

void setup(){
  // set output mode
  pinMode(pin_dcac_pwm_complement_output, OUTPUT);
  pinMode(pin_dcdc_pwm_output, OUTPUT);
  pinMode(pin_dcac_pwm_output, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(channelDCtoDC, freq_dc, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pin_dcdc_pwm_output, channelDCtoDC);
}

void measure() {
 
  //DC to DC measurements
  vd_dcdc_measurement = analogRead(pin_dcdc_vi_measurement);
  vo_dcdc_measurement = analogRead(pin_dcdc_vo_measurement);
  io_dcdc_measurement = analogRead(pin_dcdc_io_measurement);

  // DC to AC measurements
  vo_dcac_measurement = analogRead(pin_dcac_vo_measurement);
  io_dcac_measurement = analogRead(pin_dcac_io_measurement);
    
}

int Voltage_stability_DCDC_output(int dutycycle) {
  if (vo_dcdc_measurement>20)
  {
    dutycycle = dutycycle-0.1;
  }
  else if (vo_dcdc_measurement<20)
  {
    dutycycle = dutycycle+0.1;
  }
    return dutycycle;
}

int MPPT_AC(int dutycycle) {
  if (p2 - p1 == 0) 
  {
    dutycycle=dutycycle+0.1;
  } 
  else if (p2 - p1 > 0) 
    {
      if (v2 - v1 > 0) {
        dutycycle=dutycycle+0.1;
      } 
      else {
        dutycycle=dutycycle-0.1;
      }
    } 
    else if (v2 - v1 > 0) {
      dutycycle=dutycycle-0.1;
    }
    else {
    dutycycle=dutycycle+0.1;
    } 
    v1 = v2;
    i1 = i2;
    return dutycycle;
}


void DCtoDC_PWM(int dutycycle) {
  ledcWrite(channelDCtoDC, dutycycle_DC_to_DC);
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

void loop() {
  
  //measurement function
    measure();

  //calculation functions
    dutycycle_DC_to_DC = Voltage_stability_DCDC_output(dutycycle_DC_to_DC);
    dutycycle_DC_to_AC = MPPT_AC(dutycycle_DC_to_AC);  

  //pwm functions
    DCtoDC_PWM(dutycycle_DC_to_DC);
    DCtoAC_PWM_Complement(freq_ac, dutycycle_DC_to_AC);
    DCtoAC_PWM(freq_ac, dutycycle_DC_to_AC);
  
}