/*
 * MissionManagerImplementation.cpp
 *
 *  Created on: 21/06/2010
 *      Author: victor
 */

#include "server/zone/managers/mission/MissionManager.h"
#include "server/zone/objects/tangible/terminal/mission/MissionTerminal.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/objects/mission/MissionObject.h"
#include "server/zone/objects/mission/SurveyMissionObjective.h"
#include "server/zone/objects/mission/DestroyMissionObjective.h"
#include "server/zone/objects/mission/DeliverMissionObjective.h"
#include "server/zone/objects/mission/CraftingMissionObjective.h"
#include "server/zone/objects/mission/EntertainerMissionObjective.h"
#include "server/zone/objects/mission/HuntingMissionObjective.h"
#include "server/zone/objects/mission/ReconMissionObjective.h"
#include "server/zone/objects/mission/BountyMissionObjective.h"
#include "server/zone/objects/region/SpawnArea.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "templates/manager/TemplateManager.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/planet/MissionTargetMap.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/name/NameManager.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"
#include "templates/mobile/LairTemplate.h"
#include "server/zone/objects/tangible/tool/SurveyTool.h"
#include "server/zone/Zone.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/objects/player/FactionStatus.h"
#include "server/zone/managers/visibility/VisibilityManager.h"
#include "server/zone/objects/building/BuildingObject.h"

void MissionManagerImplementation::loadLuaSettings() {
	try {
		Lua* lua = new Lua();
		lua->init();

		lua->runFile("scripts/managers/mission/mission_manager.lua");

		LuaObject items = lua->getGlobalObject("crafting_mission_items");

		for (int i = 1; i <= items.getTableSize(); i++) {
			craftingMissionItems.add(items.getStringAt(i));
		}

		items.pop();

		LuaObject zones = lua->getGlobalObject("bh_target_zones");

		for (int i = 1; i <= zones.getTableSize(); i++) {
			bhTargetZones.add(zones.getStringAt(i));
		}

		zones.pop();

		LuaObject targetsAtMissionLevel = lua->getGlobalObject("bh_targets_at_mission_level");

		for (unsigned int i = 1; i <= 3; i++) {
			Vector<String> targets;
			LuaObject level = targetsAtMissionLevel.getObjectField("level" + String::valueOf(i));

			for (int j = 1; j <= level.getTableSize(); j++) {
				targets.add(level.getStringAt(j));
			}
			level.pop();

			bhTargetsAtMissionLevel.put(i, targets);
		}

		targetsAtMissionLevel.pop();

		String value = lua->getGlobalString("enable_factional_crafting_missions");

		if (value.toLowerCase() == "true") {
			enableFactionalCraftingMissions = true;
		}

		value = lua->getGlobalString("enable_factional_recon_missions");

		if (value.toLowerCase() == "true") {
			enableFactionalReconMissions = true;
		}

		value = lua->getGlobalString("enable_factional_entertainer_missions");

		if (value.toLowerCase() == "true") {
			enableFactionalEntertainerMissions = true;
		}

		value = lua->getGlobalString("enable_same_account_bounty_missions");

		if (value.toLowerCase() == "true") {
			enableSameAccountBountyMissions = true;
		}

		playerBountyKillBuffer = lua->getGlobalLong("playerBountyKillBuffer");
		playerBountyDebuffLength = lua->getGlobalLong("playerBountyDebuffLength");

		destroyMissionBaseDistance = lua->getGlobalLong("destroyMissionBaseDistance");
		destroyMissionDifficultyDistanceFactor = lua->getGlobalLong("destroyMissionDifficultyDistanceFactor");
		destroyMissionRandomDistance = lua->getGlobalLong("destroyMissionRandomDistance");
		destroyMissionDifficultyRandomDistance = lua->getGlobalLong("destroyMissionDifficultyRandomDistance");
		destroyMissionBaseReward = lua->getGlobalLong("destroyMissionBaseReward");
		destroyMissionDifficultyRewardFactor = lua->getGlobalLong("destroyMissionDifficultyRewardFactor");
		destroyMissionRandomReward = lua->getGlobalLong("destroyMissionRandomReward");
		destroyMissionDifficultyRandomReward = lua->getGlobalLong("destroyMissionDifficultyRandomReward");

		delete lua;
	}
	catch (Exception& e) {
		error(e.getMessage());
	}
}

void MissionManagerImplementation::loadPlayerBounties() {
	info("Loading player bounties from playerbounties.db");

	ObjectDatabaseManager* dbManager = ObjectDatabaseManager::instance();
	ObjectDatabase* playerBountyDatabase = dbManager->loadObjectDatabase("playerbounties", true);

	if (playerBountyDatabase == nullptr) {
		error("Could not load the player bounties database.");
		return;
	}

	int i = 0;

	try {
		ObjectDatabaseIterator iterator(playerBountyDatabase);

		uint64 objectID = 0;

		while (iterator.getNextKey(objectID)) {
			Reference<PlayerBounty*> bounty = Core::getObjectBroker()->lookUp(objectID).castTo<PlayerBounty*>();

			if (bounty != nullptr) {
				++i;

				bounty->setOnline(false);

				playerBountyList.put(bounty->getTargetPlayerID(), bounty);

				if (ConfigManager::instance()->isProgressMonitorActivated())
					printf("\r\tLoading player bounties [%d] / [?]\t", i);
			} else {
				error("Failed to deserialize player bounty with objectID: " + String::valueOf(objectID));
			}
		}
	} catch (DatabaseException& e) {
		error("Database exception in MissionManager::loadPlayerBounties(): " + e.getMessage());
	}

	info(i > 0) << "Loaded " << i << " player bounties.";
}

