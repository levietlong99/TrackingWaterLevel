#include "HCSR04.h"

void HCSR04_init(){

    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

float check_distance(){

    long int duration;
    float distance;

    digitalWrite(TRIGGER_PIN, LOW);  
    delayMicroseconds(2); 
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIGGER_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration / 2) / 29.1;
    delay(100);
  
    return distance;    
}
