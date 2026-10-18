#include "Characters.h"

#include <random>
#include <conio.h>

void characterBattle(Character& first, Character& second)
{
    first.life -= second.damage;
    second.life -= first.damage;

    first.isAlive = first.life > 0;
    second.isAlive = second.life > 0;
}

bool characterMoveCollision(
    Character& character, Vector2& newPos,
    Characters& others)
{
    for (int i = 0; i < others.size; ++i)
    {
        Character& other = others.data[i];

        // Collision with currently moving character ?
        if (other.isAlive &&
            other.pos.x == newPos.x &&
            other.pos.y == newPos.y)
        {
            if (other.type == character.type)
                return true; // Can't collide with the same character type
            else
                characterBattle(character, other);
        }
    }

    return false;
}

bool characterMove(
    Character& character, Vector2& dir,
    Map& map, Characters& characters)
{
    Vector2 newPos = character.pos;
    newPos.x += dir.x; // override + operator for less code
    newPos.y += dir.y;

    // Borders check
    if (newPos.x < 0 || newPos.x >= map.width ||
        newPos.y < 0 || newPos.y >= map.height)
        return false;

    int mapIndex = get1DCell(newPos, map.width);
    int cellType = map.cells[mapIndex];
    if (!g_cellInfos[cellType].isWalkable)
        return false;

    if (characterMoveCollision(character, newPos, characters))
        return false;

    character.pos = newPos;

    return true;
}

E_PlayerAction readPlayerInputs()
{
    char ch = _getch();

    // some people wanted to check this (cf ASCII table)
    if (ch >= 'A' && ch <= 'Z')
        ch -= 26;

    E_PlayerAction action = E_PlayerAction::None;
    switch (ch)
    {
        case 'z': action = E_PlayerAction::Move_Up; break;
        case 's': action = E_PlayerAction::Move_Down; break;
        case 'q': action = E_PlayerAction::Move_Left; break;
        case 'd': action = E_PlayerAction::Move_Right; break;
        case ' ': action = E_PlayerAction::Attack; break;
    }

    return action;
}

Vector2 getDirection(E_PlayerAction action)
{
    return g_directions[(int)action - 1]; // careful to not change action enum values
}

Vector2 getRandomDirection()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, 3);

    return g_directions[distrib(gen)];
}

Characters* createCharacters(Map& map)
{
    Characters* characters = new Characters{};
    characters->data = new Character[map.characterSpawns.size()];
    characters->size = map.characterSpawns.size();

    for (int i = 0; i < characters->size; ++i)
    {
        Character& character = characters->data[i];
        character.type = E_CharacterType::Enemy;
        character.pos = map.characterSpawns[i];
        character.isAlive = true;
        character.life = 1;
        character.damage = 1;
    }

    Character* player = &characters->data[0];
    player->type = E_CharacterType::Player;
    player->life = 3;

    return characters;
}