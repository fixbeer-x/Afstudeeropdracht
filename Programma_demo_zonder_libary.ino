#include "Portenta_H7_ISR_Servo.h"

// const int ledpin_off = PH_8;
// const int ledpin_on = PB_7;
int state_button = 0;
// Definieer de stappenmotoren pinnen
#define STEP_PIN_X PD_4
#define DIR_PIN_X PC_15
#define STEP_PIN_Y PE_3
#define DIR_PIN_Y PD_5

//Definieer de servo motoren
#define SERVO_PIN_1       D4
#define SERVO_PIN_2       D5
#define SERVO_PIN_3       D6

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS      800  // Minimum pulse width in microseconds (1 ms)
#define MAX_MICROS      2450 // Maximum pulse width in microseconds (2.45 ms)

// Timer to use for ISR
#define USE_PORTENTA_H7_TIMER_NO TIM15

//Defineer positie pinnen
int positie_x;
int positie_y;

// Definieer de servomotoren
int servo1Index;
int servo2Index;
int servo3Index;

// Definieer de limit switches
#define limitSwitchA_A PK_1       // Limit switch voor oplader A batterij A op pin 186
#define limitSwitchA_B PJ_11      // Limit switch voor oplader A batterij B op pin 187
#define limitSwitchB_A PH_15       // Limit switch voor oplader B batterij A op pin 188
#define limitSwitchB_B PG_7      // Limit switch voor oplader B batterij B op pin 189
#define limitSwitchX_Min PJ_7     // Limit switch voor X-as min op pin 190
#define limitSwitchX_Max PJ_10    // Limit switch voor X-as max op pin 191
#define limitSwitchY PH_6         // Limit switch voor Y-as op pin 192

void setup() {
  // Start de seriële communicatie
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Starting Portenta_H7_MultipleServos");

  // Select Portenta timer
  Portenta_H7_ISR_Servos.useTimer(USE_PORTENTA_H7_TIMER_NO);

  // Setup servos
  servo1Index = Portenta_H7_ISR_Servos.setupServo(SERVO_PIN_1, MIN_MICROS, MAX_MICROS);
  servo2Index = Portenta_H7_ISR_Servos.setupServo(SERVO_PIN_2, MIN_MICROS, MAX_MICROS);
  servo3Index = Portenta_H7_ISR_Servos.setupServo(SERVO_PIN_3, MIN_MICROS, MAX_MICROS);

  if (servo1Index == -1 || servo2Index == -1 || servo3Index == -1) {
    Serial.println("Servo setup failed!");
    while (true); // Stop if setup fails
  }

  // Stel limit switches in als input
  pinMode(limitSwitchA_A, INPUT_PULLUP);  // Gebruik een interne pull-up weerstand
  pinMode(limitSwitchA_B, INPUT_PULLUP);
  pinMode(limitSwitchB_A, INPUT_PULLUP);
  pinMode(limitSwitchB_B, INPUT_PULLUP);
  pinMode(limitSwitchX_Min, INPUT_PULLUP);  
  pinMode(limitSwitchX_Max, INPUT_PULLUP);
  pinMode(limitSwitchY, INPUT_PULLUP);

  // Stel led in als output
  // pinMode(ledpin_off, OUTPUT);
  // pinMode(ledpin_on, OUTPUT);

  // Homing (nulpositie bepalen)
  homeMotors();

  // Vraag de gebruiker om het programma te starten
  Serial.println("Druk op 1 en dan op Enter om het programma te starten.");
  Serial.println("Druk op 0 om en op Enter om het programma te stoppen:");
}

// Functie voor het bewegen van de X-as tot de limit switch wordt bereikt
void homeMotors() {
  
  Portenta_H7_ISR_Servos.setPosition(servo1Index, 90);
  Portenta_H7_ISR_Servos.setPosition(servo2Index, 90);
  Portenta_H7_ISR_Servos.setPosition(servo3Index, 0);

  // Beweeg de Y-as naar links totdat de limit switch wordt geactiveerd
  while (digitalRead(limitSwitchY) == HIGH) {
      digitalWrite(DIR_PIN_Y,HIGH); //Changes the rotations direction
      digitalWrite(STEP_PIN_Y,HIGH);
      delayMicroseconds(500);
      digitalWrite(STEP_PIN_Y,LOW);
      delayMicroseconds(500);
  }
  positie_y = 0;  // Stel de huidige positie in als 0

  // Beweeg naar start positie
   for(int x = 0; x < 1500; x++) {
    digitalWrite(DIR_PIN_Y,LOW);
    digitalWrite(STEP_PIN_Y,HIGH); 
    delayMicroseconds(500);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN_Y,LOW); 
    delayMicroseconds(500); 
  }
  
  // Beweeg de X-as naar links totdat de limit switch wordt geactiveerd
  while (digitalRead(limitSwitchX_Min) == HIGH) {
      digitalWrite(DIR_PIN_X,LOW); //Changes the rotations direction
      digitalWrite(STEP_PIN_X,HIGH);
      delayMicroseconds(500);
      digitalWrite(STEP_PIN_X,LOW);
      delayMicroseconds(500);
  }
  positie_x = 0;  // Stel de huidige positie in als 0

  // Beweeg naar start positie
   for(int x = 0; x < 4500; x++) {
    digitalWrite(DIR_PIN_X,HIGH);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(500);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(500); 
  }
}

