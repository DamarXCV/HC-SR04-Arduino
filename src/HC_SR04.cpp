#include "HC_SR04.h"

namespace HC_SR04 {

boolean HC_SR04::pinState;

boolean HC_SR04::interruptsAttached;
uint32_t HC_SR04::interruptStart;
uint32_t HC_SR04::interruptDuration;
boolean HC_SR04::interruptNewMessurement;

HC_SR04::HC_SR04(byte triggerPin, byte echoPin, float temperature, uint16_t maxDistance)
    : triggerPin(triggerPin)
    , echoPin(echoPin)
    , temperature(temperature)
    , maxDistance(maxDistance)
{
    pinState = false;

    interruptsAttached = false;
    interruptStart = 0;
    interruptDuration = 0;
    interruptNewMessurement = false;

    speedOfSound = (331.3f + 0.606f * temperature) * 100 / 1000000; // in cm/µs for temperatures between 0°C and 100°C
    timeout = TIMEOUT_THRESHOLD * maxDistance * 2 / speedOfSound;

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(10);
}

HC_SR04::~HC_SR04()
{
    if (interruptsAttached) {
        endInterrupt();
    }
}

void HC_SR04::startInterrupt()
{
    interruptsAttached = true;
    attachInterrupt(digitalPinToInterrupt(echoPin), HC_SR04::interruptMessure, CHANGE);

    pinState = digitalRead(echoPin);
}

void HC_SR04::endInterrupt()
{
    interruptsAttached = false;
    detachInterrupt(digitalPinToInterrupt(echoPin));
}

void HC_SR04::interruptMessure()
{
    pinState = !pinState;

    if (pinState) {
        interruptStart = micros();
    } else {
        interruptDuration = micros() - interruptStart;
        interruptNewMessurement = true;
    }
}

void HC_SR04::startInterruptMessurement()
{
    interruptNewMessurement = false;

    if (!interruptsAttached) {
        startInterrupt();
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

    return pulseIn(echoPin, HIGH, timeout) / 2.0f * speedOfSound;
}

float HC_SR04::measure(float _temperature)
{
    temperature = _temperature;
    speedOfSound = (331.3f + 0.606f * temperature) * 100 / 1000000; // in cm/µs for temperatures between 0°C and 100°C
    timeout = TIMEOUT_THRESHOLD * maxDistance * 2 / speedOfSound;

    // Initiate messurement
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(INITIATE_MESSUREMENT_PULS_DURATION);
    digitalWrite(triggerPin, LOW);

    return pulseIn(echoPin, HIGH, timeout) / 2.0f * speedOfSound;
}

} // namespace HC_SR04
