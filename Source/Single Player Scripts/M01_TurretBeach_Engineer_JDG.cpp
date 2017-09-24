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
#include "M01_TurretBeach_Engineer_JDG.h"

/*
M01 -> 101654
*/
void M01_TurretBeach_Engineer_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->goingToRepairTurret, sizeof(this->goingToRepairTurret), 1);
	Auto_Save_Variable(&this->canDoDamagedAnimation, sizeof(this->canDoDamagedAnimation), 2);
	Auto_Save_Variable(&this->killed, sizeof(this->killed), 3);
	Auto_Save_Variable(&this->turret1Killed, sizeof(this->turret1Killed), 4);
	Auto_Save_Variable(&this->turret2Killed, sizeof(this->turret2Killed), 5);

	// health is not saved here, should be a problem when save/loading the game
}

void M01_TurretBeach_Engineer_JDG::Created(GameObject *obj)
{
	this->turret1Killed = false;
	this->turret2Killed = false;
	this->killed = false;
	this->goingToRepairTurret = false;
	this->canDoDamagedAnimation = true;

	Commands->Innate_Disable(obj);

	this->health = Commands->Get_Max_Health(obj);
}

void M01_TurretBeach_Engineer_JDG::Killed(GameObject *obj, GameObject *killer)
{
	this->killed = true;

	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 222, 5.0f);
}

void M01_TurretBeach_Engineer_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	int damagerId = Commands->Get_ID(damager);
	if (damagerId)
	{
		Commands->Debug_Message("***************************engineers damager is ID %d\n", damagerId);
	}
	else
	{
		Commands->Debug_Message("***************************engineers damager is NULL\n");
	}

	if (damager == Commands->Find_Object(101477)) // GDI Gunboat
	{
		Commands->Set_Health(obj, this->health);
	}
	else
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (damager == starObj && obj)
		{
			this->health = Commands->Get_Health(obj);
			if (this->canDoDamagedAnimation)
			{
				this->goingToRepairTurret = false;

				Commands->Action_Reset(obj, 100.0f);
				Commands->Innate_Enable(obj);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 48);
				params.Set_Animation("H_A_862A", false);

				Commands->Action_Play_Animation(obj, params);

				Commands->Innate_Force_State_Enemy_Seen(obj, starObj);

				this->canDoDamagedAnimation = false;
			}
			else
			{
				this->goingToRepairTurret = false;

				Commands->Action_Reset(obj, 100.0f);
				Commands->Innate_Enable(obj);
				
				Commands->Innate_Force_State_Enemy_Seen(obj, starObj);
			}
		}
	}
}

// TODO
void M01_TurretBeach_Engineer_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		Commands->Debug_Message("***************************original engineer custom received--start acting\n");

		Commands->Enable_Hibernation(obj, false);
	}
	else if (param == 24)
	{
		Commands->Debug_Message("***************************original engineer custom received--STOP acting\n");

		Commands->Enable_Hibernation(obj, true);
	}
	else if (param == 189)
	{
		if (this->goingToRepairTurret)
		{
			Commands->Send_Custom_Event(obj, sender, 0, 221, 1.0f);
		}
		else
		{
			Commands->Debug_Message("***************************engineers has received custom to go heal turret\n");

			Commands->Innate_Disable(obj);
			Commands->Action_Reset(obj, 100.0f);

			GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
			GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);

			float turret2Health = 0.0f;
			float turret1Health = 0.0f;

			if (beachNodTurret1Obj)
			{
				turret1Health = Commands->Get_Health(beachNodTurret1Obj);
			}

			if (beachNodTurret2Obj)
			{
				turret2Health = Commands->Get_Health(beachNodTurret2Obj);
			}

			// I know below is confusing as fuck, but trust me this is how it is executed (took me some time to figure it out)
			if (turret1Health <= 0.0f || turret2Health <= 0.0f || turret1Health == turret2Health)
			{
				Commands->Send_Custom_Event(obj, sender, 0, 221, 1.0f);
			}
			else
			{
				if (turret1Health > turret2Health)
				{
					if (beachNodTurret2Obj && !this->turret2Killed)
					{
						ActionParamsStruct params;
						params.Set_Basic(this, 100.0f, 38);
						params.Set_Movement(Vector3(-86.5f, 95.5f, 2.3f), 1.0f, 0.5f);
						params.MovePathfind = false;

						Commands->Action_Goto(obj, params);

						this->goingToRepairTurret = true;
					}
				}
				else if (turret2Health > turret1Health)
				{
					if (beachNodTurret1Obj && !this->turret1Killed)
					{
						ActionParamsStruct params;
						params.Set_Basic(this, 100.0f, 39);
						params.Set_Movement(Vector3(-101.5f, 111.7f, 2.3f), 1.0f, 0.5f);
						params.MovePathfind = false;

						Commands->Action_Goto(obj, params);

						this->goingToRepairTurret = true;
					}
				}
			}
		}
	}
	else if (param == 17)
	{
		GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
		if (beachNodTurret1Obj && !this->killed && !this->turret1Killed)
		{
			Vector3 beachNodTurret1ObjPos = Commands->Get_Position(beachNodTurret1Obj);
			Commands->Create_Sound("CnC_Healer_Sound", beachNodTurret1ObjPos, obj);

			float turretMaxHealth = Commands->Get_Max_Health(beachNodTurret1Obj);
			Commands->Set_Health(beachNodTurret1Obj, turretMaxHealth);
		}

		Commands->Debug_Message("***************************turret engineer has fixed turret one--going back to hiding spot\n");

		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(Vector3(-91.1f, 106.562f, 1.7f), 1.0f, 0.25f);
		params.MovePathfind = false;

		Commands->Action_Goto(obj, params);
	}
	else if (param == 18)
	{
		GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
		if (beachNodTurret2Obj && !this->killed && !this->turret2Killed)
		{
			Vector3 beachNodTurret2ObjPos = Commands->Get_Position(beachNodTurret2Obj);
			Commands->Create_Sound("CnC_Healer_Sound", beachNodTurret2ObjPos, obj);

			float turretMaxHealth = Commands->Get_Max_Health(beachNodTurret2Obj);
			Commands->Set_Health(beachNodTurret2Obj, turretMaxHealth);
		}

		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(Vector3(-91.1f, 106.562f, 1.7f), 1.0f, 0.25f);
		params.MovePathfind = false;

		Commands->Action_Goto(obj, params);
	}
	else if (param == 217)
	{
		GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
		GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);

		if (sender == beachNodTurret1Obj)
		{
			this->turret1Killed = true;
		}

		if (sender == beachNodTurret2Obj)
		{
			this->turret2Killed = true;
		}
	}
}

void M01_TurretBeach_Engineer_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (enemy == starObj && !this->goingToRepairTurret)
	{
		if (obj)
		{
			Commands->Action_Reset(obj, 100.0f);
			Commands->Innate_Enable(obj);

			Commands->Innate_Force_State_Enemy_Seen(obj, starObj);
		}
	}
}

// TODO
void M01_TurretBeach_Engineer_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
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

				Commands->Send_Custom_Event(obj, obj, 0, 17, 1.5f);
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

			int randAnimationIndex = Commands->Get_Random_Int(0, 5);

			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 60.0f, 49);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 40)
		{
			this->goingToRepairTurret = false;
		}
	}
}

ScriptRegistrant<M01_TurretBeach_Engineer_JDG> M01_TurretBeach_Engineer_JDGRegistrant("M01_TurretBeach_Engineer_JDG", "");