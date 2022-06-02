#include <Arduino.h>

#include "HC_SR04.h"

byte triggerPin = 7;
byte echoPin = 2;
HC_SR04::HC_SR04 sonar(triggerPin, echoPin);

void setup()
{
    delayMicroseconds(1000000);

    Serial.begin(115200);
    while (!Serial) { }

    Serial.println("HC_SR04 test!");

    sonar.startInterruptMessurement();
}

void loop()
{
    if (sonar.hasNewInterruptMessurement()) {
        Serial.print("Distance: ");
        Serial.print(sonar.getInterruptMessurement());
        Serial.print(" cm");

        sonar.startInterruptMessurement();
    }

    delayMicroseconds(100);
}
