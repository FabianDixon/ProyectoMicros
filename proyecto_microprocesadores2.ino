#include <NewPing.h>
#include <Servo.h>
#define Pink1 10
#define Orange1 11
#define Blue1 12
#define Yellow1 13
#define Pink2 9
#define Orange2 8
#define Blue2 7
#define Yellow2 6
#define Trigger_pin 5
//#define Echo_pinCG  A1
#define Echo_pinBA  A2
#define Echo_pinCA  A3
#define Echo_pinDM  A4
#define led_b A0
#define led_c A1
#define led_d A5
#define boton 2
#define distancia_max 200
 
//NewPing  pilacarg (Trigger_pin, Echo_pinCG, distancia_max);
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
char QR='a';
char QR2='a';
char scajas;
char ccajas;
int distanciaint=0;
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

//*****MOVIMIENTO SERVOMOTOR******//
int pinServo=4;
Servo servo1;

//*****FIN-MOVIMIENTO SERVOMOTOR*****//

//*****SENSORES ULTRASÓNICOS********//

void distancia_pilas(){//FUNCION TOMA LAS DISTANCIAS DE LAS PILAS, PARA ENCENDIDO DE LEDS INDICADORES
contador=0;
while (contador<2){
   distancia_banano=(pilaban.ping_median())/US_ROUNDTRIP_CM;
   delay(30);
   distancia_cafe=(pilacaf.ping_median())/US_ROUNDTRIP_CM;
   delay(30);
   distancia_disp=(piladisp.ping_median())/US_ROUNDTRIP_CM;
   delay(30);
   //distancia_carga=(pilacarg.ping_median())/US_ROUNDTRIP_CM;
   contador++;
  }
 if (distancia_banano==32||distancia_banano==33){ 
  digitalWrite(led_b, HIGH);
  //Serial.write("ledb1");//manda comando para encendido de led banano
  }
 else {
  digitalWrite(led_b, LOW);
  //Serial.write("ledb0");//manda comando para apagado de led
  }
 if (distancia_cafe==32||distancia_cafe==33){ 
  digitalWrite(led_c, HIGH);
 //Serial.write("ledc1");//manda comando para encendido de led cafe
  }
 else {
  digitalWrite(led_c, LOW);
  //Serial.write("ledc0");//manda comando para apagado de led
  }
 if (distancia_disp==32||distancia_disp==33){ 
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
      //distancia=distancia_banano;
      return distancia;
      break;
    case 'c':
      distancia=(pilacaf.ping_median())/US_ROUNDTRIP_CM;
      //distancia=distancia_cafe;
      return distancia;
      break;
      
    case 'd':
      distancia=(piladisp.ping_median())/US_ROUNDTRIP_CM;
      //distancia=distancia_disp
      return distancia;
      break;
      
    default:
      break;
    }

  
  }

void distancia_prodint(){ //funcion para obtener la distancia de interes (la que producirá movimiento vertical de la grua) 
  switch (QR){
    case 'b':
      distancia_pila=distancia_banano;
      //Serial5.write(distancia_banano);
//      if (distancia_pila==32||distancia_pila==33){
//          Serial.write("inter");//se envia comando al microprocesador para producir la interrupcion en el microcontrolador
//        }
     break;
   case 'c':
     distancia_pila=distancia_cafe;
     //Serial5.write(distancia_cafe);
//      if (distancia_pila==32||distancia_pila==33){//verificar condicionales, tal vez no sean necesarios
//          Serial.write("inter");
//        }
//    
      break;
      
   case 'd':
     //Serial5.write(distancia_disp);
     distancia_pila=distancia_disp;
//     if (distancia_pila==32||distancia_pila==33){
//          Serial.write("inter");
//        }
//  
      break;
      
    default:
      break;
    }
}

void distancia_pilacarga(){
  //int distancia_c=(pilacarg.ping_median())/US_ROUNDTRIP_CM;
  //return distancia_c;
  Serial5.write("d");
  ccajas=Serial5.read();
  cajasc=ccajas-'0';
  }

int Cantidad_cajasp(int distancia){
  int cajas=0;
  switch (distancia){
    case 57:
      cajas=0; 
      return cajas;
      break;
    case 52:
      cajas=1;
      return cajas;
      break;
    case 47:
      cajas=2;
      return cajas;
      break;
    case 42:
      cajas=3;
      return cajas;
      break;
    case 37:
      cajas=4; 
      return cajas;
      break;
    case 32:
    
      //cajas=5; 
      //return cajas;
      break;
    default:
      break;
    
    }
  }
//*****FIN-SENSORES ULTRASÓNICOS*****//

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
      pinMode(Blue2, OUTPUT); 
      pinMode(Yellow2, OUTPUT);
      servo1.attach(pinServo);
      attachInterrupt(digitalPinToInterrupt(3),pilallena, RISING); //INTERRUPCION PIN 3;
      servo1.write(80);

}
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


void movimiento_horizontal (char qr){
  switch (qr){
    case 'b':
      direccion1=!direccion1;
      pasos_restantes1=330;
      while(pasos_restantes1>0){
        pasoh();
        pasos_restantes1--;
        delay (3);
      } 
      break;
    case 'c':
      direccion1=!direccion1;
      pasos_restantes1=360;
      while(pasos_restantes1>0){
        pasoh();
        pasos_restantes1--;
        delay (3);
      } 
      break;
    case 'd':
      direccion1=!direccion1;
      pasos_restantes1=390;
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
  
void distancia_vertical(int cant_cajas){
  switch (cant_cajas){
    case 0:
      direccion2=!direccion2;
      pasos_restantes2=500;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (20);
      } 
      break;
    case 1:
     direccion2=!direccion2;
      pasos_restantes2=470;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (20);
      } 
      break;
    case 2:
      direccion2=!direccion2;
      pasos_restantes2=440;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (20);
      } 
      break;
    case 3:
      direccion2=!direccion2;
      pasos_restantes2=410;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (20);
      } 
      break;
    case 4:
      direccion2=!direccion2;
      pasos_restantes2=380;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (20);
      } 
    case 5:
      direccion2=!direccion2;
      pasos_restantes2=350;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (20);
      } 
      break;
    default:
      break;
    }
  
  }

void loop() {
  Serial5.write("t");
  QR=Serial5.read();  
  distancia_pilas(); //funcion para encender leds de pilas que esten llenas 
  distancia_prodint();//funcion especifica para tomar distancia de la pila de interés
  //distancia_pila= distancia_pilaprod(QR);
  cajasp=Cantidad_cajasp(distancia_pila);
  scajas=cajasp+'0';
  Serial5.write(scajas);
  distancia_carga=distancia_pilacarga();
  //cajasc=Cantidad_cajasp(distancia_carga);
  distancia_vertical (cajasc);
  servo1.write(80);
  delay (1000);
  servo1.write (100);
  distancia_vertical (cajasc);
  movimiento_horizontal(QR);
  distancia_vertical (cajasp);
  servo1.write(80);
  delay(1000);
  //servo1.write(0);
  distancia_vertical (cajasp);
  movimiento_horizontal(QR);
}
