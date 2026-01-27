//
// Created by Brouse on 23/01/2026.
//

#include <piece.h>

pos_t rotate(const pos_t& pos, const rotation_t rotation)
{
    switch (rotation)
    {
        case rotation_t::R0:   return {  pos.x,  pos.y };
        case rotation_t::R90:  return { -pos.y,  pos.x };
        case rotation_t::R180: return { -pos.x, -pos.y };
        case rotation_t::R270: return {  pos.y, -pos.x };
    }
    return pos;
}

pos_t worldPos(const piece_entity_t &piece, const pos_t &local)
{
    const pos_t rotatedPos = rotate(local, piece.rotation);
    return {
        static_cast<int8_t>(rotatedPos.x + piece.pos.x),
        static_cast<int8_t>(rotatedPos.y + piece.pos.y)
    };
}

void getBottomPosition(const piece_entity_t &piece, StaticVector<pos_t, 4> &out)
{
    out.clear();

    const pos_t* shape = nullptr;

    switch (piece.type)
    {
    case piece_t::LINE:
        static constexpr pos_t line[4] = {
        {-1,0},{0,0},{1,0},{2,0}
        };
        shape = line;
        break;

    case piece_t::SQUARE:
        static constexpr pos_t square[4] = {
        {0,0},{1,0},{0,1},{1,1}
        };
        shape = square;
        break;

    case piece_t::T_SHAPE:
        static constexpr pos_t tshape[4] = {
        {-1,0},{0,0},{1,0},{0,1}
        };
        shape = tshape;
        break;

    case piece_t::L_SHAPE:
        static constexpr pos_t lshape[4] = {
        {-1,0},{0,0},{1,0},{1,1}
        };
        shape = lshape;
        break;

    case piece_t::Z_SHAPE:
        static constexpr pos_t zshape[4] = {
        {-1,1},{0,1},{0,0},{1,0}
        };
        shape = zshape;
        break;
    case piece::J_SHAPE:
        static constexpr pos_t jshape[4] = {
        {-1,1}, {-1,0},{0,0},{1,0}
        };
        shape = jshape;
        break;
    case piece::S_SHAPE:
        static constexpr pos_t sshape[4] = {
        {-1,0},{0,0},{0,1},{1,1}
        };
        shape = sshape;
        break;

    default: return;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        out.push_back(worldPos(piece, shape[i]));
    }
}