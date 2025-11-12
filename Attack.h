#ifndef ATTACK
#define ATTACK

struct Attack() {
	char description[255]; //describes how the attack was carried out, (e.g. "threw a shuriken")
	int power; //how much damage the attack does, added onto npc's base attack
	int pierce; //how much defense the attack pierces through

	//status effects
	//how many targets it hits (1, 3, or all)

	Attack() {

	}
}

#endif // !ATTACK