// Functie voor het bewegen van de X-as naar oplader 1
void beweegX_start_drone(int doelpositieX) {
  int currentPos = 0;
  // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
    digitalWrite(DIR_PIN_X,HIGH);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(100);
    currentPos ++;
  }
}

void beweegX_drone_oplader2(int doelpositieX) {
  int currentPos = 0;
    // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
      digitalWrite(DIR_PIN_X,HIGH);
      digitalWrite(STEP_PIN_X,HIGH); 
      delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
      digitalWrite(STEP_PIN_X,LOW); 
      delayMicroseconds(100);
      currentPos ++;
  }
}

void beweegX_oplader2_oplader1(int doelpositieX) {
  int currentPos = 0;
    // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
      digitalWrite(DIR_PIN_X,LOW);
      digitalWrite(STEP_PIN_X,HIGH); 
      delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
      digitalWrite(STEP_PIN_X,LOW); 
      delayMicroseconds(100);
      currentPos ++;
  }
}

void beweegX_oplader1_drone(int doelpositieX) {
  int currentPos = 0;
    // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
      digitalWrite(DIR_PIN_X,HIGH);
      digitalWrite(STEP_PIN_X,HIGH); 
      delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
      digitalWrite(STEP_PIN_X,LOW); 
      delayMicroseconds(100);
      currentPos ++;
  }
}

void beweegX_drone_oplader1(int doelpositieX) {
  int currentPos = 0;
    // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
      digitalWrite(DIR_PIN_X,LOW);
      digitalWrite(STEP_PIN_X,HIGH); 
      delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
      digitalWrite(STEP_PIN_X,LOW); 
      delayMicroseconds(100);
      currentPos ++;
  }
}

void beweegX_oplader1_oplader2(int doelpositieX){
  int currentPos = 0;
  // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
    digitalWrite(DIR_PIN_X,HIGH);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(100);
    currentPos ++;
  }
}

void beweegX_oplader2_drone(int doelpositieX){
  int currentPos = 0;
  // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
    digitalWrite(DIR_PIN_X,LOW);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(100);
    currentPos ++;
  }
}

void beweegX_drone_start(int doelpositieX){
  int currentPos = 0;
  // Beweeg de X-as totdat het doel is bereikt
  while(currentPos < doelpositieX && digitalRead(limitSwitchX_Max) == HIGH && digitalRead(limitSwitchX_Min) == HIGH) {
    digitalWrite(DIR_PIN_X,LOW);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(100);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(100);
    currentPos ++;
  }
}

// Functie voor het bewegen van de Y-as om naar stelling te bewegen
void beweegY_min(int doelpositieY) {
  int currentPos = 0;
  while(currentPos < doelpositieY && digitalRead(limitSwitchY) == HIGH) {
    digitalWrite(DIR_PIN_Y,HIGH);
    digitalWrite(STEP_PIN_Y,HIGH); 
    delayMicroseconds(25);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN_Y,LOW); 
    delayMicroseconds(25);
    currentPos ++;
  }
}

// Functie voor het bewegen van de Y-as om naar systeem te bewegen
void beweegY_max(int doelpositieY) {
    for(int x = 0; x < doelpositieY; x++){
      digitalWrite(DIR_PIN_Y,LOW);
      digitalWrite(STEP_PIN_Y,HIGH); 
      delayMicroseconds(25);    // by changing this time delay between the steps we can change the rotation speed
      digitalWrite(STEP_PIN_Y,LOW); 
      delayMicroseconds(25);
  }
}

// Functie voor het bewegen van Servo's grijpers naar binnen
void beweegServogrijpers_binnen(int position1, int position2) {
  Portenta_H7_ISR_Servos.setPosition(servo1Index, position1);
  delay(250); // Wacht 1 seconde
  Portenta_H7_ISR_Servos.setPosition(servo2Index, position2);
}

// Functie voor het bewegen van Servo's grijpers naar buiten
void beweegServogrijpers_buiten(int position1, int position2) {
  Portenta_H7_ISR_Servos.setPosition(servo1Index, position1);
  delay(250); // Wacht 1 seconde
  Portenta_H7_ISR_Servos.setPosition(servo2Index, position2);
}

// Functie voor het bewegen van Servo voor ontgrendelen accu's
void beweegServo_ontgrendelen(int position) { 
  Portenta_H7_ISR_Servos.setPosition(servo3Index, position);
}

