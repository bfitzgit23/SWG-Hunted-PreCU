--Copyright (C) 2025 BlazinYou Killa


object_weapon_melee_sword_crafted_saber_sword_lightsaber_one_handed_s3_gen4 = object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen4:new {
	playerRaces = {
		"object/creature/player/bothan_male.iff",
		"object/creature/player/bothan_female.iff",
		"object/creature/player/human_male.iff",
		"object/creature/player/human_female.iff",
		"object/creature/player/ithorian_male.iff",
		"object/creature/player/ithorian_female.iff",
		"object/creature/player/moncal_male.iff",
		"object/creature/player/moncal_female.iff",
		"object/creature/player/rodian_male.iff",
		"object/creature/player/rodian_female.iff",
		"object/creature/player/sullustan_male.iff",
		"object/creature/player/sullustan_female.iff",
		"object/creature/player/trandoshan_male.iff",
		"object/creature/player/trandoshan_female.iff",
		"object/creature/player/twilek_male.iff",
		"object/creature/player/twilek_female.iff",
		"object/creature/player/wookiee_male.iff",
		"object/creature/player/wookiee_female.iff",
		"object/creature/player/zabrak_male.iff",
		"object/creature/player/zabrak_female.iff"
	},

	-- MELEEATTACK, FORCEATTACK
	attackType = MELEEATTACK,

	-- ENERGY, KINETIC, ELECTRICITY, STUN, BLAST, HEAT, COLD, ACID, LIGHTSABER
	damageType = KINETIC,

	-- NONE, LIGHT, MEDIUM, HEAVY
	armorPiercing = MEDIUM,

	-- combat_meleespecialize_onehandlightsaber, combat_meleespecialize_twohandlightsaber, combat_meleespecialize_polearmlightsaber, jedi_general
	xpType = "jedi_general",

	certificationsRequired = { "cert_onehandlightsaber_training" },
	speedModifiers = { "onehandlightsaber_speed" },
	creatureAccuracyModifiers = { "onehandlightsaber_accuracy" },

	-- Leave blank for now
	damageModifiers = { },

	defenderToughnessModifiers = { "lightsaber_toughness" },
	defenderDefenseModifiers = { "melee_defense" },
	defenderSecondaryDefenseModifiers = { "saber_block" },


	-- The values below are the default values.  To be used for blue frog objects primarily
	healthAttackCost = 5,
	actionAttackCost = 5,
	mindAttackCost = 5,
	
	pointBlankRange = 0,
	pointBlankAccuracy = 20,

	idealRange = 3,
	idealAccuracy = 15,

	maxRange = 5,
	maxRangeAccuracy = 5,

	minDamage = 750,
	maxDamage = 1000,

	attackSpeed = 1.5,

	woundsRatio = 12,

	noTrade = 1,

	numberExperimentalProperties = {1, 1, 2, 2, 2, 2, 2, 1, 1, 1},
	experimentalProperties = {"XX", "XX", "CD", "OQ", "CD", "OQ", "CD", "OQ", "SR", "UT", "CD", "OQ", "OQ", "OQ", "OQ"},
	experimentalWeights = {1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1},
	experimentalGroupTitles = {"null", "null", "expDamage", "expDamage", "expDamage", "expDamage", "expEffeciency", "expEffeciency", "expEffeciency", "expEffeciency"},
	experimentalSubGroupTitles = {"null", "null", "mindamage", "maxdamage", "attackspeed", "woundchance", "forcecost", "attackhealthcost", "attackactioncost", "attackmindcost"},
	experimentalMin = {0, 0, 675, 900, 3.5, 1, 0, 5, 5, 5},
	experimentalMax = {0, 0, 825, 1100, 1.5, 3, 0, 5, 5, 5},
	experimentalPrecision = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
	experimentalCombineType = {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
}

ObjectTemplates:addTemplate(object_weapon_melee_sword_crafted_saber_sword_lightsaber_one_handed_s3_gen4, "object/weapon/melee/sword/crafted_saber/sword_lightsaber_one_handed_s3_gen4.iff")