void MissionManagerImplementation::handleMissionListRequest(MissionTerminal* missionTerminal, CreatureObject* player, int counter) {
	// newbie and statue terminals don't exist, but their templates do
	if (missionTerminal->isStatueTerminal() || missionTerminal->isNewbieTerminal()) {
		player->sendSystemMessage("@skill_teacher:skill_terminal_disabled");
		return;
	}

	if (!player->isInRange(missionTerminal, 6)) {
		StringIdChatParameter stringId("ui", "radial_out_of_range_prose"); // %TT is too far away to '%TO'.
		stringId.setTT(missionTerminal->getObjectID());
		stringId.setTO("ui_radial", "terminal_mission_list"); // List Missions
		player->sendSystemMessage(stringId);
		return;
	}

	if (missionTerminal->isBountyTerminal()) {
		if (!player->hasSkill("combat_bountyhunter_novice")) {
			player->sendSystemMessage("@mission/mission_generic:not_bounty_hunter_terminal");
			return;
		}
	}

	ManagedReference<CityRegion*> terminalCity = missionTerminal->getCityRegion().get();

	if (terminalCity != nullptr) {
		if (terminalCity.get()->isBanned(player->getObjectID())) {
			player->sendSystemMessage("@city/city:banned_services"); // You are banned from using this city's services.
			return;
		}
	}

	if (!player->checkCooldownRecovery("mission_list_request")) {
		player->sendSystemMessage("You can't request missions yet, please wait a bit before trying again.");
		return;
	}

	// UI gray out is for 1500ms, default to 1400 to give a bit of gracetime
	player->addCooldown("mission_list_request", ConfigManager::instance()->getInt("Core3.MissionManager.ListRequestCooldown", 1400));

	ManagedReference<SceneObject*> missionBag = player->getSlottedObject("mission_bag");

	if (missionBag == nullptr)
		return;

	int maximumNumberOfItemsInMissionBag = 21; // Changed from 12 to 21 to accommodate 15 destroy + 6 deliver missions

	if (enableFactionalCraftingMissions) {
		maximumNumberOfItemsInMissionBag += 6;
	}

	if (enableFactionalReconMissions) {
		maximumNumberOfItemsInMissionBag += 6;
	}

	if (enableFactionalEntertainerMissions) {
		maximumNumberOfItemsInMissionBag += 12; //Both musician and dancer.
	}

	while (missionBag->getContainerObjectsSize() < maximumNumberOfItemsInMissionBag) {
		ManagedReference<SceneObject*> mission = server->createObject(0x18e19914, 1); // empty mission
		missionBag->transferObject(mission, -1, false);
		mission->sendTo(player, true);
		//missionBag->updateToDatabaseWithoutChildren();
	}

	populateMissionList(missionTerminal, player, counter);
}

void MissionManagerImplementation::handleMissionAccept(MissionTerminal* missionTerminal, MissionObject* mission, CreatureObject* player) {
	ManagedReference<SceneObject*> missionBag = mission->getParent().get();

	if (missionBag == nullptr)
		return;

	ManagedReference<SceneObject*> bagParent = missionBag->getParent().get();

	if (bagParent != player)
		return;

	SceneObject* playerMissionBag = player->getSlottedObject("mission_bag");

	if (playerMissionBag != missionBag)
		return;

	SceneObject* datapad = player->getSlottedObject("datapad");

	if (mission->getParent().get() == datapad)
		return;

	int missionCount = 0;
	bool hasBountyMission = false;

	for (int i = 0; i < datapad->getContainerObjectsSize(); ++i) {
		SceneObject* obj = datapad->getContainerObject(i);

		if (obj->isMissionObject()) {
			++missionCount;
			MissionObject* datapadMission = cast<MissionObject*>(obj);
			if (datapadMission->getTypeCRC() == MissionTypes::BOUNTY) {
				hasBountyMission = true;
			}
		}
	}

	//Limit to two missions (only one of them can be a bounty mission)
    if (missionCount >= 12 || (hasBountyMission && mission->getTypeCRC() == MissionTypes::BOUNTY)) {
		StringIdChatParameter stringId("mission/mission_generic", "too_many_missions");
		player->sendSystemMessage(stringId);
		return;
	}

	if (mission->getTypeCRC() == MissionTypes::BOUNTY) {
		Locker listLocker(&playerBountyListMutex);

		uint64 targetID = mission->getTargetObjectId();
		if (targetID != 0) {
			PlayerBounty* bounty = playerBountyList.get(targetID);

			if (bounty == nullptr || !isBountyValidForPlayer(player, bounty)) {
				player->sendSystemMessage("Mission has expired.");
				return;
			} else {
				addBountyHunterToPlayerBounty(targetID, player->getObjectID());
			}
		}
	}

	datapad->transferObject(mission, -1, true);

	createMissionObjectives(mission, missionTerminal, player);

	player->updateToDatabaseAllObjects(false);
}

void MissionManagerImplementation::createDestroyMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	ManagedReference<DestroyMissionObjective*> objective = new DestroyMissionObjective(mission);

	Locker locker(objective);

	objective->setLairTemplateToSpawn(mission->getTargetOptionalTemplate());
	objective->setDifficultyLevel(mission->getDifficultyLevel());
	objective->setDifficulty(mission->getDifficulty());

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createDeliverMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	ManagedReference<DeliverMissionObjective*> objective = new DeliverMissionObjective(mission);

	Locker locker(objective);

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createCraftingMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	//Check if player already got an crafting mission and what item it uses.
	SceneObject* datapad = player->getSlottedObject("datapad");

	if (datapad == nullptr) {
		return;
	}

	if (mission->getFaction() != Factions::FACTIONNEUTRAL && !enableFactionalCraftingMissions) {
		//Faction mission, but faction missions are disabled.
		return;
	}

	int datapadSize = datapad->getContainerObjectsSize();
	String missionItem = "";

	for (int i = 0; i < datapadSize; ++i) {
		if (datapad->getContainerObject(i)->isMissionObject()) {
			Reference<MissionObject*> datapadMission = datapad->getContainerObject(i).castTo<MissionObject*>();

			if (datapadMission != nullptr && datapadMission->getTypeCRC() == MissionTypes::CRAFTING && datapadMission != mission) {
				//Crafting mission found, store the item.
				missionItem = datapadMission->getTemplateString1();
			}
		}
	}

	//Pick a random item for the new mission.
	int itemNumber = System::random(craftingMissionItems.size() - 1);

	//Get next item in line if the player already got a mission with the random generated item.
	if (craftingMissionItems.get(itemNumber) == missionItem) {
		itemNumber = (itemNumber + 1) % craftingMissionItems.size();
	}

	String itemTemplate = craftingMissionItems.get(itemNumber).replaceFirst("draft_schematic/item/", "tangible/mission/");
	mission->setTargetTemplate(TemplateManager::instance()->getTemplate(STRING_HASHCODE("object/tangible/mission/quest_item/attunement_grid.iff")));
	mission->setTemplateStrings(craftingMissionItems.get(itemNumber), itemTemplate);

	ManagedReference<CraftingMissionObjective*> objective = new CraftingMissionObjective(mission);

	Locker locker(objective);

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createSurveyMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	ManagedReference<SurveyMissionObjective*> objective = new SurveyMissionObjective(mission);

	Locker locker(objective);

	objective->setEfficiency(mission->getDifficultyLevel());

	String spawnName = mission->getTargetName();

	objective->setSpawnFamily(spawnName);

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createEntertainerMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	ManagedReference<EntertainerMissionObjective*> objective = new EntertainerMissionObjective(mission);

	Locker locker(objective);

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createHuntingMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	ManagedReference<HuntingMissionObjective*> objective = new HuntingMissionObjective(mission);

	Locker locker(objective);

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createReconMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	ManagedReference<ReconMissionObjective*> objective = new ReconMissionObjective(mission);

	Locker locker(objective);

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createBountyMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	ManagedReference<BountyMissionObjective*> objective = new BountyMissionObjective(mission);

	Locker locker(objective);

	ObjectManager::instance()->persistObject(objective, 1, "missionobjectives");

	mission->setMissionObjective(objective);
	objective->activate();
}

