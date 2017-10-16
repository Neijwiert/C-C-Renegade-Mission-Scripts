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
#include "M03_Ambient_Birdcall_Controller_JDG.h"

/*
M03 -> 400350
*/
void M03_Ambient_Birdcall_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->param1, sizeof(this->param1), 1);
	Auto_Save_Variable(&this->param2, sizeof(this->param2), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->param3, sizeof(this->param3), 4);
}

void M03_Ambient_Birdcall_Controller_JDG::Created(GameObject *obj)
{
	this->param1 = 100;
	this->param2 = 101;
	this->param3 = 102;

	if (obj)
	{
		Commands->Send_Custom_Event(obj, obj, 0, this->param1, 0.0f);
	}
}

void M03_Ambient_Birdcall_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received by ourselves after 0 seconds on create
	if (param == this->param1)
	{
		if (obj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, this->param3, 0.0f);
		}
	}

	// Received by ourselves after 0 seconds, see param1 or after 0 to 15 seconds after this block
	else if (param == this->param3)
	{
		GameObject *starObj = Commands->Get_A_Star(Vector3(0.0f, 0.0f, 0.0f));
		if (starObj)
		{
			Vector3 starPos = Commands->Get_Position(starObj);

			float randXOffset = Commands->Get_Random(-15.0f, 15.0f);
			float randYOffset = Commands->Get_Random(-15.0f, 15.0f);
			float randZOffset = Commands->Get_Random(5.0f, 15.0f);

			starPos.X += randXOffset;
			starPos.Y += randYOffset;
			starPos.Z += randZOffset;

			Commands->Create_Sound("Birdcall Twiddler", starPos, obj);
		}

		float randDelay = Commands->Get_Random(0.0f, 15.0f);
		if (obj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, this->param3, randDelay);
		}
	}
}

void M03_Ambient_Birdcall_Controller_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M03_Ambient_Birdcall_Controller_JDG> M03_Ambient_Birdcall_Controller_JDGRegistrant("M03_Ambient_Birdcall_Controller_JDG", "");