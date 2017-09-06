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
#include "MX0_Gun_Emplacement_DLS.h"

void MX0_Gun_Emplacement_DLS::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->attackingRightPoint, sizeof(this->attackingRightPoint), 1);
}

// On level start
void MX0_Gun_Emplacement_DLS::Created(GameObject *obj)
{
	this->attackingRightPoint = true;
}

void MX0_Gun_Emplacement_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 445002)
	{
		Timer_Expired(obj, 0);
	}
}

void MX0_Gun_Emplacement_DLS::Timer_Expired(GameObject *obj, int number)
{
	// Triggered when custom type 445002 is received or 4 seconds after this block
	if (!number)
	{
		int point;
		if (this->attackingRightPoint)
		{
			this->attackingRightPoint = false;

			point = Get_Int_Parameter("Left_Point");
		}
		else
		{
			this->attackingRightPoint = true;

			point = Get_Int_Parameter("Right_Point");
		}

		GameObject *pointObj = Commands->Find_Object(point);

		ActionParamsStruct params;
		params.Set_Basic(this, 95.0f, 10);
		params.Set_Attack(pointObj, 0.0f, 0.0f, true);
		params.AttackCheckBlocked = false;
		params.AttackForceFire = true;

		Commands->Action_Attack(obj, params);

		Commands->Start_Timer(obj, this, 4.0f, 0);
	}
}

ScriptRegistrant<MX0_Gun_Emplacement_DLS> MX0_Gun_Emplacement_DLSRegistrant("MX0_Gun_Emplacement_DLS", "Left_Point=0:int, Right_Point=0:int");