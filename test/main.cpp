#include <Arduino.h>

#include "HC_SR04.h"

// #define MESSURE_VIA_INTERRUPT

byte triggerPin = 7;
byte echoPin = 2;
HC_SR04::HC_SR04 sonar(triggerPin, echoPin);

void setup()
{
    delay(3000);

    Serial.begin(115200);
    while (!Serial) { }

    Serial.println("HC_SR04 test!");

#ifdef MESSURE_VIA_INTERRUPT
    sonar.startInterruptMessurement();
#endif
}

void loop()
{
#ifdef MESSURE_VIA_INTERRUPT
    if (sonar.hasNewInterruptMessurement()) {
        Serial.print("Distance: ");
        Serial.print(sonar.getInterruptMessurement());
        Serial.println(" cm");

        sonar.startInterruptMessurement();
    }
#else
    Serial.print("Distance: ");
    Serial.print(sonar.measure());
    Serial.println(" cm");
#endif

    delay(1000);
}
