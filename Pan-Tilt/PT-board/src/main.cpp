
/*
    Esse projeto implementa o uso do aparelho PanTilt, que é conectado a dois servos motores u-
    sados para realizar seus movimentos, no processador STM32F103C8. Os servos devem ser conec-
    tados nos pinos especificados pela constante "FIRST_PIN". Se ela estiver definida como PB8,
    por exemplo, eles serão conectados nos pinos PB8 e PB9, sempre de forma consecutiva.
    
    Os servos responsáveis pelos movimentos horizontal e vertical do PanTilt serão determinados
    pelas constantes "HORIZONTAL" e "VERTICAL", assim como o ângulo inicial (sempre será o mesmo
    para ambos) será definido pela constante "INITIAL_ANGLE".

    A conexão serial será do tipo 9600, 8, N, 1 e o comando enviado ao processador está estabele-
    cido segundo o procolo "(a,b)", segundo o qual o comando inicia com parêntese, termina com 
    parêntese e \n, e seus argumentos são separados por uma única vírgula. Os argumentos "a" e 
    "b" serão os ângulos que os servos irão assumir. Segundo o que foi estabelecido pelas constan-
    tes, "a" será o ângulo do servo que fará os movimentos horizontais, e "b" o do que fará os ver-
    ticais. 
    
    Se o comando estiver segundo o protocolo, será enviado um "1\n" como resposta. Caso contário, 
    a resposta será um "0\n". Para ambos os casos, o tempo de envio a partirdo momento em que são 
    dadas as instruções aos servos para iniciarem o movimento será determiando pela constante 
    "TIME_FOR_ANSWER".
*/

#include <Arduino.h>
#include <Servo.h>  
#include "serialReading.h"

#define HORIZONTAL      0
#define VERTICAL        1
#define INITIAL_ANGLE   90
#define BAUD_RATE       9600
#define TIME_FOR_ANSWER 700
#define FIRST_PIN       PB8

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

//Rotina principal
void setup ()
{
  int direction;
  Serial.begin(BAUD_RATE);

  serialCommand -> wasProcessed = 0;
  
  for(direction = HORIZONTAL; direction <= VERTICAL; direction++)
  { 
    panTilt.angles[direction] = INITIAL_ANGLE;
    panTilt.servos[direction].attach(direction + FIRST_PIN); // Inicializa os pinos PB8 E PB9
    panTilt.servos[direction].write(panTilt.angles[direction]);     
  }
}


void loop()
{
  bytesToRead = Serial.available();
  commandIsValid = readSerial(serialCommand, bytesToRead); 

  if(commandIsValid)
  {
    panTilt.angles[HORIZONTAL] = serialCommand -> arguments[0];
    panTilt.angles[VERTICAL]   = serialCommand -> arguments[1];
  } 
  
  panTilt.servos[HORIZONTAL].write(panTilt.angles[HORIZONTAL]);
  panTilt.servos[VERTICAL].write(panTilt.angles[VERTICAL]);

  if (serialCommand -> wasProcessed)
  {
    delay(TIME_FOR_ANSWER);
    Serial.print(commandIsValid);
    Serial.print("\n");
    serialCommand -> wasProcessed = 0;
  }
  else
    delay(5);
}
