--Copyright (C) 2025 BlazinYou Killa


object_draft_schematic_weapon_lightsaber_lightsaber_polearm_gen2 = object_draft_schematic_weapon_lightsaber_shared_lightsaber_polearm_gen2:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "Double-Bladed Second Generation Lightsaber",

   craftingToolTab = 2048, -- (See DraftSchematicObjectTemplate.h)
   complexity = 17, 
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
   resourceTypes = {"metal", "object/weapon/melee/polearm/crafted_saber/shared_sword_lightsaber_polearm_gen1.iff", "metal_nonferrous", "petrochem_inert", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_krayt_dragon_pearl.iff", "gas_inert_known", "metal_nonferrous"},
   resourceQuantities = {30, 1, 42, 20, 1, 42, 42},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/polearm/crafted_saber/sword_lightsaber_polearm_gen2.iff",

   additionalTemplates = {
              "object/weapon/melee/polearm/crafted_saber/shared_sword_lightsaber_polearm_s1_gen2.iff",
              "object/weapon/melee/polearm/crafted_saber/shared_sword_lightsaber_polearm_s2_gen2.iff",
             }

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_lightsaber_lightsaber_polearm_gen2, "object/draft_schematic/weapon/lightsaber/lightsaber_polearm_gen2.iff")
