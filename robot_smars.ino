#include <AFMotor.h>

//Declaration des deux moteurs

AF_DCMotor MotorIzq(1);
AF_DCMotor MotorDer(2);

//Declaration de l'emeteur et recepteur du capteur ultrason

int trig = 2;
long lecture_echo;
long cm;

void setup()
{
  MotorIzq.setSpeed(255);
  MotorDer.setSpeed(255);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(A0, INPUT);
}

void reculer(){
    MotorIzq.run(FORWARD);
    MotorDer.run(BACKWARD);
    delay(1000);
    return;
}

void avancer(){
    MotorIzq.run(BACKWARD);
    MotorDer.run(FORWARD);
    return;
}

void droite(){
    MotorIzq.run(BACKWARD);
    MotorDer.run(BACKWARD);
    return;
}

void gauche(){
    MotorIzq.run(FORWARD);
    MotorDer.run(FORWARD);
    delay(1000);
    return;
}


void loop()
{
   
    //Emission d'un signal et reception 10Ms plus tard, puis conversion du temps de reception en S.
    
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    lecture_echo = pulseIn(A0,HIGH);
    cm = lecture_echo /58;

    //Condition de verification de presence d'obstacle, si rien à 10cm, le robot avance...
    
    if (cm > 10){
      avancer();
    }

    //...Sinon reculer, puis pivoter à gauche.
    
    else {
      MotorIzq.run(RELEASE);
      MotorDer.run(RELEASE);
      delay(1500);
      reculer();
      MotorIzq.run(RELEASE);
      MotorDer.run(RELEASE);
      delay(1500);
      gauche();
      MotorIzq.run(RELEASE);
      MotorDer.run(RELEASE);
      delay(1500);
    }
}
