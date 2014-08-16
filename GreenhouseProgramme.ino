#include <Wire.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int highlight = 300;

int sensorLDRPin = 0;
int finalButton = 13;
int MotorA = 7; // Conectado a su transistor conmutador, Avance
int MotorB = 8;
int MotorC = 9; //Conectado al transistor para el ventilador
int ledPin = 14;


void setup(){
  /* Aqui se debe establecer la comunicacion con el movil para 
  cambiarle los valores de las variables segun la planta */
  Serial.begin(9600);
  
  dht.begin();//Asi se inicia el sensor de humedad y temperatura
  
  pinMode(ledPin, OUTPUT);
  pinMode(MotorA, OUTPUT);
  pinMode(MotorB, OUTPUT);
  //Ahora deberia ir la comunicacion entre el dispositivo y el Arduino
}

void loop(){
  
  float h = dht.readHumidity();//Establece la lectura de humedad
  float t = dht.readTemperature();//Establece la lectura de Temperatura
  
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
  
  //Final del segundo programa
  
  /* Aqui empieza el tercer programa, que trata la reduccion de la
  temperatura activando un ventilador simplemente */
  if(t > 28){
    digitalWrite(MotorC, HIGH);
  }
  //Final del tercer subprograma
  
  /* Aqui empieza el cuarto programa, que trata el encendido y apagado
  de las luces interiores */
  if ((lightValue > highlight) && (finalButton == HIGH)){
    digitalWrite(ledPin, HIGH);
  }
  
  
}
