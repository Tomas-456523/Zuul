//header file for NPCs
//this name is misleading because the yes player character also uses this class

#ifndef NPC_H
#define NPC_H

#include <vector>
#include <queue>
#include <utility>
#include <map>
#include "Room.h"
#include "Effect.h"
#include "Attack.h"
#include "Conversation.h"
using namespace std;

class Room; //forward declares room because these two classes reference each other
class Item;

class NPC { //for game testing purposes, all stats default to 20
public: //you need to set stats on creation
	NPC(const char* _title, const char* _name, const char* _description, Room* room, int _health = 20, int _defense = 20, int _attack = 20, int _toughness = 20, int _pierce = 20, int _speed = 20, int _sp = 20, int _level = 0, bool _isleader = false, bool _player = false);
	~NPC();

	//a bunch of functions to get npc variables
	const char* getTitle(); //gets the title of the character
	const char* getName(); //gets the name of the character
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
	int getSP(); //gets the current sp of the npc
	int getSPMax(); //gets the maximum sp of the npc
	Room* getHome(); //gets the home location of the npc
	Room* getRoom(bool alt = false); //get current room or the adjecent one it's blocking
	int getLevel(); //gets the level of the npc
	int xpForNextLevel();
	int xpForLevel(int level); //for the level from 0
	vector<NPC*>* getParty();
	bool getLeader(); //if npc is a leader
	bool getEscapable();
	int getXpReward(); //reward for beating them
	int getMonyReward();
	bool getEnemy(); //if npc is enemy
	Attack* getBasicAttack();
	vector<Attack*> getSpecialAttacks();
	map<Attack*, int> getWeights();
	bool getLevelUp(); //if we leveled up recently
	vector<int>& getStatChanges(); //the stat changes from the last level up
	Attack* getNewAttack();
	bool getDefeated(); //if npc has been defeated since entering the room
	Item* takeGift(); //return and nullify the gift item
	vector<Effect>& getEffects();
	int getHypnotized(); //hypnosis amount
	int getFrozen(); //freeze amount
	int getConvoSize(); //how many conversations are left to say
	bool getRespawn(); //if they respawn
	bool getBoss();

	//bunch of functions for affecting npc variables
	void setDialogue(const Conversation& _dialogue); //sets the default dialogue for the npc
	void addConversation(const Conversation& _dialogue); //add a conversation
	void addGymDialogue(const Conversation& _dialogue); //sets what the npc says in the gym
	void addRejectionDialogue(const Conversation& _dialogue); //sets the rejection dialogue for the npc
	void addRecruitmentDialogue(const Conversation& _dialogue); //sets the recruitment dialogue for the npc
	void addRecruitedDialogue(const Conversation& _dialogue); //sets the recruited dialogue for the npc
	void setRecruitDialogueChange(const Conversation& _dialogue); //sets new recruitment and normal dialogue after recruiting
	void addDismissalDialogue(const Conversation& _dialogue); //sets the dismissal dialogue for the npc
	//same stuff but they accept just one line for simplicity
	void setDialogue(const char* _dialogue);
	void addConversation(const char* _dialogue);
	void addGymDialogue(const char* _dialogue);
	void addRejectionDialogue(const char* _dialogue);
	void addRecruitmentDialogue(const char* _dialogue);
	void addRecruitedDialogue(const char* _dialogue);
	void setRecruitDialogueChange(const char* _dialogue);
	void addDismissalDialogue(const char* _dialogue);

