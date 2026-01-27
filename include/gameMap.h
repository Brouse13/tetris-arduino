//
// Created by Brouse on 23/01/2026.
//

#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH   20
#define MAP_HEIGHT  20

#include <types.h>
#include <piece.h>

#define COLLISION_NOT_DETECTED  0
#define COLLISION_DETECTED      1
#define INDEX_OUT_OF_BOUNDS     2

class GameMap
{
public:
    GameMap();

    void init();

    void clear();

    uint16_t hasCollided(const piece_entity_t &entity) const;

    uint8_t place(const piece_entity_t &entity);

    uint8_t checkLine();
private:
    uint8_t _map[MAP_WIDTH * MAP_HEIGHT];

    uint8_t fullLine(uint8_t line) const;

    static uint16_t index(pos_t position);

    static uint8_t inBounds(pos_t position);
};

#endif //MAP_H
