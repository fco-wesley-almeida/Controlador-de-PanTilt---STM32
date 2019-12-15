/* 
   Neste arquivo são definidas as estruturas e funções responsáveis por
   implementar a leitura de dados da serial.
*/

#ifndef __SERIALHEADING_H__
#define __SERIALHEADING_H__

// Definições de macro e constantes
#define angleIsValid(a)                    (a >= 0 && a <= 180)
#define isNum(c)                           (c >= '0' && c <= '9')
#define cleanSerialBuffer(initialPosition) for(int l = initialPosition; l < bytesToRead; l++) Serial.read()

// Definições de constantes
#define NO_ERROR            0 
#define FORMATTING_ERROR   1
#define INVALID_ANGLE_ERROR 2


// Estruturas 
struct SerialCommand {
  int arguments[2];
  int isBeingProcessed;
  int error;
};
typedef struct SerialCommand SerialCommand;

// Protótipos das funções
int charIsValid(int i, int bytesToRead, int tempChar, int qComma);
int readSerial (SerialCommand* serialCommand, int bytesToRead);

/* 
   Essa função verifica se cada caracter lido da serial é válido de acordo com o protocolo
   estabelecido. A string lida da serial deve ser do tipo (a,b), ou seja, começando e ter-
   minando com parênteses e com uma única vírgula separando os argumentos numéricos a e b.
*/  
int charIsValid(SerialCommand* serialCommand, int i, int bytesToRead, int tempChar, int qComma)
{
  int isValid;
  switch(tempChar)
  {
    case '(':
      isValid = (i == 0);
      break;
    
    case ')':
      isValid = (i == bytesToRead - 2);
      break;

    case ',':
      isValid = ((qComma <= 1) && (i != 1) && (i != bytesToRead - 3));
      break;

    default:
      isValid = ((isNum(tempChar)) && (i != 0) && (i != bytesToRead - 2));
      break;
  }
  serialCommand -> error = isValid ? NO_ERROR : FORMATTING_ERROR;
  return (isValid);
}

/* 
  Esta função lê os bytes da serial, se estiver algum disponível para leitura, e retorna
  1, se os parâmetros forem válidos, ou 0, caso contrário.
*/
int readSerial (SerialCommand* serialCommand, int bytesToRead)
{ 
  if (bytesToRead)
  { 
    serialCommand -> isBeingProcessed = 1;
    if(!(bytesToRead >= 6 && bytesToRead <= 10))
    {
      serialCommand -> error = FORMATTING_ERROR;
      cleanSerialBuffer(0);
      return 0;
    }  
    else
    {
      int  qComma, readingArg_0, readingArg_1, i, j, k;
      char tempChar, arg_0[5], arg_1[5];

      qComma = 0;
      readingArg_0 = 0;
      readingArg_1 = 0;

      for(i = 0, j = 0, k = 0; i < bytesToRead - 1; i++)
      {
        tempChar = Serial.read();
        
        if (tempChar == ',')
          qComma++;
        
        if (charIsValid(serialCommand, i, bytesToRead, tempChar, qComma))
        {
          if (i == 1)
            readingArg_0 = 1;

          else if(tempChar == ',')
          {
            readingArg_0 = 0;
            readingArg_1 = 1;
            continue;
          }

          if(readingArg_0)
            arg_0[j++] = tempChar;

          else if(readingArg_1)
            arg_1[k++] = tempChar;
        }
        else
          break;
      }

      cleanSerialBuffer(i);
      
      if (i < bytesToRead - 1)
        return 0;
      else
      {
        serialCommand -> arguments[0] = atoi(arg_0);
        serialCommand -> arguments[1] = atoi(arg_1);
        if(angleIsValid(serialCommand -> arguments[0]) && angleIsValid(serialCommand -> arguments[1]))
          return 1;
        else
        {
          serialCommand -> error = INVALID_ANGLE_ERROR;
          return 0;
        }
      }
    }
  }
  else
    return 0;
}
#endif