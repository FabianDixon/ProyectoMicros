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
#define Echo_pinCG  A1
#define Echo_pinBA  A2
#define Echo_pinCA  A3
#define Echo_pinDM  A4
#define distancia_max 200
 
NewPing  pilacarg (Trigger_pin, Echo_pinCG, distancia_max);
NewPing  pilaban (Trigger_pin, Echo_pinBA, distancia_max);
NewPing  pilacaf (Trigger_pin, Echo_pinCA, distancia_max);
NewPing  piladisp (Trigger_pin, Echo_pinDM, distancia_max);
int distancia_carga=0;
int distancia_pila=0;
int cajasc=0;
int cajasp=0;
char QR;
volatile char QR2;
volatile int distanciaint=0;
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
  digitalWrite(Orange2, paso_completo[pasos2][0]);
  digitalWrite(Pink2, paso_completo[pasos2][1]);
  digitalWrite(Yellow2, paso_completo[pasos2][2]);
  digitalWrite(Blue2, paso_completo[pasos2][3]);
  Direccion_girov();
  
  }//avanza un paso
//******FIN-MOVIMIENTO MOTOR A PASOS*****

//*****MOVIMIENTO SERVOMOTOR******//
int pinServo=4;
Servo servo1;

//*****FIN-MOVIMIENTO SERVOMOTOR*****//

//*****SENSORES ULTRASÓNICOS********//

int distancia_pilaprod(char qr){
  int distancia=0;
  switch (qr){
    case 'b':
      distancia=(pilaban.ping_median())/US_ROUNDTRIP_CM;
      return distancia;
      break;
    case 'c':
      distancia=(pilacaf.ping_median())/US_ROUNDTRIP_CM;
      return distancia;
      break;
      
    case 'd':
      distancia=(piladisp.ping_median())/US_ROUNDTRIP_CM;
      return distancia;
      break;
      
    default:
      break;
    }

  
  }

int distancia_pilacarga(){
  int distancia_c=(pilacarg.ping_median())/US_ROUNDTRIP_CM;
  return distancia_c;
  }

int Cantidad_cajasp(int distancia){
  int cajas=0;
  switch (distancia){
    case 30:
      cajas=0; 
      return cajas;
      break;
    case 25:
      cajas=1;
      return cajas;
      break;
    case 20:
      cajas=2;
      return cajas;
      break;
    case 15:
      cajas=3;
      return cajas;
      break;
    case 10:
      cajas=4; 
      return cajas;
      break;
    case 5:
      cajas=5; 
      return cajas;
      break;
    default:
      break;
    
    }
  }
//*****FIN-SENSORES ULTRASÓNICOS*****//

void setup() {
      Serial.begin(9600);
      pinMode(Pink1, OUTPUT); 
      pinMode(Orange1, OUTPUT); 
      pinMode(Blue1, OUTPUT); 
      pinMode(Yellow1, OUTPUT);
      pinMode(Pink2, OUTPUT); 
      pinMode(Orange2, OUTPUT); 
      pinMode(Blue2, OUTPUT); 
      pinMode(Yellow2, OUTPUT);
      servo1.attach(pinServo);
      attachInterrupt(0,pilallena, CHANGE); //INTERRUPCION PIN 3;

}
void pilallena(){
  //lectura de qr y guardado en QR2
  while (QR2==QR && distanciaint==distancia_pila){
    //abrir camara para leer qr 
    if (QR2!=QR && distanciaint!=distancia_pila){
      
      
      }
    else if (QR2!=QR){
      QR=QR2;
      distancia_pila=distancia_pilaprod(QR);
      cajasp=Cantidad_cajasp(distancia_pila);
      break;
      }
    else if (distanciaint!=distancia_pila){
      distancia_pila=distanciaint;
      //APAGAR LED RESPECTIVO
      cajasp=Cantidad_cajasp(distancia_pila);
      break;
      }
    
    }
  
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
        delay (5);
      } 
      break;
    case 1:
     direccion2=!direccion2;
      pasos_restantes2=470;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (5);
      } 
      break;
    case 2:
      direccion2=!direccion2;
      pasos_restantes2=440;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (5);
      } 
      break;
    case 3:
      direccion2=!direccion2;
      pasos_restantes2=410;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (5);
      } 
      break;
    case 4:
      direccion2=!direccion2;
      pasos_restantes2=380;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (5);
      } 
    case 5:
      direccion2=!direccion2;
      pasos_restantes2=350;
      while(pasos_restantes2>0){
        pasov();
        pasos_restantes2--;
        delay (5);
      } 
      break;
    default:
      break;
    }
  
  }

void loop() {
  // put your main code here, to run repeatedly:
  //char QR; 
  distancia_carga=distancia_pilacarga();
  distancia_pila= distancia_pilaprod(QR);
  cajasp=Cantidad_cajasp(distancia_pila);
  //DEFINIR QUE LED ENCENDER SEGUN QR Y SI PILA ESTÁ LLENA
  cajasc=Cantidad_cajasp(distancia_carga);
  distancia_vertical (cajasc);
  servo1.write(20);
  delay (1000);
  servo1.write (0);
  distancia_vertical (cajasc);
  movimiento_horizontal(QR);
  distancia_vertical (cajasp);
  servo1.write(20);
  delay(1000);
  servo1.write(0);
  distancia_vertical (cajasp);
  movimiento_horizontal(QR);
}
