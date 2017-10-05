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
#include "M01_Comm_Repair_Engineer_JDG.h"

/*
M01 -> 101949
*/
void M01_Comm_Repair_Engineer_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->healSoundId, sizeof(this->healSoundId), 1);
	Auto_Save_Variable(&this->repairingComm, sizeof(this->repairingComm), 2);
}

void M01_Comm_Repair_Engineer_JDG::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);

	this->repairingComm = false;
}

void M01_Comm_Repair_Engineer_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_SOUND_ENDED)
	{
		if (param == this->healSoundId)
		{
			this->repairingComm = 0;
		}
	}
	else if (!type)
	{
		// Received from M01_mission_Controller_JDG when param 26 is received
		if (param == 189 && !this->repairingComm)
		{
			this->repairingComm = true;

			Commands->Enable_Hibernation(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			params.Set_Attack(Vector3(-301.38199f, 551.36603f, 18.205f), 30.0f, 0.0f, false);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);

			Commands->Start_Timer(obj, this, 2.0f, 16);
		}
	}
}

void M01_Comm_Repair_Engineer_JDG::Timer_Expired(GameObject *obj, int number)
{
	// After 2 seconds when param 189 is received
	if (number == 16)
	{
		Commands->Enable_Hibernation(obj, true);
		Commands->Action_Reset(obj, 100.0f);

		GameObject *nodCommCenterBuildingObj = Commands->Find_Object(153910);
		float commMaxHealth = Commands->Get_Max_Health(nodCommCenterBuildingObj);
		float commMaxShieldStrength = Commands->Get_Max_Shield_Strength(nodCommCenterBuildingObj);

		if (Commands->Get_Health(nodCommCenterBuildingObj) > 0.0f)
		{
			Commands->Set_Health(nodCommCenterBuildingObj, commMaxHealth);
			Commands->Set_Shield_Strength(nodCommCenterBuildingObj, commMaxShieldStrength);

			this->healSoundId = Commands->Create_Sound("CnC_Healer_Sound", Vector3(-297.633f, 532.144f, 35.458f), obj);
			Commands->Monitor_Sound(obj, this->healSoundId);
		}
	}
}

ScriptRegistrant<M01_Comm_Repair_Engineer_JDG> M01_Comm_Repair_Engineer_JDGRegistrant("M01_Comm_Repair_Engineer_JDG", "");