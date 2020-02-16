
#ifndef power_sensor_h
#define power_sensor_h

// *******************************************************
// ******** CONFIGURACION SENSORES DE POTENCIA    ********
// *******************************************************

// 97.6  66.5  51.3(39.0 ohm)  20.0 (100.0 ohm)

#define NUMBER_OF_PWR_SENSORS 6
#define TRANSFORMATOR_CONSTANT 20

#define MAME_PWR_1 "Pinza_1"
#define ENTER_1 A0
#define CURRENT_CONST_1 TRANSFORMATOR_CONSTANT

#define MAME_PWR_2 "Pinza_2"
#define ENTER_2 A1
#define CURRENT_CONST_2 TRANSFORMATOR_CONSTANT

#define MAME_PWR_3 "Pinza_3"
#define ENTER_3 A2
#define CURRENT_CONST_3 TRANSFORMATOR_CONSTANT

#define MAME_PWR_4 "Pinza_4"
#define ENTER_4 A3
#define CURRENT_CONST_4 TRANSFORMATOR_CONSTANT

#define MAME_PWR_5 "Pinza_7"
#define ENTER_5 A6
#define CURRENT_CONST_5 TRANSFORMATOR_CONSTANT

#define MAME_PWR_6 "Pinza_8"
#define ENTER_6 A7
#define CURRENT_CONST_6 TRANSFORMATOR_CONSTANT


// *******************************************************

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;

/*
// variable declaration
// long 2,147,483,647 (with sing)
// 240 x 100 x 60 seg x 5 medias
// 7,200,000
*/

typedef struct {                                                      // RFM RF payload datastructure
  String name_input;
  int pin_input;
  float constant_input;
  long power_sum;
  long power_measurements;
} analog_input;

analog_input p1={MAME_PWR_1,ENTER_1,CURRENT_CONST_1,0,0};
analog_input p2={MAME_PWR_2,ENTER_2,CURRENT_CONST_2,0,0};
analog_input p3={MAME_PWR_3,ENTER_3,CURRENT_CONST_3,0,0};
analog_input p4={MAME_PWR_4,ENTER_4,CURRENT_CONST_4,0,0};
analog_input p5={MAME_PWR_5,ENTER_5,CURRENT_CONST_5,0,0};
analog_input p6={MAME_PWR_6,ENTER_6,CURRENT_CONST_6,0,0};


analog_input analog_inputs[]={p1,p2,p3,p4,p5,p6};


// Function Prototypes
void calculate_power();
void buildPowerMessage(uint8_t);

void calculate_power(){
  // uint32_t millis_init =millis();
  for (uint8_t i=0; i < NUMBER_OF_PWR_SENSORS; i++){     
    emon1.current(analog_inputs[i].pin_input,analog_inputs[i].constant_input); // calibration.
    double Irms = emon1.calcIrms(141);  // Calculate Irms only, (119 20ms for algoritm )
    analog_inputs[i].power_sum+=(Irms*230.0); 
    analog_inputs[i].power_measurements++ ;    
  }
  // Serial.println(millis()-millis_init);
}

void buildPowerMessage(uint8_t output){
  uint32_t millis_init =millis();
  for (uint8_t i=0; i < NUMBER_OF_PWR_SENSORS; i++){ 
    long power_result = analog_inputs[i].power_sum/analog_inputs[i].power_measurements;     
    if (output==0){
      if(DEBUG){
        Serial.print ("name_input: ");          Serial.println(analog_inputs[i].name_input);       
        Serial.print ("power_sum: ");           Serial.println (analog_inputs[i].power_sum); 
        Serial.print ("power_measurements: ");  Serial.println (analog_inputs[i].power_measurements);
        Serial.println (analog_inputs[i].name_input+ ":" + power_result); 
        Serial.flush();     
      }
      wifiSerialInit.println (analog_inputs[i].name_input+ ":" + power_result);
      analog_inputs[i].power_sum = 0;
      analog_inputs[i].power_measurements = 0;
      
    }
  }
  if(DEBUG){Serial.println(millis()-millis_init);}
}

#endif
