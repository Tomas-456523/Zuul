/* Tomas Carranza Echaniz
*  5/19/26
*  This is the header file for conversations
*  
*  Although the struct is called a conversation, it handles any series of lines of text, including things like room welcomes
*  or item use text. The lines of dialogue are formatted as {speaker, text}, and are printed by passing a pointer to the
*  conversation to the Helper namespace's printConversation function. If a line has a speaker, it is printed like:
*      SPEAKER - "this is my dialogue"
*  If the line has a NULL speaker, it simply prints the text on its own with no quotation marks.
*  
*  Most conversations are just a series of lines of text, but they can have more complex behavior such as branching dialogue
*  or moving onto a following Conversation. There is no helper to make these behaviors, so they must be manually set up.
*  A more common behavior is moving to an alternate Conversation if a certain world condition is true. Conversations also
*  have the ability to make world changes. This was implemented in order to make branching conversations be able to make
*  changes in one branch but not in the other. Conversations can also relay another Conversation to an NPC, in order to be
*  able tomake loops of relaying Conversations.
*/

#ifndef CONVERSATION
#define CONVERSATION

#include <vector>
#include <utility>
#include <iostream>
#include <initializer_list>
#include <memory>
#include "WorldState.h"

class NPC;
struct WorldChange;

struct Conversation {
	std::vector<std::pair<NPC*, const char*>> lines;
	std::shared_ptr<Conversation> alt; //alternate dialogue if the skipcondition is true

	std::vector<size_t> skipcondition; //if we should skip this, based on WorldConditions in Helper

	std::pair<const char*, std::shared_ptr<Conversation>> branch1{NULL, NULL}; //we can have branching dialogue based on if the player responds one or the other const char* in the branches
	std::pair<const char*, std::shared_ptr<Conversation>> branch2{NULL, NULL};

	std::pair<NPC*, std::weak_ptr<Conversation>> relay{NULL, {}}; //give this conversation to the npc here after hearing this conversation, weak_ptr cause this might eventually point back to itself

	std::shared_ptr<Conversation> next; //this conversation gets printed immediately after this one, we use this so we can have seperate chunks having their own skip conditions

	std::shared_ptr<WorldChange> convochanges; //world change that happens when this conversation is heard, only used by npc conversations, everything else which is technically a conversation not so much

	bool goToAlt() const { //gets if we should go to the alternate conversation
		bool gotoalt = false;
		for (size_t i : skipcondition) {
			if (Helper::WorldState[i]) return true;
		}
		return false;
	}

	bool getOutdated() const { //gets if we shouldn't print this conversation
		return (goToAlt() && !alt);
	}

	bool empty() const { //gets if this is a default conversation with no dialogue
		return (goToAlt() ? (alt && alt->empty()) : lines.empty());
	}

	Conversation() : skipcondition(Helper::NEVER), alt(0) {}

	Conversation(std::initializer_list<std::pair<NPC*, const char*>> _lines) : lines(_lines), skipcondition(Helper::NEVER), alt(0) {}
};

#endif