--Copyright (C) 2025 BlazinYou Killa


object_draft_schematic_weapon_lightsaber_lightsaber_training = object_draft_schematic_weapon_lightsaber_shared_lightsaber_training:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "A Training Lightsaber",

   craftingToolTab = 2048, -- (See DraftSchematicObjectTemplate.h)
   complexity = 16, 
   size = 1, 
   factoryCrateType = "object/factory/factory_crate_weapon.iff",
   
   xpType = "jedi_general", 
   xp = 0, 

   assemblySkill = "jedi_saber_assembly", 
   experimentingSkill = "jedi_saber_experimentation", 
   customizationSkill = "jedi_customization", 
   factoryCrateSize = 0,   

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   ingredientTitleNames = {"emitter_shroud", "primary_crystal", "activator", "handgrip", "focusing_crystals", "power_field_insulator", "energizers"},
   ingredientSlotType = {0, 1, 0, 0, 1, 0, 0},
   resourceTypes = {"mineral", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_force_crystal.iff", "metal", "chemical", "object/tangible/component/weapon/lightsaber/shared_lightsaber_refined_crystal_pack.iff", "gas", "metal"},
   resourceQuantities = {10, 1, 10, 12, 1, 10, 10},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/sword/crafted_saber/sword_lightsaber_training.iff",

   additionalTemplates = {
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s1_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s2_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s3_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s4_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s5_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s6_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s7_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s8_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s9_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s10_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s11_training.iff",
             }

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_lightsaber_lightsaber_training, "object/draft_schematic/weapon/lightsaber/lightsaber_training.iff")
