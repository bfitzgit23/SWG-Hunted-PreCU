--Copyright (C) 2010 <SWGEmu>


--This File is part of Core3.

--This program is free software; you can redistribute
--it and/or modify it under the terms of the GNU Lesser
--General Public License as published by the Free Software
--Foundation; either version 2 of the License,
--or (at your option) any later version.

--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
--See the GNU Lesser General Public License for
--more details.

--You should have received a copy of the GNU Lesser General
--Public License along with this program; if not, write to
--the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

--Linking Engine3 statically or dynamically with other modules
--is making a combined work based on Engine3.
--Thus, the terms and conditions of the GNU Lesser General Public License
--cover the whole combination.

--In addition, as a special exception, the copyright holders of Engine3
--give you permission to combine Engine3 program with free software
--programs or libraries that are released under the GNU LGPL and with
--code included in the standard release of Core3 under the GNU LGPL
--license (or modified versions of such code, with unchanged license).
--You may copy and distribute such a system following the terms of the
--GNU LGPL for Engine3 and the licenses of the other code concerned,
--provided that you include the source code of that other code when
--and as the GNU LGPL requires distribution of source code.

--Note that people who make modified versions of Engine3 are not obligated
--to grant this special exception for their modified versions;
--it is their choice whether to do so. The GNU Lesser General Public License
--gives permission to release a modified version without this exception;
--this exception also makes it possible to release a modified version



-- Children folder includes
includeFile("ship/base/serverobjects.lua")
includeFile("ship/player/serverobjects.lua")

