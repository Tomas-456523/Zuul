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
#include "Stats.h"
#include "WorldState.h"
#include "WorldChange.h"
using namespace std;

class Room; //forward declares room because these two classes reference each other
class Item;

class NPC {
public: //you need to set stats on creation
	NPC(const char* _title, const char* _name, const char* _description, Room* room, int _level = 0, Stats _basestats = Stats(), Stats _scale = Stats(), bool _isleader = false, bool _player = false);
	~NPC();

	NPC(const NPC& other);

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
	Attack* getRecoilAttack();
	vector<Attack*> getSpecialAttacks();
	map<Attack*, int> getWeights();
	bool getLevelUp(); //if we leveled up recently
	Stats getStatChanges(); //the stat changes from the last level up
	vector<Attack*> getNewAttacks();
	bool getDefeated(); //if npc has been defeated since entering the room
	Item* takeGift(); //return and nullify the gift item
	vector<Effect>& getEffects();
	int getHypnotized(); //hypnosis amount
	int getFrozen(); //freeze amount
	int getConvoSize(); //how many conversations are left to say
	bool getRespawn(); //if they respawn
	bool getBoss();
	float getAttMultiplier();
	float getDefMultiplier();
	float getToughMultiplier();
	float getPierceMultiplier();
	float getSPUseMultiplier();
	float getDamageMultiplier();
	time_t getGymStart();
	int getGuard();
	bool getAway();
	Effect* getEffect(const char* effect); //find the effect with that name in the npc

	NPC* getGuardian();
	NPC* getGuarding();

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
	void setScale(Stats _scale);
	void setBaseStats(Stats _stats); //reset base stats, used by viola
	void addXp(int _xp);
	void levelUp(bool trackLevelUp = false, int instant = 0);
	void setLeader(bool _leader, int _level = 0, Room* room = NULL, bool respawn = true, bool boss = false);
	int damage(float power, float pierce, int hits = 1);
	void directDamage(int damage, const char* status = NULL);
	void setLevel(int _level); //only used for enemy parties
	void setBasicAttack(Attack* attack);
	void setRecoilAttack(Attack* attack);
	void addSpecialAttack(Attack* attack);
	void removeSpecialAttack(Attack* attack);
	void blockExit(const char* _exitBlocking, const char* type, const char* reason, bool bothsides = false);
	void setEscapable(bool _escapable);
	void calculateWeights();
	void setEnemy(bool _enemy);
	void alterSp(int amount, const char* status = NULL);
	void setLevelUp(bool _leveledUp);
	void addSuffix(const char* suffix); //add suffix to end of npc name
	void setGuard(int _guard); //set guard to block attacks
	void setGift(Item* item, bool fightfirst = false); //item to give when talking
	void setEffect(Effect* effect, bool battle = true); //add an effect to the npc
	void removeEffect(Effect& effect, bool announce = true);
	void setBoss(bool boss);
	void setExtraXP(int xp);
	void setExtraMonies(int monies);
	void setTalkOnRecruit(bool talk);
	void setGuardian(NPC* npc);
	void setGuarding(NPC* npc);
	void setGymStart(time_t start);
	void setWorldCondition(size_t cond); //set a world condition for this npc to edit on defeat

	void addLinkedConvo(NPC* speaker, const Conversation& dialogue);
	void addRecruitLink(NPC* npc);
	void addDefeatRoom(NPC* npc, Room* room);
	void addLinkedDialogue(NPC* speaker, const Conversation& dialogue);
	void addLinkedTitle(NPC* npc, const char* title);
	void addLinkedDesc(NPC* npc, const char* desc);
	void addLinkedRoom(Room* room, const char* desc); //room to affect on defeat
	void addRedirect(Room* room1, Room* room2); //set room to redirect upon defeat
	void addLinkedStats(NPC* npc, Stats stats);
	void addAttackRemoval(NPC* npc, Attack* attack);
	void guardItem(Item* item); //start guarding the item
	
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

	int id; //npc's index in npcsH

	vector<NPC*> party; //the npc's party if it is a leader

	Attack* standard_attack = NULL; //the npc's normal attack for generating sp
	vector<Attack*> special_attacks; //the npc's special attacks that cost sp

	Attack* recoilattack = NULL; //the attack that happens when this npc is hit

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

	bool speakOnRecruit = false;

	Item* gift = NULL; //item that the npc holds and gifts to the player after talking
	bool battleReward = false; //if the gift is a battle reward

	bool recruitable = false;
	bool recruited = false; //if the npc is in the player party
	bool isPlayer;
	bool isLeader;
	bool isEnemy = false;
	bool isBoss = false; //bosses cannot be instakilled

	NPC* guardian = NULL; //what npc is guarding this one
	NPC* guarding = NULL; //what npc this npc is guarding

	bool isLobster = false; //if it's the lobster
	map<Room*, const char*> tunnelLinks; //tunnel links for setting them to get back from the tunnels  if it's the lobster

	//npc stats
	Stats basestats; //base stats
	Stats stats; //the actual stats including level up increases
	Stats scale; //how much each stat increases (minimum) each level up
	int health; //current health
	int sp; //sp stands for skill points
	float attackMultiplier = 1; //multipliers
	float defenseMultiplier = 1;
	float pierceMultiplier = 1;
	float toughMultiplier = 1;
	float spUseMultiplier = 1; //multiplies sp cost
	float damageMultiplier = 1; //multiplies damage taken
	
	int level = 0;
	int xp = 0; //how much xp the npc has stored up

	bool leveledUp = false; //if the npc leveled up
	Stats statChangesSum; //sum of all the stat changes that have occured and haven't been printed yet
	vector<Attack*> newAttacks; //the newest attacks we've gotten

	int hypnosis = 0; //how hypnotized the npc is
	int freeze = 0; //how frozen the npc is
	bool away = false; //if the npc is away from the battle
	bool defeated = false; //if the npc is defeated (appears in the room)
	bool respawns = true; //if the npc appears again after battle

	bool forcebattle = false; //if we force the player to battle after talking
	bool talkOnDefeat = false; //if the npc talks when defeated

	const char* exitBlocking = NULL; //enemy npcs may block an exit until they are defeated
	Room* altRoom = NULL; //enemies block exits from both sides, so they have to be in two rooms at the same time, unfortunately
	
	WorldChange changes; //stuff this npc does when defeated

	time_t gymStart = 0; //what time the npc was left at the gym (0 means is not at the gym, bad news for anybody hoping to drop their teammate off at the gym on the first second of 1970)

	bool escapable = true; //if you can escape from this enemy in a battle
	int guard = 0; //how many hits the npc can block before guard is broken

	int xpReward = 0; //extra battle rewards
	int monyReward = 0;
};
#endif