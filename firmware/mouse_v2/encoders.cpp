#include "encoders.h"

// Static instance pointers
Encoder* Encoder::instance0 = nullptr;
Encoder* Encoder::instance1 = nullptr;

// Constructor
Encoder::Encoder(uint8_t pinA, uint8_t pinB)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->ticks = 0;
}

// Init pins + attach interrupt
void Encoder::init()
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);

    // Bind encoder to correct interrupt
    if (digitalPinToInterrupt(pinA) == 0) {
        instance0 = this;
        attachInterrupt(0, Encoder::isr0, RISING);
    }
    else if (digitalPinToInterrupt(pinA) == 1) {
        instance1 = this;
        attachInterrupt(1, Encoder::isr1, RISING);
    }
}

// Core ISR logic (matches your reference code)
void Encoder::handleISR()
{
    if (digitalRead(pinB))
        ticks--;
    else
        ticks++;
}

// Static ISR wrappers
void Encoder::isr0()
{
    if (instance0)
        instance0->handleISR();
}

void Encoder::isr1()
{
    if (instance1)
        instance1->handleISR();
}

long Encoder::getTicks()
{
    noInterrupts();
    long t = ticks;
    interrupts();
    return t;
}

void Encoder::reset()
{
    noInterrupts();
    ticks = 0;
    interrupts();
}