-- Server Objects
includeFile("ship/assaultship.lua")
includeFile("ship/awing.lua")
includeFile("ship/awing_tier1.lua")
includeFile("ship/awing_tier2.lua")
includeFile("ship/awing_tier3.lua")
includeFile("ship/awing_tier4.lua")
includeFile("ship/awing_tier5.lua")
includeFile("ship/basic_hutt_light.lua")
includeFile("ship/basic_tiefighter.lua")
includeFile("ship/basic_z95.lua")
includeFile("ship/blacksun_heavy_s01.lua")
includeFile("ship/blacksun_heavy_s01_tier1.lua")
includeFile("ship/blacksun_heavy_s01_tier2.lua")
includeFile("ship/blacksun_heavy_s01_tier3.lua")
includeFile("ship/blacksun_heavy_s01_tier4.lua")
includeFile("ship/blacksun_heavy_s01_tier5.lua")
includeFile("ship/blacksun_heavy_s02.lua")
includeFile("ship/blacksun_heavy_s02_tier1.lua")
includeFile("ship/blacksun_heavy_s02_tier2.lua")
includeFile("ship/blacksun_heavy_s02_tier3.lua")
includeFile("ship/blacksun_heavy_s02_tier4.lua")
includeFile("ship/blacksun_heavy_s02_tier5.lua")
includeFile("ship/blacksun_heavy_s03.lua")
includeFile("ship/blacksun_heavy_s03_tier1.lua")
includeFile("ship/blacksun_heavy_s03_tier2.lua")
includeFile("ship/blacksun_heavy_s03_tier3.lua")
includeFile("ship/blacksun_heavy_s03_tier4.lua")
includeFile("ship/blacksun_heavy_s03_tier5.lua")
includeFile("ship/blacksun_heavy_s04.lua")
includeFile("ship/blacksun_heavy_s04_tier1.lua")
includeFile("ship/blacksun_heavy_s04_tier2.lua")
includeFile("ship/blacksun_heavy_s04_tier3.lua")
includeFile("ship/blacksun_heavy_s04_tier4.lua")
includeFile("ship/blacksun_heavy_s04_tier5.lua")
includeFile("ship/blacksun_light_s01.lua")
includeFile("ship/blacksun_light_s01_tier1.lua")
includeFile("ship/blacksun_light_s01_tier2.lua")
includeFile("ship/blacksun_light_s01_tier3.lua")
includeFile("ship/blacksun_light_s01_tier4.lua")
includeFile("ship/blacksun_light_s01_tier5.lua")
includeFile("ship/blacksun_light_s02.lua")
includeFile("ship/blacksun_light_s02_tier1.lua")
includeFile("ship/blacksun_light_s02_tier2.lua")
includeFile("ship/blacksun_light_s02_tier3.lua")
includeFile("ship/blacksun_light_s02_tier4.lua")
includeFile("ship/blacksun_light_s02_tier5.lua")
includeFile("ship/blacksun_light_s03.lua")
includeFile("ship/blacksun_light_s03_tier1.lua")
includeFile("ship/blacksun_light_s03_tier2.lua")
includeFile("ship/blacksun_light_s03_tier3.lua")
includeFile("ship/blacksun_light_s03_tier4.lua")
includeFile("ship/blacksun_light_s03_tier5.lua")
includeFile("ship/blacksun_light_s04.lua")
includeFile("ship/blacksun_light_s04_tier1.lua")
includeFile("ship/blacksun_light_s04_tier2.lua")
includeFile("ship/blacksun_light_s04_tier3.lua")
includeFile("ship/blacksun_light_s04_tier4.lua")
includeFile("ship/blacksun_light_s04_tier5.lua")
includeFile("ship/blacksun_medium_s01.lua")
includeFile("ship/blacksun_medium_s01_tier1.lua")
includeFile("ship/blacksun_medium_s01_tier2.lua")
includeFile("ship/blacksun_medium_s01_tier3.lua")
includeFile("ship/blacksun_medium_s01_tier4.lua")
includeFile("ship/blacksun_medium_s01_tier5.lua")
includeFile("ship/blacksun_medium_s02.lua")
includeFile("ship/blacksun_medium_s02_tier1.lua")
includeFile("ship/blacksun_medium_s02_tier2.lua")
includeFile("ship/blacksun_medium_s02_tier3.lua")
includeFile("ship/blacksun_medium_s02_tier4.lua")
includeFile("ship/blacksun_medium_s02_tier5.lua")
includeFile("ship/blacksun_medium_s03.lua")
includeFile("ship/blacksun_medium_s03_tier1.lua")
includeFile("ship/blacksun_medium_s03_tier2.lua")
includeFile("ship/blacksun_medium_s03_tier3.lua")
includeFile("ship/blacksun_medium_s03_tier4.lua")
includeFile("ship/blacksun_medium_s03_tier5.lua")
includeFile("ship/blacksun_medium_s04.lua")
includeFile("ship/blacksun_medium_s04_tier1.lua")
includeFile("ship/blacksun_medium_s04_tier2.lua")
includeFile("ship/blacksun_medium_s04_tier3.lua")
includeFile("ship/blacksun_medium_s04_tier4.lua")
includeFile("ship/blacksun_medium_s04_tier5.lua")
includeFile("ship/blacksun_transport.lua")
includeFile("ship/blacksun_transport_tier1.lua")
includeFile("ship/blacksun_transport_tier2.lua")
includeFile("ship/blacksun_transport_tier3.lua")
includeFile("ship/blacksun_transport_tier4.lua")
includeFile("ship/blacksun_transport_tier5.lua")
includeFile("ship/bwing.lua")
includeFile("ship/bwing_tier1.lua")
includeFile("ship/bwing_tier2.lua")
includeFile("ship/bwing_tier3.lua")
includeFile("ship/bwing_tier4.lua")
includeFile("ship/bwing_tier5.lua")
includeFile("ship/civshuttle.lua")
includeFile("ship/civspeeder1.lua")
includeFile("ship/civspeeder2.lua")
includeFile("ship/civtaxi.lua")
includeFile("ship/civtransport.lua")
includeFile("ship/corvette.lua")
includeFile("ship/corvette_interior.lua")
includeFile("ship/decimator.lua")
includeFile("ship/decimator_tier1.lua")
includeFile("ship/decimator_tier2.lua")
includeFile("ship/decimator_tier3.lua")
includeFile("ship/decimator_tier4.lua")
includeFile("ship/decimator_tier5.lua")
includeFile("ship/droid_fighter.lua")
includeFile("ship/droid_fighter_tier1.lua")
includeFile("ship/droid_fighter_tier2.lua")
includeFile("ship/droid_fighter_tier3.lua")
includeFile("ship/droid_fighter_tier4.lua")
includeFile("ship/droid_fighter_tier5.lua")
includeFile("ship/escape_pod.lua")
includeFile("ship/escape_pod_tier1.lua")
includeFile("ship/escape_pod_tier2.lua")
includeFile("ship/escape_pod_tier3.lua")
includeFile("ship/escape_pod_tier4.lua")
includeFile("ship/escape_pod_tier5.lua")
includeFile("ship/firespray.lua")
includeFile("ship/firespray_tier1.lua")
includeFile("ship/firespray_tier2.lua")
includeFile("ship/firespray_tier3.lua")
includeFile("ship/firespray_tier4.lua")
includeFile("ship/firespray_tier5.lua")
includeFile("ship/freighterheavy.lua")
includeFile("ship/freighterheavy_tier1.lua")
includeFile("ship/freighterheavy_tier2.lua")
includeFile("ship/freighterheavy_tier3.lua")
includeFile("ship/freighterheavy_tier4.lua")
includeFile("ship/freighterheavy_tier5.lua")
includeFile("ship/freighterlight.lua")
includeFile("ship/freighterlight_tier1.lua")
includeFile("ship/freighterlight_tier2.lua")
includeFile("ship/freighterlight_tier3.lua")
includeFile("ship/freighterlight_tier4.lua")
includeFile("ship/freighterlight_tier5.lua")
includeFile("ship/freightermedium.lua")
includeFile("ship/freightermedium_tier1.lua")
includeFile("ship/freightermedium_tier2.lua")
includeFile("ship/freightermedium_tier3.lua")
includeFile("ship/freightermedium_tier4.lua")
includeFile("ship/freightermedium_tier5.lua")
includeFile("ship/hutt_heavy_s01.lua")
includeFile("ship/hutt_heavy_s01_tier1.lua")
includeFile("ship/hutt_heavy_s01_tier2.lua")
includeFile("ship/hutt_heavy_s01_tier3.lua")
includeFile("ship/hutt_heavy_s01_tier4.lua")
includeFile("ship/hutt_heavy_s01_tier5.lua")
includeFile("ship/hutt_heavy_s02.lua")
includeFile("ship/hutt_heavy_s02_tier1.lua")
includeFile("ship/hutt_heavy_s02_tier2.lua")
includeFile("ship/hutt_heavy_s02_tier3.lua")
includeFile("ship/hutt_heavy_s02_tier4.lua")
includeFile("ship/hutt_heavy_s02_tier5.lua")
includeFile("ship/hutt_light_s01.lua")
includeFile("ship/hutt_light_s01_tier1.lua")
includeFile("ship/hutt_light_s01_tier2.lua")
includeFile("ship/hutt_light_s01_tier3.lua")
includeFile("ship/hutt_light_s01_tier4.lua")
includeFile("ship/hutt_light_s01_tier5.lua")
includeFile("ship/hutt_light_s02.lua")
includeFile("ship/hutt_light_s02_tier1.lua")
includeFile("ship/hutt_light_s02_tier2.lua")
includeFile("ship/hutt_light_s02_tier3.lua")
includeFile("ship/hutt_light_s02_tier4.lua")
includeFile("ship/hutt_light_s02_tier5.lua")
includeFile("ship/hutt_medium_s01.lua")
includeFile("ship/hutt_medium_s01_tier1.lua")
includeFile("ship/hutt_medium_s01_tier2.lua")
includeFile("ship/hutt_medium_s01_tier3.lua")
includeFile("ship/hutt_medium_s01_tier4.lua")
includeFile("ship/hutt_medium_s01_tier5.lua")
includeFile("ship/hutt_medium_s02.lua")
includeFile("ship/hutt_medium_s02_tier1.lua")
includeFile("ship/hutt_medium_s02_tier2.lua")
includeFile("ship/hutt_medium_s02_tier3.lua")
includeFile("ship/hutt_medium_s02_tier4.lua")
includeFile("ship/hutt_medium_s02_tier5.lua")
includeFile("ship/hutt_transport.lua")
includeFile("ship/hutt_transport_tier1.lua")
includeFile("ship/hutt_transport_tier2.lua")
includeFile("ship/hutt_transport_tier3.lua")
includeFile("ship/hutt_transport_tier4.lua")
includeFile("ship/hutt_transport_tier5.lua")
includeFile("ship/hutt_turret_ship.lua")
includeFile("ship/imperial_gunboat.lua")
includeFile("ship/imperial_gunboat_tier1.lua")
includeFile("ship/imperial_gunboat_tier2.lua")
includeFile("ship/imperial_gunboat_tier3.lua")
includeFile("ship/imperial_gunboat_tier4.lua")
includeFile("ship/imperial_gunboat_tier5.lua")
includeFile("ship/lambdashuttle.lua")
includeFile("ship/lambdashuttle_tier1.lua")
includeFile("ship/lambdashuttle_tier2.lua")
includeFile("ship/lambdashuttle_tier3.lua")
includeFile("ship/lambdashuttle_tier4.lua")
includeFile("ship/lambdashuttle_tier5.lua")
includeFile("ship/merchant_cruiser_light_tier1.lua")
includeFile("ship/merchant_cruiser_light_tier2.lua")
includeFile("ship/merchant_cruiser_light_tier3.lua")
includeFile("ship/merchant_cruiser_light_tier4.lua")
includeFile("ship/merchant_cruiser_light_tier5.lua")
includeFile("ship/merchant_cruiser_medium.lua")
includeFile("ship/merchant_cruiser_medium_tier1.lua")
includeFile("ship/merchant_cruiser_medium_tier2.lua")
includeFile("ship/merchant_cruiser_medium_tier3.lua")
includeFile("ship/merchant_cruiser_medium_tier4.lua")
includeFile("ship/merchant_cruiser_medium_tier5.lua")
includeFile("ship/nebulon.lua")
includeFile("ship/probe_droid_pod.lua")
includeFile("ship/probe_droid_pod_tier1.lua")
includeFile("ship/probe_droid_pod_tier2.lua")
includeFile("ship/probe_droid_pod_tier3.lua")
includeFile("ship/probe_droid_pod_tier4.lua")
includeFile("ship/probe_droid_pod_tier5.lua")
includeFile("ship/prototype_hutt_light.lua")
includeFile("ship/prototype_tiefighter.lua")
includeFile("ship/prototype_z95.lua")
includeFile("ship/rebel_gunboat.lua")
includeFile("ship/rebel_gunboat_tier1.lua")
includeFile("ship/rebel_gunboat_tier2.lua")
includeFile("ship/rebel_gunboat_tier3.lua")
includeFile("ship/rebel_gunboat_tier4.lua")
includeFile("ship/rebel_gunboat_tier5.lua")
includeFile("ship/rebel_shuttle_tier1.lua")
includeFile("ship/rebel_shuttle_tier2.lua")
includeFile("ship/rebel_shuttle_tier3.lua")
includeFile("ship/rebel_shuttle_tier4.lua")
includeFile("ship/rebel_shuttle_tier5.lua")
includeFile("ship/smuggler_warlord_ship_tier1.lua")
includeFile("ship/smuggler_warlord_ship_tier2.lua")
includeFile("ship/smuggler_warlord_ship_tier3.lua")
includeFile("ship/smuggler_warlord_ship_tier4.lua")
includeFile("ship/smuggler_warlord_ship_tier5.lua")
includeFile("ship/sorosuub_space_yacht.lua")
includeFile("ship/spacestation_corellia.lua")
includeFile("ship/spacestation_dantooine.lua")
includeFile("ship/spacestation_dathomir.lua")
includeFile("ship/spacestation_endor.lua")
includeFile("ship/spacestation_freedom.lua")
includeFile("ship/spacestation_imperial.lua")
includeFile("ship/spacestation_lok.lua")
includeFile("ship/spacestation_naboo.lua")
includeFile("ship/spacestation_neutral.lua")
includeFile("ship/spacestation_rebel.lua")
includeFile("ship/spacestation_rori.lua")
includeFile("ship/spacestation_talus.lua")
includeFile("ship/spacestation_tatooine.lua")
includeFile("ship/spacestation_yavin4.lua")
includeFile("ship/star_destroyer.lua")
includeFile("ship/supplyship.lua")
includeFile("ship/tie_in.lua")
includeFile("ship/tie_light_duty.lua")
includeFile("ship/tieadvanced.lua")
includeFile("ship/tieadvanced_tier1.lua")
includeFile("ship/tieadvanced_tier2.lua")
includeFile("ship/tieadvanced_tier3.lua")
includeFile("ship/tieadvanced_tier4.lua")
includeFile("ship/tieadvanced_tier5.lua")
includeFile("ship/tieaggressor.lua")
includeFile("ship/tieaggressor_tier1.lua")
includeFile("ship/tieaggressor_tier2.lua")
includeFile("ship/tieaggressor_tier3.lua")
includeFile("ship/tieaggressor_tier4.lua")
includeFile("ship/tieaggressor_tier5.lua")
includeFile("ship/tiebomber.lua")
includeFile("ship/tiebomber_tier1.lua")
includeFile("ship/tiebomber_tier2.lua")
includeFile("ship/tiebomber_tier3.lua")
includeFile("ship/tiebomber_tier4.lua")
includeFile("ship/tiebomber_tier5.lua")
includeFile("ship/tiefighter.lua")
includeFile("ship/tiefighter_tier1.lua")
includeFile("ship/tiefighter_tier2.lua")
includeFile("ship/tiefighter_tier3.lua")
includeFile("ship/tiefighter_tier4.lua")
includeFile("ship/tiefighter_tier5.lua")
includeFile("ship/tieinterceptor.lua")
includeFile("ship/tieinterceptor_tier1.lua")
includeFile("ship/tieinterceptor_tier2.lua")
includeFile("ship/tieinterceptor_tier3.lua")
includeFile("ship/tieinterceptor_tier4.lua")
includeFile("ship/tieinterceptor_tier5.lua")
includeFile("ship/tieoppressor.lua")
includeFile("ship/tieoppressor_tier1.lua")
includeFile("ship/tieoppressor_tier2.lua")
includeFile("ship/tieoppressor_tier3.lua")
includeFile("ship/tieoppressor_tier4.lua")
includeFile("ship/tieoppressor_tier5.lua")
includeFile("ship/transport_science.lua")
includeFile("ship/transport_science_tier1.lua")
includeFile("ship/transport_science_tier2.lua")
includeFile("ship/transport_science_tier3.lua")
includeFile("ship/transport_science_tier4.lua")
includeFile("ship/transport_science_tier5.lua")
includeFile("ship/xwing.lua")
includeFile("ship/xwing_tier1.lua")
includeFile("ship/xwing_tier2.lua")
includeFile("ship/xwing_tier3.lua")
includeFile("ship/xwing_tier4.lua")
includeFile("ship/xwing_tier5.lua")
includeFile("ship/ykl37r.lua")
includeFile("ship/ykl37r_tier1.lua")
includeFile("ship/ykl37r_tier2.lua")
includeFile("ship/ykl37r_tier3.lua")
includeFile("ship/ykl37r_tier4.lua")
includeFile("ship/ykl37r_tier5.lua")
includeFile("ship/yt1300.lua")
includeFile("ship/yt1300_interior.lua")
includeFile("ship/yt1300_tier1.lua")
includeFile("ship/yt1300_tier2.lua")
includeFile("ship/yt1300_tier3.lua")
includeFile("ship/yt1300_tier4.lua")
includeFile("ship/yt1300_tier5.lua")
includeFile("ship/ywing.lua")
includeFile("ship/ywing_longprobe.lua")
includeFile("ship/ywing_tier1.lua")
includeFile("ship/ywing_tier2.lua")
includeFile("ship/ywing_tier3.lua")
includeFile("ship/ywing_tier4.lua")
includeFile("ship/ywing_tier5.lua")
includeFile("ship/z95.lua")
includeFile("ship/z95_tier1.lua")
includeFile("ship/z95_tier2.lua")
includeFile("ship/z95_tier3.lua")
includeFile("ship/z95_tier4.lua")
includeFile("ship/z95_tier5.lua")