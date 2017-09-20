/*
* A Command & Conquer: Renegade SSGM Plugin, containing all the single player mission scripts
* Copyright(C) 2017  Neijwiert
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "General.h"
#include "M00_Soldier_Powerup_Grant.h"

/*
M01 -> 122041 116388 116387 116384 103272 103270 103271 103375 103374 103373 103335 103287 103286 103279 103278 103277 103276 103274 103273 103101 102185 102051 101948 101939 101686 101685 101439 101399 101357 101313 101312 101311 101305 101301 101299 101297 101296 101294 101293 101175 101173 101172 101171 101170 101168 101167 101166 101165 101164 101163 101162 100893 100894 100896 100898 100899 101121 101122 101123 101125 101126 101321 101336 101337 101512 101654 101946 101949 101999 102000 102360 102357 102358 103318 103340 103341 103342 101298 102001 102002 102003 101998 106266 101936 106907 106908 106909 106910 106911 114146 106265
M02 -> 401195 401194 401193 401192 401191 401190 401189 401122 401121 401120 401119 401118 401117 401116 401115 401097 401096 401095 401094 401089 401088 401087 401086 401085 401069 401068 401067 401053 401052 401051 401050 401049 401047 401046 400510 400505 400503 400453 400451 400415 400413 400411 400410 400345 400344 400319 400318 400317 400293 400292 400287 400286 400285 400213 400212 400211 400210 400209 400208 400207 400199 400200 400206 400275 400276 400277 400372 400373 400374 400553 400554 400555 400556 400557 400558 400559 400560 400977 400978 400979 400980 400981 400982 400983 400984 401002 401003 401045 401056 401057 401058 401060 401063 401064 401065 401071 401072 401073 401074 401075 401076 401077 401078 401103 401104 401105 401106 401107 401108 401109 401112 401124 401125 401126 401127 401129 401132 401133 401134 401135 401136 401137 401138 401139 401141 401142 401143 401144 401145 406724 406725
M03 -> 300052 1204952 1204951 1201633 1201632 1147534 1147533 1147532 1147531 1147530 1147529 1147527 1146701 1146700 2021360 2021359 300009 300028 2013085 1144724 1144723 1144722 1144721 1144720 1144719 1144718 1144717 1144716 1144715 1144714 1144713 1144712 1144711 1144653 300032 1141162 300043 300034 300033 1144452 1144451 1144450 1144672 1144673 1144682 1144448 1144731 1144732 1144500 300010 1145889
M04 -> 102634 102550 102545 103320 103318 102546 102548 102551 102549 102638 102639 102635 102633 102636 101277 102651 102650 102542 102541 102543 102544 102547 100399 102540 102539 103319 103321 102564 102563 102562 102561 102148 102147 101693 101692 101691 101690 101534 101465 101464 101463 101197 101198 101199 101200 101201 101762 101988 101989 105173
M05 -> 101358 100648 100170 100139 101359 100158 100109 100100 100099 101242 100606 100605 100103 101363 101362 101361 101360 101357 101356 100647 100264 100157 100052 100051 100057 100058 100064 100065 100108 100110 100111 100169 100172 101243 100037
M06 -> 110907 101400 101399 101398 101397 101391 101385 101384 101358 101009 101502 101375 101374 101363 101516 101396 101383 101364 101360 101357 101356 101105 101097 101098 101099 101037 101008 101153 101154 101365 101366 101369 101370 101393 101403
M07 -> 125863 125862 125861 125860 123259 123258 123257 101101 101100 114490 114489 113833 113832 101086 100672 100671 100732 100733 100901 100902 100907 101052 101053 101054 101055 101092 114491 114493 114494 121918 121922 121923 121924 121925
M08 -> 100649 100648 106819 106818 106816 100676 100664 100662 100651 100340 100259 100249 100246 100254 100253 100772 100771 100770 100537 100536 100534 100171 100163 100162 100191 100192 100195 100224 100225 100226 100227 100229 100230 100231 100233 100236 100237 100242 100245 100247 100248 100255 100258 100339 109597 100612 100613 100672 100673 100674 100703 103184 100178 100650 100652 100663 100675 100687 100688 100689 100690 100696
M09 -> 1201321 2005480 2000147 2000146 2000145 2000143 2001274 2001273 2001272 2001271 2001270 2001269 2001043 2000893 2000892 2000891 2000631 2000630 2000632 2006877 2006876 2000662 2000660 2000659 2000663 2000661 2005742 2005741 2005740 2005739 2005738 2005737 2000561 2000562 2000557 2000558 2000559 2000564 2000565 2000140 2000142 2000139 2000141 2000136 2000137 2000135 2000150 2004492 2004493 2004494 2004497 2000534 2004262 2004495 2000538 2004496 2004519 2004520 2000124 2000125 2000123 2000128 2000129 2000127 2000130 2000133 2000132 2000131 2000570 2000568 2000569 2005475 2005486 2005499 2005500 2005506 2000637 2005505 2000640 2006392 2006393 2006394 2006397 2006398 2006399 1101310 1101311 1101312 1101807 1101808 1101815 1101816 1101818 1101819 1101820 1101821 1101822 1101823 1100730 1101839 1101840 1101841 1101842 1101843 1201634 1201635 1201636 1201637 1201638 1201639
M10 -> 2008186 2008185 1100134 2009686 2009685 2000800 1100257 1100226 1100136 1100135 1111329 2011893 2000890 1207892 1208600
M11 -> 101964 101997 101996 101995 101279 101278 101277 101276 101275 101273 101159 101158 101157 101156 100394 101101 101100 100378 100260 100224 100223 100222 100775 100667 100666 100665 100664 100403 100353 100345 100344 100343 100342 100258 100257 100256 100227 100225 100374 100375 100388 100389 100400 100415 100432 100433 100434 100536 100537 100538 100662 100663 100450 100451 100186 100189 101249 101250 101402 101403 101963
M13 -> 1400150 1400149 1500046 1500047
MTU -> 400011
*/
void M00_Soldier_Powerup_Grant::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->isDropDisabled, sizeof(this->isDropDisabled), 1);
}

