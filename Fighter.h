#ifndef FIGHTER
#define FIGHTER

#include <cstring>

struct Fighter {
	char name[255];
	char description[255];
	int health;
	int maxHealth;
	int defense;
	int attack;
	int toughness;
	int pierce;
	int speed;

	Fighter() {} //default constructor so NPC doesn't explode

	Fighter(const char _name[255], const char _description[255], int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed) {
		strcpy(name, _name);
		strcpy(description, _description);
		maxHealth = _health;
		health = _health;
		defense = _defense;
		attack = _attack;
		toughness = _toughness;
		pierce = _pierce;
		speed = _speed;
	}
};
#endif