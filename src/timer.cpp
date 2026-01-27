//
// Created by Brouse on 25/01/2026.
//

#include <timer.h>

void Timer::setup(const uint8_t mode, const scaling_t scaling)
{
    // Clear old WGM bits
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B &= ~((1 << WGM12) | (1 << WGM13));

    // Set new mode
    TCCR1A |= ((mode & (1 << 0)) ? (1 << WGM10) : 0) |
        ((mode & (1 << 1)) ? (1 << WGM11) : 0);

    TCCR1B |= ((mode & (1 << 2)) ? (1 << WGM12) : 0) |
        ((mode & (1 << 3)) ? (1 << WGM13) : 0);

    // Enable compare match interrupt
    TIMSK1 |= (1 << OCIE1A);

    uint8_t value = TCCR1B & 0b11111000;
    switch (scaling)
    {
    case SCALING8: value |= 0b010; break;
    case SCALING64: value |= 0b011; break;
    case SCALING256: value |= 0b100; break;
    case SCALING1024: value |= 0b101; break;
    }
    TCCR1B = value;

    TCNT1 = 0;
}

void Timer::start(const uint16_t ticks) const
{
    TCNT1 = 0;
    OCR1A = ticks;
}

void Timer::stop() const
{
    TCCR1B &= ~0b111;
    TCNT1 = 0;
}

void Timer::pause() const
{
    TCCR1B &= ~0b111;
}