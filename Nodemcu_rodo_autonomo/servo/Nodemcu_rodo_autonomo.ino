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

float dist_right;                             //Armazena a distância em centímetros da direita
float dist_left;  

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
   // digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,minSpeed);
    //analogWrite(LeftMotorSpeed,minSpeed);
  }
void esquerda(){
   // digitalWrite(RightMotorDir,LOW); 
    digitalWrite(LeftMotorDir,HIGH);
    //analogWrite(RightMotorSpeed,minSpeed);
    analogWrite(LeftMotorSpeed,minSpeed);
  }

  

void decision()                              //Compara as distâncias e decide qual melhor caminho a seguir
{
   freia();                              //Para o robô
   delay(500);                               //Aguarda 500ms
   myServo.write(0);                          //Move sensor para direita através do servo
   delay(500);                               //Aguarda 500ms
   dist_right = distanceSensor.measureDistanceCm();           //Mede distância e armazena em dist_right
   delay(2000);                              //Aguarda 2000ms
   myServo.write(175);                        //Move sensor para esquerda através do servo
   delay(500);                               //Aguarda 500ms
   dist_left = distanceSensor.measureDistanceCm();            //Mede distância e armazena em dis_left
   delay(2000);                               //Aguarda 2000ms
   myServo.write(80);                         //Centraliza servo
   delay(500);
   
   if(dist_right > dist_left)                //Distância da direita maior que da esquerda?
   {                                         //Sim...
   
      re();                           //Move o robô para trás
      delay(600);                            //Por 600ms
      direita();                          //Move o robô para direita
      delay(2000);                           //Por 2000ms
      frente();                          //Move o robô para frente
   
   } //end if
   
   else                                      //Não...
   {
   
      re();                           //Move o robô para trás
      delay(600);                            //Por 600ms
      esquerda();                           //Move o robô para esquerda
      delay(2000);                            //Por 2000ms
      frente();                          //Move o robô para frente
   
   } //end else
   

} //end decision


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

  frente();
  
  dist_cm = distanceSensor.measureDistanceCm();
  
  if(dist_cm < 30 or dist_cm  == -1) //distância menor que 20 cm?
  {
      decision();
  }

   
   delay(100);
}
