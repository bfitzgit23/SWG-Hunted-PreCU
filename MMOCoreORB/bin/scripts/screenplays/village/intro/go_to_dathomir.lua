local ObjectManager = require("managers.object.object_manager")
local QuestManager = require("managers.quest.quest_manager")
require("utils.helpers")

GoToDathomir = GoToLocation:new {
	-- Task properties
	taskName = "GoToDathomir",
	-- GoToLocation properties
	waypointDescription = "@quest/force_sensitive/intro:goto_dath_sum",
	spawnPoint = { x = 5306, y = -4145 },
	spawnPlanet = "dathomir",
	spawnRadius = 128,
	onFailedSpawn = nil,
	onSuccessfulSpawn = nil,
	onEnteredActiveArea = nil
}

-- Event handler for the enter active area event.
-- The event will complete the task.
-- @param pPlayer pointer to the creature object of the player.
function GoToDathomir:onEnteredActiveArea(pPlayer)
	if (pPlayer == nil) then
		return
	end

	QuestManager.completeQuest(pPlayer, QuestManager.quests.FS_VILLAGE_ELDER)
	self:finish(pPlayer)

	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_heightened_senses_luck")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_heightened_senses_surveying")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_heightened_senses_persuasion")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_heightened_senses_healing")

	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_crafting_mastery_technique")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_crafting_mastery_experimentation")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_crafting_mastery_repair")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_crafting_mastery_assembly")

	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_combat_prowess_melee_speed")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_combat_prowess_melee_accuracy")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_combat_prowess_ranged_accuracy")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_combat_prowess_ranged_speed")

	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_enhanced_reflexes_ranged_defense")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_enhanced_reflexes_melee_defense")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_enhanced_reflexes_vehicle_control")
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_enhanced_reflexes_survival")
	
end

-- Event handler for the onSuccessfulSpawn.
-- The event will activate the quest.
-- @param pPlayer pointer to the creature object of the player.
function GoToDathomir:onSuccessfulSpawn(pPlayer)
	if (pPlayer == nil) then
		return
	end

	if (not QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_VILLAGE_ELDER)) then
		local pGhost = CreatureObject(pPlayer):getPlayerObject()

		if (pGhost == nil) then
			return
		end

		QuestManager.activateQuest(pPlayer, QuestManager.quests.FS_VILLAGE_ELDER)
		VillageJediManagerCommon.setJediProgressionScreenPlayState(pPlayer, VILLAGE_JEDI_PROGRESSION_HAS_VILLAGE_ACCESS)
		CreatureObject(pPlayer):sendSystemMessage("@quest/force_sensitive/intro:force_sensitive")

		if (not PlayerObject(pGhost):isJedi()) then
			PlayerObject(pGhost):setJediState(1)
		end

		awardSkill(pPlayer, "force_title_jedi_novice")
	end
end

return GoToDathomir
