#include "HC_SR04.h"

namespace HC_SR04 {

HC_SR04::HC_SR04(byte triggerPin, byte echoPin, float temperature, uint16_t maxDistance)
    : triggerPin(triggerPin)
    , echoPin(echoPin)
    , temperature(temperature)
    , maxDistance(maxDistance)
{
    interruptsAttached = false;
    interruptStart = 0;
    interruptDuration = 0;
    interruptNewMessurement = false;
    
    speedOfSound = (331.3 + 0.606 * temperature) * 100 * 1000000; // in cm/µs for temperatures between 0°C and 100°C
    timeout = TIMEOUT_THRESHOLD * maxDistance / speedOfSound;

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(10);
}

HC_SR04::~HC_SR04()
{
    if (interruptsAttached) {
        detachInterrupt();
    }
}

void HC_SR04::attachInterrupt()
{
    interruptsAttached = true;
    attachInterrupt(digitalPinToInterrupt(echoPin), HC_SR04::interruptStartMessurement, RISING);
    attachInterrupt(digitalPinToInterrupt(echoPin), HC_SR04::interruptEndMessurement, FALLING);
}

void HC_SR04::detachInterrupt()
{
    interruptsAttached = false;
    detachInterrupt(digitalPinToInterrupt(echoPin));
    detachInterrupt(digitalPinToInterrupt(echoPin));
}

void HC_SR04::interruptStartMessurement()
{
    interruptStart = micros();
}

void HC_SR04::interruptEndMessurement()
{
    interruptDuration = micros() - interruptStart;
    interruptNewMessurement = true;
}

void HC_SR04::startInterruptMessurement()
{
    interruptNewMessurement = false;

    if (!interruptsAttached) {
        attachInterrupt();
    }

    // Initiate messurement
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(INITIATE_MESSUREMENT_PULS_DURATION);
    digitalWrite(triggerPin, LOW);
}

float HC_SR04::getInterruptMessurement()
{
    interruptNewMessurement = false;
    return interruptDuration / 2.0f * speedOfSound;
}

boolean HC_SR04::hasNewInterruptMessurement()
{
    return interruptNewMessurement;
}

float HC_SR04::measure()
{
    // Initiate messurement
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(INITIATE_MESSUREMENT_PULS_DURATION);
    digitalWrite(triggerPin, LOW);

    return pulseIn(echoPin, HIGH, timeout) / 2.0 * speedOfSound;
}

float HC_SR04::measure(float _temperature)
{
    temperature = _temperature;
    speedOfSound = (331.3 + 0.606 * temperature) * 100 * 1000000; // in cm/µs for temperatures between 0°C and 100°C
    timeout = TIMEOUT_THRESHOLD * maxDistance / speedOfSound;

    // Initiate messurement
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(INITIATE_MESSUREMENT_PULS_DURATION);
    digitalWrite(triggerPin, LOW);

    return pulseIn(echoPin, HIGH, timeout) / 2.0 * speedOfSound;
}

} // namespace HC_SR04
