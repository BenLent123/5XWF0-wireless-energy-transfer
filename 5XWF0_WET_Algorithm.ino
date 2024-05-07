#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//constants
const int pin1 = 1;    // pin for output
const int pin2 = 2;    // pin for output
const int pin3 = 3;    // pin for output 
const int pin_supply = 4;
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
  // configure LED PWM functionalitites
  ledcSetup(channelDCtoDC, freq, resolution);
  ledcSetup(channelDCtoAC, freq, resolution);
  ledcSetup(channelDCtoAC2, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pin1, channelDCtoDC);
  ledcAttachPin(pin2, channelDCtoAC);
  ledcAttachPin(pin3, channelDCtoAC2);

  // one time start of prediction
  dutycycle_DC_to_DC_prediction = DC_to_DC_converter_predicted_output(dutycycle_DC_to_DC_prediction);
  dutycycle_DC_to_DC= dutycycle_DC_to_DC_prediction;

}

void measure() {
  //DC to DC measurements
  vd = analogRead(pin_supply);
  v2 = analogRead(pin1); 
  // calculate i2 using v2
  i2 = v2*measurement_resistor; 
  // DC to AC measurments
  v_ac = analogRead(pin2);

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

void pwmDCtoAC() {
  ledcWrite(channelDCtoAC, dutycycle_DC_to_AC);
  ledcWrite(channelDCtoAC2, 1-dutycycle_DC_to_AC);
}

void loop() {
  //measurement function
  measure();

  //calculation function
  dutycycle_DC_to_DC_prediction = DC_to_DC_converter_predicted_output(dutycycle_DC_to_DC_prediction);
  dutycycle_DC_to_DC = MPPT(dutycycle_DC_to_DC); //--> maybe input prediction
  

  int diff_D = abs((dutycycle_DC_to_DC_prediction / dutycycle_DC_to_DC)*100); // CHECK VARIABLE

  //pwm functions
  pwmDCtoDC();
  pwmDCtoAC();


}
