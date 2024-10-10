#include <AccelStepper.h>
#include <Servo.h>

int state_button;
const int relaypin = PD_5;
const int ledpin_off = PH_8;
const int ledpin_on = PB_7;

// Definieer de stappenmotoren (Type 1 = DRIVER)
AccelStepper stepperX(AccelStepper::DRIVER, PD_4, PC_15);  // X-as motor: Step op pin 162, Direction op pin 161
AccelStepper stepperY(AccelStepper::DRIVER, PG_10, PG_3);  // Y-as motor: Step op pin 166, Direction op pin 165

// Definieer de servomotoren
Servo servo1;
Servo servo2;
Servo servo3;

// Definieer de limit switches
const int limitSwitchA_A = PG_7;     // Limit switch voor oplader A batterij A op pin 186
const int limitSwitchA_B = PJ_11;     // Limit switch voor oplader A batterij B op pin 187
const int limitSwitchB_A = PK_1;     // Limit switch voor oplader B batterij A op pin 188
const int limitSwitchB_B = PH_15;     // Limit switch voor oplader B batterij B op pin 189
const int limitSwitchX_Min = PJ_7;   // Limit switch voor X-as min op pin 190
const int limitSwitchX_Max = PJ_10;   // Limit switch voor X-as max op pin 191
const int limitSwitchY = PH_6;       // Limit switch voor Y-as op pin 192

void setup() {
  // Start de seriële communicatie
  Serial.begin(9600);

  // Stel limit switches in als input
  pinMode(limitSwitchA_A, INPUT_PULLDOWN);  // Gebruik een interne pull-down weerstand
  pinMode(limitSwitchA_B, INPUT_PULLDOWN);
  pinMode(limitSwitchB_A, INPUT_PULLDOWN);
  pinMode(limitSwitchB_B, INPUT_PULLDOWN);
  pinMode(limitSwitchX_Min, INPUT_PULLDOWN);  
  pinMode(limitSwitchX_Max, INPUT_PULLDOWN);
  pinMode(limitSwitchY, INPUT_PULLDOWN);

  // Stel led in als output
  pinMode(ledpin_off, OUTPUT);
  pinMode(ledpin_on, OUTPUT);

  // Stel relais in als output
  pinMode(relaypin, OUTPUT);

  // Zet het relais uit (laag signaal)
  digitalWrite(relaypin, LOW);

  // Stel maximale snelheid en acceleratie in voor beide motoren
  stepperX.setMaxSpeed(150);  
  stepperX.setAcceleration(10);  

  stepperY.setMaxSpeed(150);  
  stepperY.setAcceleration(10);  

  // Servomotoren koppelen aan pinnen
  servo1.attach(PA_8);  // Servo 1 op pin 183
  servo2.attach(PC_6);  // Servo 3 op pin 184
  servo3.attach(PC_7);  // Servo 3 op pin 185

  // Zet de servomotoren in de neutrale positie (90 graden)
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);

  // Homing (nulpositie bepalen)
  homeMotors();
}

// Functie voor het bewegen van de X-as tot de limit switch wordt bereikt
void homeMotors() {
  // Zet het relais aan (hoog signaal)
  digitalWrite(relaypin, HIGH);

  // Beweeg de X-as naar links totdat de limit switch wordt geactiveerd
  while (digitalRead(limitSwitchX_Min) == LOW) {
    stepperX.setSpeed(-20);  // Beweeg naar links (negatieve snelheid)
    stepperX.runSpeed();
  }
  stepperX.setCurrentPosition(0);  // Stel de huidige positie in als 0

  // Beweeg de Y-as naar links totdat de limit switch wordt geactiveerd
  while (digitalRead(limitSwitchY) == LOW) {
    stepperY.setSpeed(-20);  // Beweeg naar links (negatieve snelheid)
    stepperY.runSpeed();
  }
  stepperY.setCurrentPosition(0);  // Stel de huidige positie in als 0

  // Beweeg naar start positie
  beweegX_start(10); // beweeg naar 10 stappen
}

