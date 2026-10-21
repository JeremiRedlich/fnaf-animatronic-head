#include <Adafruit_MotorShield.h>
#include <Servo.h>

// --- KONFIGURACJA SPRZĘTU I PINÓW ---
const int PIN_LED_TRYB = 2;       // Czerwona dioda LED umieszczona w głowie
const int PIN_SERWO_OCZY = 4;     // Sterowanie ruchem oczu (PWM)
const int PIN_SERWO_UCHO_P = 5;   // Sterowanie prawym uchem (PWM)
const int PIN_SERWO_SZCZEKA = 6;  // Sterowanie otwarciem szczęki (PWM)
const int PIN_SERWO_UCHO_L = 7;   // Sterowanie lewym uchem (PWM)
const int PIN_PRZYCISK_START = 8; // Przycisk aktywacji ruchu
const int PIN_PRZYCISK_TRYB = 9;  // Przycisk wyboru trybu (Normalny / Agresywny)

// --- OBIEKTY SILNIKÓW I SERW ---
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Silnik krokowy 28BYJ-48 (przełożenie/kroki: 48 na obrót w bibliotece, port M3/M4)
Adafruit_StepperMotor *glowa = AFMS.getStepper(48, 2);

Servo uchoP;
Servo uchoL;
Servo szczeka;
Servo oczy;

int losowaLiczba; // Zmienna do losowania sekwencji ruchów

// --- DEKLARACJE FUNKCJI ---
void trybNormalny();
void trybAgresywny();

void setup() {
  // Inicjalizacja nakładki Adafruit Motor Shield v2
  if (!AFMS.begin()) {
    while (1); // Błąd inicjalizacji - zatrzymaj program
  }
  
  glowa->setSpeed(1000); // Ustawienie prędkości obrotu głowy

  // Przypisanie pinów PWM do serwomechanizmów
  oczy.attach(PIN_SERWO_OCZY);
  szczeka.attach(PIN_SERWO_SZCZEKA);
  uchoL.attach(PIN_SERWO_UCHO_L);
  uchoP.attach(PIN_SERWO_UCHO_P);

  // Konfiguracja pinów wejściowych i wyjściowych
  pinMode(PIN_PRZYCISK_START, INPUT);
  pinMode(PIN_PRZYCISK_TRYB, INPUT);
  pinMode(PIN_LED_TRYB, OUTPUT);
}

void loop() {
  // Pozycja spoczynkowa / bazowa animatronika
  uchoL.write(90);
  uchoP.write(90);
  szczeka.write(110);
  oczy.write(30);

  // Sprawdzenie stanu przycisków: START wciśnięty, TRYB wyłączony -> Tryb Normalny
  if ((digitalRead(PIN_PRZYCISK_START) == HIGH) && (digitalRead(PIN_PRZYCISK_TRYB) == LOW)) {
    digitalWrite(PIN_LED_TRYB, LOW);
    losowaLiczba = random(5);
    trybNormalny();
  }

  // Sprawdzenie stanu przycisków: START wciśnięty, TRYB wciśnięty -> Tryb Agresywny
  if ((digitalRead(PIN_PRZYCISK_START) == HIGH) && (digitalRead(PIN_PRZYCISK_TRYB) == HIGH)) {
    digitalWrite(PIN_LED_TRYB, HIGH);
    losowaLiczba = random(4);
    trybAgresywny();
  }

  // Powrót do pozycji spoczynkowej po wykonaniu sekwencji
  uchoL.write(90);
  uchoP.write(90);
  szczeka.write(110);
  oczy.write(30);
}

