#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC;
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int option;

int highlight;
int minilight;//Igual que la luz cuando esta nublado
int tempmax; //Temperatura maxima dentro
int hwater; //Hora de Regar
int mwater;

int sensorLDRPin = 0;
int finalButton = 13;
int MotorA = 7; // Conectado a su transistor conmutador, Avance
int MotorB = 8;
int MotorC = 9; //Conectado al transistor para el ventilador
int MotorD = 10;
int ledPin = 14;


void setup(){
  
  Serial.begin(9600);
  
  RTC.begin();//Se inicia el reloj
  dht.begin();//Asi se inicia el sensor de humedad y temperatura
  
  pinMode(ledPin, OUTPUT);
  pinMode(MotorA, OUTPUT);
  pinMode(MotorB, OUTPUT);
  pinMode(MotorC, OUTPUT);
  pinMode(MotorD, OUTPUT);
}

void loop(){
  /* Aqui se debe establecer la comunicacion con el movil para 
  cambiarle los valores de las variables segun la planta */
  if (Serial.available()>0){
    option=Serial.read();
    if(option=='a'){
      highlight = 300;
      minilight = 100;
      tempmax = 30;
      hwater = 20;
      mwater = 30;
    }
    if(option=='b'){
      highlight = 200;
      minilight = 100;
      tempmax = 28;
      hwater = 16;
      mwater = 30;
    }
  }
  
  float h = dht.readHumidity();//Establece la lectura de humedad
  float t = dht.readTemperature();//Establece la lectura de Temperatura
  
  DateTime now = RTC.now();//Se inicia el relojTime now = RTC.now();
  
  int lightValue;
  lightValue = analogRead(sensorLDRPin);//Lectura de luz ambiental
  
  /* El primero de los condicionales trata el cierre o apertura de la 
  tapadera de aluminio segun la señal del LDR */
  
  if ((lightValue > highlight) && (finalButton == LOW)){
    digitalWrite(MotorA, HIGH);
    delay(5000);
    digitalWrite(MotorA, LOW);
  }
  
  if ((lightValue <= highlight) && (finalButton == HIGH)){
    digitalWrite(MotorB, HIGH);
    delay(5000);
    digitalWrite(MotorB, LOW);
  }
  delay(50);
  //Final del primer subprograma
  
  /* Ahora debe ir el regadio automatico, hay que investigar la 
  entrada de datos del reloj */
  if ((now.hour() == hwater) && (now.minute() == mwater) && (h < 60)){
    digitalWrite(MotorD, HIGH);
    delay(20000);
    digitalWrite(MotorD, LOW);
  }
  //Final del segundo programa
  
  /* Aqui empieza el tercer programa, que trata la reduccion de la
  temperatura activando un ventilador simplemente */
  if(t > tempmax){
    digitalWrite(MotorC, HIGH);
    delay(30000);
    digitalWrite(MotorC, LOW);
  }
  //Final del tercer subprograma
  
  /* Aqui empieza el cuarto programa, que trata el encendido y apagado
  de las luces interiores */
  if ((lightValue > highlight) && (finalButton == HIGH)){
    digitalWrite(ledPin, HIGH);
  }
  
  if((lightValue < minilight) && (06 < now.hour() < 20)){
    digitalWrite(ledPin, HIGH);
  }
  
  if((minilight < lightValue < highlight) || (00 <= now.hour() <= 06 ) || (21 <= now.hour() <= 23)){
    digitalWrite(ledPin, LOW);
  }    
}
