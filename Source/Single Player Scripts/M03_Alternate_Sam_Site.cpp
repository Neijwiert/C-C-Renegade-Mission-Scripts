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
#include "M03_Alternate_Sam_Site.h"

/*
M03 -> 1100020
*/
void M03_Alternate_Sam_Site::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M03_Alternate_Sam_Site::Created(GameObject *obj)
{
	this->field_1C = false;
}

// Never receives a custom
void M03_Alternate_Sam_Site::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (Commands->Find_Object(1100003)) // GDI Gunboat
	{
		GameObject *paramObj = Commands->Find_Object(param);

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(5.0f, 5.0f, 5.0f));
		int invisObjId = Commands->Get_ID(invisObj);

		if (paramObj)
		{
			int chinookControllerId = Get_Int_Parameter("Chinook_Controller_ID");

			char buffer[48];
			sprintf(buffer, "%d,%d", chinookControllerId, invisObjId);
			Commands->Attach_Script(paramObj, "M03_Destroyed_Chinook", buffer);
		}

		Commands->Attach_To_Object_Bone(invisObj, paramObj, "ROTOR00");

		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 0);
		params.Set_Attack(invisObj, 0.0f, 0.0f, true);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);

		Commands->Start_Timer(obj, this, 1.0f, invisObjId);
	}
}

void M03_Alternate_Sam_Site::Timer_Expired(GameObject *obj, int number)
{
	GameObject *invisObj = Commands->Find_Object(number);
	Vector3 pos = Commands->Get_Position(obj);

	if (invisObj)
	{
		Vector3 invisObjPos = Commands->Get_Position(invisObj);
		if (Commands->Get_Distance(pos, invisObjPos) <= 60.0f)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 99.0f, 0);
			params.Set_Attack(invisObj, 200.0f, 0.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}

		Commands->Start_Timer(obj, this, 1.0f, number);
	}
}

ScriptRegistrant<M03_Alternate_Sam_Site> M03_Alternate_Sam_SiteRegistrant("M03_Alternate_Sam_Site", "Chinook_Controller_ID:int");