void MissionManagerImplementation::createMissionObjectives(MissionObject* mission, MissionTerminal* missionTerminal, CreatureObject* player) {
	uint32 missionType = mission->getTypeCRC();

	switch (missionType) {
	case MissionTypes::SURVEY:
		createSurveyMissionObjectives(mission, missionTerminal, player);
		break;
	case MissionTypes::DESTROY:
		createDestroyMissionObjectives(mission, missionTerminal, player);
		break;
	case MissionTypes::DELIVER:
		createDeliverMissionObjectives(mission, missionTerminal, player);
		break;
	case MissionTypes::HUNTING:
		createHuntingMissionObjectives(mission, missionTerminal, player);
		break;
	case MissionTypes::RECON:
		createReconMissionObjectives(mission, missionTerminal, player);
		break;
	case MissionTypes::BOUNTY:
		createBountyMissionObjectives(mission, missionTerminal, player);
		break;
	case MissionTypes::CRAFTING:
		createCraftingMissionObjectives(mission, missionTerminal, player);
		break;
	case MissionTypes::DANCER:
	case MissionTypes::MUSICIAN:
		createEntertainerMissionObjectives(mission, missionTerminal, player);
		break;
	default:
		break;
	}
}

void MissionManagerImplementation::removeMission(MissionObject* mission, CreatureObject* player) {
	ManagedReference<MissionObject*> ref = mission;

	ManagedReference<SceneObject*> missionParent = mission->getParent().get();
	SceneObject* datapad = player->getSlottedObject("datapad");

	if (missionParent != datapad)
		return;

	uint64 targetId = 0;

	if (mission->getTypeCRC() == MissionTypes::BOUNTY) {
		targetId = mission->getTargetObjectId();
		removeBountyHunterFromPlayerBounty(targetId, player->getObjectID());
	}

	Locker mlocker(mission);

	mission->destroyObjectFromWorld(true);
	mission->sendDestroyTo(player);

	mission->destroyObjectFromDatabase(true);
	player->updateToDatabaseAllObjects(false);

	mlocker.release();

	if (targetId != 0) {
		ManagedReference<CreatureObject*> target = server->getObject(targetId).castTo<CreatureObject*>();

		if (target != nullptr)
			target->sendPvpStatusTo(player);
	}

	if (player->isGrouped() && player->getGroup() != nullptr) {
		Reference<GroupObject*> group = player->getGroup();

		Locker locker(group);

		group->scheduleUpdateNearestMissionForGroup(player->getPlanetCRC());
	}
}

void MissionManagerImplementation::handleMissionAbort(MissionObject* mission, CreatureObject* player, bool questMessage) {
	if (player->isIncapacitated()) {
		player->sendSystemMessage("You cannot abort a mission while incapacitated.");
		return;
	}

	if (player->isInCombat()) {
		player->sendSystemMessage("You cannot abort a mission while in combat.");
		return;
	}

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

	if (ghost != nullptr) {
		if (mission->getTypeCRC() == MissionTypes::BOUNTY && ghost->hasBhTef()) {
			player->sendSystemMessage("You cannot abort a bounty hunter mission this soon after being in combat with the mission target.");
			return;
		}

		// Space Missions
		uint32 questCRC = mission->getQuestCRC();

		if (questCRC > 0) {
			ghost->clearJournalQuest(questCRC, false);

			if (questMessage) {
				String questString = "@spacequest/" + mission->getQuestType() + "/" + mission->getQuestName() + ":title";

				StringIdChatParameter spaceAbort("space/quest", "quest_aborted");
				spaceAbort.setTO(questString);

				player->sendSystemMessage(spaceAbort);

				player->playMusicMessage("sound/music_themequest_fail_criminal.snd");
			}
		}
	}

	mission->abort();

	removeMission(mission, player);
}

void MissionManagerImplementation::populateMissionList(MissionTerminal* missionTerminal, CreatureObject* player, int counter) {
	Locker crossLocker(missionTerminal, player);

	bool slicer = missionTerminal->isSlicer(player);

	if (missionTerminal->isGeneralTerminal()) {
		randomizeGeneralTerminalMissions(player, counter, slicer);
	} else if (missionTerminal->isArtisanTerminal()) {
		randomizeArtisanTerminalMissions(player, counter, slicer);
	} else if (missionTerminal->isEntertainerTerminal()) {
		randomizeEntertainerTerminalMissions(player, counter, slicer);
	} else if (missionTerminal->isScoutTerminal()) {
		randomizeScoutTerminalMissions(player, counter, slicer);
	} else if (missionTerminal->isBountyTerminal()) {
		randomizeBountyTerminalMissions(player, counter);
	} else if (missionTerminal->isImperialTerminal()) {
		randomizeFactionTerminalMissions(player, counter, slicer, Factions::FACTIONIMPERIAL);
	} else if (missionTerminal->isRebelTerminal()) {
		randomizeFactionTerminalMissions(player, counter, slicer, Factions::FACTIONREBEL);
	}

	// Remove the Slicer from the List. They have received their one time mission reward increase.
	if (slicer)
		missionTerminal->removeSlicer(player);

}