void M00_Soldier_Powerup_Grant::Created(GameObject *obj)
{
	this->isDropDisabled = false;
}

void M00_Soldier_Powerup_Grant::Killed(GameObject *obj, GameObject *killer)
{
	static const char *soldierPowerupTable[23][3] =
	{
		{ "Mutant_0_Mutant", "tw_POW00_Armor", "tw_POW00_Armor" },
		{ "Mutant_1Off_Acolyte", "POW_TiberiumAutoRifle_Player", "tw_POW02_LaserRifle" },
		{ "Mutant_2SF_Templar", "POW_TiberiumAutoRifle_Player", "tw_POW01_TiberiumAutoRifle" },
		{ "Nod_FlameThrower_2SF", "POW_LaserRifle_Player", "tw_POW02_LaserRifle" },
		{ "Nod_FlameThrower_1Off", "POW_ChemSprayer_Player", "tw_POW01_Chemsprayer" },
		{ "Nod_FlameThrower_0", "POW_Flamethrower_Player", "tw_POW00_Flamethrower" },
		{ "Nod_Minigunner_0", "POW_AutoRifle_Player", "tw_POW00_AutoRifle" },
		{ "Nod_Minigunner_1Off_LaserChaingun", "POW_LaserChaingun_Player", "tw_POW01_LaserChaingun" },
		{ "Nod_Minigunner_1Off_Shotgun", "tw_POW00_Health", "tw_POW00_Health" },
		{ "Nod_Minigunner_1Off", "POW_Chaingun_Player", "tw_POW01_Chaingun" },
		{ "Nod_Minigunner_2SF_AutoRifle", "POW_AutoRifle_Player", "tw_POW02_AutoRifle" },
		{ "Nod_Minigunner_2SF_Chaingun", "POW_Chaingun_Player", "tw_POW02_Chaingun" },
		{ "Nod_Minigunner_2SF_Ramjet", "POW_RamjetRifle_Player", "tw_POW02_RamjetRifle" },
		{ "Nod_Minigunner_2SF_Stationary", "POW_SniperRifle_Player", "tw_POW02_SniperRifle" },
		{ "Nod_Minigunner_2SF_Shotgun", "tw_POW00_Health", "tw_POW02_SniperRifle" },
		{ "Nod_Minigunner_2SF_LaserRifle", "tw_POW00_Health", "tw_POW00_Health" },
		{ "Nod_Minigunner_2SF", "POW_SniperRifle_Player", "tw_POW00_Health" },
		{ "Nod_RocketSoldier_2SF_AutoRifle", "POW_AutoRifle_Player", "tw_POW02_AutoRifle" },
		{ "Nod_RocketSoldier_2SF_Chaingun", "POW_Chaingun_Player", "tw_POW02_Chaingun" },
		{ "Nod_RocketSoldier_2SF_VoltAutoRifle", "POW_VoltAutoRifle_Player", "tw_POW02_VoltAutoRifle" },
		{ "Nod_RocketSoldier_2SF_LaserRifle", "tw_POW00_Health", "tw_POW00_Health" },
		{ "Nod_RocketSoldier", "POW_RocketLauncher_Player", "tw_POW00_RocketLauncher" },
		{ "", "tw_POW00_Health", "tw_POW00_Health" }
	};

	if (!this->isDropDisabled)
	{
		if (killer)
		{
			if (Commands->Is_A_Star(killer))
			{
				int randInt = Commands->Get_Random_Int(0, 3);
				if (randInt >= Commands->Get_Difficulty_Level())
				{
					const char *presetName = Commands->Get_Preset_Name(obj);
					Commands->Debug_Message("Soldier_Powerup_Grant for %s\n", presetName);

					int soldierPowerupIndex = 0;
					while (strncmp(soldierPowerupTable[soldierPowerupIndex][0], presetName, strlen(soldierPowerupTable[soldierPowerupIndex][0])))
					{
						if (++soldierPowerupIndex >= 23)
						{
							Commands->Debug_Message("Soldier_Powerup_Grant failed to find name match %s\n", presetName);

							soldierPowerupIndex = 0;

							break;
						}
					}

					Commands->Debug_Message("Soldier_Powerup_Grant: index %d\n", soldierPowerupIndex);

					float starHealth = Commands->Get_Health(killer);
					float starMaxHealth = Commands->Get_Max_Health(killer);

					float starHealthPercentage = 0.0f;
					if (starMaxHealth != 0.0f)
					{
						starHealthPercentage = starHealth / starMaxHealth;
					}

					float starShieldStrength = Commands->Get_Shield_Strength(killer);
					float starMaxShieldStrength = Commands->Get_Max_Shield_Strength(killer);

					float starShieldStrengthPercentage = 0.0f;
					if (starMaxShieldStrength != 0.0f)
					{
						starShieldStrengthPercentage = starShieldStrength / starMaxShieldStrength;
					}

					Vector3 pos = Commands->Get_Position(obj);
					pos.Z = pos.Z + 0.75f;

					GameObject *dropObj = NULL;
					if (starHealthPercentage < 0.25f)
					{
						Commands->Debug_Message("Soldier_Powerup_Grant: Star's Health < 25%%.  Dropping Health\n");

						dropObj = Commands->Create_Object("tw_POW00_Health", pos);
					}
					else if (starShieldStrengthPercentage < 0.25f)
					{
						Commands->Debug_Message("Soldier_Powerup_Grant: Star's Shield < 25%%.  Dropping Sheild\n");

						dropObj = Commands->Create_Object("tw_POW00_Armor", pos);
					}
					else if (starHealthPercentage > 0.75f)
					{
						if (soldierPowerupTable[soldierPowerupIndex][1] && soldierPowerupTable[soldierPowerupIndex][1][0] != '\0')
						{
								Commands->Debug_Message("Soldier_Powerup_Grant: Star's Health > 75%%.  Dropping soldier's weapon %s\n", soldierPowerupTable[soldierPowerupIndex][1]);

								dropObj = Commands->Create_Object(soldierPowerupTable[soldierPowerupIndex][1], pos);
						}
					}
					else
					{
						if (soldierPowerupTable[soldierPowerupIndex][2] && soldierPowerupTable[soldierPowerupIndex][2][0] != '\0')
						{
							Commands->Debug_Message("Soldier_Powerup_Grant: Dropping twiddler %s\n", soldierPowerupTable[soldierPowerupIndex][2]);

							dropObj = Commands->Create_Object(soldierPowerupTable[soldierPowerupIndex][2], pos);
						}
					}

					if (dropObj)
					{
						Commands->Attach_Script(dropObj, "M00_Powerup_Destroy", "");
					}
				}
			}
		}
	}
}

void M00_Soldier_Powerup_Grant::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Used to disable dropping
	if (type == 9024)
	{
		this->isDropDisabled = true;
	}
}

ScriptRegistrant<M00_Soldier_Powerup_Grant> M00_Soldier_Powerup_GrantRegistrant("M00_Soldier_Powerup_Grant", "");