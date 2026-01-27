//
// Created by Brouse on 23/01/2026.
//

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

typedef enum class rotation : uint8_t
{
    R0      = 1,
    R90     = 2,
    R180    = 3,
    R270    = 4
} rotation_t;

typedef enum class direction : uint8_t
{
    UP      = 1,
    DOWN    = 2,
    LEFT    = 3,
    RIGHT   = 4,
    NONE    = 5
} direction_t;

typedef struct pos
{
    int8_t x;  /// X value of the position
    int8_t y;  /// Y value of the position

    constexpr pos(int x, int y) :
        x(static_cast<int8_t>(x)),
        y(static_cast<int8_t>(y)) {}

    pos() : x(0), y(0) { }
} pos_t;

#endif //TYPES_H
