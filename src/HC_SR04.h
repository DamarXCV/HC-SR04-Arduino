#pragma once

#include <Arduino.h>

#define INITIATE_MESSUREMENT_PULS_DURATION 10
#define TIMEOUT_THRESHOLD 1.33f

class HC_SR04 {
private:
    uint8_t triggerPin;
    uint8_t echoPin;

    float temperature;
    float speedOfSound;

    uint16_t maxDistance;
    uint16_t timeout;

    void startInterrupt();
    void endInterrupt();

    static void interruptMessure();

    static boolean pinState;

    static boolean interruptsAttached;
    static uint32_t interruptStart;
    static uint32_t interruptDuration;
    static boolean interruptNewMessurement;

public:
    HC_SR04(uint8_t triggerPin, uint8_t echoPin, float temperature = 20.0f, uint16_t maxDistance = 400);
    ~HC_SR04();

    float measure();
    float measure(float _temperature);

    void startInterruptMessurement();
    float getInterruptMessurement();
    boolean hasNewInterruptMessurement();
};
