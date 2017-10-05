/*
* A Command & Conquer: Renegade SSGM Plugin, containing all the single player mission scripts
* Copyright(C) 2017  Neijwiert
*
* This program is free software : you can redistribute it and / or modifyM01_Hon_WarroomC
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
#include "M01_HON_WarroomController_JDG.h"

/*
M01 -> 124044
*/
void M01_HON_WarroomController_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->honChinookSpawnedSoldier1GDIObjId, sizeof(this->honChinookSpawnedSoldier1GDIObjId), 1);
	Auto_Save_Variable(&this->honChinookSpawnedSoldier2GDIObjId, sizeof(this->honChinookSpawnedSoldier2GDIObjId), 2);
	Auto_Save_Variable(&this->starInWarRoom, sizeof(this->starInWarRoom), 3);
	Auto_Save_Variable(&this->field_25, sizeof(this->field_25), 4);
	Auto_Save_Variable(&this->chinookSpawnedGDISoldierDeathCount, sizeof(this->chinookSpawnedGDISoldierDeathCount), 5);
}

void M01_HON_WarroomController_JDG::Created(GameObject *obj)
{
	this->honChinookSpawnedSoldier1GDIObjId = 0;
	this->honChinookSpawnedSoldier2GDIObjId = 0;
	this->starInWarRoom = false;
	this->field_25 = false;
	this->chinookSpawnedGDISoldierDeathCount = 0;
}

void M01_HON_WarroomController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (obj)
		{
			// Received from M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG when created
			if (param == 12)
			{
				this->honChinookSpawnedSoldier1GDIObjId = Commands->Get_ID(sender);
			}

			// Received from M01_HON_Chinook_Spawned_Soldier_02_GDI_JDG when created
			else if (param == 13)
			{
				this->honChinookSpawnedSoldier2GDIObjId = Commands->Get_ID(sender);
			}

			// Received from M01_Havoc_In_WarroomZone_JDG when entered
			else if (param == 139)
			{
				this->starInWarRoom = true;
			}

			// Received from M01_Havoc_Out_WarroomZone_JDG when entered
			else if (param == 140)
			{
				this->starInWarRoom = false;
			}

			// Received from M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG or M01_HON_Chinook_Spawned_Soldier_02_GDI_JDG when killed
			else if (param == 22)
			{
				if (++this->chinookSpawnedGDISoldierDeathCount == 2)
				{
					this->chinookSpawnedGDISoldierDeathCount = 0;

					GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-147.1f, 558.8f, 4.4f));
					Commands->Set_Facing(invisObj, -130.0f);
					Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_GDI_HON_TroopDrop01a.txt");
				}
			}
		}
	}
}

void M01_HON_WarroomController_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *honChinookSpawnedSoldier1GDIObj = Commands->Find_Object(this->honChinookSpawnedSoldier1GDIObjId);
	GameObject *honChinookSpawnedSoldier2GDIObj = Commands->Find_Object(this->honChinookSpawnedSoldier2GDIObjId);
	if (enterer == honChinookSpawnedSoldier1GDIObj)
	{
		if (!this->starInWarRoom)
		{
			Commands->Apply_Damage(honChinookSpawnedSoldier1GDIObj, 10000.0f, "STEEL", NULL);
		}
		else
		{
			if (honChinookSpawnedSoldier1GDIObj)
			{
				Commands->Send_Custom_Event(obj, honChinookSpawnedSoldier1GDIObj, 0, 16, 0.0f);
			}
		}
	}
	else if (enterer == honChinookSpawnedSoldier2GDIObj)
	{
		if (!this->starInWarRoom)
		{
			Commands->Apply_Damage(honChinookSpawnedSoldier2GDIObj, 10000.0f, "STEEL", NULL);
		}
		else
		{
			if (honChinookSpawnedSoldier2GDIObj)
			{
				Commands->Send_Custom_Event(obj, honChinookSpawnedSoldier2GDIObj, 0, 16, 0.0f);
			}
		}
	}
}

ScriptRegistrant<M01_HON_WarroomController_JDG> M01_HON_WarroomController_JDGRegistrant("M01_HON_WarroomController_JDG", "");