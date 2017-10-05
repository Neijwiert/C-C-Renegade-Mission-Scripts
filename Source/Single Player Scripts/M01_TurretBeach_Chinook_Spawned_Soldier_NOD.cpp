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
#include "M01_TurretBeach_Chinook_Spawned_Soldier_NOD.h"

void M01_TurretBeach_Chinook_Spawned_Soldier_NOD::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->killed, sizeof(this->killed), 1);
	Auto_Save_Variable(&this->movedOffTheBeach, sizeof(this->movedOffTheBeach), 2);
}

// After 401 cinematic frames in X1I_Nod_TurretBeach_TroopDrop.txt
void M01_TurretBeach_Chinook_Spawned_Soldier_NOD::Created(GameObject *obj)
{
	this->killed = false;
	this->movedOffTheBeach = false;

	GameObject *M01GunboatActionControllerJDGObj = Commands->Find_Object(113325);
	if (M01GunboatActionControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GunboatActionControllerJDGObj, 0, 14, 0.0f);
	}

	GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
	if (beachNodTurret1Obj)
	{
		Commands->Send_Custom_Event(obj, beachNodTurret1Obj, 0, 14, 0.0f);
	}

	GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
	if (beachNodTurret2Obj)
	{
		Commands->Send_Custom_Event(obj, beachNodTurret2Obj, 0, 14, 0.0f);
	}
}

void M01_TurretBeach_Chinook_Spawned_Soldier_NOD::Killed(GameObject *obj, GameObject *killer)
{
	this->killed = true;

	GameObject *M01MissionControllerObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 222, 5.0f);
}

void M01_TurretBeach_Chinook_Spawned_Soldier_NOD::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		Commands->Innate_Enable(obj);
	}
}

// TODO
void M01_TurretBeach_Chinook_Spawned_Soldier_NOD::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 5 seconds after this block
	// Received from ourselves after 5 seconds when animation is complete
	if (param == 40)
	{
		if (!this->movedOffTheBeach)
		{
			GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
			GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
			if (beachNodTurret1Obj || beachNodTurret2Obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 40, 5.0f);
			}
			else
			{
				Commands->Action_Reset(obj, 100.0f);
			}
		}
	}

	// Received from M01_GunboatAction_Controller_JDG when param 16 is received
	else if (param == 16)
	{
		Commands->Debug_Message("***************************new engineer custom received--start acting\n");

		Commands->Enable_Hibernation(obj, false);

		Commands->Send_Custom_Event(obj, obj, 0, 224, 1.0f);
	}

	// Received from M01_GunboatAction_Controller_JDG when param 24 is received
	else if (param == 24)
	{
		Commands->Debug_Message("***************************new engineer custom received--STOP acting\n");

		Commands->Enable_Hibernation(obj, true);
	}

	// Received from ourselves after 1 second after param 16 or 1 second after this block
	// Received from ourselves after 1 second after action with id 40 or 42 is complete
	else if (param == 224)
	{
		GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
		GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);

		float turret1Health = 0.0f;
		float turret2Health = 0.0f;

		if (beachNodTurret1Obj)
		{
			turret1Health = Commands->Get_Health(beachNodTurret1Obj);
		}

		if (beachNodTurret2Obj)
		{
			turret2Health = Commands->Get_Health(beachNodTurret2Obj);
		}

		if (turret1Health > turret2Health)
		{
			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Movement(Vector3(-101.5f, 111.7f, 2.3f), 1.0f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
		else if (turret2Health > turret1Health)
		{
			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(Vector3(-86.5f, 95.5f, 2.3f), 1.0f, 0.5f);

			Commands->Action_Goto(obj, params);
		}

		if (turret1Health <= turret2Health)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 224, 1.0f);
		}
	}

	// Received from ourselves after 0.5 seconds after action with id 38 is complete
	else if (param == 17)
	{
		GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
		if (beachNodTurret1Obj && !this->killed)
		{
			Vector3 beachNodTurret1ObjPos = Commands->Get_Position(beachNodTurret1Obj);
			Commands->Create_Sound("CnC_Healer_Sound", beachNodTurret1ObjPos, obj);

			float maxHealth = Commands->Get_Max_Health(beachNodTurret1Obj);
			Commands->Set_Health(beachNodTurret1Obj, maxHealth);
		}

		Commands->Debug_Message("***************************turret engineer has fixed turret one--going back to hiding spot\n");
	}

	// Received from ourselves after 0.5 seconds after action with id 39 is complete
	else if (param == 18)
	{
		GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
		if (beachNodTurret2Obj && !this->killed)
		{
			Vector3 beachNodTurret2ObjPos = Commands->Get_Position(beachNodTurret2Obj);
			Commands->Create_Sound("CnC_Healer_Sound", beachNodTurret2ObjPos, obj);

			float maxHealth = Commands->Get_Max_Health(beachNodTurret2Obj);
			Commands->Set_Health(beachNodTurret2Obj, maxHealth);
		}

		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(Vector3(-91.1f, 106.562f, 1.7f), 1.0f, 0.25f);
		params.MovePathfind = false;

		Commands->Action_Goto(obj, params);
	}
}

// TODO
void M01_TurretBeach_Chinook_Spawned_Soldier_NOD::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
			if (beachNodTurret1Obj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 17);
				params.Set_Attack(beachNodTurret1Obj, 50.0f, 0.0f, false);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
				Commands->Send_Custom_Event(obj, obj, 0, 17, 0.5f);
			}
		}
		else if (action_id == 39)
		{
			GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
			if (beachNodTurret2Obj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 18);
				params.Set_Attack(beachNodTurret2Obj, 50.0f, 0.0f, false);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);

				Commands->Send_Custom_Event(obj, obj, 0, 18, 0.5f);
			}
		}
		else if (action_id == 48)
		{
			static const char *animations[5] =
			{
				"H_A_A0A0_L20",
				"H_A_A0A0_L21",
				"H_A_A0A0_L36",
				"H_A_A0A0_L52",
				"H_A_J21C"
			};

			int randomAnimationIndex = Commands->Get_Random_Int(0, 5);

			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 60.0f, 49);
			params.Set_Animation(animations[randomAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if(action_id == 40)
		{
			this->movedOffTheBeach = true;

			Commands->Send_Custom_Event(obj, obj, 0, 224, 1.0f);
		}
		else if (action_id == 42)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 224, 1.0f);
		}
	}
}

void M01_TurretBeach_Chinook_Spawned_Soldier_NOD::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "s_a_human.h_a_troopdrop"))
	{
		GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
		GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
		if (beachNodTurret1Obj || beachNodTurret2Obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 40);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
			params.WaypathID = 109629;

			Commands->Action_Goto(obj, params);

			Commands->Send_Custom_Event(obj, obj, 0, 40, 5.0f);
		}
		else
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 40);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
			params.WaypathID = 109629;

			Commands->Action_Goto(obj, params);
		}
	}
}

ScriptRegistrant<M01_TurretBeach_Chinook_Spawned_Soldier_NOD> M01_TurretBeach_Chinook_Spawned_Soldier_NODRegistrant("M01_TurretBeach_Chinook_Spawned_Soldier_NOD", "");