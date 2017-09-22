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
#include "M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG.h"

void M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 3);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 4);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 5);
}

// After 401 cinematic frames in X1I_GDI_HON_TroopDrop01a.txt
void M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG::Created(GameObject *obj)
{
	this->field_20 = 0;
	this->field_24 = 0;
	this->field_1C = false;
	this->field_1D = false;
	this->field_1E = true;

	Commands->Enable_Hibernation(obj, false);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, false);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj, false);

	GameObject *M01HONWarRoomControllerJDGObj = Commands->Find_Object(124044);
	if (M01HONWarRoomControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01HONWarRoomControllerJDGObj, 0, 12, 0.0f);
	}

	GameObject *M01GiveMCTSpeechZoneJDGObj = Commands->Find_Object(124260);
	if (M01GiveMCTSpeechZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GiveMCTSpeechZoneJDGObj, 0, 12, 0.0f);
	}
}

void M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG::Killed(GameObject *obj, GameObject *killer)
{
	if (this->field_1E)
	{
		GameObject *M01HONWarRoomControllerJDGObj = Commands->Find_Object(124044);
		if (M01HONWarRoomControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01HONWarRoomControllerJDGObj, 0, 22, 0.0f);
		}
	}
}

// TODO
void M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (obj && !type)
	{
		if (param == 38)
		{
			if (!this->field_1C)
			{
				GameObject *nodHonBuildingObj = Commands->Find_Object(153909);
				if (Commands->Get_Health(nodHonBuildingObj) <= 0.0f)
				{
					Commands->Send_Custom_Event(obj, obj, 0, 25, 0.0f);
				}
				else
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 60.0f, 38);
					params.Set_Movement(Vector3(-187.84801f, 526.47998f, 3.5179999f), 0.8f, 1.0f);

					Commands->Action_Goto(obj, params);

					Commands->Send_Custom_Event(obj, obj, 0, 38, 5.0f);
				}
			}
		}
		else if (param == 16)
		{
			if (!this->field_1D)
			{
				this->field_1C = true;

				GameObject *nodHonBuildingObj = Commands->Find_Object(153909);
				if (Commands->Get_Health(nodHonBuildingObj) <= 0.0f)
				{
					Commands->Send_Custom_Event(obj, obj, 0, 25, 0.0f);
				}
				else
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 60.0f, 39);
					params.Set_Movement(Vector3(-181.084f, 541.058f, 3.518f), 0.3f, 1.0f);

					Commands->Action_Goto(obj, params);

					Commands->Send_Custom_Event(obj, obj, 0, 16, 5.0f);
				}
			}
		}
		else if (param == 25)
		{
			this->field_1E = false;

			Commands->Set_Innate_Is_Stationary(obj, false);
			Commands->Enable_Hibernation(obj, true);

			ActionParamsStruct params;
			params.Set_Basic(this, 60.0f, 40);
			params.Set_Movement(Vector3(-181.163f, 534.529f, 10.034f), 0.8f, 5.0f);

			Commands->Action_Goto(obj, params);
			
			Commands->Send_Custom_Event(obj, obj, 0, 25, 5.0f);
		}
	}
}

// TODO
void M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL && action_id == 39)
	{
		GameObject *nodHonBuildingObj = Commands->Find_Object(153909);
		if (Commands->Get_Health(nodHonBuildingObj) <= 0.0f)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 25, 0.0f);
		}
		else
		{
			this->field_1D = true;

			Commands->Set_Innate_Is_Stationary(obj, true);

			GameObject *M01GiveMCTSpeechZoneJDGObj = Commands->Find_Object(124260);
			if (M01GiveMCTSpeechZoneJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GiveMCTSpeechZoneJDGObj, 0, 16, 0.0f);
			}
		}
	}
}

void M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "s_a_human.h_a_troopdrop"))
	{
		Commands->Send_Custom_Event(obj, obj, 0, 38, 0.0f);
		Commands->Send_Custom_Event(obj, obj, 0, 27, 5.0f);
	}
}

ScriptRegistrant<M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG> M01_HON_Chinook_Spawned_Soldier_01_GDI_JDGRegistrant("M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG", "");