// Functie voor het bewegen van de X-as naar oplader 1
void beweegX_start(int doelpositieX) {
  stepperX.moveTo(doelpositieX);  // Beweeg naar de doelpositie op de X-as
  
  // Beweeg de X-as totdat het doel is bereikt
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();
  }
}

// Functie voor het bewegen van de X-as naar oplader 1
void beweegX_oplader1(int doelpositieX) {
  stepperX.moveTo(doelpositieX);  // Beweeg naar de doelpositie op de X-as
  
  // Beweeg de X-as totdat het doel is bereikt
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();
  }
}

// Functie voor het bewegen van de X-as naar oplader 2
void beweegX_oplader2(int doelpositieX) {
  stepperX.moveTo(doelpositieX);  // Beweeg naar de doelpositie op de X-as
  
  // Beweeg de X-as totdat het doel is bereikt
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();
  }
}

// Functie voor het bewegen van de X-as naar dronekop
void beweegX_drone(int doelpositieX) {
  stepperX.moveTo(doelpositieX);  // Beweeg naar de doelpositie op de X-as
  
  // Beweeg de X-as totdat het doel is bereikt
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();
  }
}

// Functie voor het bewegen van de Y-as om naar stelling te bewegen
void beweegY_min(int doelpositieY) {
  stepperY.moveTo(doelpositieY);  // Beweeg naar de doelpositie op de Y-as
  
  // Beweeg de Y-as totdat het doel is bereikt
  while (stepperY.distanceToGo() != 0) {
    stepperY.run();
  }
}

// Functie voor het bewegen van de Y-as om naar systeem te bewegen
void beweegY_max(int doelpositieY) {
  stepperY.moveTo(doelpositieY);  // Beweeg naar de doelpositie op de Y-as
  
  // Beweeg de Y-as totdat het doel is bereikt
  while (stepperY.distanceToGo() != 0) {
    stepperY.run();
  }
}

// Functie voor het bewegen van Servo's grijpers naar binnen
void beweegServogrijpers_binnen() {
  servo1.write(180);  // Draai Servo 1 naar 180 graden (90 graden vooruit)
  servo1.write(0);  // Breng Servo 2 terug naar 0 graden (90 graden achteruit)
  delay(1000);  // Wacht 1 seconde
}

// Functie voor het bewegen van Servo's grijpers naar buiten
void beweegServogrijpers_buiten() {
  servo1.write(90);  // Draai Servo 1 naar 90 graden (90 graden achteruit)
  servo1.write(90);  // Breng Servo 2 terug naar 90 graden (90 graden vooruit)
  delay(1000);  // Wacht 1 seconde
}

// Functie voor het bewegen van Servo voor ontgrendelen accu's
void beweegServo_ontgrendelen() {
  servo3.write(180);  // Draai Servo 3 naar 180 graden (90 graden vooruit)
  delay(1000);  // Wacht 1 seconde
}

// Functie voor het bewegen van Servo voor vergrendelen accu's
void beweegServo_vergrendelen() {
  servo3.write(90);  // Draai Servo 3 naar 90 graden (90 graden achteruit)
  delay(1000);  // Wacht 1 seconde
}

