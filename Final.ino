#include <NewPing.h>
#include <Servo.h>
//Pines de control motor Horizontal
#define Pink1 9 
#define Orange1 8
#define Blue1 7
#define Yellow1 6
//Pines de control motor Vertical
#define Pink2 13
#define Orange2 12
#define Blue2 11
#define Yellow2 10 
//Pines de control sensores Ultrasónicos
#define Trigger_pin 2
#define Echo_pinBA  A0
#define Echo_pinDM  A1
#define Echo_pinCA  A2
#define distancia_max 200
//LEDs
#define led_b A4
#define led_c A3
#define led_d A5
//Interrupción por botón
#define boton 3
//Pin de control Servomotor
#define pinServo 4

NewPing  pilaban (Trigger_pin, Echo_pinBA, distancia_max);
NewPing  pilacaf (Trigger_pin, Echo_pinCA, distancia_max);
NewPing  piladisp (Trigger_pin, Echo_pinDM, distancia_max);

int distancia_carga=0;
int contador=0;
int distancia_banano=0;
int distancia_cafe=0;
int distancia_disp=0;
int distancia_pila=0;
int cajasc=0;
int cajasp=0;
volatile int contInicio = 0;
char inicio = 'x';
char QR='a';
char QR2='a';
char scajas;
char ccajas;
int distanciaint=0;


//*****MOVIMIENTO SERVOMOTOR******//

Servo servo1;

//*****FIN-MOVIMIENTO SERVOMOTOR*****//

//******MOVIMIENTO MOTOR A PASOS*****
int pasos_restantes1=0;
int pasos_restantes2=0;//1=movimiento horizontal; 2=movimiento vertical
boolean direccion1=false;
boolean direccion2=false;
int pasos1=0; //Recorrido de matriz de excitación 
int pasos2=0;

int paso_completo [4][4]=//matriz de excitación
{ {1, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 1},
  {1, 0, 0, 1}
};

int paso_completo2 [4][4]=//matriz de excitación
{ {1, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 1},
  {1, 0, 0, 1}
};

void Direccion_giroh(){//cambia dirección de giro
  if (direccion1)
  pasos1++;
  else
  pasos1--;

  if (pasos1>3)
  pasos1 = 0;
  if (pasos1<0)
  pasos1 = 3;
  
   }
void Direccion_girov(){
  if (direccion2)
  pasos2++;
  else
  pasos2--;

  if (pasos2 > 3)
  pasos2 = 0;
  if (pasos2 < 0)
  pasos2 = 3;
   
   }

void pasoh (){
  digitalWrite(Orange1, paso_completo[pasos1][0]);
  digitalWrite(Pink1, paso_completo[pasos1][1]);
  digitalWrite(Yellow1, paso_completo[pasos1][2]);
  digitalWrite(Blue1, paso_completo[pasos1][3]);
  Direccion_giroh();
  
  }//avanza un paso

void pasov (){
  digitalWrite(Orange2, paso_completo2[pasos2][0]);
  digitalWrite(Pink2, paso_completo2[pasos2][1]);
  digitalWrite(Yellow2, paso_completo2[pasos2][2]);
  digitalWrite(Blue2, paso_completo2[pasos2][3]);
  Direccion_girov();
  
  }//avanza un paso
//******FIN-MOVIMIENTO MOTOR A PASOS*****

//*****SENSORES ULTRASÓNICOS********//

void distancia_pilas(){//FUNCION TOMA LAS DISTANCIAS DE LAS PILAS, PARA ENCENDIDO DE LEDS INDICADORES
   distancia_banano=(pilaban.ping_median())/US_ROUNDTRIP_CM;
   SerialUSB.println("distancia banano: ");
   SerialUSB.println(distancia_banano);
   delay(30);
   distancia_cafe=(pilacaf.ping_median())/US_ROUNDTRIP_CM;
   SerialUSB.println("distancia cafe: ");
   SerialUSB.println(distancia_cafe);
   delay(30);
   distancia_disp=(piladisp.ping_median())/US_ROUNDTRIP_CM;
   SerialUSB.println("distancia dispositivos medicos: ");
   SerialUSB.println(distancia_disp);
   delay(30);
   //distancia_carga=(pilacarg.ping_median())/US_ROUNDTRIP_CM;
 if (distancia_banano<=34){ 
  digitalWrite(led_b, HIGH);
  //Serial.write("ledb1");//manda comando para encendido de led banano
  }
 else {
  digitalWrite(led_b, LOW);
  //Serial.write("ledb0");//manda comando para apagado de led
  }
 if (distancia_cafe<=34){ 
  digitalWrite(led_c, HIGH);
 //Serial.write("ledc1");//manda comando para encendido de led cafe
  }
 else {
  digitalWrite(led_c, LOW);
  //Serial.write("ledc0");//manda comando para apagado de led
  }
 if (distancia_disp<=34){ 
  digitalWrite(led_d, HIGH);
  //Serial.write("ledd1");//manda comando para encendido de led dispositivos medicos
  }
 else {
  digitalWrite(led_d, LOW);
  //Serial.write("ledd0");//manda comando para apagado de led
  }
}

