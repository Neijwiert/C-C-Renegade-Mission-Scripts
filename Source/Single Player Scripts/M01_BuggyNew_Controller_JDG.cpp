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
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_21, sizeof(this->field_21), 3);
}

void M01_BuggyNew_Controller_JDG::Created(GameObject *obj)
{
	this->field_20 = false;
	this->field_21 = false;
}

// TODO
void M01_BuggyNew_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		GameObject *nodBuggyObj = Commands->Create_Object("Nod_Buggy", Vector3(-233.946f, 494.424f, 5.234f));
		Commands->Set_Facing(nodBuggyObj, -135.0f);
		Commands->Attach_Script(nodBuggyObj, "M01_BuggyScript_New_JDG", "");

		this->nodBuggyObjId = Commands->Get_ID(nodBuggyObj);
	}
	else if (param == 27)
	{
		this->field_20 = true;
		
		int senderId = Commands->Get_ID(sender);
		GameObject *nodBuggyObj = Commands->Find_Object(this->nodBuggyObjId);
		if (nodBuggyObj)
		{
			Commands->Send_Custom_Event(obj, nodBuggyObj, 28, senderId, 0.0f);
			Commands->Send_Custom_Event(obj, sender, 27, this->nodBuggyObjId, 0.0f);
		}
	}
	else if (param == 28)
	{
		this->field_21 = true;

		int senderId = Commands->Get_ID(sender);
		GameObject *nodBuggyObj = Commands->Find_Object(this->nodBuggyObjId);
		if (nodBuggyObj)
		{
			Commands->Send_Custom_Event(obj, nodBuggyObj, 29, senderId, 0.0f);
			Commands->Send_Custom_Event(obj, sender, 27, this->nodBuggyObjId, 0.0f);
		}
	}
}

ScriptRegistrant<M01_BuggyNew_Controller_JDG> M01_BuggyNew_Controller_JDGRegistrant("M01_BuggyNew_Controller_JDG", "");