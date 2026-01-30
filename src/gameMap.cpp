//
// Created by Brouse on 23/01/2026.
//

#include <gameMap.h>

GameMap::GameMap() : _map{} { }

void GameMap::init()
{
    clear();
}

void GameMap::clear()
{
    memset(_map, 0, sizeof(_map));
}

uint16_t GameMap::hasCollided(const piece_entity_t &entity) const
{
    StaticVector<pos_t, 4> blocks;
    getBottomPosition(entity, blocks);

    for (const auto &block : blocks)
    {
        const uint16_t idx = index(block);

        if (!inBounds(block))   return INDEX_OUT_OF_BOUNDS;

       /*
        Serial.print(idx);
         Serial.print(" : ");
         Serial.print(_map[idx]);
         Serial.print(", ");
        */

        if (_map[idx] != 0)     return COLLISION_DETECTED;
    }
    //Serial.println("");

    return COLLISION_NOT_DETECTED;
}

uint8_t GameMap::place(const piece_entity_t &entity)
{
    StaticVector<pos, 4> blocks;
    getBottomPosition(entity, blocks);
    for (const auto &block : blocks)
    {
        const uint16_t idx = index(block);
        /*
        Serial.print(idx);
        Serial.print(" ");
        */

        _map[idx] = static_cast<uint8_t>(entity.type);
    }
    //Serial.println("");
    return 1;
}

uint8_t GameMap::checkLine()
{
    uint8_t clearedLines = 0;

    for (int y = MAP_HEIGHT - 1; y >= 0; --y)
    {
        if (!fullLine(y))
            continue;

        constexpr size_t rowSize = MAP_WIDTH * sizeof(uint8_t);

        memmove(
            &_map[index({0, 1})],   // destination
            &_map[index({0, 0})],   // source
            rowSize * y                           // number of bytes
        );

        // Clear the top row
        memset(&_map[index({0, 0})], 0, rowSize);

        clearedLines++;
        ++y;
    }

    return clearedLines;
}


uint8_t GameMap::fullLine(const uint8_t line) const
{
    const uint16_t start = index({0, line});
    const uint16_t end   = start + MAP_WIDTH;

    for (uint16_t i = start; i < end; ++i)
    {
        if (_map[i] == 0) return 0;
    }
    return 1;
}


uint16_t GameMap::index(const pos_t position)
{
    return position.y * MAP_WIDTH + position.x;
}

uint8_t GameMap::inBounds(const pos_t position)
{
    return position.x >= 0 && position.x < MAP_WIDTH && position.y >= 0 && position.y < MAP_HEIGHT;
}
