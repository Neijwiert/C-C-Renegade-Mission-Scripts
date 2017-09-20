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
#include "MX0_Obelisk_Weapon_DLS.h"

/*
M13 -> 1500020
*/
void MX0_Obelisk_Weapon_DLS::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canDoWarmupAnimation, sizeof(this->canDoWarmupAnimation), 1);
	Auto_Save_Variable(&this->currentTargetObjId, sizeof(this->currentTargetObjId), 2);
	Auto_Save_Variable(&this->obeliskEffectObjId, sizeof(this->obeliskEffectObjId), 3);
	Auto_Save_Variable(&this->humveeObjId, sizeof(this->humveeObjId), 4);
	Auto_Save_Variable(&this->gdiMinigunner10FFObjId, sizeof(this->gdiMinigunner10FFObjId), 5);
	Auto_Save_Variable(&this->medTankObjId, sizeof(this->medTankObjId), 6);
}

void MX0_Obelisk_Weapon_DLS::Created(GameObject *obj)
{
	this->obeliskEffectObjId = 0;
	this->currentTargetObjId = 0;
	this->canDoWarmupAnimation = true;

	Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_NOD);
	Commands->Set_Is_Rendered(obj, false);
	Commands->Enable_Hibernation(obj, false);

	Vector3 pos = Commands->Get_Position(obj);

	GameObject *obeliskEffectObj = Commands->Create_Object("Obelisk Effect", pos);
	if (obeliskEffectObj)
	{
		this->obeliskEffectObjId = Commands->Get_ID(obeliskEffectObj);
		Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
	}
}

void MX0_Obelisk_Weapon_DLS::Destroyed(GameObject *obj)
{
	if (this->obeliskEffectObjId)
	{
		GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
		if (obeliskEffectObj)
		{
			Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
		}
	}
}

void MX0_Obelisk_Weapon_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from MX0_Area4_Controller_DLS when timer number 1 triggers
	if (type == 445003)
	{
		Commands->Send_Custom_Event(obj, obj, 2, this->humveeObjId, 0.0f);
	}

	// Received from MX0_GDI_Killed_DLS when killed
	if (type == 445004)
	{
		Commands->Send_Custom_Event(obj, obj, 2, this->gdiMinigunner10FFObjId, 1.0f);
	}

	// TODO (No custom)
	if (type == 445005)
	{
		Commands->Send_Custom_Event(obj, obj, 2, this->medTankObjId, 0.0f);
	}

	// Received from various M00_Send_Object_ID
	else if (type == 9035)
	{
		switch (param)
		{
			case 1:
				this->humveeObjId = Commands->Get_ID(sender);
				break;
			case 2:
				this->gdiMinigunner10FFObjId = Commands->Get_ID(sender);
				break;
			case 3:
				this->medTankObjId = Commands->Get_ID(sender);
				break;
			default:
				break;
		}
	}

	// TODO (No custom)
	else if (type == 1)
	{
		if (param)
		{
			Commands->Enable_Enemy_Seen(obj, true);

			this->canDoWarmupAnimation = true;
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj, false);

			this->canDoWarmupAnimation = false;

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 0);
			params.Set_Attack(obj, 0.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
			Commands->Action_Reset(obj, 100.0f);

			if (this->obeliskEffectObjId)
			{
				GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
				if (obeliskEffectObj)
				{
					Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
				}
			}
		}
	}

	// Reveived from ourselves after custom type 445003/445004/445005
	else if (type == 2)
	{
		if (this->canDoWarmupAnimation)
		{
			GameObject *paramObj = Commands->Find_Object(param);
			if (paramObj)
			{
				Vector3 paramObjPos = Commands->Get_Position(paramObj);
				Vector3 pos = Commands->Get_Position(obj);

				float distance = Commands->Get_Distance(pos, paramObjPos);

				paramObjPos.Z = 0.0f;
				pos.Z = 0.0f;

				float distance2 = Commands->Get_Distance(pos, paramObjPos);
				float maxRange = Get_Float_Parameter("Max_Range");

				if(distance2 <= 15.0f || distance >= maxRange)
				{
					if (this->obeliskEffectObjId)
					{
						GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
						if (obeliskEffectObj)
						{
							Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
						}
					}

					Commands->Action_Reset(obj, 100.0f);
				}
				else
				{
					this->currentTargetObjId = param;
					this->canDoWarmupAnimation = false;

					Commands->Start_Timer(obj, this, 2.5f, 1);

					GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
					if (obeliskEffectObj)
					{
						Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 1);
					}

					Vector3 pos = Commands->Get_Position(obj);
					pos.Z = -20.0f;

					Commands->Create_Sound("Obelisk_Warm_Up", pos, obj);
				}
			}
		}
	}
}

void MX0_Obelisk_Weapon_DLS::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (!action_id)
	{
		Commands->Debug_Message("Action_Complete Obelisk Firing");
	}
}

void MX0_Obelisk_Weapon_DLS::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 2.5 seconds after custom type 2 (preparing to attack something)
	if (number == 1)
	{
		GameObject *currentTargetObj = Commands->Find_Object(this->currentTargetObjId);

		Vector3 pos = Commands->Get_Position(obj);
		Vector3 currentTargetObjPos = Commands->Get_Position(currentTargetObj);

		float distance = Commands->Get_Distance(pos, currentTargetObjPos);

		pos.Z = 0.0f;
		currentTargetObjPos.Z = 0.0f;

		float distance2 = Commands->Get_Distance(pos, currentTargetObjPos);
		float maxRange = Get_Float_Parameter("Max_Range");

		if (currentTargetObj && distance2 > 15.0f && distance < maxRange)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 0);
			params.Set_Attack(currentTargetObj, maxRange, 0.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);

			this->currentTargetObjId = 0;

			Commands->Start_Timer(obj, this, 2.0f, 2);
		}
		else
		{
			if (this->obeliskEffectObjId)
			{
				GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
				if (obeliskEffectObj)
				{
					Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
				}
			}

			Commands->Action_Reset(obj, 100.0f);

			this->canDoWarmupAnimation = true;
		}
	}

	// Triggered 2 seconds after succesfully attacking something (timer number 1)
	else if (number == 2)
	{
		if (this->obeliskEffectObjId)
		{
			GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
			if (obeliskEffectObj)
			{
				Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
			}
		}

		this->canDoWarmupAnimation = true;
	}
}

ScriptRegistrant<MX0_Obelisk_Weapon_DLS> MX0_Obelisk_Weapon_DLSRegistrant("MX0_Obelisk_Weapon_DLS", "Max_Range=75.0f:float");