#pragma once

#include "Utilities.h"
#include "Map.h"
#include "Characters.h"
#include "Spell.h"

struct FSM {
	void (*onEnter[2])(Character&, Spell&);
	void (*onExit[2])(Character&, Spell&);
	int (*onUpdate[2])(Map&, Character&, Characters&, Spell&);
};

void movement_onEnter(Character& player, Spell& spell);
void movement_onExit(Character& player, Spell& spell);
int movement_onUpdate(
	Map& currentMap, Character& player,
	Characters& characters, Spell& spell);

void spell_onEnter(Character& player, Spell& spell);
void spell_onExit(Character& player, Spell& spell);
int spell_onUpdate(
	Map& currentMap, Character& player,
	Characters& characters, Spell& spell);