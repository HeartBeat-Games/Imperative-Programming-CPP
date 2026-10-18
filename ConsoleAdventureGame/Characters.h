#pragma once

#include "Utilities.h"
#include "Map.h"

enum E_CharacterType {
    Player = 0,
    Enemy,
    CharacterType_Count
};

enum E_PlayerAction
{
    None = 0,
    Move_Up,
    Move_Down,
    Move_Left,
    Move_Right,
    Attack
};

static const Vector2 g_directions[4] = {
    {0,-1},  // Up
    {0, 1},  // Down
    {-1, 0}, // Left
    {1, 0}   // Right
};

struct Character {
    E_CharacterType type = E_CharacterType::Enemy;
    Vector2         pos = Vector2{};
    bool            isAlive = true;

    int             life = 1;
    int             damage = 1;
};

struct Characters {
    Character* data;
    size_t     size;
};

const char characterSymbols[E_CharacterType::CharacterType_Count] = {
    'H',  // Player
    'X',  // Enemy
};


void characterBattle(Character& first, Character& second);
bool characterMoveCollision(
    Character& character, Vector2& newPos,
    Characters& others);
bool characterMove(
    Character& character, Vector2& dir,
    Map& map, Characters& characters);

E_PlayerAction readPlayerInputs();
Vector2 getDirection(E_PlayerAction action);
Vector2 getRandomDirection();
Characters* createCharacters(Map& map);