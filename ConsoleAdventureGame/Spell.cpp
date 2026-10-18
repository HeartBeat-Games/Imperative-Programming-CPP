#include "Spell.h"

void initializeSpell(Spell& spell, Character& player)
{
	spell.origin = player.pos;
	spell.cursor = spell.origin;
	spell.damage = player.damage;
	spell.range = 1;

	spell.isVisible = true;
}

void stopSpell(Spell& spell)
{
	spell.isVisible = false;
}

bool moveCursor(Spell& spell, Vector2& dir)
{
	Vector2 newPos = spell.cursor;

	newPos.x += dir.x;
	newPos.y += dir.y;

	int distanceX = abs(newPos.x - spell.origin.x);
	int distanceY = abs(newPos.y - spell.origin.y);

	if (distanceX <= spell.range &&
		distanceY <= spell.range)
	{
		spell.cursor = newPos;
		return true;
	}
	return false;
}

void spellDamage(Spell& spell, Characters& characters)
{
	// skip player
	for (int i = 1; i < characters.size; ++i)
	{
		Character& target = characters.data[i];
		if (target.pos.x == spell.cursor.x &&
			target.pos.y == spell.cursor.y)
		{
			target.life -= spell.damage;
			target.isAlive = target.life > 0;
			break;
		}
	}
}