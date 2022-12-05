// Includes the Servo library
#include <Servo.h> 
#include <HCSR04.h>

//MOTOR:
#define RightMotorSpeed 5
#define RightMotorDir   0 
#define LeftMotorSpeed  4
#define LeftMotorDir    2



int minRange = 300;
int maxRange = 900;
int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;

// DEFININDO PINOS DO SENSOR ULTRASONICO
const int trigPin = D7;
const int echoPin = D6;
UltraSonicDistanceSensor distanceSensor(trigPin, echoPin); 

// VARIAVEIS DE DURACAO E DISTANCIA

long duration;
int distance;

//DEFININDO SERVO 
Servo myServo; 

int vel,situacao;
float dist_cm,distMinima=45;  

void frente(){
    digitalWrite(RightMotorDir,HIGH); 
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);

    
  }
void re(){
  digitalWrite(RightMotorDir,LOW); 
  digitalWrite(LeftMotorDir,LOW);
  analogWrite(RightMotorSpeed,maxSpeed);
  analogWrite(LeftMotorSpeed,maxSpeed);
}

void freia(){
    digitalWrite(RightMotorDir,HIGH); 
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,noSpeed);
    analogWrite(LeftMotorSpeed,noSpeed);
  }

void direita(){
    digitalWrite(RightMotorDir,HIGH); 
    digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);
  }
void esquerda(){
    digitalWrite(RightMotorDir,LOW); 
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);
  }

  
void escolhelado(){
  float d1,d2;
  d1=d2=0;
  
  delay(500);
  myServo.write(0);
  delay(500);
  d1=distanceSensor.measureDistanceCm();
  delay(250);
  myServo.write(180);
  delay(750);
  d2=distanceSensor.measureDistanceCm();
  myServo.write(90);
  delay(250);
  
  if(d1<d2){
    Serial.println(" Virando a Direita");
    Serial.println(d1);
    }else{
      Serial.println(" Virando a Esquerda");
      Serial.println(d2);
      }
  }
void setup() {
//setando os pinos referentes aos motores
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  
//iniciando o carrinho com os motores desligados e sentido de rotação à frente
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir,HIGH);
//
  
  pinMode(trigPin, OUTPUT); // SENSOR ULTRASONICO OUTPUT
  pinMode(echoPin, INPUT); // SENSOR ULTRASONICO INPUT
  
  Serial.begin(9600);
  myServo.attach(2); // DEFININDO PINO DO SERVO
  myServo.write(90);
}

void loop() {
     //Efetua a leitura da distancia em centimetros
    Serial.print("Distancia: ");
    Serial.print(distanceSensor.measureDistanceCm());
    Serial.println(" cm");
    dist_cm=distanceSensor.measureDistanceCm();
    if (dist_cm > distMinima or dist_cm == -1 ){
      frente();
      }else{
      freia();
      escolhelado();
        }
}