int distancia_pilaprod(char qr){
  int distancia=0;
  switch (qr){
    case 'b':
      distancia=(pilaban.ping_median())/US_ROUNDTRIP_CM;
      //SerialUSB.println(distancia_banano);
      //distancia=distancia_banano;
      return distancia;
      break;
    case 'c':
      distancia=(pilacaf.ping_median())/US_ROUNDTRIP_CM;
      //distancia=distancia_cafe;
      //SerialUSB.println(distancia_cafe);
      return distancia;
      break;
      
    case 'd':
      distancia=(piladisp.ping_median())/US_ROUNDTRIP_CM;
      //SerialUSB.println(distancia_disp);
      //distancia=distancia_disp
      return distancia;
      break;
      
    default:
      return distancia;
      break;
    }

  
  }

void distancia_prodint(){ //funcion para obtener la distancia de interes (la que producirá movimiento vertical de la grua) 
  switch (QR){
    case 'b':
      distancia_pila=(pilaban.ping_median())/US_ROUNDTRIP_CM;
      //Serial5.write(distancia_banano);
//      if (distancia_pila==32||distancia_pila==33){
//          Serial.write("inter");//se envia comando al microprocesador para producir la interrupcion en el microcontrolador
//        }
     break;
   case 'c':
     distancia_pila=(pilacaf.ping_median())/US_ROUNDTRIP_CM;
     //Serial5.write(distancia_cafe);
//      if (distancia_pila==32||distancia_pila==33){//verificar condicionales, tal vez no sean necesarios
//          Serial.write("inter");
//        }
//    
      break;
      
   case 'd':
     //Serial5.write(distancia_disp);
     distancia_pila=(piladisp.ping_median())/US_ROUNDTRIP_CM;
//     if (distancia_pila==32||distancia_pila==33){
//          Serial.write("inter");
//        }
//  
      break;
      
    default:
      break;
    }
}

int Cantidad_cajasp(int distancia){
  int cajas=0;
    if (distancia >= 55){
      cajas=0; 
    }
    else if ((distancia >= 50) && (distancia <= 54)){
      cajas=1;
    }
    else if ((distancia >= 45) && (distancia <= 49)){
      cajas=2;
    }
    else if ((distancia >=40) && (distancia <=44)){
      cajas=3;
    }
    else if ((distancia >=35) && (distancia <=39)){
      cajas=4; 
    }
    else if (distancia <=34){
      cajas=5; 
      return cajas;
    }return cajas;
  }
//*****FIN-SENSORES ULTRASÓNICOS*****//

//*****Función Blink LED*****//
void calibracion(char Q){
 int i=0; 
 if (Q == 'L'){
   for (i=0; i<2; i++){
     digitalWrite(led_b, HIGH);
     digitalWrite(led_c, HIGH);
     digitalWrite(led_d, HIGH);
     delay (20);
     digitalWrite(led_b, LOW);
     digitalWrite(led_c, LOW);
     digitalWrite(led_d, LOW);
     delay (20);
   }
 }else if (Q == 'E'){
    for (i=0; i<2; i++){
      digitalWrite(led_b, HIGH);
      delay (6);
      digitalWrite(led_b, LOW);
      delay (6);
      digitalWrite(led_c, HIGH);
      delay (6);
      digitalWrite(led_c, LOW);
      delay (6);
      digitalWrite(led_d, HIGH);
      delay (6);
      digitalWrite(led_d, LOW);
      delay (6);  
    }
 }
}

//*****Interrupción*****//
void botonazo(){
  contInicio = 0;
  Serial5.write('q');
}

