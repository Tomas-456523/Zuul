//header file for NPCs
//this name is misleading because the yes player character also uses this class

//npcs can be recruited (bool for if they want to be recruited)
//when recruited, they follow you around and are not listed in the description of a room
//recruited npcs and their stats can be listed using the PARTY command
//recruited npcs can be dismissed, and they will then go back home
//you can have up to 3 npcs in your party, not including yourself
//

#ifndef NPC_H
#define NPC_H

#include <vector>
#include <stack>
#include <map>
#include "Room.h"
#include "Attack.h"
#include "Effect.h"
using namespace std;

class Room; //forward declares room because these two classes reference each other
//struct Fighter;

class NPC {
public: //you need to set stats on creation
	NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _sp, int _level = 0, bool _isleader = false, bool _player = false);
	~NPC();

	char* getTitle(); //gets the title of the character
	char* getName(); //gets the name of the character
	char* getDescription(); //gets the description of the character
	char* getDialogue(); //gets the current dialogue
	char* getRejectionDialogue(); //gets the rejection dialogue for the npc
	char* getRecruitmentDialogue(); //gets the recruitment dialogue for the npc
	char* getDismissalDialogue(); //gets the dismissal dialogue for the npc
	char* getBattleCry(); //gets a random battle cry
	char* getAttackDescription(); //gets the description of the npc's attack
	bool getRecruitable(); //gets the recruitable status of the character
	bool getRecruited(); //gets the recruited status of the npc (if they're already in the party)
	bool getPlayerness(); //gets if the character is a player character (true) or if they're truly an npc (false)
	int getHealth(); //gets the current health of the npc
	int getHealthMax(); //gets the maximum health of the npc
	int getDefense(); //gets the defense of the npc
	int getAttack(); //gets the damage the npc can deal
	int getToughness(); //gets the toughness of the npc
	int getPierce(); //gets the pierce of the npc
	int getSpeed(); //gets the speed of the npc
	int getSP();
	int getSPMax();
	Room* getHome(); //gets the home location of the npc
	int getLevel(); //gets the level of the npc
	int xpForNextLevel();
	int xpForLevel(int level);
	vector<NPC*>* getParty();
	bool getLeader();
	bool getHypnotized();
	bool getEscapable();
	Attack* getCheapestAttack();
	int getXpReward();
	int getMonyReward();
	bool getEnemy();
	Attack* getBasicAttack();
	vector<Attack*> getSpecialAttacks();
	map<Attack*, int> getWeights();

	void setDialogue(const char _dialogue[255]); //sets the dialogue for the npc
	void setRejectionDialogue(const char _dialogue[255]); //sets the rejection dialogue for the npc
	void setRecruitmentDialogue(const char _dialogue[255]); //sets the recruitment dialogue for the npc
	void setDismissalDialogue(const char _dialogue[255]); //sets the dismissal dialogue for the npc
	void addBattleCry(); //adds a random battle cry to the npc
	void setAttackDescription(); //sets the description of the attack
	void setRecruitable(bool _recruitable);
	void Recruit();
	void Dismiss();
	void setRoom(Room* _room);
	void setParty(NPC* npc1 = NULL, NPC* npc2 = NULL, NPC* npc3 = NULL, NPC* npc4 = NULL);
	void setName(const char _name[255]);
	void setScale(int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _sp);
	void addXp(int _xp);
	void levelUp();
	void setLeader(bool _leader, int _level = 0, Room* room = NULL);
	void setHypnotized(bool _hypnotized);
	int damage(float power, float pierce);
	void setLevel(int _level); //only used for enemy parties
	void setBasicAttack(Attack* attack);
	void addSpecialAttack(Attack* attack);
	void blockExit(char* _exitBlocking, char* type, const char reason[255]);
	void setEscapable(bool _escapable);
	void calculateWeights();
	void setEnemy(bool _enemy);
	void alterSp(int amount);

	void defeat();
protected:
	char title[255]; //the title of the character (eg. VILLAGE ELDER)
	char name[255];
	char description[255];

	Room* home;
	Room* currentRoom;

	vector<NPC*> party;

	Attack* standard_attack;
	vector<Attack*> special_attacks;
	Attack* cheapest_attack;

	vector<Effect> effects;
	map<Attack*, int> attackWeight;

	//dialogue that the npc says when asked
	char dialogue[255];
	char recruitedDialogue[255]; //dialogue that the npc says while recruited
	char rejectionDialogue[255];
	char recruitmentDialogue[255];
	char dismissalDialogue[255];
	//the npcs say a random one of these phrases when attacking
	char battleCry[255];

	char attackDescription[255]; //describes how the npc is attacking (eg. shot a laser at)

	bool recruitable = false;
	bool recruited = false;
	bool isPlayer;
	bool isLeader;
	bool isEnemy = false;

	int health;
	int maxHealth;
	int defense;
	int attack;
	int toughness;
	int pierce;
	int speed;
	int sp; //sp stands for skill points
	int maxSP;
	
	int level = 0;
	int xp = 0;

	int healthScale = 0;
	int defenseScale = 0;
	int attackScale = 0;
	int toughnessScale = 0;
	int pierceScale = 0;
	int speedScale = 0;
	int spScale = 0;

	bool hypnotized = false;
	bool defeated = false;

	char* exitBlocking; //enemy npcs may block an exit until they are defeated
	bool escapable = true;

	int xpReward;
	int monyReward;
};
#endif