void MissionManagerImplementation::randomizeGeneralTerminalMissions(CreatureObject* player, int counter, bool slicer) {
	SceneObject* missionBag = player->getSlottedObject("mission_bag");
	int bagSize = missionBag->getContainerObjectsSize();

	for (int i = 0; i < bagSize; ++i) {
		Reference<MissionObject*> mission = missionBag->getContainerObject(i).castTo<MissionObject*>( );

		Locker locker(mission);

		//Clear mission type before calling mission generators.
		mission->setTypeCRC(0);

		if (i < 15) { // Changed from 6 to 15 destroy missions
			randomizeGenericDestroyMission(player, mission, Factions::FACTIONNEUTRAL);
		} else if (i < 21) { // Adjusted starting index for deliver missions
			randomizeGenericDeliverMission(player, mission, Factions::FACTIONNEUTRAL);
		}

		if (slicer) {
			mission->setRewardCredits(mission->getRewardCredits() * 1.5);
		}

		float cityBonus = 1.f + player->getSkillMod("private_spec_missions") / 100.f;
		mission->setRewardCredits(mission->getRewardCredits() * cityBonus);

		mission->setRefreshCounter(counter, true);
	}
}

void MissionManagerImplementation::randomizeArtisanTerminalMissions(CreatureObject* player, int counter, bool slicer) {
	SceneObject* missionBag = player->getSlottedObject("mission_bag");
	int bagSize = missionBag->getContainerObjectsSize();

	for (int i = 0; i < bagSize; ++i) {
		Reference<MissionObject*> mission = missionBag->getContainerObject(i).castTo<MissionObject*>( );

		Locker locker(mission);

		//Clear mission type before calling mission generators.
		mission->setTypeCRC(0);

		if (i < 6) {
			randomizeGenericSurveyMission(player, mission, Factions::FACTIONNEUTRAL);
		} else if (i < 12) {
			randomizeGenericCraftingMission(player, mission, Factions::FACTIONNEUTRAL);
		}

		if (slicer) {
			mission->setRewardCredits(mission->getRewardCredits() * 1.5);
		}

		float cityBonus = 1.f + player->getSkillMod("private_spec_missions") / 100.f;
		mission->setRewardCredits(mission->getRewardCredits() * cityBonus);

		mission->setRefreshCounter(counter, true);
	}
}

void MissionManagerImplementation::randomizeEntertainerTerminalMissions(CreatureObject* player, int counter, bool slicer) {
	SceneObject* missionBag = player->getSlottedObject("mission_bag");
	int bagSize = missionBag->getContainerObjectsSize();

	for (int i = 0; i < bagSize; ++i) {
		Reference<MissionObject*> mission = missionBag->getContainerObject(i).castTo<MissionObject*>( );

		Locker locker(mission);

		//Clear mission type before calling mission generators.
		mission->setTypeCRC(0);

		if (i < 6) {
			randomizeGenericEntertainerMission(player, mission, Factions::FACTIONNEUTRAL, MissionTypes::DANCER);
		} else if (i < 12) {
			randomizeGenericEntertainerMission(player, mission, Factions::FACTIONNEUTRAL, MissionTypes::MUSICIAN);
		}

		if (slicer) {
			mission->setRewardCredits(mission->getRewardCredits() * 1.5);
		}

		float cityBonus = 1.f + player->getSkillMod("private_spec_missions") / 100.f;
		mission->setRewardCredits(mission->getRewardCredits() * cityBonus);

		mission->setRefreshCounter(counter, true);
	}
}

void MissionManagerImplementation::randomizeScoutTerminalMissions(CreatureObject* player, int counter, bool slicer) {
	SceneObject* missionBag = player->getSlottedObject("mission_bag");
	int bagSize = missionBag->getContainerObjectsSize();

	for (int i = 0; i < bagSize; ++i) {
		Reference<MissionObject*> mission = missionBag->getContainerObject(i).castTo<MissionObject*>( );

		Locker locker(mission);

		//Clear mission type before calling mission generators.
		mission->setTypeCRC(0);

		if (i < 6) {
			randomizeGenericReconMission(player, mission, Factions::FACTIONNEUTRAL);
		} else if (i < 12) {
			randomizeGenericHuntingMission(player, mission, Factions::FACTIONNEUTRAL);
		}

		if (slicer) {
			mission->setRewardCredits(mission->getRewardCredits() * 1.5);
		}

		float cityBonus = 1.f + player->getSkillMod("private_spec_missions") / 100.f;
		mission->setRewardCredits(mission->getRewardCredits() * cityBonus);

		mission->setRefreshCounter(counter, true);
	}
}

void MissionManagerImplementation::randomizeBountyTerminalMissions(CreatureObject* player, int counter) {
	SceneObject* missionBag = player->getSlottedObject("mission_bag");
	int bagSize = missionBag->getContainerObjectsSize();

	Vector<ManagedReference<PlayerBounty*>> potentialTargets = getPotentialPlayerBountyTargets(player);

	for (int i = 0; i < bagSize; ++i) {
		Reference<MissionObject*> mission = missionBag->getContainerObject(i).castTo<MissionObject*>( );

		Locker locker(mission);

		//Clear mission type before calling mission generators.
		mission->setTypeCRC(0);

		if (i < 10) {
			randomizeGenericBountyMission(player, mission, Factions::FACTIONNEUTRAL, &potentialTargets);
		}

		float cityBonus = 1.f + player->getSkillMod("private_spec_missions") / 100.f;
		mission->setRewardCredits(mission->getRewardCredits() * cityBonus);

		mission->setRefreshCounter(counter, true);
	}
}

