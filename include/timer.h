//
// Created by Brouse on 25/01/2026.
//

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

#define MODE_NORMAL     0b0000
#define MODE_CTC        0b0100

#define TICK_1024_1s    15625

typedef enum scaling { SCALING8, SCALING64, SCALING256, SCALING1024 } scaling_t;

class Timer {
public:
    /**
     * Begin the timer, this will set up the working mode and the scaling type.
     *
     * @param mode working mode
     * @param scaling scaling type
     */
    static void setup(uint8_t mode, scaling scaling);

    /**
     * Start the timer with a given number of ticks, this will also clear the current count.
     * But won't reconfigure the timer.
     *
     * @param ticks number of ticks
     */
    void start(uint16_t ticks) const;

    /**
     * Stops the timer and reset the configuration and the count.
     */
    void stop() const;

    /**
     * Stops the timer and reset the configuration.
     */
    void pause() const;
};

#endif //TIMER_H