#include <Arduino.h>
#include "ESP32_Servo.h"
#include "HX711.h"
#include "DFRobotDFPlayerMini.h"
#include <Stepper.h>
// 34 y 39 no se usan
#define pasosDisp 200
#define boya 26
#define frecAgua 3000
#define pinServo 33
#define pinMotor 25
#define LED_BUILTIN 2


u_long t_ant = 0;
u_long t_ant2 = 0;
uint16_t frecuenciaPeso = 10000;
long peso = 0;
int comando = 0;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 35;
const int LOADCELL_SCK_PIN = 32;

DFRobotDFPlayerMini parlante;
HX711 balanza;
Servo compuerta;
Stepper dispensador(pasosDisp, 27, 14, 12, 13);

void setup() {
  Serial.begin(115200);
  dispensador .setSpeed(60);
  Serial.println("Setup 1");
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(boya, INPUT_PULLUP);
  pinMode(pinMotor, OUTPUT);
  Serial.println("Setup 3");
  parlante.begin(Serial1,0,0);
  Serial.println("INIT");
  parlante.volume(30);
  Serial.println("Ayrton 1");
  compuerta.attach(pinServo, 500, 2400);
  Serial.println("Ayrton 2");
  compuerta.write(90);
  Serial.println("Ayrton 4");
  balanza.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println("Ayrton 5");
  balanza.tare(10); // Poner balanza en 0
  Serial.println("Ayrton 6");
  balanza.set_scale(-95); // Poner en gramos
  Serial.println("Ayrton 7");
  t_ant = millis();
}

void loop() {
  if (millis() > t_ant + frecAgua) {
    if (digitalRead(boya) == 0){
      Serial.print("Boya ");
      Serial.println(1);
    } else {
      Serial.print("Boya ");
      Serial.println(2);
    }
    t_ant = millis();
  }
  if (millis() > t_ant2 + frecuenciaPeso) {
    if (balanza.wait_ready_timeout(1000)) {
      peso = balanza.get_units(1);
      Serial.println(peso);
      if (peso < 30) {
        frecuenciaPeso = 700;
        compuerta.write(0);
        digitalWrite(pinMotor, 1);
      } else if (peso > 200) {
        compuerta.write(90);
        digitalWrite(pinMotor, 0);
        frecuenciaPeso = 10000;
      }
    }
    t_ant2 = millis();
  }
  if (Serial2.available()) {
    int in = Serial2.parseInt();
    Serial.print("comando -> ");
    Serial.println(in);
    switch (in)
    {
    case 1: // No premio
      break;
    case 2: // Premio
      dispensador.step(pasosDisp);
      break;
    case 3: // Comida
      frecuenciaPeso = 2000;
      compuerta.write(0);
      digitalWrite(pinMotor, 1);
      break;
    case 4: // Musica
      parlante.play();
      break;
    case 5: // Mute musica
      parlante.pause();
      break;
    Serial.println(in);
    in = 0;
    }
  }
}
