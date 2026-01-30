//
// Created by Brouse on 24/01/2026.
//

#include <tetrisGame.h>
#include <piece.h>

void saveCurrentPostion(uint8_t *data, piece_entity_t piece);

TetrisGame::TetrisGame(MqttClient &mqttClient) : _selected_piece(), _next_piece(), _sack_piece(), _score(0),
                                                 _loose(0), _playing(0), _seed(0)
{
    _mqttClient = &mqttClient;
}

void TetrisGame::init()
{
    _seed = analogRead(A8);
    randomSeed(_seed);
    generatePiece(_next_piece);
    generatePiece(_selected_piece);
    _sack_piece = piece_t::NONE;
    _gameMap.clear();
    _score = 0;
    _loose = 0;
    _playing = 0;

    uint8_t data[4];

    // We send start information
    data[0] = static_cast<uint8_t>(_seed >> 8);
    data[1] = static_cast<uint8_t>(_seed & 0xFF);
    data[2] = static_cast<uint8_t>(_selected_piece.type);
    data[3] = static_cast<uint8_t>(_next_piece.type);
    _mqttClient->publish("tetris/startUp", data, 4);
}

void TetrisGame::start()
{
    _playing = 1;
}

void TetrisGame::stop()
{
    _playing = 0;
}

void TetrisGame::move(const direction_t direction)
{
    const auto currentPosition = _selected_piece.pos;

    switch (direction)
    {
        case direction_t::LEFT:     _selected_piece.pos.x--; break;
        case direction_t::RIGHT:    _selected_piece.pos.x++; break;
        case direction_t::DOWN:     _selected_piece.pos.y++; break;
        default:                                             break;
    }

    if (_gameMap.hasCollided(_selected_piece) == COLLISION_NOT_DETECTED)
    {
        uint8_t data[4 * 2 * 2];
        saveCurrentPostion(data, _selected_piece);
        _mqttClient->publish("tetris/newPos", data, 4 * 2 * 2);
        return;
    }

    _selected_piece.pos = currentPosition;
}

void TetrisGame::rotate()
{
    const auto originalRotation = _selected_piece.rotation;
    const auto originalPos = _selected_piece.pos;

    switch (_selected_piece.rotation) {
        case rotation_t::R0:   _selected_piece.rotation = rotation_t::R90;  break;
        case rotation_t::R90:  _selected_piece.rotation = rotation_t::R180; break;
        case rotation_t::R180: _selected_piece.rotation = rotation_t::R270; break;
        case rotation_t::R270: _selected_piece.rotation = rotation_t::R0;   break;
    }

    if (_gameMap.hasCollided(_selected_piece) == COLLISION_NOT_DETECTED)
    {
        publishNewPos();
        return;
    }

    _selected_piece.pos.x += 1;
    if (_gameMap.hasCollided(_selected_piece) == COLLISION_NOT_DETECTED) {
        publishNewPos();
        return;
    }

    _selected_piece.pos.x -= 2;
    if (_gameMap.hasCollided(_selected_piece) == COLLISION_NOT_DETECTED) {
        publishNewPos();
        return;
    }

    _selected_piece.pos = originalPos;
    _selected_piece.rotation = originalRotation;
}

// Función auxiliar para no repetir código del MQTT
void TetrisGame::publishNewPos() const
{
    uint8_t data[16]; // 4 bloques * 2 ejes * 2 bytes (si usas int16) o ajustes según tu protocolo
    saveCurrentPostion(data, _selected_piece);
    _mqttClient->publish("tetris/newPos", data, 16);
}

void TetrisGame::sack()
{
    if (_this_tick) return;

    uint8_t data[4 * 2 * 2 + 1];
    if (_sack_piece == piece_t::NONE)
    {
        _sack_piece = _selected_piece.type;
        generatePiece(_selected_piece);
        _selected_piece.type = _next_piece.type;
        generatePiece(_next_piece);
        data[0] = static_cast<uint8_t>(_next_piece.type);
    }
    else
    {
        const auto piece = _sack_piece;
        _sack_piece = _selected_piece.type;
        generatePiece(_selected_piece);
        _selected_piece.type = piece;
        _this_tick = true;
        data[0] = 0;
    }

    saveCurrentPostion(&data[1], _selected_piece);
    _mqttClient->publish("tetris/sack", data, 4 * 2 * 2 + 1);
}


void TetrisGame::tick()
{
    if (!_playing) return;

    constexpr uint8_t MAX_DATA = 4 * 2 * 2 + 1;
    uint8_t data[2];
    const uint8_t collided = _gameMap.hasCollided(_selected_piece);

    if (collided == COLLISION_NOT_DETECTED)
    {
        saveCurrentPostion(data, _selected_piece);
        _mqttClient->publish("tetris/newPos", data, MAX_DATA - 1);
        _selected_piece.pos.y++;
        return;
    }

    if (_selected_piece.pos.y == 0)
    {
        _loose = 1;
        _playing = 0;
        data[0] = 1;
        _mqttClient->publish("tetris/loose", data, 1);
        return;
    }

    _selected_piece.pos.y--;
    _gameMap.place(_selected_piece);
    _selected_piece = _next_piece;
    generatePiece(_next_piece);

    // FIXME Fix checkLine method
    const uint8_t lines = _gameMap.checkLine();
    _score += lines * SCORE_LINE_MULTIPLIER;
    _score += SCORE_PIECE_PLACEMENT;

    // Sending a new piece means the prev was places, and switched, and we need a new one
    data[0] = static_cast<uint8_t>(_next_piece.type);
    saveCurrentPostion(&data[1], _selected_piece);
    _mqttClient->publish("tetris/newPiece", data, MAX_DATA);

    // Send score
    data[0] = static_cast<uint8_t>(_score >> 8);
    data[1] = static_cast<uint8_t>(_score & 0xF);
    _mqttClient->publish("tetris/score", data, 2);

    _this_tick = false;
}

void TetrisGame::generatePiece(piece_entity_t &piece)
{
    uint8_t rand = random(1, 8);
    piece.type = static_cast<piece_t>(rand);
    piece.rotation = rotation_t::R0;
    piece.pos = START_PIECE_POSITION;
}

void saveCurrentPostion(uint8_t *data, const piece_entity_t piece)
{
    // 4 block 2 coords with 2 bytes each piece

    StaticVector<pos_t, 4> pieceData;
    getBottomPosition(piece, pieceData);

    uint8_t i = 0;
    for (const auto &newPos : pieceData)
    {
        data[i]     = static_cast<uint8_t>(newPos.x >> 8);
        data[i+1]   = static_cast<uint8_t>(newPos.x & 0xFF);
        data[i+2]   = static_cast<uint8_t>(newPos.y >> 8);
        data[i+3]   = static_cast<uint8_t>(newPos.y & 0xFF);

        i += 4;
    }

}

