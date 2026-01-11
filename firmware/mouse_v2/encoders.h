#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder
{
private:
    uint8_t pinA;
    uint8_t pinB;

    volatile long ticks;

    // Internal ISR handler (called from static ISR)
    void handleISR();

public:
    Encoder(uint8_t pinA, uint8_t pinB);

    void init();
    void reset();

    long getTicks();

    // ISR glue
    static void isr0();
    static void isr1();

    // Instances bound to ISRs
    static Encoder* instance0;
    static Encoder* instance1;
};

#endif
