/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCETHROW1COMMAND_H_
#define FORCETHROW1COMMAND_H_

#include "ForcePowersQueueCommand.h"

class ForceThrow1Command : public ForcePowersQueueCommand {
public:

	ForceThrow1Command(const String& name, ZoneProcessServer* server)
		: ForcePowersQueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

//////////////		if (isWearingArmor(creature)) {
//////////////			return NOJEDIARMOR;
//////////////		}

		return doCombatAction(creature, target);
	}

};

#endif //FORCETHROW1COMMAND_H_
