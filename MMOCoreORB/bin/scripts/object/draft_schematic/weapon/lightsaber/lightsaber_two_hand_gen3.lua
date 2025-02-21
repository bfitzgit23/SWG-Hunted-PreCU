--Copyright (C) 2025 BlazinYou Killa


object_draft_schematic_weapon_lightsaber_lightsaber_two_hand_gen3 = object_draft_schematic_weapon_lightsaber_shared_lightsaber_two_hand_gen3:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "Two-Handed Third Generation Lightsaber",

   craftingToolTab = 2048, -- (See DraftSchematicObjectTemplate.h)
   complexity = 18, 
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
   resourceTypes = {"metal_ferrous", "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_gen2.iff", "aluminum_titanium", "petrochem_inert_polymer", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_krayt_dragon_pearl.iff", "gas_inert_culsion", "copper_polysteel"},
   resourceQuantities = {35, 1, 20, 25, 1, 25, 25},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/2h_sword/crafted_saber/sword_lightsaber_two_handed_gen3.iff",

   additionalTemplates = {
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s1_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s2_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s3_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s4_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s5_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s6_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s7_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s8_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s9_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s10_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s11_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s12_gen3.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s13_gen3.iff",
             }

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_lightsaber_lightsaber_two_hand_gen3, "object/draft_schematic/weapon/lightsaber/lightsaber_two_hand_gen3.iff")
