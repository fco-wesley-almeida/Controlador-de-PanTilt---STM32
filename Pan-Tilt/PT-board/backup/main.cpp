#include <Arduino.h>
#include <Servo.h>
#include "serialReading.h"

#define HORIZONTAL 0
#define VERTICAL   1
#define INITIAL_ANGLE 90
#define BAUD_RATE 9600

// Estruturas
struct PanTilt
{
  Servo servos[2];
  int angles[2];
};
typedef struct PanTilt PanTilt;

//Rotina principal
void setup ()
{
  SerialCommand* serialCommand = (SerialCommand*) malloc(sizeof(SerialCommand));
  PanTilt panTilt;
  int direction, bytesToRead, sucessfullReading;

  Serial.begin(BAUD_RATE);

  for(direction = HORIZONTAL; direction <= VERTICAL; direction++)
  { 
    panTilt.angles[direction] = INITIAL_ANGLE;
    panTilt.servos[direction].attach(direction + PB8);
    panTilt.servos[direction].write(panTilt.angles[direction]);     
  }  

  while(1)
  {
      bytesToRead = Serial.available();
      sucessfullReading = readSerial(serialCommand, bytesToRead); 
      if(sucessfullReading)
      {
        panTilt.angles[HORIZONTAL] = serialCommand -> arguments[0];
        panTilt.angles[VERTICAL] = serialCommand -> arguments[1];
      } 
      panTilt.servos[HORIZONTAL].write(panTilt.angles[HORIZONTAL]);
      panTilt.servos[VERTICAL].write(panTilt.angles[VERTICAL]);
      delay(5);
  }
}