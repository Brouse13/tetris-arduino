//
// Created by Brouse on 23/01/2026.
//

#ifndef PIECE_H
#define PIECE_H

#include <types.h>
#include <vector.h>

typedef enum class piece : uint8_t
{
    LINE    = 1,
    SQUARE  = 2,
    L_SHAPE = 3,
    T_SHAPE = 4,
    Z_SHAPE = 5,
    J_SHAPE = 6,
    S_SHAPE = 7,
} piece_t;

typedef struct piece_entity
{
    piece_t     type;
    rotation_t  rotation;
    pos_t       pos;
} piece_entity_t;

pos_t rotate(const pos_t &pos, rotation_t rotation);

pos_t worldPos(const piece_entity_t &piece, const pos_t &local);

void getBottomPosition(const piece_entity_t &piece, StaticVector<pos_t, 4> &out);

#endif //PIECE_H
