#pragma once

#include "Utilities.h"
#include "Map.h"
#include "Characters.h"
#include "Spell.h"

enum E_State {
	Move = 0,
	Fire_Spell,
	End_Turn,
	Count_State
};

struct FSM {
	void (*onEnter[Count_State - 1])(Character&, Spell&);
	void (*onExit[Count_State - 1])(Character&, Spell&);
	int (*onUpdate[Count_State - 1])(Map&, Character&, Characters&, Spell&);
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