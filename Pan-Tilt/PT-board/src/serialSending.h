/* 
   Neste arquivo é definida a função responsável por implementar a escrita de dados da serial.
*/

#ifndef __SERIALSENDING_H__
#define __SERIALSENDING_H__

// Declaração de variáveis
static char errorsList[3][30] = {"", "FORMATTING_ERROR.\n", "INVALID_ANGLE_ERROR.\n"};

// Protótipos de funções
void answerToUser(SerialCommand* serialCommand);

// Rotina de resposta ao usuário
void answerToUser(SerialCommand* serialCommand)
{
  char answer[100];
  if (serialCommand -> error)
  {
    strcpy(answer, "Command NOT recognized successfully: ");
    strcat(answer, errorsList[serialCommand -> error]);
  }
  else
  {
    char arg0[3], arg1[3];
    itoa(serialCommand->arguments[0], arg0, 10);
    itoa(serialCommand->arguments[1], arg1, 10);

    strcpy(answer, "Command recognized successfully! Current position: ");
    strcat(answer, "(");
    strcat(answer, arg0);
    strcat(answer, ",");
    strcat(answer, arg1);    
    strcat(answer, ")\n");
  }
  Serial.print(answer);
}

#endif