// Functie voor het bewegen van Servo voor vergrendelen accu's
void beweegServo_vergrendelen(int position) {
  Portenta_H7_ISR_Servos.setPosition(servo3Index, position);
}

void loop() {
  // In het hoofdprogramma kun je de bewegingen oproepen nadat de motoren zijn gehomed

  // Converteer de string naar een integer
  int state_button = Serial.parseInt();

  if(state_button == 1){
    // digitalWrite(ledpin_off, LOW);
    // digitalWrite(ledpin_on, HIGH);
   
    //Beweeg naar de drone
    beweegX_start_drone(22220); // Beweeg naar 100 stappen
    delay(500); // Wacht 1 seconde
    
    //Haal de batterij uit de drone
    beweegY_max(169000); // Beweeg naar 50 stappen
    delay(500); // Wacht 1 seconde
    beweegServogrijpers_binnen(0, 180);
    delay(500); // Wacht 1 seconde
    beweegServo_ontgrendelen(100);
    delay(500);
    beweegY_min(169000); // Beweeg naar 5 stappen
    delay(500);  // Wacht 1 seconde

      // Detecteer welke oplader leeg is
    if(digitalRead(limitSwitchA_A) == LOW && digitalRead(limitSwitchA_B) == LOW ){
      // Breng de batterijen naar de lege oplader
      beweegX_drone_oplader2(22320); // Beweeg naar 150 stappen
      delay(500); // Wacht 1 seconde

      // Zet de lege batterijen in de oplader
      beweegY_max(169000); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde
      beweegServogrijpers_buiten(90, 90);
      delay(500); // Wacht 1 seconde
      beweegY_min(169000); // Beweeg naar 5 stappen
      delay(500);  // Wacht 1 seconde

      // Beweeg naar de volle batterijen
      beweegX_oplader2_oplader1(44590); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde

      // Pak de volle batterijen uit de oplader
      beweegY_max(169000); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde
      beweegServogrijpers_binnen(0, 180);
      delay(500); // Wacht 1 seconde
      beweegY_min(169000); // Beweeg naar 5 stappen
      delay(500); // Wacht 1 seconde

      // Beweeg naar de drone
      beweegX_oplader1_drone(22320); // Beweeg naar 100 stappen
      delay(500); // Wacht 1 seconde

      // Zet de volle batterijen in de drone
      beweegY_max(170000); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde
      beweegServogrijpers_buiten(90, 90);
      delay(500); // Wacht 1 seconde
      beweegServo_vergrendelen(0);
      delay(500); // Wacht 1 seconde
      beweegY_min(170000); // Beweeg naar 5 stappen
      delay(500); // Wacht 1 seconde   

      //Beweeg naar start positie
      beweegX_drone_start(22220); // Beweeg naar 10 stappen
      homeMotors();
      state_button = 0;
    }  
    
    else if(digitalRead(limitSwitchB_A) == LOW && digitalRead(limitSwitchB_B) == LOW){
      // Breng de batterijen naar de lege oplader
      beweegX_drone_oplader1(22220); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde

      // Zet de lege batterijen in de oplader
      beweegY_max(169000); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde
      beweegServogrijpers_buiten(90, 90);
      delay(500); // Wacht 1 seconde
      beweegY_min(169000); // Beweeg naar 5 stappen
      delay(500); // Wacht 1 seconde

      // Beweeg naar de volle batterijen
      beweegX_oplader1_oplader2(44590); // Beweeg naar 150 stappen
      delay(500); // Wacht 1 seconde

      // Pak de volle batterijen uit de oplader
      beweegY_max(169000); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde
      beweegServogrijpers_binnen(0, 180);
      delay(500); // Wacht 1 seconde
      beweegY_min(169000); // Beweeg naar 5 stappen
      delay(500); // Wacht 1 seconde

      // Beweeg naar de drone
      beweegX_oplader2_drone(22320); // Beweeg naar 100 stappen
      delay(500); // Wacht 1 seconde

      // Zet de volle batterijen in de drone
      beweegY_max(170000); // Beweeg naar 50 stappen
      delay(500); // Wacht 1 seconde
      beweegServogrijpers_buiten(90, 90);
      delay(500); // Wacht 1 seconde
      beweegServo_vergrendelen(0);
      delay(500); // Wacht 1 seconde
      beweegY_min(170000); // Beweeg naar 5 stappen
      delay(500); // Wacht 1 seconde   

      // Beweeg naar start positie
      beweegX_drone_start(22220); // Beweeg naar 10 stappen
      homeMotors();
      state_button = 0;        
    }
    state_button = 0;
  }

  // else{
  //   // digitalWrite(ledpin_off, HIGH);
  //   // digitalWrite(ledpin_on, LOW);
  // }       
}

