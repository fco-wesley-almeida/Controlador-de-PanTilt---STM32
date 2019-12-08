/* 
   Neste arquivo são definidas as estruturas e funções responsáveis por
   implementar a leitura de dados da serial.
*/

#ifndef __SERIALHEADING_H__
#define __SERIALHEADING_H__

// Definições de macro
#define angleIsValid(a)                    (a >= 0 && a <= 180)
#define isNum(c)                           (c >= '0' && c <= '9')
#define cleanSerialBuffer(initialPosition) for(int l = initialPosition; l < bytesToRead; l++) Serial.read()

// Estruturas 
struct SerialCommand {
  int arguments[2];
  int wasProcessed;
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
int charIsValid(int i, int bytesToRead, int tempChar, int qComma)
{
  switch(tempChar)
  {
    case '(':
      return (i == 0);
      break;
    
    case ')':
      return (i == bytesToRead - 2);
      break;

    case ',':
      return ((qComma <= 1) && (i != 1) && (i != bytesToRead - 3));
      break;

    default:
      return (isNum(tempChar) && (i != 0) && (i != bytesToRead - 2));
      break;
  }
}

/* 
  Esta função lê os bytes da serial, se estiver algum disponível para leitura, e retorna
  1, se os parâmetros forem válidos, ou 0, caso contrário.
*/
int readSerial (SerialCommand* serialCommand, int bytesToRead)
{ 
  if (bytesToRead)
  { 
    serialCommand -> wasProcessed = 1;
    if(!(bytesToRead >= 6 && bytesToRead <= 10))
    {
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
        
        if (charIsValid(i, bytesToRead, tempChar, qComma))
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
        return (angleIsValid(serialCommand -> arguments[0]) && angleIsValid(serialCommand -> arguments[1]));
      }
    }
  }
  else
    return 0;
}
#endif