	void setRecruitable(bool _recruitable); //set if you can recruit them
	void Recruit(); //set recurited to true
	void Dismiss(bool gohome = true); //dismiss them and go home if specified
	void setRoom(Room* _room); //move the npc
	void setHome(Room* room);
	void setParty(NPC* npc1 = NULL, NPC* npc2 = NULL, NPC* npc3 = NULL, NPC* npc4 = NULL, NPC* npc5 = NULL, NPC* npc6 = NULL, NPC* npc7 = NULL, NPC* npc8 = NULL, NPC* npc9 = NULL, NPC* npc10 = NULL, NPC* npc11 = NULL, NPC* npc12 = NULL, NPC* npc13 = NULL, NPC* npc14 = NULL, NPC* npc15 = NULL, NPC* npc16 = NULL, NPC* npc17 = NULL, NPC* npc18 = NULL, NPC* npc19 = NULL);
	void setName(const char* _name);
	void setTitle(const char* _title);
	void setDescription(const char* _description);
	void setScale(int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _sp);
	void addXp(int _xp);
	void levelUp(bool trackLevelUp = false);
	void setLeader(bool _leader, int _level = 0, Room* room = NULL, bool respawn = true);
	int damage(float power, float pierce, int hits = 1);
	void directDamage(int damage, const char* status = NULL);
	void setLevel(int _level); //only used for enemy parties
	void setBasicAttack(Attack* attack);
	void addSpecialAttack(Attack* attack);
	void blockExit(const char* _exitBlocking, const char* type, const char* reason, bool bothsides = false);
	void setEscapable(bool _escapable);
	void calculateWeights();
	void setEnemy(bool _enemy);
	void alterSp(int amount, const char* status = NULL);
	void setLevelUp(bool _leveledUp);
	void addSuffix(const char* suffix); //add suffix to end of npc name
	void setGuard(int _guard); //set guard to block attacks
	void setGift(Item* item); //item to give when talking
	void setEffect(Effect* effect, bool battle = true); //add an effect to the npc
	void removeEffect(Effect& effect);
	void setBoss(bool boss);
	void setExtraXP(int xp);
	void setExtraMonies(int monies);

	void addLinkedConvo(NPC* speaker, const Conversation& dialogue);
	void addRecruitLink(NPC* npc);
	void addDefeatRoom(NPC* npc, Room* room);
	void addLinkedDialogue(NPC* speaker, const Conversation& dialogue);
	void addLinkedTitle(NPC* npc, const char* title);
	void addLinkedDesc(NPC* npc, const char* desc);
	void addLinkedRoom(Room* room, const char* desc); //room to affect on defeat
	void addRedirect(Room* room1, Room* room2); //set room to redirect upon defeat
	
	void printDialogue(Conversation* thisone = NULL); //optionally pass a conversation to print, used by these 3 functions below
	void printRejectionDialogue(); //prints the rejection dialogue for the npc
	void printRecruitmentDialogue(); //prints the recruitment dialogue for the npc
	void printDismissalDialogue(); //prints the dismissal dialogue for the npc

	void printDamage(int damage, const char* status = NULL);
	void printEffects();
	const char* getDescription(); //gets the description of the character
	

	bool getTalkOnDefeat(); //gets if the npc talks after being defeated
	void setTalkOnDefeat(bool talk = true); //sets if the npc talks after being defeated

	bool getForceBattle(); //gets if the npc forces battle after talking
	void setForceBattle(bool force = true); //set if the npc forces battle after talking

	bool getLobster(); //get if it's the lobster
	void setLobster(Room* tunnels, bool lobster = true); //set if it's a lobster plus set its home to the tunnels since it actually starts somewhere else

	const char* getTunnelDirection(Room* room); //gets the direction back to the given room from the tunnels (for lobster only)
	void setTunnelDirection(Room* room, const char* direction); //sets tunnel directions (for lobster only)

	void defeat(); //set the enemy to defeated and do a bunch of defeated processes if applicable
	void undefeat(); //set the enemy to not defeated
protected:
	const char* title; //the title of the character (eg. VILLAGE ELDER)
	char name[255]; //the name, must not be const because we add suffixes sometimes
	const char* description; //npc's description when analyzed

	Room* home; //where the npc goes after being dismissed
	Room* currentRoom;

	vector<NPC*> party; //the npc's party if it is a leader

