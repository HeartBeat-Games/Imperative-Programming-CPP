#pragma once

#include "Utilities.h"
#include "Characters.h"

struct Spell {
	bool	isVisible = false;
	
	Vector2 origin = Vector2{};
	Vector2 cursor = Vector2{};;

	int		range = 0;
	int		damage = 0;
};

void initializeSpell(Spell& spell, Character& player);
void stopSpell(Spell& spell);
bool moveCursor(Spell& spell, Vector2& dir);
void spellDamage(Spell& spell, Characters& characters);