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
#include "M03_Engineer_Repair.h"

/*
M03 -> 1145889
M07 -> 125863 125862 125861 125860 121918 121922 121923 121924 121925
*/
void M03_Engineer_Repair::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->reppingTarget, sizeof(this->reppingTarget), 1);
	Auto_Save_Variable(&this->objId, sizeof(this->objId), 2);
	Auto_Save_Variable(&this->repTargetObjId, sizeof(this->repTargetObjId), 3);
	Auto_Save_Variable(&this->repairPriority, sizeof(this->repairPriority), 4);
}

void M03_Engineer_Repair::Created(GameObject *obj)
{
	this->reppingTarget = false;
	this->repairPriority = Get_Int_Parameter("Repair_Priority");
}

void M03_Engineer_Repair::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 40012)
	{
		if (!this->reppingTarget)
		{
			int creatorPlayerType = Commands->Get_Player_Type(sound.Creator);
			if (creatorPlayerType == Commands->Get_Player_Type(obj))
			{
				this->objId = Commands->Get_ID(obj);
				this->reppingTarget = true;

				Commands->Send_Custom_Event(obj, sound.Creator, 40011, this->objId, 0.0f);

				this->repTargetObjId = Commands->Get_ID(sound.Creator);

				ActionParamsStruct params;
				params.Set_Basic(this, static_cast<float>(this->repairPriority), 40015);

				Vector3 creatorPos = Commands->Get_Position(sound.Creator);

				params.Set_Movement(creatorPos, 0.8f, 5.0f);

				Commands->Action_Goto(obj, params);
			}
		}
	}
	if (sound.sound == 40014)
	{
		if (Commands->Get_ID(sound.Creator) == this->repTargetObjId)
		{
			Commands->Action_Reset(obj, 100.0f);

			this->reppingTarget = false;
		}
	}
}

void M03_Engineer_Repair::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When we moved to the rep location, see sound heard
	if (action_id == 40015)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, static_cast<float>(this->repairPriority), 40016);
		
		GameObject *repTargetObj = Commands->Find_Object(this->repTargetObjId);

		params.Set_Attack(repTargetObj, 50.0f, 0.0f, false);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M03_Engineer_Repair> M03_Engineer_RepairRegistrant("M03_Engineer_Repair", "Repair_Priority=96:int");