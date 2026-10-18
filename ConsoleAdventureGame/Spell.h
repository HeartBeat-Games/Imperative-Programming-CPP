#pragma once

#include "Utilities.h"
#include "Characters.h"

struct Spell {
	bool	isVisible;
	
	Vector2 origin;
	Vector2 cursor;

	int		range;
	int		damage;
};

void initializeSpell(Spell& spell, Character& player);
void stopSpell(Spell& spell);
bool moveCursor(Spell& spell, Vector2& dir);
void spellDamage(Spell& spell, Characters& characters);