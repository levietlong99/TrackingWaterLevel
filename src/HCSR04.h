#ifndef HCSR04_H_
#define HCSR04_H_

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#define TRIGGER_PIN 0
#define ECHO_PIN    2

void HCSR04_init();
float check_distance();

#endif 