	Attack* standard_attack; //the npc's normal attack for generating sp
	vector<Attack*> special_attacks; //the npc's special attacks that cost sp

	vector<Effect> effects; //the effects affecting this npc
	map<Attack*, int> attackWeight; //the weight of the npc's attacks

	queue<Conversation> conversations; //npcs can have discussions with the player character, and they're stored as a queue of vectors of pairs of dialogue and the npc that spoke it

	Conversation dialogue; //dialogue that the npc says when asked, and no conversations are left
	queue<Conversation> recruitedDialogue; //dialogue that the npc says while currently recruited
	queue<Conversation> rejectionDialogue; //dialogue that the npc says when rejecting recruitment offer
	queue<Conversation> recruitmentDialogue; //dialogue that the npc says when recruited
	queue<Conversation> dismissalDialogue; //dialogue that the npc says when dismissed
	queue<Conversation> gymDialogue; //dialogue the character says when at the gym

	Conversation newDialogue; //regular dialogue after having been recruited

	Item* gift; //item that the npc holds and gifts to the player after talking

	bool recruitable = false;
	bool recruited = false; //if the npc is in the player party
	bool isPlayer;
	bool isLeader;
	bool isEnemy = false;
	bool isBoss = false; //bosses cannot be instakilled

	bool isLobster = false; //if it's the lobster
	map<Room*, const char*> tunnelLinks; //tunnel links for setting them to get back from the tunnels  if it's the lobster

	//npc stats
	int health; //current health
	int maxHealth;
	int defense;
	int attack;
	int toughness; //how much defense resists attack and pierce
	int pierce; //how much defense is ignored
	int speed; //speed determines the order of movement in battle
	int sp; //sp stands for skill points
	int maxSP;
	
	int level = 0;
	int xp = 0; //how much xp the npc has stored up

	bool leveledUp = false; //if the npc leveled up
	vector<int> statChanges = {0, 0, 0, 0, 0, 0, 0};
	Attack* newAttack; //the newest attack we got

	int healthScale = 0; //how much each stat increases (minimum) each level up
	int defenseScale = 0;
	int attackScale = 0;
	int toughnessScale = 0;
	int pierceScale = 0;
	int speedScale = 0;
	int spScale = 0;

	int hypnosis = 0; //how hypnotized the npc is
	int freeze = 0; //how frozen the npc is
	bool defeated = false; //if the npc is defeated (appears in the room)
	bool respawns = true; //if the npc appears again after battle

	bool forcebattle = false; //if we force the player to battle after talking
	bool talkOnDefeat = false; //if the npc talks when defeated

	const char* exitBlocking = NULL; //enemy npcs may block an exit until they are defeated
	Room* altRoom = NULL; //enemies block exits from both sides, so they have to be in two rooms at the same time, unfortunately
	
	queue<NPC*> recruitLinks; //these nps are set to recruitable when this npc is defeated
	queue<pair<NPC*, Conversation>> linkedConversations; //we add these conversations to the npc when defeated
	queue<pair<NPC*, Conversation>> linkedDialogue; //we set the linked npcs' dialogue to this when defeated
	queue<pair<NPC*, const char*>> linkedTitles; //we reset these npc's titles when defeated
	queue<pair<NPC*, const char*>> linkedDescriptions; //we reset these npc's descriptions when defeated

	queue<pair<NPC*, Room*>> defeatRooms; //move these npcs to these rooms when defeated

	queue<pair<Room*, const char*>> roomChanges; //we edit the description of these rooms to these descriptions after being defeated
	queue<pair<Room*, Room*>> redirectRooms; //after defeat, the first rooms are redirected to the second rooms

	bool escapable = true; //if you can escape from this enemy in a battle
	int guard = 0; //how many hits the npc can block before guard is broken

	int xpReward = 0; //extra battle rewards
	int monyReward = 0;
};
#endif