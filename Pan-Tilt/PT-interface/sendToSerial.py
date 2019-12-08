# Essa aplicação tem a finalidade de testar o envio e recepção de dados via porta serial.

# !/usr/bin/python
# -*- coding: iso-8859-1 -*-

import serial
import os
import time

# Definição de constante
__BAUD_RATE__ = 9600

# Função que retorna a primeira porta ativa para comunicação de dados
def activePort():
    for p in range (0, 33):
        thisPort = 'COM' + str(p)
        try:
            port = serial.Serial(thisPort, __BAUD_RATE__)
            port.close
            return thisPort
        except:
            continue
    return ''
_port = activePort()

# Verifica se existe alguma porta ativa
if (_port != ''):
    # Configurações iniciais
    serial = serial.Serial(_port, __BAUD_RATE__)
    thisTitle = "SERIAL COMMUNICATION BY PORT " + _port 
    os.system("title " + thisTitle)

    # Rotina principal do programa
    while(True):
        os.system("cls")
        print ("********",thisTitle,"********\n")

        data = input("Enter the data to be sent by serial: ")
        
        if(data != 'out'):
            try:
                serial.write((data+"\n").encode())
                answer = serial.readline()
                
                print("Data sent:", data) 
                print("Answer   :", answer)

                if  (answer == b'1\n'):
                    print("Command recognized successfully!\n")
                elif(answer == b'0\n'):
                    print("Command NOT recognized successfully!\n")

            except: # O programa entrará nessa exceção se a porta for desconectada, 
                    # ou seja, quando a conexão for interrompida.
                print ("Communication error in port " + _port + "!")
                break;

        else:       # O programa entrará aqui quando o usuário digitar "out" no cam-
                    # po de entrada dos dados.  
            serial.close()
            print("END!")
            break
        input()     # Dá uma pausa quando um ciclo de envio e retorno de dados for concluído.

else: # Se não existir nenhuma porta ativa, então o programa é encerrado.
    print("No port found!")



    




