import serial
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
b=c=m=0
def Horario(data):
    h = open('Historial.txt','a')
    if data=='banano':
        d1 = str(datetime.now())
        #print ('Se cumple la condicion 1')
        h.write('\n'+'\n' + 'Se agarró una caja de banano a las: ')
        h.write(d1)
        h.close()
    elif data=='cafe':
        d1 = str(datetime.now())
        #print ('Se cumple la condicion 2')
        h.write('\n'+'\n' + 'Se agarró una caja de café a las: ')
        h.write(d1)
        h.close()
    elif data=='dispositivosmedicos':
        d1 = str(datetime.now())
        #print ('Se cumple la condicion 3')
        h.write('\n'+'\n' + 'Se agarró una caja de instrumentos médicos a las: ')
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
        
## Función básica para calcular distancia entre 2 puntos. 
## Se utiliza para calcular el ancho del Qr percibido por la cámara. 
def distancia(x2,x1,y2,y1):
    a = (x2-x1)**2
    b2 = (y2-y1)**2
    c2 = a+b2
    d = math.sqrt(c2)
    return d

## Función para calcular la distancia focal o "focal length".
## Se hace el cálculo con valores conocidos del ancho de la marca y 
## la distancia a la que se encuentra de la cámara en ese instante.
## Este valor obtenido se utiliza para el cálculo de las demás distancias.
def distanciaFocal(p):
    w = 45
    d2 = 11
    a = p*d2
    f = a/w
    return f

def calibracionCamara(captura):
    valIni, frameIni = captura.read() ## Lee y guarda un frame del video en las
                                      ## variables "valIni" y "frameIni".
     
    if valIni:
       print 1
       imagen_zbar= pyzbar.decode(frameIni) ## Decodifica el frame guardado
                                            ## en formato de lista.
       for codigo_qrIni in imagen_zbar: ## Recorre la lista con la variable llamada:
                                         ## "codigo_qrIni"
            
                print 2
                coorIni = codigo_qrIni.polygon ## Guarda las coordenadas de los 4 bordes
                                               ## del código Qr en formato de lista de tuplas.
                
                anchoPIni = distancia(coorIni[1][0],coorIni[0][0],coorIni[1][1],coorIni[0][1]) ## Calcula el ancho del código
                                                                                               ## percibido por la cámara.
                x = distanciaFocal(anchoPIni) ## Calcula la distancia focal y devuelve su valor.
    return x

## Función para calcular la distancia del código a la cámara.
## Se utiliza la misma ecuación de distancia Focal pero se despeja
## la variable de distancia esta vez. Utilizando la distancia focal conocida
## y el ancho conocido se puede aproximar la distancia a la cámara.
def distanciaAcamara(f,P):
    w = 45
    a = w*f
    D = a/P
    return D

Boton = 'x'
while (Boton != 'q'):
    Boton = arduino.read()
    #print Boton

#Inicializar la camara
if Boton == 'q' :
   capture = cv2.VideoCapture(0)
   distanciaF = calibracionCamara(capture) 
   #if distanciaF > 0:  #Envía una señal para encender los LEDs de calibración 
        #arduino.write('L')
        #print("Calibrado.")
   #else:
        #arduino.write('E')
        #print("Error en la calibracion.")
    
    
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
        #comando = arduino.read()            
  
    if comando=='d':
        time.sleep(10)
        #Capturar un frame
        val, frame = capture.read()
     
        if val:
           imagen_zbar= pyzbar.decode(frame)
           #print (imagen_zbar) ## Este print tiene como fin ver el formato en el que se obtiene la decodificación
           for codigo_qr in imagen_zbar:
                    #dat = codigo_qr.data ## Se obtiene el dato guardado en el código
                    #dim = codigo_qr.rect ## Se obtienen las dimensiones de un rectangulo que encierra el Qr en su totalidad.
                    coor = codigo_qr.polygon 
                    anchoP = distancia(coor[1][0],coor[0][0],coor[1][1],coor[0][1]) ##Se calcula el ancho aparente de cada Qr.
                    distanciaCamara = distanciaAcamara(distanciaF,anchoP) ## Se aproxima la distancia según este ancho y la distancia focal previamente calibrada.
                    #print ("ancho percibido por la cámara(pixeles): ",anchoP)
                    dist=round(distanciaCamara,0)
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
    if comando=='r':
        Horario(dat)
        Informe(b,c,m,contadore)
    if comando=='e':
        contadore+=1
        if dat=='banano':
            d1 = str(datetime.now())
            #print ('Se cumple la condicion 2')
            h.write('\n'+'\n' + 'Se produjo un error por pila de banano llena a las: ')
            h.write(d1)
            h.close()
        elif dat=='cafe':
            d1 = str(datetime.now())
            #print ('Se cumple la condicion 2')
            h.write('\n'+'\n' + 'Se produjo un error por pila de cafe llena a las: ')
            h.write(d1)
            h.close()
        elif dat=='dispositivosmedicos':
            d1 = str(datetime.now())
            #print ('Se cumple la condicion 2')
            h.write('\n'+'\n' + 'Se produjo un error por pila de dispositivos medicos llena a las: ')
            h.write(d1)
            h.close()
        Informe(b,c,m,contadore)    
    if comando=='f':
        contadore+=1
        d1 = str(datetime.now())
        #print ('Se cumple la condicion 2')
        h.write('\n'+'\n' + 'Se produjo un error por falta de cajas que distribuir a las: ')
        h.write(d1)
        h.close()
        Informe(b,c,m,contadore)
    
        
        
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

