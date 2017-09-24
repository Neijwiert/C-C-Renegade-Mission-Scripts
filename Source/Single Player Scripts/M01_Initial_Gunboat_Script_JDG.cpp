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
#include "M01_Initial_Gunboat_Script_JDG.h"

/*
M01 -> 101477
*/
void M01_Initial_Gunboat_Script_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->healthBelow75Percent, sizeof(this->healthBelow75Percent), 1);
	Auto_Save_Variable(&this->healthBelow50Percent, sizeof(this->healthBelow50Percent), 2);
	Auto_Save_Variable(&this->startPos, sizeof(this->startPos), 3);
	Auto_Save_Variable(&this->startFacing, sizeof(this->startFacing), 4);
}

void M01_Initial_Gunboat_Script_JDG::Created(GameObject *obj)
{
	this->healthBelow75Percent = false;
	this->healthBelow50Percent = false;
	this->startPos = Commands->Get_Position(obj);
	this->startFacing = Commands->Get_Facing(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 39);
	params.AttackActive = false;
	params.AttackCheckBlocked = false;

	Commands->Action_Attack(obj, params);
}

void M01_Initial_Gunboat_Script_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	if (M01MissionControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 113, 0.0f);
	}

	GameObject *sinkingGunboatObj = Commands->Create_Object("M01_GDI_Gunboat", this->startPos);
	Commands->Set_Facing(sinkingGunboatObj, this->startFacing);
	Commands->Attach_Script(sinkingGunboatObj, "M01_Sinking_Gunboat_JDG", "");
}

void M01_Initial_Gunboat_Script_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		float health = Commands->Get_Health(obj);
		float maxHealth = Commands->Get_Max_Health(obj);

		float percentageHealthLeft = health / maxHealth * 100.0f;
		if (health <= 10.0f)
		{
			Commands->Set_Health(obj, 10.0f); // Health warnings my ass, it cannot die by damage
		}

		if (percentageHealthLeft <= 50.0f)
		{
			if (!this->healthBelow50Percent)
			{
				this->healthBelow50Percent = true;

				// Locke, where's that shoreline support team?!
				// Take out the turrets to protect the gunboat and we'll land some firepower.
				// You're worse than a pager, Locke.
				int conversationId = Commands->Create_Conversation("M01_Gunboat_HealthWarning_02", 100, 1000.0f, false);
				Commands->Join_Conversation(NULL, conversationId, false, false, true);
				Commands->Join_Conversation(NULL, conversationId, false, false, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, conversationId, false, false, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
			
		}
		else if (percentageHealthLeft <= 75.0)
		{
			if (!this->healthBelow75Percent)
			{
				this->healthBelow75Percent = true;

				// Available units, Gunboat Tango under heavy off-shore fire!  Destroy those turrets or we're gone!
				// Take out the turrets to protect the gunboat and we'll land some firepower.
				int conversationId = Commands->Create_Conversation("M01_Gunboat_HealthWarning_01", 100, 1000.0f, false);
				Commands->Join_Conversation(NULL, conversationId, false, false, true);
				Commands->Join_Conversation(NULL, conversationId, false, false, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}
	}
}

// TODO
void M01_Initial_Gunboat_Script_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 29)
	{
		GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
		GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);

		if (beachNodTurret1Obj || beachNodTurret2Obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);

			float rand = Commands->Get_Random(0.5f, 2.5f);
			if (rand >= 0.5f && rand < 1.5f)
			{
				if (beachNodTurret1Obj)
				{
					Vector3 beachNodTurret1ObjPos = Commands->Get_Position(beachNodTurret1Obj);
					beachNodTurret1ObjPos.Z += 2.0f;

					params.Set_Attack(beachNodTurret1ObjPos, 2000.0f, 0.0f, true);
				}
				else if (beachNodTurret2Obj)
				{
					Vector3 beachNodTurret2ObjPos = Commands->Get_Position(beachNodTurret2Obj);
					beachNodTurret2ObjPos.Z += 2.0f;

					params.Set_Attack(beachNodTurret2ObjPos, 2000.0f, 0.0f, true);
				}
			}
			else
			{
				if (beachNodTurret2Obj)
				{
					Vector3 beachNodTurret2ObjPos = Commands->Get_Position(beachNodTurret2Obj);
					beachNodTurret2ObjPos.Z += 2.0f;

					params.Set_Attack(beachNodTurret2ObjPos, 2000.0f, 0.0f, true);
				}
				else if (beachNodTurret1Obj)
				{
					Vector3 beachNodTurret1ObjPos = Commands->Get_Position(beachNodTurret1Obj);
					beachNodTurret1ObjPos.Z += 2.0f;

					params.Set_Attack(beachNodTurret1ObjPos, 2000.0f, 0.0f, true);
				}
			}

			params.AttackActive = true;
			params.AttackCheckBlocked = false;

			Commands->Modify_Action(obj, 39, params, true, true);

			float randDelay = Commands->Get_Random(4.0f, 6.0f);
			Commands->Send_Custom_Event(obj, obj, 0, 29, randDelay);
		}
	}
	else if (param == 16)
	{
		Commands->Debug_Message("***************************gunboat custom received--start acting\n");

		Commands->Enable_Hibernation(obj, false);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 39);
		params.AttackActive = false;

		Commands->Modify_Action(obj, 39, params, true, true);

		float randDelay = Commands->Get_Random(0.0f, 2.0f);
		Commands->Send_Custom_Event(obj, obj, 0, 29, randDelay);
	}
	else if (param == 24)
	{
		Commands->Debug_Message("***************************gunboat custom received--STOP acting\n");

		Commands->Enable_Hibernation(obj, true);
	}
	else if (param == 112)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.7f, 10.0f);
		params.WaypathID = 103454;

		Commands->Action_Goto(obj, params);

		GameObject *M01GunboatSpawnHovercraftZoneJDGObj = Commands->Find_Object(102500);
		if (M01GunboatSpawnHovercraftZoneJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01GunboatSpawnHovercraftZoneJDGObj, 0, 27, 0.0f);
		}
	}
}

// TODO
void M01_Initial_Gunboat_Script_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 40)
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Initial_Gunboat_Script_JDG> M01_Initial_Gunboat_Script_JDGRegistrant("M01_Initial_Gunboat_Script_JDG", "");