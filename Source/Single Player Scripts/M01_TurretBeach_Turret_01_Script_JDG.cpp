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
#include "M01_TurretBeach_Turret_01_Script_JDG.h"

/*
M01 -> 101434 101435
*/
void M01_TurretBeach_Turret_01_Script_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->engineerObjId, sizeof(this->engineerObjId), 1);
	Auto_Save_Variable(&this->fodderHovercraftObjId, sizeof(this->fodderHovercraftObjId), 2);
}

void M01_TurretBeach_Turret_01_Script_JDG::Created(GameObject *obj)
{
	this->engineerObjId = 101654;
}

void M01_TurretBeach_Turret_01_Script_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01ObjectivePogControllerjDGObj = Commands->Find_Object(105828);
	if (M01ObjectivePogControllerjDGObj)
	{
		Commands->Send_Custom_Event(obj, M01ObjectivePogControllerjDGObj, 7, 109, 0.0f);
	}

	GameObject *engineerObj = Commands->Find_Object(this->engineerObjId);
	if (engineerObj)
	{
		Commands->Send_Custom_Event(obj, engineerObj, 0, 217, 0.0f);
	}

	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 217, 5.0f);

	Vector3 pos = Commands->Get_Position(obj);
	float facing = Commands->Get_Facing(obj);
	GameObject *destroyedTurretObj = Commands->Create_Object("Nod_Turret_Destroyed", pos);
	Commands->Set_Facing(destroyedTurretObj, facing);
}

void M01_TurretBeach_Turret_01_Script_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		GameObject *engineerObj = Commands->Find_Object(this->engineerObjId);
		if (damager && damager == engineerObj)
		{
			float maxHealth = Commands->Get_Max_Health(obj);
			Commands->Set_Health(obj, maxHealth);
		}
		else if (engineerObj)
		{
			Commands->Send_Custom_Event(obj, engineerObj, 0, 189, 0.0f);
		}
	}
}

void M01_TurretBeach_Turret_01_Script_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_TurretBeach_FodderHovercraft_Controller_JDG after custom  type <= 0 and param <= 0 is received with param = hovercraft id
	if (type == 218)
	{
		this->fodderHovercraftObjId = param;

		GameObject *fodderHovercraftObj = Commands->Find_Object(param);
		if (fodderHovercraftObj)
		{
			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 17);
			params.Set_Attack(fodderHovercraftObj, 3000.0f, 0.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
	}
	else if(!type)
	{
		// Received from M01_mission_Controller_JDG when param 128 is received
		if (param == 16)
		{
			GameObject *gdiGunBoatObj = Commands->Find_Object(101477);
			if (gdiGunBoatObj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 16);
				params.Set_Attack(gdiGunBoatObj, 3000.0f, 3.0f, true);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
			}
		}

		// Received from M01_TurretBeach_Engineer_JDG after 1 second when param 189 is received
		if (param == 221)
		{
			GameObject *engineerObj = Commands->Find_Object(this->engineerObjId);
			if (engineerObj)
			{
				Commands->Send_Custom_Event(obj, engineerObj, 0, 189, 1.0f);
			}
		}

		// Received from M01_TurretBeach_Chinook_Spawned_Soldier_NOD when created
		else if (param == 14)
		{
			this->engineerObjId = Commands->Get_ID(sender);
		}
	}
}

void M01_TurretBeach_Turret_01_Script_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When done attacking the hovercraft, see param 218
	if (action_id == 17)
	{
		GameObject *gdiGunBoatObj = Commands->Find_Object(101477);
		if (gdiGunBoatObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			params.Set_Attack(gdiGunBoatObj, 3000.0f, 3.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
	}
}

ScriptRegistrant<M01_TurretBeach_Turret_01_Script_JDG> M01_TurretBeach_Turret_01_Script_JDGRegistrant("M01_TurretBeach_Turret_01_Script_JDG", "");