void MissionManagerImplementation::randomizeFactionTerminalMissions(CreatureObject* player, int counter, bool slicer, const uint32 faction) {
	SceneObject* missionBag = player->getSlottedObject("mission_bag");
	int bagSize = missionBag->getContainerObjectsSize();

	int numberOfCraftingMissions = 0;
	int numberOfReconMissions = 0;
	int numberOfDancerMissions = 0;
	int numberOfMusicianMissions = 0;

	for (int i = 0; i < bagSize; ++i) {
		Reference<MissionObject*> mission = missionBag->getContainerObject(i).castTo<MissionObject*>( );

		Locker locker(mission);

		//Clear mission type before calling mission generators.
		mission->setTypeCRC(0);

		if (i < 6) {
			randomizeGenericDestroyMission(player, mission, faction);
		} else if (i < 12) {
			randomizeGenericDeliverMission(player, mission, faction);
		} else {
			if (enableFactionalCraftingMissions && numberOfCraftingMissions < 6) {
				randomizeGenericCraftingMission(player, mission, faction);
				numberOfCraftingMissions++;
			} else if (enableFactionalReconMissions && numberOfReconMissions < 6) {
				randomizeGenericReconMission(player, mission, faction);
				numberOfReconMissions++;
			} else if (enableFactionalEntertainerMissions && numberOfDancerMissions < 6) {
				randomizeGenericEntertainerMission(player, mission, faction, MissionTypes::DANCER);
				numberOfDancerMissions++;
			} else if (enableFactionalEntertainerMissions && numberOfMusicianMissions < 6) {
				randomizeGenericEntertainerMission(player, mission, faction, MissionTypes::MUSICIAN);
				numberOfMusicianMissions++;
			}
		}

		if (slicer) {
			mission->setRewardCredits(mission->getRewardCredits() * 1.5);
		}

		float cityBonus = 1.f + player->getSkillMod("private_spec_missions") / 100.f;
		mission->setRewardCredits(mission->getRewardCredits() * cityBonus);

		mission->setRefreshCounter(counter, true);
	}
}

void MissionManagerImplementation::randomizeGenericDestroyMission(CreatureObject* player, MissionObject* mission, const uint32 faction) {
	Zone* zone = player->getZone();

	if (zone == nullptr) {
		return;
	}

	LairSpawn* randomLairSpawn = getRandomLairSpawn(player, faction, MissionTypes::DESTROY);

	if (randomLairSpawn == nullptr) {
		return;
	}

	String lairTemplate = randomLairSpawn->getLairTemplateName();
	LairTemplate* lairTemplateObject = CreatureTemplateManager::instance()->getLairTemplate(lairTemplate.hashCode());

	if (lairTemplateObject == nullptr) {
		return;
	}

	int playerLevel = server->getPlayerManager()->calculatePlayerLevel(player);
	int maxDiff = randomLairSpawn->getMaxDifficulty();
	int minDiff = randomLairSpawn->getMinDifficulty();
	int difficultyLevel = System::random(maxDiff - minDiff) + minDiff;
	int difficulty = (difficultyLevel - minDiff) / ((maxDiff > (minDiff + 5) ? maxDiff - minDiff : 5) / 5);

	if (difficulty == 5)
		difficulty = 4;

	int diffDisplay = difficultyLevel < 5 ? 4 : difficultyLevel;

	PlayerObject* targetGhost = player->getPlayerObject();
    
    String level = targetGhost->getScreenPlayData("mission_level_choice", "levelChoice");
    
    int levelChoice = Integer::valueOf(level);
    
	if(levelChoice > 0){
		diffDisplay += levelChoice;
	}else{
		if (player->isGrouped()) {
			bool includeFactionPets = faction != Factions::FACTIONNEUTRAL || ConfigManager::instance()->includeFactionPetsForMissionDifficulty();
			Reference<GroupObject*> group = player->getGroup();

			if (group != nullptr) {
				Locker locker(group);
				diffDisplay += group->getGroupLevel(includeFactionPets);
			}
		} else {
			diffDisplay += playerLevel;
		}
	}

	//info(true) << "playerLevel: " << String::valueOf(playerLevel) << " maxDiff: " << String::valueOf(maxDiff) << " minDiff: " << String::valueOf(minDiff) << " difficultyLevel: " << String::valueOf(difficultyLevel) << " difficulty: " << String::valueOf(difficulty) << " levelChoice: " << String::valueOf(levelChoice);

	String dir = targetGhost->getScreenPlayData("mission_direction_choice", "directionChoice");
    float dirChoise = Float::valueOf(dir);
	String building = lairTemplateObject->getMissionBuilding(difficulty);

	if (building.isEmpty()) {
		return;
	}

	SharedObjectTemplate* templateObject = TemplateManager::instance()->getTemplate(building.hashCode());

	if (templateObject == nullptr || !templateObject->isSharedTangibleObjectTemplate()) {
		error("incorrect template object in randomizeDestroyMission " + building);
		return;
	}

	NameManager* nm = processor->getNameManager();

	TerrainManager* terrain = zone->getPlanetManager()->getTerrainManager();

	Vector3 startPos;

	bool foundPosition = false;
	int maximumNumberOfTries = 20;
	while (!foundPosition && maximumNumberOfTries-- > 0) {
		foundPosition = true;

		//int distance = destroyMissionBaseDistance + destroyMissionDifficultyDistanceFactor * difficultyLevel;
		//distance += System::random(destroyMissionRandomDistance) + System::random(destroyMissionDifficultyRandomDistance * difficultyLevel);
		//startPos = player->getWorldCoordinate((float)distance, (float)System::random(360), false);

		float direction = (float)System::random(360);
        
        if (dirChoise > 0){
            int dev = System::random(8);
            int isMinus = System::random(200);
            
            if (isMinus > 49)
                dev *= -1;
            
            direction = dirChoise + dev;
            
            if (direction > 360)
                direction -= 360;
        }
        
        int distance = System::random(500) + 500;
        float angleRads = direction * (M_PI / 180.0f);
        float newAngle = angleRads + (M_PI / 2);
        
        startPos.setX(player->getWorldPositionX() + (cos(newAngle) * distance));
        startPos.setY(player->getWorldPositionY() + (sin(newAngle) * distance));
        startPos.setZ(0.0f);
        
		if (zone->isWithinBoundaries(startPos)) {
			float height = zone->getHeight(startPos.getX(), startPos.getY());
			float waterHeight = height * 2;
			bool result = terrain->getWaterHeight(startPos.getX(), startPos.getY(), waterHeight);

			if (!result || waterHeight <= height) {
				//Check that the position is outside cities.
				SortedVector<ManagedReference<ActiveArea* > > activeAreas;
				zone->getInRangeActiveAreas(startPos.getX(), startPos.getZ(), startPos.getY(), &activeAreas, true);
				for (int i = 0; i < activeAreas.size(); ++i) {
					ActiveArea* area = activeAreas.get(i);

					if (area == nullptr)
						continue;

					if (area->isCityRegion()) {
						foundPosition = false;
					}
				}
			} else {
				foundPosition = false;
			}
		} else {
			foundPosition = false;
		}
	}

	if (!foundPosition) {
		return;
	}

	int randTexts = System::random(34) + 1;

	mission->setMissionNumber(randTexts);

	mission->setStartPosition(startPos.getX(), startPos.getY(), zone->getZoneName());
	mission->setCreatorName(nm->makeCreatureName());

	mission->setMissionTargetName("@lair_n:" + lairTemplateObject->getName());
	mission->setTargetTemplate(templateObject);
	mission->setTargetOptionalTemplate(lairTemplate);

	int reward = destroyMissionBaseReward + destroyMissionDifficultyRewardFactor * difficultyLevel;
	reward += System::random(destroyMissionRandomReward) + System::random(destroyMissionDifficultyRandomReward * difficultyLevel);
	mission->setRewardCredits(reward);

	mission->setMissionDifficulty(difficultyLevel, diffDisplay, difficulty);
	mission->setSize(randomLairSpawn->getSize());
	mission->setFaction(faction);

	int factionPointsReward = randomLairSpawn->getMinDifficulty();
	if (factionPointsReward > 32)
	{
		factionPointsReward = 32;
	}

	String messageDifficulty;
	String missionType;

	if (difficulty < 2)
		messageDifficulty = "_easy";
	else if (difficulty == 2)
		messageDifficulty = "_medium";
	else
		messageDifficulty = "_hard";

	String groupSuffix;
    
        if (lairTemplateObject->getMobType() == LairTemplate::NPC){
            missionType = "_npc";
            groupSuffix =" camp.";
        }
        else{
            missionType = "_creature";
            groupSuffix = " lair.";
        }
    
        const VectorMap<String, int>* mobiles = lairTemplateObject->getMobiles();
        String mobileName ="mysterious";
    
        if (mobiles->size() > 0){
            mobileName = mobiles->elementAt(0).getKey();
        }

	//mission->setMissionTitle("mission/mission_destroy_neutral" + messageDifficulty + missionType, "m" + String::valueOf(randTexts) + "t");
	mission->setMissionTitle("CL" + String::valueOf(diffDisplay), " Destroy the " + mobileName.replaceAll("_", " ") + groupSuffix);
	mission->setMissionDescription("mission/mission_destroy_neutral" +  messageDifficulty + missionType, "m" + String::valueOf(randTexts) + "d");

	switch (faction) {
	case Factions::FACTIONIMPERIAL:
		mission->setRewardFactionPointsImperial(factionPointsReward * 2);
		mission->setRewardFactionPointsRebel(-factionPointsReward);
		generateRandomFactionalDestroyMissionDescription(player, mission, "imperial");
		break;
	case Factions::FACTIONREBEL:
		mission->setRewardFactionPointsImperial(-factionPointsReward);
		mission->setRewardFactionPointsRebel(factionPointsReward * 2);
		generateRandomFactionalDestroyMissionDescription(player, mission, "rebel");
		break;
	default:
		mission->setRewardFactionPointsImperial(0);
		mission->setRewardFactionPointsRebel(0);
		break;
	}

	mission->setTypeCRC(MissionTypes::DESTROY);
}