//*****Interrupción de funcionamiento por pila llena*****//
void pilallena(){
  //lectura de qr y guardado en QR2
  QR2=QR;
  distanciaint=distancia_pila;
  while (QR2==QR || distanciaint==distancia_pila){
    //abrir camara para leer qr
    Serial5.write("t");
    QR2=Serial5.read();
    distanciaint=distancia_pilaprod(QR2);
    if (distanciaint!=distancia_pila && QR2==QR){
      distancia_pila=distanciaint;
      Serial5.write(distancia_pila);
      break;
      //APAGAR LED RESPECTIVO
      //cajasp=Cantidad_cajasp(distancia_pila);
      }
   }
   QR=QR2;
   distancia_pila=distanciaint;
  }


//*****Función para movimiento horizontal*****//
void movimiento_horizontal (char qr){
  switch (qr){
    case 'b':
      direccion1=!direccion1;
      pasos_restantes1=3700;
      while(pasos_restantes1>0){
        pasoh();
        pasos_restantes1--;
        delay (3);
      } 
      break;
    case 'c':
      direccion1=!direccion1;
      pasos_restantes1=11100;
      while(pasos_restantes1>0){
        pasoh();
        pasos_restantes1--;
        delay (3);
      } 
      break;
    case 'd':
      direccion1=!direccion1;
      pasos_restantes1=7400;
      while(pasos_restantes1>0){
        pasoh();
        pasos_restantes1--;
        delay (3);
      } 
      break;

    default:
      break;
    
    }
  }

//*****Función para movimiento vertical*****//
void distancia_vertical(int cant_cajas){
  switch (cant_cajas){
    case 0:
      direccion2=!direccion2;
      pasos_restantes2=1625;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (16);
      } 
      break;
    case 1:
     direccion2=!direccion2;
      pasos_restantes2=1225;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (16);
      } 
      break;
    case 2:
      direccion2=!direccion2;
      pasos_restantes2=825;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (16);
      } 
      break;
    case 3:
      direccion2=!direccion2;
      pasos_restantes2=425;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (16);
      } 
      break;
    case 4:
      direccion2=!direccion2;
      pasos_restantes2=25;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (16);
      } 
    case 5:
      direccion2=!direccion2;
      pasos_restantes2=25;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (16);
      } 
      break;
    default:
      break;
    }
  }


void setup() {
      Serial5.begin(9600);
      pinMode(led_b, OUTPUT);
      pinMode(led_c, OUTPUT);
      pinMode(led_d, OUTPUT);
      pinMode(Pink1, OUTPUT); 
      pinMode(Orange1, OUTPUT); 
      pinMode(Blue1, OUTPUT); 
      pinMode(Yellow1, OUTPUT);
      pinMode(Pink2, OUTPUT); 
      pinMode(Orange2, OUTPUT); 
    //calibracion(inicio);
      pinMode(Blue2, OUTPUT); 
      pinMode(Yellow2, OUTPUT);
      servo1.attach(pinServo);
      attachInterrupt(digitalPinToInterrupt(boton),botonazo, RISING); //INTERRUPCION PIN 3;
      servo1.write(0);
}


void loop(){
  //while (((inicio != 'L' ) || (inicio != 'E'))&& (contInicio==0)){
    //contInicio += 1;
    //inicio = Serial5.read();
  //}
  //if ((inicio == 'L' ) || (inicio == 'E')){
  //}
  //QR=Serial5.read();  
  QR = 'b';
  while ((QR == 'b') || (QR == 'c') || (QR == 'd')){
     delay(1000);
     distancia_pilas(); //funcion para encender leds de pilas que esten llenas 
     distancia_prodint();//funcion especifica para tomar distancia de la pila de interés
     distancia_pila= distancia_pilaprod(QR);
     SerialUSB.println("Distancia a pila: ");
     SerialUSB.println(distancia_pila);
     cajasp=Cantidad_cajasp(distancia_pila);
     SerialUSB.println("Cantidad de cajas: ");
     SerialUSB.println(cajasp);
     //scajas=cajasp+'0';
     //Serial5.write(scajas);
     //distancia_pilacarga();
     cajasc=1;//Cantidad_cajasp(distancia_carga);
     distancia_vertical (cajasc);
     delay(1000);
     servo1.write(0);
     delay (1000);
     servo1.write (68);
     delay(1000);
     distancia_vertical (cajasc);
     delay(1000);
     movimiento_horizontal(QR);
     distancia_vertical (cajasp);
     delay(1000);
     servo1.write(0);
     delay(1000);
     distancia_vertical (cajasp);
     delay(1000);
     movimiento_horizontal(QR); 
    }
}
