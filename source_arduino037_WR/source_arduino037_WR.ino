/*
     Curso de Arduino WR Kits Channel
     
     Aula 37 - Robô Autônomo Completo com Arduino e Sensor de Ultrassom
     
     
     Software de controle do robô
     
     
     Plataforma: Arduino Duemilanove
     
     Sensor: HC-SR04
     
     Driver motores: L293D
     
     
     
     Autor: Eng. Wagner Rambo     Data: Outubro de 2015

     www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits

*/

// --- Bibliotecas Auxiliares ---
#include <Servo.h>                            //Biblioteca para controle de servos


// --- Mapeamento de Hardware ---
#define IN1    10                             //Driver L293D
#define IN2    11                             //Driver L293D
#define EN1     5                             //Enable driver
#define EN2     6                             //Enable driver
#define IN3     9                             //Driver L293D
#define IN4     8                             //Driver L293D
#define trig   13                             //Saída para o pino de trigger do sensor
#define echo   12                             //Entrada para o pino de echo do sensor
#define serv    7                             //Saída para o servo motor 
#define pwm    A0                             //Entrada analógica para ajuste de velocidade


// --- Protótipo das Funções Auxiliares ---
float measureDistance();                      //Função para medir, calcular e retornar a distância em cm
void trigPulse();                             //Função que gera o pulso de trigger de 10µs
void speedAdjust();                           //Função para ajustar velocidade dos motores
void decision();                              //Função para tomada de decisão. Qual melhor caminho?
void robotAhead();                            //Função para mover o robô para frente
void robotBack();                             //Função para mover o robô para trás
void robotRight();                            //Função para mover o robô para direita
void robotLeft();                             //Função para mover o robô para esquerda
void robotStop();                             //Função para parar o robô


// --- Objetos ---
Servo servo1;                                 //Cria objeto para o servo motor


// --- Variáveis Globais ---
float dist_cm;                                //Armazena a distância em centímetros entre o robô e o obstáculo
float dist_right;                             //Armazena a distância em centímetros da direita
float dist_left;                              //Armazena a distância em centímetros da esquerda
int   adc = 0x00;                             //Armazena o valor lido na entrada analógica


// --- Configurações Iniciais ---
void setup() 
{
 for(char i=9; i<12; i++) pinMode(i, OUTPUT); //Saídas para o driver L293D
 pinMode(EN1, OUTPUT);                        //Saída para controle de PWM do driver
 pinMode(EN2, OUTPUT);                        //Saída para controle de PWM do driver
 pinMode(trig, OUTPUT);                       //Saída para o pulso de trigger
 pinMode(serv, OUTPUT);                       //Saída para o servo motor
 pinMode(echo, INPUT);                        //Entrada para o pulso de echo
 
 servo1.attach(serv);                         //Objeto servo1 no pino de saída do servo
   
 digitalWrite(trig, LOW);                     //Pino de trigger inicia em low
   
 servo1.write(80);                            //Centraliza servo
  
 delay(500);                                  //Aguarda meio segundo antes de iniciar

} //end setup


// --- Loop Infinito ---
void loop() 
{
  

  speedAdjust();
  
  robotAhead();
  
  dist_cm = measureDistance();
  
  if(dist_cm < 20) //distância menor que 20 cm?
  {
      decision();
  }

   
   delay(100);
  

} //end loop


// --- Desenvolvimento das Funções Auxiliares ---


float measureDistance()                       //Função que retorna a distância em centímetros
{
  float pulse;                                //Armazena o valor de tempo em µs que o pino echo fica em nível alto
        
  trigPulse();                                //Envia pulso de 10µs para o pino de trigger do sensor
  
  pulse = pulseIn(echo, HIGH);                //Mede o tempo em que echo fica em nível alto e armazena na variável pulse
    
  /*
    >>> Cálculo da Conversão de µs para cm:
    
   Velocidade do som = 340 m/s = 34000 cm/s
   
   1 segundo = 1000000 micro segundos
   
      1000000 µs - 34000 cm/s
            X µs - 1 cm
            
                  1E6
            X = ------- = 29.41
                 34000
                 
    Para compensar o ECHO (ida e volta do ultrassom) multiplica-se por 2
    
    X' = 29.41 x 2 = 58.82
 
  */
  
  return (pulse/58.82);                      //Calcula distância em centímetros e retorna o valor
  
  
} //end measureDistante


