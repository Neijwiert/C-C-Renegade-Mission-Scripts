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
#include "M00_Nod_Obelisk.h"

/*
M02 -> 1153845
M07 -> 168926
M10 -> 1153938
*/
void M00_Nod_Obelisk::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->weaponObjId, sizeof(this->weaponObjId), 1);
}

void M00_Nod_Obelisk::Created(GameObject *obj)
{
	this->weaponObjId = 0;

	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += 45.0f;                             
	
	GameObject *weaponObj = Commands->Create_Object("Nod_Obelisk", pos);
	if (weaponObj)
	{
		Commands->Attach_Script(weaponObj, "M00_Obelisk_Weapon", "");

		Commands->Start_Timer(obj, this, 1.0f, 1);

		this->weaponObjId = Commands->Get_ID(weaponObj);
		
		int controllerId = Get_Int_Parameter("Controller_ID");

		GameObject *controllerObj = Commands->Find_Object(controllerId);
		Commands->Send_Custom_Event(obj, controllerObj, 9033, this->weaponObjId, 4.0f);
	}
}

// TODO
void M00_Nod_Obelisk::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 1 && param == 1)
	{
		GameObject *weaponObj = Commands->Find_Object(this->weaponObjId);
		if (weaponObj)
		{
			Commands->Destroy_Object(weaponObj);
		}
	}
}

void M00_Nod_Obelisk::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 1 second on creation or after 1 second after this block
	if (number == 1)
	{
		if (Commands->Get_Health(obj) != 0.0f && Commands->Get_Building_Power(obj))
		{
			GameObject *weaponObj = Commands->Find_Object(this->weaponObjId);
			if (weaponObj)
			{
				Commands->Send_Custom_Event(obj, weaponObj, 1, 1, 0.0f);
			}
		}
		else
		{
			GameObject *weaponObj = Commands->Find_Object(this->weaponObjId);
			if (weaponObj)
			{
				Commands->Send_Custom_Event(obj, weaponObj, 1, 0, 0.0f);
			}
		}

		Commands->Start_Timer(obj, this, 1.0f, 1);
	}
}

ScriptRegistrant<M00_Nod_Obelisk> M00_Nod_ObeliskRegistrant("M00_Nod_Obelisk", "Controller_ID=0:int");