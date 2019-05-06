import serial
import RPi.GPIO as GPIO
from pyzbar import pyzbar
import numpy as np
import cv2
import time
from datetime import datetime
h = open('Historial.txt.','w')
h.write('Historial de carga:')
h.close()
contadore=0
arduino=serial.Serial('/dev/ttyACM0',9600)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(7,GPIO.OUT)#Pin 7, pila banano
GPIO.setup(8,GPIO.OUT)#Pin 8, pila cafe
GPIO.setup(9,GPIO.OUT)#Pin 9, pila dispositivos medicos
GPIO.setup(10,GPIO.OUT)#Pin 10 para activar interrupcion por hardware en microcontrolador

b=c=m=0
def Horario(data):
    h = open('Historial.txt','a')
    if data=='banano':
        d = str(datetime.now())
        #print ('Se cumple la condicion 1')
        h.write('\n'+'\n' + 'Se agarró una caja de banano a las: ')
        h.write(d)
        h.close()
    elif data=='cafe':
        d = str(datetime.now())
        #print ('Se cumple la condicion 2')
        h.write('\n'+'\n' + 'Se agarró una caja de café a las: ')
        h.write(d)
        h.close()
    elif data=='dispositivosmedicos':
        d = str(datetime.now())
        #print ('Se cumple la condicion 3')
        h.write('\n'+'\n' + 'Se agarró una caja de instrumentos médicos a las: ')
        h.write(d)
        h.close()
def Informe(b,c,m,contadore):
    b1=str(b)
    c1=str(c)
    m1=str(m)
    e1=str(contadore)
    i= open('Informe.txt','a')
    i.write('\n'+'\n' + 'Cajas de banano: ')
    i.write(b1)
    i.write('\n'+'\n' + 'Cajas de cafe: ')
    i.write(c1)
    i.write('\n'+'\n' + 'Cajas de dispositivos medicos: ')
    i.write('\n'+'\n' + 'Errores en operacion: ')
    i.write(e1)
    i.close()
        
    
#Inicializar la camara
capture = cv2.VideoCapture(0)
 
while 1:
    comando=arduino.read();
    if(comando=='tomafoto'):
        #time.sleep(10)
        #Capturar un frame
        val, frame = capture.read()
     
        if val:

           imagen_zbar= pyzbar.decode(frame)
     
        for codigo_qr in imagen_zbar:
                dat = codigo_qr.data
                if dat=='banano':
                    arduino.write('b')
                    b+=1
                elif dat=='cafe':
                    arduino.write('c')
                    c+=1
                elif dat=='dispositivosmedicos':
                    arduino.write('d')
                    m+=1
                #Horario(dat)
                #i = open('Informe.txt.','w')
                #i.write('Informe de carga:')
                #i.close()
                #Informe(b,c,m)
                #print(dat)
    elif comando=='ledb1':
        GPIO.output(7,True)
    elif comando=='ledb0':
        GPIO.output(7,False)
    elif comando=='ledc1':
        GPIO.output(8,True)
    elif comando=='ledc0':
        GPIO.output(8,False)
    elif comando=='ledd1':
        GPIO.output(9,True)
    elif comando=='ledd0':
        GPIO.output(0,False)
    if (dat=='banano'):
        distanciab=arduino.read();
    elif (dat=='cafe'):
        distanciac=arduino.read();
    elif (dat=='dispositivosmedicos'):
        distanciad=arduino.read();
    if (distanciab==32 and GPIO.input(7)):
        GPIO.output(10,True)
        contadore+=1
        d = str(datetime.now())
        h.write('\n'+'\n' + 'Se produjo un error por espacio lleno en pila de banano a las: ')
        h.write(d)
        h.close()
    else:
        Horario(dat)
        GPIO.output(10,False)
    if (distanciac==32 and GPIO.input(8)):
        GPIO.output(10,True)
        contadore+=1
        d = str(datetime.now())
        h.write('\n'+'\n' + 'Se produjo un error por espacio lleno en pila de cafe a las: ')
        h.write(d)
        h.close()
    else:
        Horario(dat)
        GPIO.output(10,False)
    if (distanciac==32 and GPIO.input(8)):
        GPIO.output(10,True)
        contadore+=1
        d = str(datetime.now())
        h.write('\n'+'\n' + 'Se produjo un error por espacio lleno en pila de dispositivos medicos a las: ')
        h.write(d)
        h.close()
    else:
        Horario(dat)
        GPIO.output(10,False)
        
    i = open('Informe.txt.','w')
    i.write('Informe de carga:')
    i.close()
    Informe(b,c,m,contadore)
    #print(dat)
        
        
        

     
        #Salir con 'ESC'
        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break
 
 
cv2.destroyAllWindows()
arduino.close()