void trigPulse()                             //Função para gerar o pulso de trigger para o sensor HC-SR04
{
   digitalWrite(trig,HIGH);                  //Saída de trigger em nível alto
   delayMicroseconds(10);                    //Por 10µs ...
   digitalWrite(trig,LOW);                   //Saída de trigger volta a nível baixo

} //end trigPulse


void speedAdjust()                           //Função para ajustar velocidade dos motores
{
    adc = analogRead(pwm);                   //Lê entrada analógica e armazena valor em adc
    
    adc = map(adc, 0, 1023, 0, 255);         //Ajusta valor para variar entre 0 e 255
    
    analogWrite(EN1,adc);                    //PWM de acordo com ajuste do potenciômetro em EN1
    analogWrite(EN2,adc);                    //PWM de acordo com ajuste do potenciômetro em EN2

} //end speedAdjust


void decision()                              //Compara as distâncias e decide qual melhor caminho a seguir
{
   robotStop();                              //Para o robô
   delay(500);                               //Aguarda 500ms
   servo1.write(0);                          //Move sensor para direita através do servo
   delay(500);                               //Aguarda 500ms
   dist_right = measureDistance();           //Mede distância e armazena em dist_right
   delay(2000);                              //Aguarda 2000ms
   servo1.write(175);                        //Move sensor para esquerda através do servo
   delay(500);                               //Aguarda 500ms
   dist_left = measureDistance();            //Mede distância e armazena em dis_left
   delay(2000);                               //Aguarda 2000ms
   servo1.write(80);                         //Centraliza servo
   delay(500);
   
   if(dist_right > dist_left)                //Distância da direita maior que da esquerda?
   {                                         //Sim...
   
      robotBack();                           //Move o robô para trás
      delay(600);                            //Por 600ms
      robotRight();                          //Move o robô para direita
      delay(2000);                           //Por 2000ms
      robotAhead();                          //Move o robô para frente
   
   } //end if
   
   else                                      //Não...
   {
   
      robotBack();                           //Move o robô para trás
      delay(600);                            //Por 600ms
      robotLeft();                           //Move o robô para esquerda
      delay(2000);                            //Por 2000ms
      robotAhead();                          //Move o robô para frente
   
   } //end else
   

} //end decision


void robotAhead()                            //Move o robô para frente
{
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2,  LOW);
   digitalWrite(IN3,  LOW);
   digitalWrite(IN4, HIGH);

} //end robotAhead


void robotBack()                            //Move o robô para trás
{
   digitalWrite(EN1, HIGH);
   digitalWrite(EN2, HIGH);
   digitalWrite(IN1,  LOW);
   digitalWrite(IN2, HIGH);
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4,  LOW);

} //end robotBack


void robotRight()                            //Move o robô para direita
{
   digitalWrite(EN1, HIGH);
   digitalWrite(EN2, HIGH);
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2,  LOW);
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4,  LOW);

} //end robotRight


void robotLeft()                            //Move o robô para esquerda
{
   digitalWrite(EN1, HIGH);
   digitalWrite(EN2, HIGH);
   digitalWrite(IN1,  LOW);
   digitalWrite(IN2, HIGH);
   digitalWrite(IN3,  LOW);
   digitalWrite(IN4, HIGH);

} //end robotLeft


void robotStop()                            //Para o robô
{
   
   digitalWrite(IN1,  LOW);
   digitalWrite(IN2,  LOW);
   digitalWrite(IN3,  LOW);
   digitalWrite(IN4,  LOW);

} //end robotStop