// --- SEKWENCJE RUCHÓW: TRYB NORMALNY ---
void trybNormalny() {
  if (losowaLiczba == 0) {
    oczy.write(140);
    for (int i = 0; i < 3; i++) {
      uchoL.write(25);
      delay(150);
      uchoL.write(90);
      delay(150);
    }
  } 
  else if (losowaLiczba == 1) {
    oczy.write(140);
    uchoL.write(25);
    uchoP.write(25);
    glowa->step(150, FORWARD, DOUBLE);
    for (int i = 0; i < 3; i++) {
      szczeka.write(150);
      delay(100);
      szczeka.write(120);
      delay(100);
    }
    delay(200);
    uchoL.write(90);
    uchoP.write(90);
    glowa->step(150, BACKWARD, DOUBLE);
  } 
  else if (losowaLiczba == 2) {
    oczy.write(140);
    uchoL.write(25);
    uchoP.write(25);
    glowa->step(250, BACKWARD, DOUBLE);
    for (int i = 0; i < 3; i++) {
      szczeka.write(150);
      uchoP.write(90);
      oczy.write(140);
      delay(150);
      szczeka.write(120);
      uchoP.write(25);
      oczy.write(30);
      delay(100);
    }
    delay(50);
    uchoL.write(90);
    uchoP.write(90);
    glowa->step(250, FORWARD, DOUBLE);
  } 
  else if (losowaLiczba == 3) {
    oczy.write(140);
    uchoL.write(25);
    uchoP.write(25);
    oczy.write(30);
    glowa->step(300, BACKWARD, DOUBLE);
    delay(200);
    szczeka.write(150);
    uchoL.write(90);
    uchoP.write(90);
    delay(200);
    szczeka.write(110);
    uchoL.write(25);
    uchoP.write(25);
    glowa->step(600, FORWARD, DOUBLE);
    szczeka.write(150);
    delay(200);
    szczeka.write(110);
    uchoL.write(90);
    uchoP.write(90);
    glowa->step(300, BACKWARD, DOUBLE);
  } 
  else if (losowaLiczba == 4) {
    oczy.write(140);
    uchoL.write(25);
    oczy.write(30);
    glowa->step(150, BACKWARD, DOUBLE);
    delay(200);
    szczeka.write(150);
    uchoL.write(90);
    uchoP.write(25);
    delay(200);
    glowa->step(300, FORWARD, DOUBLE);
    delay(200);
    szczeka.write(110);
    for (int i = 0; i < 3; i++) {
      glowa->step(75, BACKWARD, DOUBLE);
      delay(200);
      glowa->step(75, FORWARD, DOUBLE);
      delay(200);
    }
    glowa->step(150, BACKWARD, DOUBLE);
    szczeka.write(150);
    oczy.write(30);
    delay(200);
    szczeka.write(110);
    uchoL.write(25);
    delay(300);
    uchoL.write(90);
    uchoP.write(90);
  }
}

// --- SEKWENCJE RUCHÓW: TRYB AGRESYWNY ---
void trybAgresywny() {
  if (losowaLiczba == 0) {
    glowa->step(350, FORWARD, DOUBLE);
    for (int i = 0; i < 5; i++) {
      uchoP.write(25);
      uchoL.write(25);
      oczy.write(150);
      szczeka.write(150);
      glowa->step(50, FORWARD, DOUBLE);
      delay(50);
      uchoP.write(90);
      uchoL.write(90);
      oczy.write(30);
      szczeka.write(110);
      glowa->step(50, BACKWARD, DOUBLE);
    }
    glowa->step(300, BACKWARD, DOUBLE);
  } 
  else if (losowaLiczba == 1) {
    glowa->step(350, BACKWARD, DOUBLE);
    oczy.write(150);
    for (int i = 0; i < 6; i++) {
      uchoP.write(25);
      uchoL.write(90);
      szczeka.write(150);
      glowa->step(75, FORWARD, DOUBLE);
      delay(50);
      uchoP.write(90);
      uchoL.write(25);
      szczeka.write(110);
      glowa->step(75, BACKWARD, DOUBLE);
    }
    glowa->step(300, FORWARD, DOUBLE);
    oczy.write(30);
  } 
  else if (losowaLiczba == 2) {
    uchoP.write(25);
    for (int i = 0; i < 4; i++) {
      glowa->step(75, FORWARD, DOUBLE);
      oczy.write(150);
      uchoL.write(25);
      delay(50);
      glowa->step(75, BACKWARD, DOUBLE);
      oczy.write(30);
      uchoL.write(90);
      delay(50);
    }
    uchoP.write(90);
    for (int i = 0; i < 5; i++) {
      szczeka.write(150);
      uchoL.write(45);
      uchoP.write(40);
      delay(50);
      szczeka.write(110);
      uchoL.write(90);
      uchoP.write(90);
    }
  } 
  else if (losowaLiczba == 3) {
    oczy.write(150);
    for (int i = 0; i < 5; i++) {
      szczeka.write(150);
      uchoL.write(45);
      uchoP.write(40);
      delay(50);
      szczeka.write(110);
      uchoL.write(90);
      uchoP.write(90);
    }
    oczy.write(30);
    delay(50);
    oczy.write(150);
  }
}
