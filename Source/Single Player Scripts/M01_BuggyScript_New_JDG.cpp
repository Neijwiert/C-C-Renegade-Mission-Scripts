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
#include "M01_BuggyScript_New_JDG.h"

void M01_BuggyScript_New_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

// When M01_BuggyNew_Controller_JDG receives custom type any with param 16
void M01_BuggyScript_New_JDG::Created(GameObject *obj)
{
	Commands->Innate_Soldier_Enable_Enemy_Seen(obj, true);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 38);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 5.0f);
	params.WaypathID = 103289;
	params.WaypointStartID = 103290;
	params.AttackActive = false;

	Commands->Action_Attack(obj, params);

	Commands->Send_Custom_Event(obj, obj, 0, 27, 2.0f);
	
	float halfMaxHealth = Commands->Get_Max_Health(obj) * 0.5f;
	Commands->Set_Health(obj, halfMaxHealth);

	float halfMaxShieldStrength = Commands->Get_Max_Shield_Strength(obj) * 0.5f;
	Commands->Set_Shield_Strength(obj, halfMaxShieldStrength);
}

void M01_BuggyScript_New_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Sound("EVA_Nod_Unit_Destroyed", Vector3(0.0f, 0.0f, 0.0f), obj);

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-217.0f, 499.4f, 4.0f));
	Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GDIPowerUpDrop.txt");
}

// TODO
void M01_BuggyScript_New_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_BuggyNew_Controller_JDG when param 27 is received with param = TODO
	if (type == 28)
	{
		this->field_1C = param;

		Commands->Send_Custom_Event(obj, obj, 0, 30, 0.0f);
	}

	// Received from M01_BuggyNew_Controller_JDG when param 28 is received with param = TODO
	else if (type == 29)
	{
		this->field_20 = param;
		
		Commands->Send_Custom_Event(obj, obj, 0, 30, 0.0f);
	}

	// Received from ourselves after 2 seconds on create
	else if (param == 27)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 5.0f);
		params.WaypathID = 103289;

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 50.0f, 2.0f, true);
		
		Commands->Modify_Action(obj, 38, params, false, true);
	}

	// Received from ourselves after param 28 or 29
	// Received from ourselves after 1 second when action with id 38 is complete 
	else if (param == 30)
	{
		GameObject *field1CObj = Commands->Find_Object(this->field_1C);
		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		if (field1CObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 5.0f);
			params.WaypathID = 103289;
			params.Set_Attack(field1CObj, 50.0f, 2.0f, true);

			Commands->Modify_Action(obj, 38, params, false, true);
		}
		else if (field20Obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 5.0f);
			params.WaypathID = 103289;
			params.Set_Attack(field20Obj, 50.0f, 2.0f, true);

			Commands->Modify_Action(obj, 38, params, false, true);
		}
		else
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 5.0f);
			params.WaypathID = 103289;

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 50.0f, 2.0f, true);

			Commands->Modify_Action(obj, 38, params, false, true);
		}
	}
}

// TODO
void M01_BuggyScript_New_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 38)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 5.0f);;
		params.WaypathID = 103289;
		params.AttackActive = false;

		Commands->Action_Attack(obj, params);

		Commands->Send_Custom_Event(obj, obj, 0, 30, 1.0f);
	}
}

ScriptRegistrant<M01_BuggyScript_New_JDG> M01_BuggyScript_New_JDGRegistrant("M01_BuggyScript_New_JDG", "");