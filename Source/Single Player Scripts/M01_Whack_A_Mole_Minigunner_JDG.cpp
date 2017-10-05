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
#include "M01_Whack_A_Mole_Minigunner_JDG.h"

void M01_Whack_A_Mole_Minigunner_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->preventDeath, sizeof(this->preventDeath), 1);
	Auto_Save_Variable(&this->wasDamagedByStar, sizeof(this->wasDamagedByStar), 2);
	Auto_Save_Variable(&this->maxHealth, sizeof(this->maxHealth), 3);
	Auto_Save_Variable(&this->movementSpeed, sizeof(this->movementSpeed), 4);
	Auto_Save_Variable(&this->points, sizeof(this->points), 5);
	Auto_Save_Variable(&this->isFollowingRightWayPath, sizeof(this->isFollowingRightWayPath), 6);
}

// When M01_Mission_Controller_JDG receives custom type 0 with param 203
void M01_Whack_A_Mole_Minigunner_JDG::Created(GameObject *obj)
{
	this->preventDeath = false;
	this->wasDamagedByStar = false;
	this->maxHealth = Commands->Get_Max_Health(obj);
	this->movementSpeed = 0.3f;
	this->points = 1.0f;

	ActionParamsStruct params;
	params.Set_Basic(this, 45.0f, 38);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), this->movementSpeed, 0.25f);
	params.WaypathID = 101221;

	Commands->Action_Goto(obj, params);

	this->isFollowingRightWayPath = 1;
}

void M01_Whack_A_Mole_Minigunner_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager && damager == Commands->Get_A_Star(pos) && this->preventDeath)
	{
		this->wasDamagedByStar = true;

		Commands->Set_Health(obj, this->maxHealth);

		this->points *= 2.0f;

		this->movementSpeed += 0.1f;
		if (this->movementSpeed > 1.0f)
		{
			this->movementSpeed = 1.0f;
		}

		Commands->Give_Points(obj, this->points, false);
		Commands->Send_Custom_Event(obj, obj, 0, 46, 0.0f);
	}
}

void M01_Whack_A_Mole_Minigunner_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_mission_Controller_JDG when param 204 is received
	if (param == 204)
	{
		if (obj) 
		{
			this->preventDeath = false;
			Commands->Innate_Enable(obj);
		}
	}

	// Received from ourselves when damaged by star
	else if (param == 46)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 46);
		params.Set_Animation("H_A_J21C", false);

		Commands->Action_Play_Animation(obj, params);
	}

	// Received by ourselves when action with id 46 is complete
	else if (param == 67)
	{
		Commands->Action_Reset(obj, 100.0f);

		if (this->isFollowingRightWayPath == 1)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 39);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), this->movementSpeed, 0.25f);
			params.WaypathID = 101210;

			Commands->Action_Goto(obj, params);

			this->isFollowingRightWayPath = 0;
		}
		else if (!this->isFollowingRightWayPath)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), this->movementSpeed, 0.25f);
			params.WaypathID = 101221; // right

			Commands->Action_Goto(obj, params);

			this->isFollowingRightWayPath = 1;
		}
	}

	// Received from ourselves when action with id 39 is complete
	else if (param == 65)
	{
		if (obj)
		{
			this->preventDeath = false;

			Commands->Innate_Enable(obj);

			Commands->Grant_Key(obj, 1, true);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 65);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Movement(starObj, 0.3f, 5.0f);
			params.Set_Attack(starObj, 15.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

void M01_Whack_A_Mole_Minigunner_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		// When done with animation, see param 46
		if (action_id == 46)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 67, 0.0f);
		}

		// When done following waypath, see created, param 67 or action id 39
		else if (action_id == 38)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 39);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), this->movementSpeed, 0.25f);
			params.WaypathID = 101210;

			Commands->Action_Goto(obj, params);

			this->isFollowingRightWayPath = 0;
		}

		// When done following waypath, see param 67 or action id 38
		else if (action_id == 39)
		{
			if (this->wasDamagedByStar)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 65, 0.0f);
			}
			else
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 45.0f, 38);
				params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), this->movementSpeed, 0.25f);
				params.WaypathID = 101221;

				Commands->Action_Goto(obj, params);

				this->isFollowingRightWayPath = 1;
			}
		}
	}
}

ScriptRegistrant<M01_Whack_A_Mole_Minigunner_JDG> M01_Whack_A_Mole_Minigunner_JDGRegistrant("M01_Whack_A_Mole_Minigunner_JDG", "");