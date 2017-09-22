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
#include "M01_Base_GDI_Grenadier_JDG.h"

/*
M01 -> 116383
*/
void M01_Base_GDI_Grenadier_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->health, sizeof(this->health), 1);
	Auto_Save_Variable(&this->preventDeath, sizeof(this->preventDeath), 2);
	Auto_Save_Variable(&this->field_21, sizeof(this->field_21), 3);
	Auto_Save_Variable(&this->canSeeEnemies, sizeof(this->canSeeEnemies), 4);
	Auto_Save_Variable(&this->field_23, sizeof(this->field_23), 5);
	Auto_Save_Variable(&this->attackedByNodMinigunners, sizeof(this->attackedByNodMinigunners), 6);
}

void M01_Base_GDI_Grenadier_JDG::Created(GameObject *obj)
{
	this->attackedByNodMinigunners = false;
	this->field_23 = true;
	this->preventDeath = true;
	this->field_21 = false;
	this->health = Commands->Get_Health(obj);

	Commands->Set_Innate_Is_Stationary(obj, true);

	Commands->Send_Custom_Event(obj, obj, 0, 27, 10.0f);
}

void M01_Base_GDI_Grenadier_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	GameObject *nodMinigunner1Obj = Commands->Find_Object(116388);
	GameObject *nodMinigunner2Obj = Commands->Find_Object(116387);
	if ((damager == nodMinigunner1Obj || damager == nodMinigunner2Obj) && !this->attackedByNodMinigunners)
	{
		this->attackedByNodMinigunners = true;

		Commands->Send_Custom_Event(obj, obj, 0, 41, 0.0f);
	}

	if (this->preventDeath)
	{
		if (Commands->Get_Health(obj) <= 10.0f)
		{
			Commands->Set_Health(obj, 10.0f);
		}
	}
	else if (damager)
	{
		if (damager == obj)
		{
			Commands->Set_Health(obj, this->health);
		}
		else
		{
			this->health = Commands->Get_Health(obj);
		}
	}
}

// TODO
void M01_Base_GDI_Grenadier_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 27)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(Vector3(38.873f, -42.18f, -0.432f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (param == 17)
		{
			GameObject *nodMinigunner3Obj = Commands->Find_Object(116384);
			if (nodMinigunner3Obj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 17);
				params.Set_Attack(nodMinigunner3Obj, 1000.0f, 0.0f, true);
				params.AttackCrouched = true;
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
			}
			else
			{
				Commands->Action_Reset(obj, 100.0f);
			}
		}
		else if (param == 16)
		{
			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 0.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);

			float randDelay = Commands->Get_Random(1.0f, 2.0f);
			Commands->Send_Custom_Event(obj, obj, 0, 28, randDelay);
		}
		else if (param == 28)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 46);
			params.Set_Animation("H_A_J19A", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (param == 29)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 40);
			params.Set_Movement(Vector3(26.767f, 18.529f, 3.139f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (param == 9)
		{
			Commands->Enable_Hibernation(obj, false);

			this->field_21 = true;

			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 9);
			params.Set_Movement(Vector3(41.3f, 27.4f, -0.2f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (param == 4001)
		{
			Commands->Action_Reset(obj, 100.0f);

			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Set_Innate_Is_Stationary(obj, false);

				Vector3 M01GDIBaseCommanderEvacControllerJDGObjPos = Commands->Get_Position(M01GDIBaseCommanderEvacControllerJDGObj);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 4001);
				params.Set_Movement(M01GDIBaseCommanderEvacControllerJDGObjPos, 0.8f, 1.0f);

				Commands->Action_Goto(obj, params);
			}
		}
		else if (param == 32)
		{
			if (!this->field_21)
			{
				this->field_23 = false;

				Commands->Action_Reset(obj, 100.0f);

				this->canSeeEnemies = true;

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 43);
				params.Set_Movement(Vector3(111.21f, 85.507f, 8.934f), 0.8f, 1.0f);
				params.AttackActive = false;

				Commands->Action_Attack(obj, params);
			}
		}
		else if (param == 22)
		{
			if (this->field_23)
			{
				this->preventDeath = false;

				Commands->Apply_Damage(obj, 100000.0f, "Steel", NULL);
			}
		}
		else if (param == 41)
		{
			GameObject *nodMinigunner2Obj = Commands->Find_Object(116387);
			if (nodMinigunner2Obj)
			{
				Commands->Set_Innate_Is_Stationary(obj, false);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 41);
				params.Set_Movement(Vector3(53.363f, 54.018f, -0.439f), 0.8f, 1.0f);
				params.Set_Attack(nodMinigunner2Obj, 20.0f, 0.0f, true);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
			}
			else
			{
				Commands->Set_Innate_Is_Stationary(obj, false);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 41);
				params.Set_Movement(Vector3(53.363f, 54.018f, -0.439f), 0.8f, 1.0f);

				Commands->Action_Goto(obj, params);
			}
		}
	}
}

void M01_Base_GDI_Grenadier_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (this->canSeeEnemies)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 43);
		params.Set_Movement(enemy, 0.3f, 5.0f);
		params.Set_Attack(enemy, 1000.0f, 0.0f, true);
		params.AttackActive = true;

		Commands->Modify_Action(obj, 43, params, true, true);
	}
}

// TODO
void M01_Base_GDI_Grenadier_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			params.Set_Attack(Vector3(28.449f, -23.325f, 3.032f), 1000.0f, 0.0f, true);
			params.AttackCrouched = true;
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
		else if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 47);
			params.Set_Animation("H_A_J19S", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 48);
			params.Set_Animation("H_A_J19C", false);
			
			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 9)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);

			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 27, 0.0f);
			}
		}
		else if (action_id == 4001)
		{
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.75f;

			static const char *drops[2] =
			{
				"POW_Health_100",
				"POW_Armor_100"
			};

			int randInt = Commands->Get_Random_Int(0, 2);
			Commands->Create_Object(drops[randInt], pos);

			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 10, 0.0f);
			}

		}
		else if (action_id == 40)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 41, 0.0f);
		}
		else if (action_id == 41)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 42);
			params.Set_Movement(Vector3(53.363f, 54.018f, -0.439f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (action_id == 42)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);
		}
		else if (action_id == 43)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 43);
			params.Set_Movement(Vector3(111.21f, 85.507f, 8.934f), 0.8f, 1.0f);
			params.AttackActive = false;
			
			Commands->Action_Attack(obj, params);
		}
	}
}

void M01_Base_GDI_Grenadier_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Base_GDI_Grenadier_JDG> M01_Base_GDI_Grenadier_JDGRegistrant("M01_Base_GDI_Grenadier_JDG", "");