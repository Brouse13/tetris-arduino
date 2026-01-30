//
// Created by Brouse on 23/01/2026.
//

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <types.h>
#include <gameMap.h>
#include <piece.h>

#include "mqttClient.h"

#define SCORE_LINE_MULTIPLIER   200
#define SCORE_PIECE_PLACEMENT   10

#define START_PIECE_POSITION    {MAP_WIDTH / 2, 0}

class TetrisGame
{
public:
    explicit TetrisGame(MqttClient &mqttClient);

    void init();

    void start();

    void stop();

    void move(direction_t direction);

    void rotate();
    void publishNewPos() const;

    void sack();

    void tick();
private:
    MqttClient*     _mqttClient;
    GameMap         _gameMap;
    piece_entity_t  _selected_piece;
    piece_entity_t  _next_piece;
    piece_t         _sack_piece;
    uint8_t         _this_tick;
    uint16_t        _score;
    uint8_t         _loose;
    uint8_t         _playing;
    uint16_t        _seed;

    static void generatePiece(piece_entity_t &piece);
};

#endif //TETRIS_GAME_H
