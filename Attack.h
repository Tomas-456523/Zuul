//header file for attacks

#ifndef ATTACK
#define ATTACK

#include <vector>
#include <utility>
#include <cstring>
#include "Effect.h"
#include "Conversation.h"
using namespace std;

class NPC;

struct Attack; //forward declare itself so it can forward declare attacksH
namespace Helper { //forward declare Helper to add attacks to attacksH
	extern vector<Attack*> attacksH;
}

struct Attack {
	const char* name; //the name of the attack and how you call it
	const char* description; //describes how the attack was carried out, (e.g. "threw a shuriken at")
	const char* afterdesc = ""; //description after the attack (e.g. threw a shuriken at BOB" with expert precision")
	const char* trueDesc; //actual description of the attack; only for player attacks
	int cost; //how much sp the attack costs to use
	int power; //how much damage the attack does, added onto npc's base attack
	int pierce; //how much defense the attack pierces through
	int targets; //how many targets the attack hits (1 hits just the target, 3 hits their surrounding allies, and 5 goes further, etc. there are no even target amounts)
	int minhits; //the attack can hit from minhits to maxhits amount of times
	int maxhits;
	bool targetAlly = false; //if the attack is supposed to hit your own team
	bool targetFainted = false; //if the attack is supposed to hit 0 hp npcs
	bool instakill = false; //if it instantly defeats non-boss npcs
	bool focushits = true; //if every hit of multihit moves should hit the same target, as opposed to random targets each time
	int recoil = 0; //how much damage the user takes (calculated later using their own attack stat)
	int guardset = 0; //set guard on the attacker
	int targuard = 0; //set guard on the target
	bool percentagebased = false; //if the attack power is the % of the health it takes, versus just normal power which is used to calculate damage

	bool spbomb = false;
	bool protect = false; //if this is a move that makes the mover start defending the target
	bool parry = false; //if this is the parry move and we have to tell the attacker to start parrying the target

	int minLevel; //must be at least this level to use the attack
	double hpthreshold = 0; //attacker must be at <= max hp * this in order to use the attack

	double lifesteal; //what % of damage dealt is stolen and added to the npc's health
	int spleak; //how much sp the attack removes from the target

	Effect* appliedeffect = NULL; //targets get affected by this effect when hit
	bool redundanteffect = true; //allow doing effect attack if the target has the effect already
	Effect* selfeffect = NULL; //attacker gets affected by this effect after using
	vector<Effect*> synergies; //if the target has these effects effect, the attack power goes up by 1.5x each
	vector<Effect*> cancels; //attack removes these effects
	Effect* ignoreeffect = NULL; //don't target npcs who have this effect
	Effect* selfcancel = NULL; //attack removes this effect from self

	Effect* nonspecificeffect = NULL; //for multiple-target attacks, there might be a different applied effect
	NPC* specifictarget = NULL; //if there's a specificeffect set, we store who the specific target is here (more convenient)

	bool affectselfbeforeattack = false; //if we should apply self stat changes before the attack as opposed to as a result of it afterwards

	Attack* recoilatt = NULL;
	double recoilchance = 0;

	NPC* summon = NULL; //the npc that the attack summons
	int summonamount = 0; //how many of the summon to summon
	bool enemysummon = false; //if the summon goes to the enemy team

	int copyamount = 0; //how many times the target gets duplicated

	bool contact; //if the attacker makes contact with the target
	bool prioritizeleader = false; //if this attack prioritizes hitting the leader, so if they're not incapacitated
	bool prioritizenonleader = false; //if this attack prioritizes not hitting the leader
	bool donotplayer = false; //if we should never ever hit the player
	bool onlyplayer = false; //if we should only hit the player
	bool targetself = false; //self-explanatory
	bool donotself = false; //if this attack should under no circumstances be targeted at the attacker
	bool targetshark = false; //if this attack should only target sharks
	bool skiptarget = false; //if in a multi-target attack, it hits everything around the target instead of including the target in the blast
	bool nottoohypno = false; //only do this attack if not too many of the target team is hypnotized (balance/gameplay feel reasons)

	int extralives = 0; //how many extra lives to give to the target

	bool risky = false; //if we shouldn't do this move to a teammate below half health

	bool take = false; //if this attack is an attack that makes the attacker take the target

	bool trashsummons = false; //if this attack also incapacitates all of the attacker's summons

	bool transformtotar = false; //if this attack makes the attacker transform into the target (if this is true, NPC* transformation will be overridden on use)
	NPC* transformation = NULL; //the attacker transforms into this npc

	double statchip = 0; //how much % of the target's stats this attack chips away (random between 0 and this)

	Conversation attackconvo; //conversation that happens when doing the attack (prints instead of "ATTACKER used ATTACK")
	bool repeatconvo = false; //if the conversation prints every time the attack goes instead of just the first time you ever see it
	vector<pair<NPC*, Conversation>> specificconvo; //the attack conversation is this when using it on this specific npc

	//constructs the attack
	//default stats are for testing purposes
	Attack(const char* _name, const char* _description, bool _contact, int _cost = 2, int _power = 20, int _pierce = 20, int _minhits = 1, int _maxhits = 1, int _targets = 1, bool _targetAlly = false, int _minlevel = 0, int _spleak = 0, double _lifesteal = 0) {
		name = _name;
		description = _description;
		contact = _contact;
		cost = _cost;
		power = _power;
		pierce = _pierce;
		targets = _targets;
		minhits = _minhits;
		maxhits = _maxhits;
		targetAlly = _targetAlly;
		minLevel = _minlevel;
		spleak = _spleak;
		lifesteal = _lifesteal;
		Helper::attacksH.push_back(this); //store a pointer to this attack in the attacks vector
	}

	//I don't need these functions since everything's public in a struct but I like having functions, especially since I don't have to define them in seperate header and cpp files

	//sets an effect for the attack to apply on hit
	void addEffect(Effect* effect) {
		appliedeffect = effect;
	}

	//sets the true description of the attack, so that it can be seen in the attacks menu
	void addDescription(const char* desc) {
		trueDesc = desc;
	}

	//gets a conversation or the specific one for the npc if there is one
	Conversation getConvo(NPC* npc) {
		Conversation theconvo = attackconvo;
		if (!repeatconvo) attackconvo = {}; //reset the attack conversation
		for (vector<pair<NPC*, Conversation>>::iterator nc = specificconvo.begin(); nc != specificconvo.end(); nc++) {
			if (nc->first == npc) {
				theconvo = nc->second;
				//remove this conversation if we don't repeat the conversation
				if (!repeatconvo) specificconvo.erase(nc);
				break;
			}
		}
		return theconvo;
	}

	//I just havet his helper function cause it looks nicer than spamming attack->specificconvo.push_back({npc, {{npc1, "dialogue"}, {npc2, "ok"}}});
	void setTargetConv(NPC* npc, const Conversation& conv) {
		specificconvo.push_back({npc, conv});
	}

	//gets (estimates) if you want to be hit by this attack, like buffs or heals and stuff like that
	bool getBeneficial() {
		if (power < 0 || spleak < 0 || extralives || targuard || protect || copyamount) return true; //obvious positives
		if (risky) return true; //we wouldn't clarify that it's risky unless it was for a teammate, why wouldn't we want to give something risky to an enemy
		if (power > 0 || take || spleak > 0) return false; //obvious negatives
		if (appliedeffect) return appliedeffect->getBeneficial(); //see if the effect is beneficial and the attack's beneficialness probably matches
		return false; //default to bad because it's an attack after all, you don't wanna get attacked, and also most attacks definitely are not beneficial
	}
};

#endif // !ATTACK