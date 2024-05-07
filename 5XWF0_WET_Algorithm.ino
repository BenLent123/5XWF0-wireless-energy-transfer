#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arduino.h"
//constants
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
//DCTODC
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
  d =  (20)/(vd-20);
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


void pwmDCtoDC() {
  ledcWrite(channelDCtoDC, dutycycle_DC_to_DC);
}

void pwmDCtoAC(int freq, int dutycycle) {
  int t_tot = 1/freq;
  int t_on = t_tot*(1-dutycycle);
  int t_off = t_tot-t_on;
  // PWM for output 1
  ledcWrite(channelDCtoAC, dutycycle_DC_to_AC);
  //  low for output 2
  digitalWrite(pin_dcac_pwm_complement_output, LOW);
  //time on - deadtime
  delayMicroseconds(t_on-0.1*t_tot); 
  // high for output 2
  digitalWrite(pin_dcac_pwm_output, LOW);
  digitalWrite(pin_dcac_pwm_complement_output, HIGH);
  //time off - deadtime
  delayMicroseconds(t_off-0.1*t_tot);
  
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
  pwmDCtoDC();
  pwmDCtoAC(freq, dutycycle_DC_to_AC);


}
