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
#include "M01_Interrogation_Room_Surprise_Guy_JDG.h"

/*
M01 -> 101357
*/
void M01_Interrogation_Room_Surprise_Guy_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->interrogationRoomLocation, sizeof(this->interrogationRoomLocation), 1);
	Auto_Save_Variable(&this->canMoveToInterrogationRoom, sizeof(this->canMoveToInterrogationRoom), 2);
}

void M01_Interrogation_Room_Surprise_Guy_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);

	this->canMoveToInterrogationRoom = true;
}

// TODO
void M01_Interrogation_Room_Surprise_Guy_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Left_Interrogation_Room_Enter_Zone_JDG when entered
	if (param == 155)
	{
		this->interrogationRoomLocation = Vector3(-182.199f, 529.167f, -31.13f);

		ActionParamsStruct params;
		params.Set_Basic(this, 80.0f, 38);
		params.Set_Movement(this->interrogationRoomLocation, 0.75f, 0.05f);

		Commands->Action_Goto(obj, params);

		this->canMoveToInterrogationRoom = false;
	}

	// Received from M01_Right_Interrogation_Room_Enter_Zone_JDG when entered
	else if (param == 156)
	{
		this->interrogationRoomLocation = Vector3(-182.753f, 539.906f, -31.13f);

		ActionParamsStruct params;
		params.Set_Basic(this, 80.0f, 38);
		params.Set_Movement(this->interrogationRoomLocation, 0.75f, 0.05f);

		Commands->Action_Goto(obj, params);

		this->canMoveToInterrogationRoom = false;
	}

	// received from ourselves after 0.5 to 2 seconds when an enemy is seen or 1 to 3 seconds after action with id 38 is complete
	else if (param == 27)
	{
		if (obj)
		{
			Commands->Grant_Key(obj, 1, true);
			Commands->Grant_Key(obj, 2, true);
			Commands->Grant_Key(obj, 3, true);

			Commands->Innate_Enable(obj);

			ActionParamsStruct params;
			params.Set_Basic(this, 80.0f, 65);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 20.0f, 0.0f, true);
			params.AttackActive = true;
			params.AttackCheckBlocked = true;

			Commands->Action_Attack(obj, params);
		}
	}
}

void M01_Interrogation_Room_Surprise_Guy_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (this->canMoveToInterrogationRoom && obj)
	{
		Commands->Innate_Enable(obj);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Sound("Huh_2", pos, obj);

		float randDuration = Commands->Get_Random(0.5f, 2.0f);
		Commands->Send_Custom_Event(obj, obj, 0, 27, randDuration);

		this->canMoveToInterrogationRoom = false;

		GameObject *M01LeftInterrogationRoomEnterZoneJDGObj = Commands->Find_Object(101354);
		if (M01LeftInterrogationRoomEnterZoneJDGObj)
		{
			Commands->Destroy_Object(M01LeftInterrogationRoomEnterZoneJDGObj);
		}

		GameObject *M01RightInterrogationRoomEnterZoneJDGObj = Commands->Find_Object(101355);
		if (M01RightInterrogationRoomEnterZoneJDGObj)
		{
			Commands->Destroy_Object(M01RightInterrogationRoomEnterZoneJDGObj);
		}
	}
}

// TODO
void M01_Interrogation_Room_Surprise_Guy_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 38 && complete_reason == ACTION_COMPLETE_NORMAL)
	{
		Commands->Create_Sound("TargetHasBeenEngaged_2", this->interrogationRoomLocation, obj);

		ActionParamsStruct params;
		params.Set_Basic(this, 80.0f, 65);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 20.0f, 0.0f, true);
		params.AttackActive = true;
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);

		float randDuration = Commands->Get_Random(1.0f, 3.0f);
		Commands->Send_Custom_Event(obj, obj, 0, 27, randDuration);
	}
}

ScriptRegistrant<M01_Interrogation_Room_Surprise_Guy_JDG> M01_Interrogation_Room_Surprise_Guy_JDGRegistrant("M01_Interrogation_Room_Surprise_Guy_JDG", "");