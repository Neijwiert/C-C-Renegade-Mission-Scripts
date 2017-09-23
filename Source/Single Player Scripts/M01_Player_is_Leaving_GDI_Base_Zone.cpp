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
#include "M01_Player_is_Leaving_GDI_Base_Zone.h"

/*
M01 -> 101465
*/
void M01_Player_is_Leaving_GDI_Base_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canSendCustomToGDIBaseSpawnerController, sizeof(this->canSendCustomToGDIBaseSpawnerController), 1);
}

void M01_Player_is_Leaving_GDI_Base_Zone::Created(GameObject *obj)
{
	this->canSendCustomToGDIBaseSpawnerController = true;
}

void M01_Player_is_Leaving_GDI_Base_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		if (this->canSendCustomToGDIBaseSpawnerController)
		{
			this->canSendCustomToGDIBaseSpawnerController = false;

			GameObject *M01GDIBaseSpawnerControllerJDGObj = Commands->Find_Object(102335);
			if (M01GDIBaseSpawnerControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseSpawnerControllerJDGObj, 0, 68, 0.0f);
			}
		}

		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 128, 0.0f);
	}
}

ScriptRegistrant<M01_Player_is_Leaving_GDI_Base_Zone> M01_Player_is_Leaving_GDI_Base_ZoneRegistrant("M01_Player_is_Leaving_GDI_Base_Zone", "");