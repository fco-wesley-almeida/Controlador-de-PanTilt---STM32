
/*
    Esse projeto implementa o uso do aparelho PanTilt, que é conectado a dois servos motores u-
    sados para realizar seus movimentos, no processador STM32F103C8.
*/

#include <Arduino.h>
#include <Servo.h>  
#include "serialReading.h"
#include "serialSending.h"
#include <string.h>

#define HORIZONTAL                0
#define HORIZONTAL_SERVO_PIN      PB8
#define HORIZONTAL_INITIAL_ANGLE  90
#define VERTICAL                  1
#define VERTICAL_SERVO_PIN        PB9
#define VERTICAL_INITIAL_ANGLE    0
#define BAUD_RATE                 9600
#define TIME_FOR_ANSWER           900

// Estruturas
struct PanTilt
{
  Servo servos[2];
  int angles[2];
};
typedef struct PanTilt PanTilt;

// Declaração de variáveis
static SerialCommand* serialCommand = (SerialCommand*) malloc(sizeof(SerialCommand));
static PanTilt panTilt;
static int bytesToRead, commandIsValid;

// Rotina principal
void setup ()
{
  Serial.begin(BAUD_RATE);

  panTilt.angles[HORIZONTAL] = HORIZONTAL_INITIAL_ANGLE;
  panTilt.servos[HORIZONTAL].attach(HORIZONTAL_SERVO_PIN);
  panTilt.servos[HORIZONTAL].write(panTilt.angles[HORIZONTAL]);

  panTilt.angles[VERTICAL] = VERTICAL_INITIAL_ANGLE;
  panTilt.servos[VERTICAL].attach(VERTICAL_SERVO_PIN);
  panTilt.servos[VERTICAL].write(panTilt.angles[VERTICAL]);
}

void loop()
{
  serialCommand -> isBeingProcessed = 0;
  bytesToRead = Serial.available();
  commandIsValid = readSerial(serialCommand, bytesToRead); 

  if(commandIsValid)
  {
    panTilt.angles[HORIZONTAL] = serialCommand -> arguments[0];
    panTilt.angles[VERTICAL]   = serialCommand -> arguments[1];
  } 
  
  panTilt.servos[HORIZONTAL].write(panTilt.angles[HORIZONTAL]);
  panTilt.servos[VERTICAL].write(panTilt.angles[VERTICAL]);

  if (serialCommand -> isBeingProcessed)
  {
    delay(TIME_FOR_ANSWER);
    answerToUser(serialCommand);
  }
  else
    delay(5);
}
