# Essa aplicação tem a finalidade de testar o envio e recepção de dados via porta serial.

# !/usr/bin/python
# -*- coding: iso-8859-1 -*-

import serial
import os
import time
import math

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
    
# Verifica se existe alguma porta ativa
def walk(_port):
    if (_port != ''):
        while(True):
            horizontal_angle = 0
            vertical_angle = 0
            for t in range(42):
            
                time.sleep(0.2)
                horizontal_angle += fHorizStep(t)
                vertical_angle   += fVertStep(t)
                messageToBoard = "(" + str(horizontal_angle) + "," + str(vertical_angle) + ")\n"

                serial.write(messageToBoard.encode())
                answer = str(serial.readline())
                answer = answer[2: len(answer) - 3]
                print(answer)
    else: # Se não existir nenhuma porta ativa, então o programa é encerrado.
        print("No port found!")

_port = activePort()
serial = serial.Serial(_port, __BAUD_RATE__)
walk(_port)

    




