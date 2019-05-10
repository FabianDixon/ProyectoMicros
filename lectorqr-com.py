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
#GPIO.setup(7,GPIO.OUT)#Pin 7, pila banano
#GPIO.setup(8,GPIO.OUT)#Pin 8, pila cafe
#GPIO.setup(9,GPIO.OUT)#Pin 9, pila dispositivos medicos
GPIO.setup(12,GPIO.OUT)#Pin 12 para activar interrupcion por hardware en microcontrolador

b=c=m=0
def Horario(data):
    h = open('Historial.txt','a')
    if data=='banano':
        d1 = str(datetime.now())
        #print ('Se cumple la condicion 1')
        h.write('\n'+'\n' + 'Se agarr� una caja de banano a las: ')
        h.write(d1)
        h.close()
    elif data=='cafe':
        d1 = str(datetime.now())
        #print ('Se cumple la condicion 2')
        h.write('\n'+'\n' + 'Se agarr� una caja de caf� a las: ')
        h.write(d1)
        h.close()
    elif data=='dispositivosmedicos':
        d1 = str(datetime.now())
        #print ('Se cumple la condicion 3')
        h.write('\n'+'\n' + 'Se agarr� una caja de instrumentos m�dicos a las: ')
        h.write(d1)
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
        
## Funci�n b�sica para calcular distancia entre 2 puntos. 
## Se utiliza para calcular el ancho del Qr percibido por la c�mara. 
def distancia(x2,x1,y2,y1):
    a = (x2-x1)**2
    b2 = (y2-y1)**2
    c2 = a+b2
    d = math.sqrt(c)
    return d

## Funci�n para calcular la distancia focal o "focal length".
## Se hace el c�lculo con valores conocidos del ancho de la marca y 
## la distancia a la que se encuentra de la c�mara en ese instante.
## Este valor obtenido se utiliza para el c�lculo de las dem�s distancias.
def distanciaFocal(p):
    w = 45
    d2 = 20
    a = p*d2
    f = a/w
    return f

def calibracionCamara(captura):
    valIni, frameIni = captura.read() ## Lee y guarda un frame del video en las
                                      ## variables "valIni" y "frameIni".
     
    if valIni:
       imagen_zbar= pyzbar.decode(frameIni) ## Decodifica el frame guardado
                                            ## en formato de lista.
       for codigo_qrIni in imagen_zbar: ## Recorre la lista con la variable llamada:
                                         ## "codigo_qrIni"
            
                coorIni = codigo_qrIni.polygon ## Guarda las coordenadas de los 4 bordes
                                               ## del c�digo Qr en formato de lista de tuplas.
                
                anchoPIni = distancia(coorIni[1][0],coorIni[0][0],coorIni[1][1],coorIni[0][1]) ## Calcula el ancho del c�digo
                                                                                               ## percibido por la c�mara.
                x = distanciaFocal(anchoPIni) ## Calcula la distancia focal y devuelve su valor.
    return x

## Funci�n para calcular la distancia del c�digo a la c�mara.
## Se utiliza la misma ecuaci�n de distancia Focal pero se despeja
## la variable de distancia esta vez. Utilizando la distancia focal conocida
## y el ancho conocido se puede aproximar la distancia a la c�mara.
def distanciaAcamara(f,P):
    w = 45
    a = w*f
    D = a/P
    return D

#Inicializar la camara
capture = cv2.VideoCapture(0)
distanciaF = calibracionCamara(capture) 
print("distancia focal: ", distanciaF)
 
while 1:
    comando=arduino.read();
    if(comando=='t'):
        time.sleep(4)
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
   # elif comando=='ledb1':
   #     GPIO.output(7,True)
   # elif comando=='ledb0':
   #     GPIO.output(7,False)
   # elif comando=='ledc1':
   #     GPIO.output(8,True)
   # elif comando=='ledc0':
   #     GPIO.output(8,False)
   # elif comando=='ledd1':
   #     GPIO.output(9,True)
   # elif comando=='ledd0':
   #     GPIO.output(0,False)
   # if (dat=='banano'):
   #     distanciab=arduino.read()#arduino.readline();
   # elif (dat=='cafe'):
   #     distanciac=arduino.read();
   # elif (dat=='dispositivosmedicos'):
   #     distanciad=arduino.read();
    if (dat=='banano' and comando=='5')): #activa interrupci�n en arduino M0
        GPIO.output(10,True)
        contadore+=1
        d1 = str(datetime.now())
        h.write('\n'+'\n' + 'Se produjo un error por espacio lleno en pila de banano a las: ')
        h.write(d1)
        h.close()
    else:
        Horario(dat)
        GPIO.output(10,False)
    if (dat=='cafe' and comando=='5'):
        GPIO.output(10,True)
        contadore+=1
        d1 = str(datetime.now())
        h.write('\n'+'\n' + 'Se produjo un error por espacio lleno en pila de cafe a las: ')
        h.write(d1)
        h.close()
    else:
        Horario(dat)
        GPIO.output(10,False)
    if (dat='dispositivosmedicos' and comando=='5'):
        GPIO.output(10,True)
        contadore+=1
        d1 = str(datetime.now())
        h.write('\n'+'\n' + 'Se produjo un error por espacio lleno en pila de dispositivos medicos a las: ')
        h.write(d1)
        h.close()
    else:
        Horario(dat)
        GPIO.output(10,False)

    if comando=='d':
        time.sleep(10)
        #Capturar un frame
        val, frame = capture.read()
     
        if val:
           imagen_zbar= pyzbar.decode(frame)
           #print (imagen_zbar) ## Este print tiene como fin ver el formato en el que se obtiene la decodificaci�n
           for codigo_qr in imagen_zbar:
                    #dat = codigo_qr.data ## Se obtiene el dato guardado en el c�digo
                    #dim = codigo_qr.rect ## Se obtienen las dimensiones de un rectangulo que encierra el Qr en su totalidad.
                    coor = codigo_qr.polygon 
                    anchoP = distancia(coor[1][0],coor[0][0],coor[1][1],coor[0][1]) ##Se calcula el ancho aparente de cada Qr.
                    distanciaCamara = distanciaAcamara(distanciaF,anchoP) ## Se aproxima la distancia seg�n este ancho y la distancia focal previamente calibrada.
                    #print ("ancho percibido por la c�mara(pixeles): ",anchoP)
                    dist=round(distanciaAcamara,0)
                    if (dist== 5):
                        arduino.write('5')
                    elif (dist==10):
                        arduino.write('4')
                    elif (dist==15):
                        arduino.write('3')
                    elif (dist==20):
                        arduino.write('2')
                    elif (dist==25):
                        arduino.write('1')
                    print ("distancia a camara: ", distanciaCamara)
                    
        
        
        
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

