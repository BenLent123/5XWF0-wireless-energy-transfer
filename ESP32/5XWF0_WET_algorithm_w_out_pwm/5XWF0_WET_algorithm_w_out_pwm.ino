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


void setup(){

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

void loop() {
  
  //measurement function
    measure();

  //calculation functions
    dutycycle_DC_to_DC = Voltage_stability_DCDC_output(dutycycle_DC_to_DC);
    dutycycle_DC_to_AC = MPPT_AC(dutycycle_DC_to_AC);  

  //pwm functions
  
}