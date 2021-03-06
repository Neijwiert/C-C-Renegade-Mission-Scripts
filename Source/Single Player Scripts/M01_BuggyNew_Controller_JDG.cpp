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
#include "M01_BuggyNew_Controller_JDG.h"

/*
M01 -> 103367
*/
void M01_BuggyNew_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->nodBuggyObjId, sizeof(this->nodBuggyObjId), 1);
	Auto_Save_Variable(&this->gdiSoldier3MovedToUs, sizeof(this->gdiSoldier3MovedToUs), 2);
	Auto_Save_Variable(&this->gdiSoldier4MovedToUs, sizeof(this->gdiSoldier4MovedToUs), 3);
}

void M01_BuggyNew_Controller_JDG::Created(GameObject *obj)
{
	this->gdiSoldier3MovedToUs = false;
	this->gdiSoldier4MovedToUs = false;
}

void M01_BuggyNew_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Announce_Hand_of_Nod_Zone when entered 
	if (param == 16)
	{
		GameObject *nodBuggyObj = Commands->Create_Object("Nod_Buggy", Vector3(-233.946f, 494.424f, 5.234f));
		Commands->Set_Facing(nodBuggyObj, -135.0f);
		Commands->Attach_Script(nodBuggyObj, "M01_BuggyScript_New_JDG", "");

		this->nodBuggyObjId = Commands->Get_ID(nodBuggyObj);
	}

	// Received from M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG when action with id 38 is complete
	else if (param == 27)
	{
		this->gdiSoldier3MovedToUs = true;
		
		int honChinookSpawnedSoldier3GDIObjId = Commands->Get_ID(sender);
		GameObject *nodBuggyObj = Commands->Find_Object(this->nodBuggyObjId);
		if (nodBuggyObj)
		{
			Commands->Send_Custom_Event(obj, nodBuggyObj, 28, honChinookSpawnedSoldier3GDIObjId, 0.0f);
			Commands->Send_Custom_Event(obj, sender, 27, this->nodBuggyObjId, 0.0f);
		}
	}

	// Received from M01_HON_Chinook_Spawned_Soldier_04_GDI_JDG when action with id 38 is complete
	else if (param == 28)
	{
		this->gdiSoldier4MovedToUs = true;

		int honChinookSpawnedSoldier4GDIObjId = Commands->Get_ID(sender);
		GameObject *nodBuggyObj = Commands->Find_Object(this->nodBuggyObjId);
		if (nodBuggyObj)
		{
			Commands->Send_Custom_Event(obj, nodBuggyObj, 29, honChinookSpawnedSoldier4GDIObjId, 0.0f);
			Commands->Send_Custom_Event(obj, sender, 27, this->nodBuggyObjId, 0.0f);
		}
	}
}

ScriptRegistrant<M01_BuggyNew_Controller_JDG> M01_BuggyNew_Controller_JDGRegistrant("M01_BuggyNew_Controller_JDG", "");