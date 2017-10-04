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
#include "M01_GuardTower_Sniper_Target_JDG.h"

void M01_GuardTower_Sniper_Target_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->waveLocation, sizeof(this->waveLocation), 1);
	Auto_Save_Variable(&this->deleteLocation, sizeof(this->deleteLocation), 2);
}

// When M01_Nod_GuardTower_01_Enter_Zone_JDG is entered
// When M01_GDI_GuardTower_02_Enter_Zone_JDG is entered
// When M01_Nod_GuardTower_02_Enter_Zone_JDG is entered
void M01_GuardTower_Sniper_Target_JDG::Created(GameObject *obj)
{
	this->waveLocation = Get_Vector3_Parameter("wave_location");
	this->deleteLocation = Get_Vector3_Parameter("delete_location");

	Commands->Enable_Hibernation(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 38);
	params.Set_Movement(this->waveLocation, 0.8f, 1.0f);

	Commands->Action_Goto(obj, params);
}

void M01_GuardTower_Sniper_Target_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += 0.75f;

	static const char *drops[2] =
	{
		"POW_Health_100",
		"POW_Armor_100"
	};

	int randDropIndex = Commands->Get_Random_Int(0, 2);
	Commands->Create_Object(drops[randDropIndex], pos);
}

void M01_GuardTower_Sniper_Target_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Triggered after 1 second when action with id 38 is complete
	if (param == 63)
	{
		static const char *animations[4] =
		{
			"H_A_J01C",
			"H_A_J11C",
			"H_A_J23C",
			"H_A_J27C"
		};

		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 46);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);
	}
}

void M01_GuardTower_Sniper_Target_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		// When we moved to the delete location at action id 46
		if (action_id == 39)
		{
			Commands->Destroy_Object(obj);
		}

		// When we finished the animation in custom
		else if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Movement(this->deleteLocation, 0.8f, 5.0f);

			Commands->Action_Goto(obj, params);
		}

		// When moved to the wavelocation on create 
		else if (action_id == 38)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 63);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 0.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);

			Commands->Send_Custom_Event(obj, obj, 0, 63, 1.0f);
		}
	}
}

ScriptRegistrant<M01_GuardTower_Sniper_Target_JDG> M01_GuardTower_Sniper_Target_JDGRegistrant("M01_GuardTower_Sniper_Target_JDG", "wave_location:vector3,  delete_location:vector3"); // Wow, they actually use script parameters here. Who would've thought.