//header file for conversations

#ifndef CONVERSATION
#define CONVERSATION

#include <vector>
#include <utility>
#include <iostream>
#include <initializer_list>
#include <memory>
#include "WorldState.h"

class NPC;

struct Conversation {
	std::vector<std::pair<NPC*, const char*>> lines;
	std::shared_ptr<Conversation> alt; //alternate dialogue if the skipcondition is true

	vector<int> skipcondition; //if we should skip this, based on WorldConditions in Helper
	bool altdialogue; //if skipcondition is true, if we should use alternate dialogue instead of just skipping the conversation

	std::pair<const char*, std::shared_ptr<Conversation>> branch1{NULL, NULL}; //we can have branching dialogue based on if the player responds one or the other const char* in the branches
	std::pair<const char*, std::shared_ptr<Conversation>> branch2{NULL, NULL};

	std::pair<NPC*, std::weak_ptr<Conversation>> relay{NULL, {}}; //give this conversation to the npc here after hearing this conversation, weak_ptr cause this might eventually point back to itself

	bool getOutdated() {
		if (Helper::WorldState[skipcondition] && !altdialogue) return true;
		return false;
	}

	bool empty() {
		return (Helper::WorldState[skipcondition] ? (alt && alt->empty()) : lines.empty());
	}

	Conversation() : skipcondition(Helper::NEVER), altdialogue(false), alt(0) {}

	Conversation(std::initializer_list<std::pair<NPC*, const char*>> _lines) : lines(_lines), skipcondition(Helper::NEVER), altdialogue(false), alt(0) {}
};

#endif