void MissionManagerImplementation::randomizeGenericSurveyMission(CreatureObject* player, MissionObject* mission, const uint32 faction) {
	int maxLevel = 50;
	int minLevel = 50;
	Zone* playerZone = player->getZone();

	if (playerZone == nullptr)
		return;

	long long surveySkill = player->getSkillMod("surveying");
	if (surveySkill > 30) {
		maxLevel += 10;
	}
	if (surveySkill > 50) {
		maxLevel += 10;
	}
	if (surveySkill > 70) {
		maxLevel += 10;
	}
	if (surveySkill > 90) {
		maxLevel += 10;
	}
	if (surveySkill > 100) {
		//Max mission level is 95.
		maxLevel += 5;
	}

	//Mission level used as needed concentration in increments of 5. I.e. 50, 55, 60 etc. up to 95.
	int randLevel = minLevel + 5 * System::random((maxLevel - minLevel) / 5);

	if (randLevel > maxLevel)
		randLevel = maxLevel;

	ResourceManager* manager = server->getResourceManager();

	String zoneName = playerZone->getZoneName();

	Vector<ManagedReference<ResourceSpawn*> > resources;

	int toolType = SurveyTool::MINERAL;

	//75 % mineral, 25 % chemical.
	if (System::random(3) == 0) {
		toolType = SurveyTool::CHEMICAL;
	}

	manager->getResourceListByType(resources, toolType, zoneName);

	ManagedReference<ResourceSpawn*> spawn = resources.get(System::random(resources.size() - 1));
	uint32 containerCRC = spawn->getContainerCRC();
	SharedObjectTemplate* templateObject = TemplateManager::instance()->getTemplate(containerCRC);

	NameManager* nm = processor->getNameManager();

	int texts = System::random(50);

	if (texts == 0)
		texts = 1;

	mission->setMissionTargetName(spawn->getSurveyMissionSpawnFamilyName());
	mission->setTargetTemplate(templateObject);

	//Reward depending on mission level.
	mission->setRewardCredits(400 + (randLevel - minLevel) * 20 + System::random(100));

	mission->setFaction(faction);

	mission->setMissionDifficulty(randLevel);
	mission->setStartPosition(player->getPositionX(), player->getPositionY(), playerZone->getZoneName());
	mission->setMissionTitle("mission/mission_npc_survey_neutral_easy", "m" + String::valueOf(texts) + "t");
	mission->setMissionDescription("mission/mission_npc_survey_neutral_easy", "m" + String::valueOf(texts) + "o");
	mission->setCreatorName(nm->makeCreatureName());

	mission->setTypeCRC(MissionTypes::SURVEY);
}

