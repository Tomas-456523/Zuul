//header file for conversations

#ifndef CONVERSATION
#define CONVERSATION

#include <queue>
#include <utility>
#include <iostream>
#include "Helper.h"

class NPC;

struct Conversation {
    std::vector<pair<NPC*, const char*>> lines;
    std::vector<pair<NPC*, const char*>> altlines;

    int skipcondition; //if we should skip this, based on WorldConditions in Helper
    bool altdialogue; //if skipcondition is true, if we should use alternate dialogue instead of just skipping the conversation

    bool getOutdated() {
        if (skipcondition && !altdialogue) return true;
        return false;
    }

    void printDialogue() {
        cout << "\n";
        vector<pair<NPC*, const char*>>& convo = (Helper::WorldState[skipcondition] ? altlines : lines);
        for (int i = 0; i < convo.size(); i++) { //prints all the dialogue in the conversation
            if (convo[i].first != NULL) {
                cout << convo[i].first->getName() << " - \"" << convo[i].second << "\"";
            } else {
                cout << convo[i].second;
            }
            if (i + 1 < convo.size()) { //if it's not the last one we do a pause, so the last one lets the player type
                Helper::CinPause();
            }
        }
    }

    Conversation() : skipcondition(Helper::NEVER), altdialogue(false) {}
};

#endif