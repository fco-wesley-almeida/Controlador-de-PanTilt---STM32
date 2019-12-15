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


#  Função degrau
def u(t):
    if t > 0:
        return 1
    else:
        return 0
# Função que define o passo horizontal do caminhar do panTilt
def fHorizStep(t):
    result = 0
    for i in range (0,6):
        result += ((-1)**i) * u(t-7*i) + ((-1)**(i+1)) * u(t-6-7*i) 
    return 30 * result 

# Função que define o passo vertical do caminhar do panTilt
def fVertStep(t):
    if((t) % 7 == 0 and t>0 and t!=21):
        return (45) if (t<24) else (-45)
    else:
        return 0
    
# Caminhar automático do panTilt
def walk(_port, cicles):
    for i in range(cicles):
        horizontal_angle = 0
        vertical_angle = 0
        for t in range(42):
            try:
                time.sleep(0.2)
                horizontal_angle += fHorizStep(t)
                vertical_angle   += fVertStep(t)
                messageToBoard = "(" + str(horizontal_angle) + "," + str(vertical_angle) + ")\n"

                serial.write(messageToBoard.encode())
                answer = str(serial.readline())
                answer = answer[2: len(answer) - 3]
                print(answer)
            except:
                print ("Communication error in port " + _port + ".")
                return False
    return True

# Rotina principal
def program(_port):
    
    if (_port != ''):
        thisTitle = "SERIAL COMMUNICATION BY PORT " + _port 
        os.system("title " + thisTitle)
        # Rotina principal do programa
        while(True):
            os.system("cls")
            print ("********",thisTitle,"********\n")

            data = input("Enter the data to be sent by serial: ")
            
            if(data != 'out' and data != 'auto'):
                try:
                    serial.write((data+"\n").encode())
                    answer = serial.readline()
                    
                    print("Data sent:", data) 
                    print("Answer   :", answer)
                    input()

                except: # O programa entrará nessa exceção se a porta for desconectada, 
                        # ou seja, quando a conexão for interrompida.
                    print ("Communication error in port " + _port + ".")
                    break;
            
            elif(data == 'out'):
                serial.close()
                break

            elif(data == 'auto'):
                cicles = int(input("Enter how many drill cycles will be done: "))
                print("")
                if (walk(_port, cicles) == False):
                    serial.close()
                    break

    else: # Se não existir nenhuma porta ativa, então o programa é encerrado.
        print("No port found!")
    print("END!")

_port = activePort()
serial = serial.Serial(_port, __BAUD_RATE__)
try:
    program(_port)
except:
    print("END!")

    




