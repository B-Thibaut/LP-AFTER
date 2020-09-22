#include <Servo.h>

//Declaration variable de fonctionnement

bool piece = false;
bool entre = false;
bool sortie = false;

//Declaration servo du bras

Servo pince;
Servo bras1;
Servo bras2;
Servo rotation;

void setup() {
  
  //Declaration des capteurs infrarouge.
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  //Assignation des sortie aux moteurs.
  
  pince.attach(6);
  bras1.attach(9);
  bras2.attach(10);
  rotation.attach(11);

  //mise en position initial du bras.
  
  rotation.write(90);
  bras2.write(60);
  pince.write(100);
}

void loop() {

  //Lecture de la présence de produit dans l'entrée par le detecteur infrarouge.
  
  entre = digitalRead(2);

  //Si il y a présence de produit(entre) et que la pince est vide(piece) alors le bras viens chercher la piece et se met en position neutre.
  
  if (entre == false && piece == false){
  
    for(int i=90; i <= 145; i++){
      rotation.write(i);
      delay(20);
    }
    
    for(int i=100; i <= 160; i++){
      bras1.write(i);
      delay(20);
    }
  
    delay(500);
    pince.write(180); 
    delay(500);
    piece = true;
    for(int i=160; i >= 100; i--){
      bras1.write(i);
      delay(20);
    }
    for(int i=145; i >= 65; i--){
      rotation.write(i);
      delay(20);
    }  
  
  }

  //Si il y a absence de produit(sortie) à la place de reception et que la pince est pleine(piece) alors le bras viens deposer la piece et se met en position neutre.
  
  sortie = digitalRead(3);
  if (sortie == true && piece == true){  
    for(int i=100; i <= 160; i++){
      bras1.write(i);
      delay(20);
    }
  
    delay(500);
    pince.write(100); 
    delay(500);
    piece = false;
    for(int i=160; i >= 100; i--){
      bras1.write(i);
      delay(20);
    }
  
    for(int i=65; i <= 90; i++){
      rotation.write(i);
      delay(20);
    }
  }
}
