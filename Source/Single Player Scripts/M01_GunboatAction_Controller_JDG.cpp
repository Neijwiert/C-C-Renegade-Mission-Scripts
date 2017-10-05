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
#include "M01_GunboatAction_Controller_JDG.h"

/*
M01 -> 113325
*/
void M01_GunboatAction_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->engineerObjId, sizeof(this->engineerObjId), 2);
}

void M01_GunboatAction_Controller_JDG::Created(GameObject *obj)
{
	this->field_1C = false;
	this->engineerObjId = 101654;
}

// TODO
void M01_GunboatAction_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_PlayerEntering_BarnArea_Zone_JDG when entered
		if (param == 16)
		{
			if (!this->field_1C)
			{
				this->field_1C = true;

				GameObject *gdiGunBoatObj = Commands->Find_Object(101477);
				GameObject *engineerObj = Commands->Find_Object(this->engineerObjId);

				if (gdiGunBoatObj)
				{
					Commands->Send_Custom_Event(obj, gdiGunBoatObj, 0, 16, 0.0f);
				}

				if (engineerObj)
				{
					Commands->Send_Custom_Event(obj, engineerObj, 0, 16, 0.0f);
				}
			}
		}

		// Received from M01_PlayerLeaving_BarnArea_Zone_JDG when entered
		else if (param == 24)
		{
			if (this->field_1C)
			{
				this->field_1C = false;

				GameObject *gdiGunBoatObj = Commands->Find_Object(101477);
				GameObject *engineerObj = Commands->Find_Object(this->engineerObjId);

				if (gdiGunBoatObj)
				{
					Commands->Send_Custom_Event(obj, gdiGunBoatObj, 0, 24, 0.0f);
				}

				if (engineerObj)
				{
					Commands->Send_Custom_Event(obj, engineerObj, 0, 24, 0.0f);
				}
			}
		}

		// Received from M01_TurretBeach_Chinook_Spawned_Soldier_NOD when created
		else if (param == 14)
		{
			this->engineerObjId = Commands->Get_ID(sender);
		}
	}
}

ScriptRegistrant<M01_GunboatAction_Controller_JDG> M01_GunboatAction_Controller_JDGRegistrant("M01_GunboatAction_Controller_JDG", "");