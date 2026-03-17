//header file for these thingies, en extension of Helper.h, but must be seperate file due to Conversation needing to know these but not being able to include Helper.h due to the whole circular dependency situation

#ifndef WORLDCONDITION
#define WORLDCONDITION

namespace Helper {
	enum WorldCondition {
		RECOILED,
		VIOLADEF,
		VALVEUSED,
		CEOQUEST,
		TEMPLEQUEST,
		BURGERMANDEF,
		BURGERMENDEF,
		NEVER //never set never to true
	};

	extern bool WorldState[NEVER+1]; //the state of the world! (must be as sized according to the last condition in WorldCondition, works since enumerators are technically ints!)
}

#endif