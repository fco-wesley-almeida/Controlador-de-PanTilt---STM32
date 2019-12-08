/* 
   Neste arquivo são definidas as estruturas e funções responsáveis por
   implementar a leitura de dados da serial.
*/

#ifndef __SERIALHEADING_H__
#define __SERIALHEADING_H__

#define angleIsValid(a)     (a >= 0 && a <= 180)
#define isNum(c)            (c >= '0' && c <= '9') //   O código ASCII de '0' é 48 e de '9' 57. Por isso,
                                                 //   se o ASCII de um char estiver entre 48 e 57, 
                                                 //   certamente este será um número.*/
#define allCharsAreNum(str) (isNum(str[0]) && isNum(str[1]) && isNum(str[2]))

// Estruturas 
struct SerialCommand {
  char* str_reading;
  int arguments[2];
  int protocol;
};

typedef struct SerialCommand SerialCommand;

// Protótipos de funções
int argumentsAreValid(int protocol, int argument_1, int argument_2);
int defineProtocolAndArguments (SerialCommand* serialCommand, int bytesToRead);
int readSerial (SerialCommand* serialCommand, int bytesToRead);

// Esta função verifica se, caso o protocolo seja para mudança de ângulos, estes estejam entre 0 e 180.
int argumentsAreValid(int protocol, int arg_1, int arg_2, char* str_arg_1, char* str_arg_2)
{
  if(protocol == 1)
    return (angleIsValid(arg_1) && angleIsValid(arg_2) && allCharsAreNum(str_arg_1) && allCharsAreNum(str_arg_2));
  else
    return 0;
}

// Esta função devolve os parâmetros do comando
int defineProtocolAndArguments (SerialCommand* serialCommand, int bytesToRead)
{ 
    if  (bytesToRead == 9 || bytesToRead == 8) 
    {
      /*O comando deve ser do tipo "01234567", no qual o primeiro número é o 
      protocolo de envio, que especifica qual deve ser a finalidade do comando
      (se é especificação de ângulo dos motores servo, por exemplo). O segundo
      e o terceiro número são os dados enviados.*/

      char str_protocol[2], str_argument_1[3], str_argument_2[3];
      int protocol, argument_1, argument_2;
      
      str_protocol   [0] = serialCommand -> str_reading[0];
      str_protocol   [1] = serialCommand -> str_reading[1];
      str_argument_1 [0] = serialCommand -> str_reading[2]; 
      str_argument_1 [1] = serialCommand -> str_reading[3]; 
      str_argument_1 [2] = serialCommand -> str_reading[4];                                                            
      str_argument_2 [0] = serialCommand -> str_reading[5]; 
      str_argument_2 [1] = serialCommand -> str_reading[6]; 
      str_argument_2 [2] = serialCommand -> str_reading[7];

      protocol   = atoi(str_protocol);      
      argument_1 = atoi(str_argument_1);
      argument_2 = atoi(str_argument_2);

      if (!protocol)
        return 0;
      
      if (argumentsAreValid(protocol, argument_1, argument_2, str_argument_1, str_argument_2))
      {
        serialCommand -> protocol  = protocol;
        serialCommand -> arguments[0] = argument_1;
        serialCommand -> arguments[1] = argument_2;

        // Serial.print("-> Protocolo  : ");
        // Serial.println(serialCommand -> protocol);
        // Serial.print("-> Argumento 1: ");
        // Serial.println(serialCommand -> arguments[0]);
        // Serial.print("-> Argumento 2: ");
        // Serial.println(serialCommand -> arguments[1]);
        // Serial.println("");
        return 1; 
      } 
    }
    return 0;
}  

// Esta função lê os bytes da serial, se estiver algum disponível para leitura, e retorna 1, se os parâmetros forem válidos, e 0, caso contrário.
int readSerial (SerialCommand* serialCommand, int bytesToRead)
{
  if (bytesToRead)
  {
    char* str_reading = (char*) malloc (sizeof(bytesToRead));
    int parametersAreValid;

    // free(serialCommand -> str_reading);
    serialCommand -> str_reading = str_reading;
    // Serial.print("COMANDO: \n-> ");
    for(int i = 0; i < bytesToRead; i++)
    {
      serialCommand -> str_reading[i] = Serial.read();
      // Serial.print(serialCommand -> str_reading[i]);
    }
      
    parametersAreValid = defineProtocolAndArguments(serialCommand, bytesToRead);
    Serial.println(parametersAreValid);
    // if (!parametersAreValid)
    //   Serial.println("Comando não reconhecido.\n");
    // else
    //   Serial.println("Sucesso!");
    
    return parametersAreValid;
  }
  else
    return 0;
}

#endif