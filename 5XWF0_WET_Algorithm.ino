#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arduino.h"
//constants
  //timer
unsigned long frequency_of_timers = 1000;
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
const int pin_dcdc_vi_measurement = 4; // pin for inpu
  //values
const int freq = 40000;  //freq
const int resolution = 8;
const int channelDCtoDC = 0;
const int channelDCtoAC = 1;
const int channelDCtoAC2 = 1;
const int measurement_resistor = 10000;
//variables
  //DCTODC
int DCtoAC_pwm_pin_state = LOW;
int DCtoAC_pwm_pin_complement_state = HIGH;
int dutycycle_DC_to_DC = 50;
float dutycycle_DC_to_DC_prediction = 0;
float v2 = 0;
float vo = 20;
float i2 = 0;
float p1 = 0;
float v1 = 0; 
float i1 = 0;
float vd = 0;
  //DCTOAC
int dutycycle_DC_to_AC=0;
float v_ac = 0;
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

  // one time start of prediction
  dutycycle_DC_to_DC_prediction = DC_to_DC_converter_predicted_output(dutycycle_DC_to_DC_prediction);
  dutycycle_DC_to_DC= dutycycle_DC_to_DC_prediction;
}

void measure() {
 
  //DC to DC measurements
  vd = analogRead(pin_dcdc_vi_measurement);
  v2 = analogRead(pin_dcdc_vo_measurement); 
    // calculate i2 using v2
    i2 = v2*measurement_resistor; 

  // DC to AC measurments
  v_ac = analogRead(pin_dcac_vo_measurement);
}

int DC_to_DC_converter_predicted_output(int d){
  d =  (20)/(vd-20); //predict duty cycle required to get 20 volts using equation
  return d;
}

int MPPT(int d) {
  float p2 = v2 * i2;
  if (p2 - p1 == 0) 
  {
    d=d+0.1;
  } 
  else if (p2 - p1 > 0) 
    {
      if (v2 - v1 > 0) {
        d=d+0.1;
      } 
      else {
        d=d-0.1;
      }
    } 
    else if (v2 - v1 > 0) {
      d=d-0.1;
    }
    else {
    d=d+0.1;
    } 
    v1 = v2;
    i1 = i2;
    return d;
}

void DCtoAC_PWM(int freq, int dutycycle){
  unsigned long frequency_of_timer_1 = freq;
  time_in_miliseconds = (1/(frequency_of_timer_1))*1000+dead_time_timer_1_milisec;
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
  time_in_miliseconds = (1/(frequency_of_timer_2))*1000-dead_time_timer_2_milisec; // converts frequency to the time period in miliseconds
 if (millis() - current_time_timer_2 > (time_in_miliseconds*dutycycle)) //check if time interval has passed where time interval is time period * dutycycle 
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
  //measurement function
  measure();

  //calculation functions
    //predict dutycycle using buck-boost equation
    dutycycle_DC_to_DC_prediction = DC_to_DC_converter_predicted_output(dutycycle_DC_to_DC_prediction);
    //change duty cycle using MPPT algorithm
    dutycycle_DC_to_DC = MPPT(dutycycle_DC_to_DC); 
    //variable to check difference of duty cycle output of equation vs MPPT --> no actual effect
    int diff_D = abs((dutycycle_DC_to_DC_prediction / dutycycle_DC_to_DC)*100); 

  //pwm functions
  pwmDCtoDC(dutycycle_DC_to_DC);
  DCtoAC_PWM(freq, dutycycle_DC_to_AC);
  DCtoAC_PWM_Complement(freq, dutycycle_DC_to_AC);
}
