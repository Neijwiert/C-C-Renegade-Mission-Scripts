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
#include "M01_HON_Engineer02_JDG.h"

/*
M01 -> 101948
*/
void M01_HON_Engineer02_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->doingHealSound, sizeof(this->doingHealSound), 1);
}

void M01_HON_Engineer02_JDG::Created(GameObject *obj)
{
	this->doingHealSound = false;

	Commands->Set_Innate_Is_Stationary(obj, true);
}

void M01_HON_Engineer02_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_mission_Controller_JDG when param 26 is received
	if (param == 189)
	{
		Commands->Enable_Hibernation(obj, false);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 16);
		params.Set_Attack(Vector3(-178.925f, 542.94299f, 4.9320002f), 10.0f, 0.0f, false);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);

		Commands->Start_Timer(obj, this, 2.0f, 16);
	}
}

void M01_HON_Engineer02_JDG::Timer_Expired(GameObject *obj, int number)
{
	// After 16 seconds, see param 189
	if (number == 16)
	{
		Commands->Enable_Hibernation(obj, true);
		Commands->Action_Reset(obj, 100.0f);

		GameObject *nodHonBuildingObj = Commands->Find_Object(153909);
		float maxHealth = Commands->Get_Max_Health(nodHonBuildingObj);
		float maxShieldStrength = Commands->Get_Max_Shield_Strength(nodHonBuildingObj);

		if (Commands->Get_Health(nodHonBuildingObj) > 0.0f)
		{
			Commands->Set_Health(nodHonBuildingObj, maxHealth);
			Commands->Set_Shield_Strength(nodHonBuildingObj, maxShieldStrength);

			if (!this->doingHealSound)
			{
				Commands->Create_Sound("CnC_Healer_Sound", Vector3(-175.074f, 533.086f, 19.327f), obj);

				this->doingHealSound = true;

				Commands->Start_Timer(obj, this, 5.0f, 24);
			}
		}
	}

	// After 5 seconds, see timer number 16
	else if (number == 24)
	{
		this->doingHealSound = false;
	}
}

ScriptRegistrant<M01_HON_Engineer02_JDG> M01_HON_Engineer02_JDGRegistrant("M01_HON_Engineer02_JDG", "");