void loop() {
  // In het hoofdprogramma kun je de bewegingen oproepen nadat de motoren zijn gehomed
  // Vraag de gebruiker om het programma te starten
  Serial.println("Druk op 1 en dan op Enter om het programma te starten.");
  Serial.println("Druk op 0 om en op Enter om het programma te stoppen:");
  
  // Controleer of er gegevens beschikbaar zijn
  if (Serial.available() > 0) {
    // Lees de binnenkomende gegevens als een string
    String inputString = Serial.readStringUntil('\n');

    // Converteer de string naar een integer
    int state_button = inputString.toInt();
  }

  if(state_button == 1){
    digitalWrite(ledpin_off, LOW);
    digitalWrite(ledpin_on, HIGH);
   
    // Beweeg naar de drone
    beweegX_drone(100); // Beweeg naar 100 stappen
    delay(1000); // Wacht 1 seconde
    
    // Haal de batterij uit de drone
    beweegY_max(50); // Beweeg naar 50 stappen
    delay(1000); // Wacht 1 seconde
    beweegServogrijpers_binnen();
    delay(1000); // Wacht 1 seconde
    beweegServo_ontgrendelen();
    delay(1000); // Wacht 1 seconde
    beweegY_min(5); // Beweeg naar 5 stappen
    delay(1000);  // Wacht 1 seconde

    // Detecteer welke oplader leeg is
    if(limitSwitchA_A == HIGH && limitSwitchA_B == HIGH){
      // Breng de batterijen naar de lege oplader
      beweegX_oplader2(150); // Beweeg naar 150 stappen
      delay(1000); // Wacht 1 seconde

      // Zet de lege batterijen in de oplader
      beweegY_max(50); // Beweeg naar 50 stappen
      delay(1000); // Wacht 1 seconde
      beweegServogrijpers_buiten();
      delay(1000); // Wacht 1 seconde
      beweegY_min(5); // Beweeg naar 5 stappen
      delay(1000);  // Wacht 1 seconde

      // Beweeg naar de volle batterijen
      beweegX_oplader1(50); // Beweeg naar 50 stappen
      delay(1000); // Wacht 1 seconde

      // Pak de volle batterijen uit de oplader
      beweegY_max(50); // Beweeg naar 50 stappen
      delay(1000); // Wacht 1 seconde
      beweegServogrijpers_binnen();
      delay(1000); // Wacht 1 seconde
      beweegY_min(5); // Beweeg naar 5 stappen
      delay(1000); // Wacht 1 seconde
    }

    else if(limitSwitchB_A == HIGH && limitSwitchB_B == HIGH){
      // Breng de batterijen naar de lege oplader
      beweegX_oplader1(50); // Beweeg naar 50 stappen
      delay(1000); // Wacht 1 seconde

      // Zet de lege batterijen in de oplader
      beweegY_max(50); // Beweeg naar 50 stappen
      delay(1000); // Wacht 1 seconde
      beweegServogrijpers_buiten();
      delay(1000); // Wacht 1 seconde
      beweegY_min(5); // Beweeg naar 5 stappen
      delay(1000); // Wacht 1 seconde

      // Beweeg naar de volle batterijen
      beweegX_oplader2(150); // Beweeg naar 150 stappen
      delay(1000); // Wacht 1 seconde

      // Pak de volle batterijen uit de oplader
      beweegY_max(50); // Beweeg naar 50 stappen
      delay(1000); // Wacht 1 seconde
      beweegServogrijpers_binnen();
      delay(1000); // Wacht 1 seconde
      beweegY_min(5); // Beweeg naar 5 stappen
      delay(1000); // Wacht 1 seconde
    }

    // Beweeg naar de drone
    beweegX_drone(100); // Beweeg naar 100 stappen
    delay(1000); // Wacht 1 seconde

    // Zet de volle batterijen in de drone
    beweegY_max(50); // Beweeg naar 50 stappen
    delay(1000); // Wacht 1 seconde
    beweegServo_vergrendelen();
    delay(1000); // Wacht 1 seconde
    beweegServogrijpers_buiten();
    delay(1000); // Wacht 1 seconde
    beweegY_min(5); // Beweeg naar 5 stappen
    delay(1000); // Wacht 1 seconde   

    // Beweeg naar start positie
    beweegX_start(10); // Beweeg naar 10 stappen
    state_button = 0;
  }

  else{
    digitalWrite(ledpin_off, HIGH);
    digitalWrite(ledpin_on, LOW);

  }      
  
}

