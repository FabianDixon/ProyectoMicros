#######Importación de librerías a utilizar######
from pyzbar import pyzbar ##Librería para códigos Qr o de barras
import numpy as np   
import cv2  ##Librería para utilizar cámaras con python
import time ##librería para delays o funciones de tiempo
import math ##Librería para funciones matemáticas

## Función básica para calcular distancia entre 2 puntos. 
## Se utiliza para calcular el ancho del Qr percibido por la cámara. 
def distancia(x2,x1,y2,y1):
    a = (x2-x1)**2
    b = (y2-y1)**2
    c = a+b
    d = math.sqrt(c)
    return d

## Función para calcular la distancia focal o "focal length".
## Se hace el cálculo con valores conocidos del ancho de la marca y 
## la distancia a la que se encuentra de la cámara en ese instante.
## Este valor obtenido se utiliza para el cálculo de las demás distancias.
def distanciaFocal(p):
    w = 45
    d = 20
    a = p*d
    f = a/w
    return f

## Función para calibrar la cámara.
## Se obtiene un frame del video con un código Qr a una distancia conocida,
## luego se calcula su ancho aparente para la cámara, y con este
## la distancia focal.
def calibracionCamara(captura):
    valIni, frameIni = captura.read() ## Lee y guarda un frame del video en las
                                      ## variables "valIni" y "frameIni".
     
    if valIni:
       imagen_zbar= pyzbar.decode(frameIni) ## Decodifica el frame guardado
                                            ## en formato de lista.
       for codigo_qrIni in imagen_zbar: ## Recorre la lista con la variable llamada:
                                         ## "codigo_qrIni"
            
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
    
#Inicializar la camara
capture = cv2.VideoCapture(0)
comando = input("ingrese el comando qr para iniciar la captura: ")

distanciaF = calibracionCamara(capture) 
print("distancia focal: ", distanciaF)

while 1:  
    if(comando=='qr'):
        
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
                    print ("ancho percibido por la cámara(pixeles): ",anchoP) 
                    print ("distancia a camara: ", distanciaCamara)

    #Salir con 'ESC'
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break
 
 
cv2.destroyAllWindows()
