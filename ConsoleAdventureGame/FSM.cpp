#include "FSM.h"

void movement_onEnter(Character& _, Spell& __) {}
void movement_onExit(Character& _, Spell& __) {}

int movement_onUpdate(Map& currentMap, Character& player, Characters& characters, Spell& _)
{
    E_PlayerAction playerAction;
    Vector2 dir;
    bool moved;
    do {
        playerAction = readPlayerInputs();
        if (playerAction == E_PlayerAction::Attack)
            return 1; // Go to spell state
        dir = getDirection(playerAction);
        moved = characterMove(player, dir, currentMap, characters);
    } while (!moved);

    return 2;
}

void spell_onEnter(Character& player, Spell& spell)
{
    initializeSpell(spell, player);
}

void spell_onExit(Character& _, Spell& spell)
{
    stopSpell(spell);
}

int spell_onUpdate(Map& currentMap, Character& player, Characters& characters, Spell& spell)
{
    E_PlayerAction playerAction;
    Vector2 dir;
    bool moved;
    do {
        playerAction = readPlayerInputs();
        if (playerAction == E_PlayerAction::Attack)
        {
            spellDamage(spell, characters);
            return 2; // Out
        }
        dir = getDirection(playerAction);
        moved = moveCursor(spell, dir);
    } while (!moved);
}