void MissionManagerImplementation::randomizeGenericBountyMission(CreatureObject* player, MissionObject* mission, const uint32 faction, Vector<ManagedReference<PlayerBounty*>>* potentialTargets) {
	if (!player->hasSkill("combat_bountyhunter_novice")) {
		player->sendSystemMessage("@mission/mission_generic:not_bounty_hunter_terminal");
		return;
	}

	Zone* playerZone = player->getZone();

	if (playerZone == nullptr) {
		return;
	}

	int level = 1;
	int randomTexts = 25;
	if (player->hasSkill("combat_bountyhunter_investigation_03")) {
		level = 3;
	} else if (player->hasSkill("combat_bountyhunter_investigation_01")) {
		level = 2;
		randomTexts = 50;
	}

	NameManager* nm = processor->getNameManager();

	bool playerTarget = false;
	int size = potentialTargets->size();

	if (level == 3 && size > 0) {
		int compareValue = size > 25 ? 25 : size < 5 ? 5 : size;
		if (System::random(100) < compareValue) {
			playerTarget = true;
			randomTexts = 6;
		}
	}

	mission->setStartPosition(player->getPositionX(), player->getPositionY(), playerZone->getZoneName());

	mission->setTargetTemplate(TemplateManager::instance()->getTemplate(STRING_HASHCODE("object/tangible/mission/mission_bounty_target.iff")));

	mission->setMissionLevel(level);
	mission->setFaction(faction);

	if (playerTarget) {
		int index = System::random(size - 1);

		PlayerBounty* target = potentialTargets->get(index);

		potentialTargets->removeElementAt(index);

		if (target != nullptr) {
			mission->setTargetObjectId(target->getTargetPlayerID());
			mission->setEndPosition(0, 0, "", true);
			mission->setTargetOptionalTemplate("");

			ManagedReference<CreatureObject*> creature = server->getObject(target->getTargetPlayerID()).castTo<CreatureObject*>();
			String name = "";

			if (creature != nullptr && ConfigManager::instance()->getBool("Core3.MissionManager.AnonymousBountyTerminals", false)) {
				if (creature->getFaction() == Factions::FACTIONIMPERIAL)
					name = "Imperial Jedi";
				else if (creature->getFaction() == Factions::FACTIONREBEL)
					name = "Rebel Jedi";
				else
					name = "Neutral Jedi";

				ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();

				int rewardCreds = 0;
				if (ghost->getJediState() >= 4)
					rewardCreds = 50000;
				else
					rewardCreds = 25000;

				mission->setRewardCredits(rewardCreds);
				int totalCreds = getRealBountyReward(creature, target);
				int bonusCreds = totalCreds - rewardCreds;

				if (bonusCreds > 0)
					mission->setBonusCredits(bonusCreds);
			} else {
				if (creature != nullptr) {
					name = creature->getFirstName() + " " + creature->getLastName();
					name = name.trim();
				}

				mission->setRewardCredits(getRealBountyReward(creature, target));
			}

			mission->setMissionTargetName(name);
			mission->setMissionDifficulty(75);

			// Set the Title, Creator, and Description of the mission.

			int randTexts = 0;

			String stfFile = "mission/mission_bounty_jedi";

			UnicodeString numberOfEntries = StringIdManager::instance()->getStringId(String::hashCode("@" + stfFile  + ":" + "number_of_entries"));

			if (!numberOfEntries.isEmpty()) {
				randTexts = System::random(Integer::valueOf(numberOfEntries.toString()) - 1) + 1;
			} else {
				randTexts = (target->getTargetPlayerID() % randomTexts) + 1;
			}

			mission->setMissionNumber(randTexts);

			UnicodeString possibleCreatorName = StringIdManager::instance()->getStringId(String::hashCode("@" + stfFile + "m" + String::valueOf(randTexts) + "o"));
			String creatorName = "";


			if (!possibleCreatorName.isEmpty()) {
				creatorName = possibleCreatorName.toString();
			} else {
				creatorName = nm->makeCreatureName();
			}

			mission->setCreatorName(creatorName);
			mission->setMissionTitle(stfFile, "m" + String::valueOf(randTexts) + "t");
			mission->setMissionDescription(stfFile, "m" + String::valueOf(randTexts) + "d");
		}
	} else {
		mission->setMissionTargetName(nm->makeCreatureName());

		String planet = playerZone->getZoneName();
		if (level == 3 && bhTargetZones.size() > 0) {
			planet = getRandomBountyPlanet();
		}

		Vector3 endPos = getRandomBountyTargetPosition(player, planet);
		mission->setEndPosition(endPos.getX(), endPos.getY(), planet, true);

		String targetTemplate = bhTargetsAtMissionLevel.get((unsigned int)level).get(System::random(bhTargetsAtMissionLevel.get((unsigned int)level).size() - 1));
		mission->setTargetOptionalTemplate(targetTemplate);

		CreatureTemplate* creoTemplate = CreatureTemplateManager::instance()->getTemplate(mission->getTargetOptionalTemplate());

		int reward = 1000;
		int creoLevel = 1;

		if (creoTemplate != nullptr) {
			creoLevel = creoTemplate->getLevel();
		}

		if (level == 1) {
			reward = creoLevel * (200 + System::random(200));
		} else if (level == 2) {
			reward = creoLevel * (250 + System::random(250));
		} else if (level == 3) {
			reward = creoLevel * (300 + System::random(300));
		}

		mission->setRewardCredits(reward);

		String diffString = "easy";

		if (level == 3) {
			diffString = "hard";
		} else if (level == 2) {
			diffString = "medium";
		}

		mission->setTargetObjectId(0);

		// Set the Title, Creator, and Description of the mission.

		int randTexts = 0;

		String stfFile = "mission/mission_bounty_neutral_";

		UnicodeString numberOfEntries = StringIdManager::instance()->getStringId(String::hashCode("@" + stfFile + diffString + ":" + "number_of_entries"));

		if (!numberOfEntries.isEmpty()) {
			randTexts =  System::random(Integer::valueOf(numberOfEntries.toString()) - 1) + 1;
		} else {
			randTexts = System::random(randomTexts - 1) + 1;
		}

		mission->setMissionNumber(randTexts);
		mission->setMissionDifficulty(3 * creoLevel + 7);

		UnicodeString possibleCreatorName = StringIdManager::instance()->getStringId(String::hashCode("@" + stfFile + diffString + ":" + "m" + String::valueOf(randTexts) + "o"));
		String creatorName = "";


		if (!possibleCreatorName.isEmpty()) {
			creatorName = possibleCreatorName.toString();
		} else {
			creatorName = nm->makeCreatureName();
		}

		mission->setCreatorName(creatorName);
		mission->setMissionTitle(stfFile + diffString, "m" + String::valueOf(randTexts) + "t");
		mission->setMissionDescription(stfFile + diffString, "m" + String::valueOf(randTexts) + "d");
	}

	mission->setTypeCRC(MissionTypes::BOUNTY);
}

