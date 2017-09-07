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