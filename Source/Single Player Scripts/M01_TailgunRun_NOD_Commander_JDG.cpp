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
#include "M01_TailgunRun_NOD_Commander_JDG.h"

/*
M01 -> 102358
*/
void M01_TailgunRun_NOD_Commander_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->killed, sizeof(this->killed), 1);
	Auto_Save_Variable(&this->canDoDamagedAnimation, sizeof(this->canDoDamagedAnimation), 2);
	Auto_Save_Variable(&this->enemySeen, sizeof(this->enemySeen), 2);
}

void M01_TailgunRun_NOD_Commander_JDG::Created(GameObject *obj)
{
	this->killed = false;
	this->canDoDamagedAnimation = true;
	this->enemySeen = false;

	Commands->Set_Innate_Is_Stationary(obj, true);
}

void M01_TailgunRun_NOD_Commander_JDG::Killed(GameObject *obj, GameObject *killer)
{
	this->killed = true;
}

void M01_TailgunRun_NOD_Commander_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (damager == starObj && !this->killed && this->canDoDamagedAnimation)
		{
			Vector3 starPos = Commands->Get_Position(starObj);
			if (Commands->Get_Distance(pos, starPos) >= 35.0f)
			{
				this->canDoDamagedAnimation = false;

				static const char *animations[5] =
				{
					"H_A_A0A0_L20",
					"H_A_A0A0_L21",
					"H_A_A0A0_L36",
					"H_A_A0A0_L52",
					"H_A_J21C"
				};

				int randAnimationIndex = Commands->Get_Random_Int(0, 5);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 47);
				params.Set_Animation(animations[randAnimationIndex], false);

				Commands->Action_Play_Animation(obj, params);
			}
		}
	}
}

void M01_TailgunRun_NOD_Commander_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_mission_Controller_JDG after 8 seconds when param 134 is received
		if (param == 16)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 27, 0.0f);
			Commands->Send_Custom_Event(obj, obj, 0, 66, 0.0f);
		}

		// Received from ourselves when param 16 is received or 60 to 120 seconds after this block
		else if (param == 27)
		{
			if (obj)
			{
				float delay;

				int difficultyLevel = Commands->Get_Difficulty_Level();
				switch (difficultyLevel)
				{
					case 1:
						delay = 90.0f;

						break;
					case 2:
						delay = 60.0f;

						break;
					default:
						delay = 120.0f;

						break;
				}

				Commands->Send_Custom_Event(obj, obj, 0, 73, delay);
				Commands->Send_Custom_Event(obj, obj, 0, 27, delay);
			}
		}

		// Received from ourselves after 60 to 120 seconds after param 27 is received
		else if (param == 73)
		{
			if (obj)
			{
				GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
				Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 73, 0.0f);
			}
		}

		// Received from ourselves when param 16 is received or 15 to 30 seconds after this block
		else if (param == 66)
		{
			if (obj && !this->killed)
			{
				GameObject *M01NodCommanderConversationControllerGDIObj = Commands->Find_Object(103398);
				if (M01NodCommanderConversationControllerGDIObj)
				{
					Commands->Send_Custom_Event(obj, M01NodCommanderConversationControllerGDIObj, 0, 27, 0.0f);
				}

				float randDelay = Commands->Get_Random(15.0, 30.0);
				Commands->Send_Custom_Event(obj, obj, 0, 66, randDelay);
			}
		}
	}
}

void M01_TailgunRun_NOD_Commander_JDG::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (sound.Creator == starObj && sound.sound == SOUND_TYPE_BULLET_HIT && obj && !this->killed && !this->enemySeen)
	{
		Vector3 starPos = Commands->Get_Position(starObj);
		if (Commands->Get_Distance(pos, starPos) >= 15.0f)
		{
			static const char *animations[5] =
			{
				"H_A_A0A0_L20",
				"H_A_A0A0_L21",
				"H_A_A0A0_L36",
				"H_A_A0A0_L52",
				"H_A_J21C"
			};

			int randAnimationIndex = Commands->Get_Random_Int(0, 5);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 47);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}
	}
}

void M01_TailgunRun_NOD_Commander_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enemy == Commands->Get_A_Star(pos))
	{
		this->enemySeen = true;
	}
}

ScriptRegistrant<M01_TailgunRun_NOD_Commander_JDG> M01_TailgunRun_NOD_Commander_JDGRegistrant("M01_TailgunRun_NOD_Commander_JDG", "");