void MissionManagerImplementation::randomizeGenericDeliverMission(CreatureObject* player, MissionObject* mission, const uint32 faction) {
	//Randomize in city or between city missions.
	bool inTownMission = true;
	if (System::random(1) == 1) {
		inTownMission = false;
	}

	if (!randomGenericDeliverMission(player, mission, inTownMission, faction)) {
		//In town or out of town mission failed try the other sort of mission instead.
		inTownMission = !inTownMission;
		randomGenericDeliverMission(player, mission, inTownMission, faction);
	}
}

String MissionManagerImplementation::getDeliveryMissionFileName(const uint32 faction) {
	switch (faction) {
	case Factions::FACTIONIMPERIAL:
		return "mission/mission_deliver_imperial_easy";
	case Factions::FACTIONREBEL:
		return "mission/mission_deliver_rebel_easy";
	default:
		return "mission/mission_deliver_neutral_easy";
	}
}

int MissionManagerImplementation::getDeliverMissionNumberOfMissions(const uint32 faction) {
	switch (faction) {
	case Factions::FACTIONIMPERIAL:
		return 24;
	case Factions::FACTIONREBEL:
		return 24;
	default:
		return 29;
	}
}

int MissionManagerImplementation::getDeliverMissionSpawnType(const uint32 faction) {
	switch (faction) {
	case Factions::FACTIONIMPERIAL:
		return NpcSpawnPoint::IMPERIALSPAWN;
	case Factions::FACTIONREBEL:
		return NpcSpawnPoint::REBELSPAWN;
	default:
		return NpcSpawnPoint::NEUTRALSPAWN;
	}
}

bool MissionManagerImplementation::randomGenericDeliverMission(CreatureObject* player, MissionObject* mission, bool inTownMission, const uint32 faction) {
	//Get the current planet and position of the player.
	ManagedReference<Zone*> zone = player->getZone();

	if (zone == nullptr)
		return false;

	String planetName = zone->getZoneName();

	Vector3 playerPosition = player->getWorldPosition();
	playerPosition.setZ(0);
	Vector3* startPosition = &playerPosition;

	//Lock spawn point map for the search.
	Locker missionSpawnLocker(&missionNpcSpawnMap);

	//Find a spawn point in current city.
	float minDistance = 10.0f;
	float maxDistance = 300.0f;
	auto startNpc = missionNpcSpawnMap.getRandomNpcSpawnPoint(planetName.hashCode(), startPosition, getDeliverMissionSpawnType(faction), minDistance, maxDistance);

	if (startNpc == nullptr) {
		//Couldn't find a suitable spawn point.
		return false;
	}

	//Find a spawn point for the delivery target.
	const Vector3* endPosition = startPosition;
	if (!inTownMission) {
		//Find city center of another city and use as position to search for spawn points from.
		endPosition = missionNpcSpawnMap.getRandomCityCoordinates(planetName.hashCode(), startPosition);
	}

	//Search in all parts of the city for the end spawn.
	minDistance = 15.0f;
	maxDistance = 1500.0f;

	const NpcSpawnPoint* endNpc = nullptr;
	int retries = 10;
	while ((endNpc == nullptr || endNpc == startNpc) && (retries > 0)) {
		endNpc = missionNpcSpawnMap.getRandomNpcSpawnPoint(planetName.hashCode(), endPosition, getDeliverMissionSpawnType(faction), minDistance, maxDistance);
		retries--;
	}

	if (endNpc == nullptr || endNpc == startNpc) {
		//Couldn't find a suitable spawn point.
		return false;
	}

	//Randomize mission description strings/delivery object.
	int randomTexts = System::random(getDeliverMissionNumberOfMissions(faction)) + 1;

	//Setup mission object.
	mission->setMissionNumber(randomTexts);

	NameManager* nm = processor->getNameManager();
	mission->setCreatorName(nm->makeCreatureName());
	mission->setMissionTargetName(TemplateManager::instance()->getTemplate(STRING_HASHCODE("object/tangible/mission/mission_datadisk.iff"))->getObjectName());

	String planet = zone->getZoneName();
	mission->setStartPosition(startNpc->getPosition()->getX(), startNpc->getPosition()->getY(), planet, true);
	mission->setEndPosition(endNpc->getPosition()->getX(), endNpc->getPosition()->getY(), planet);

	mission->setTargetTemplate(TemplateManager::instance()->getTemplate(STRING_HASHCODE("object/tangible/mission/mission_datadisk.iff")));

	int baseCredits = 40;
	int deliverDistanceCredits = (playerPosition.distanceTo(*(startNpc->getPosition())) + startNpc->getPosition()->distanceTo(*(endNpc->getPosition()))) / 10;

	mission->setRewardCredits(baseCredits + deliverDistanceCredits);

	switch (faction) {
	case Factions::FACTIONIMPERIAL:
		mission->setRewardFactionPointsImperial(5);
		mission->setRewardFactionPointsRebel(0);
		break;
	case Factions::FACTIONREBEL:
		mission->setRewardFactionPointsImperial(0);
		mission->setRewardFactionPointsRebel(5);
		break;
	default:
		mission->setRewardFactionPointsImperial(0);
		mission->setRewardFactionPointsRebel(0);
		break;
	}

	mission->setMissionDifficulty(5);
	mission->setMissionTitle(getDeliveryMissionFileName(faction), "m" + String::valueOf(randomTexts) + "t");
	mission->setMissionDescription(getDeliveryMissionFileName(faction), "m" + String::valueOf(randomTexts) + "d");

	mission->setTypeCRC(MissionTypes::DELIVER);
	mission->setFaction(faction);

	return true;
}

NpcSpawnPoint* MissionManagerImplementation::getFreeNpcSpawnPoint(unsigned const int planetCRC, const float x, const float y, const int spawnType, const float maxRange) {
	Locker missionSpawnLocker(&missionNpcSpawnMap);

	Vector3 pos(x, y, 0);

	//First try for an exact match
	auto npc = missionNpcSpawnMap.findSpawnAt(planetCRC, &pos);

	if (npc != nullptr && npc->getInUse() == 0 && (npc->getSpawnType() & spawnType) == spawnType) {
		return npc;
	}

	//Next try to find a free NPC spawn point in a circle with a radius of max.
	float min = 0.0f;
	float max = 50.0f;

	while (max <= maxRange) {
		npc = missionNpcSpawnMap.getRandomNpcSpawnPoint(planetCRC, &pos, spawnType, min, max);
		if (npc != nullptr && npc->getInUse() == 0... (35 KB left)
