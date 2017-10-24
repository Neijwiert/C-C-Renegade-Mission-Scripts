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
#include "M10_Elevator_All_Controller.h"

/*
M03 -> 1148345
*/
void M10_Elevator_All_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 3);
	Auto_Save_Variable(&this->rubbleStub1ObjId, sizeof(this->rubbleStub1ObjId), 4);
	Auto_Save_Variable(&this->rubbleStub2ObjId, sizeof(this->rubbleStub2ObjId), 5);
	Auto_Save_Variable(&this->rubbleStub3ObjId, sizeof(this->rubbleStub3ObjId), 6);
	Auto_Save_Variable(&this->rubbleStub4ObjId, sizeof(this->rubbleStub4ObjId), 7);
	Auto_Save_Variable(&this->rubbleStub5ObjId, sizeof(this->rubbleStub5ObjId), 8);
	Auto_Save_Variable(&this->rubbleStub6ObjId, sizeof(this->rubbleStub6ObjId), 9);
	Auto_Save_Variable(&this->rubbleStub7ObjId, sizeof(this->rubbleStub7ObjId), 10);
	Auto_Save_Variable(&this->rubbleStub8ObjId, sizeof(this->rubbleStub8ObjId), 11);
}

void M10_Elevator_All_Controller::Created(GameObject *obj)
{
	this->field_1C = false;
	this->field_1D = false;
	this->rubbleStub4ObjId = 0;
	this->rubbleStub3ObjId = 0;
	this->rubbleStub2ObjId = 0;
	this->rubbleStub1ObjId = 0;
	this->rubbleStub8ObjId = 0;
	this->rubbleStub7ObjId = 0;
	this->rubbleStub6ObjId = 0;
	this->rubbleStub5ObjId = 0;

	Commands->Static_Anim_Phys_Goto_Last_Frame(1300881, "cave_lift.cave_lift");
}

void M10_Elevator_All_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M10_Elevator_All_Zone after 0 seconds when entered (id = 1100005 1148346)
	if (type == 40022)
	{
		Commands->Start_Timer(obj, this, 5.0f, 333);

		GameObject *rubbleStub1Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-117.32f, 83.098f, 0.656f));
		this->rubbleStub1ObjId = Commands->Get_ID(rubbleStub1Obj);
		Commands->Set_Facing(rubbleStub1Obj, 90.0f);
		Commands->Set_Is_Rendered(rubbleStub1Obj, false);

		GameObject *rubbleStub2Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-113.706f, 78.884f, 0.656f));
		this->rubbleStub2ObjId = Commands->Get_ID(rubbleStub2Obj);
		Commands->Set_Is_Rendered(rubbleStub2Obj, false);

		GameObject *rubbleStub3Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-117.624f, 73.787f, 0.656f));
		this->rubbleStub3ObjId = Commands->Get_ID(rubbleStub3Obj);
		Commands->Set_Is_Rendered(rubbleStub3Obj, false);
		Commands->Set_Facing(rubbleStub3Obj, -90.0f);

		GameObject *rubbleStub4Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-122.458f, 78.303f, 0.656f));
		this->rubbleStub4ObjId = Commands->Get_ID(rubbleStub4Obj);
		Commands->Set_Is_Rendered(rubbleStub4Obj, false);

		GameObject *rubbleStub5Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-117.32f, 83.098f, 3.906f));
		this->rubbleStub5ObjId = Commands->Get_ID(rubbleStub5Obj);
		Commands->Set_Facing(rubbleStub5Obj, 90.0f);
		Commands->Set_Is_Rendered(rubbleStub5Obj, false);

		GameObject *rubbleStub6Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-113.706f, 78.884f, 3.906f));
		this->rubbleStub6ObjId = Commands->Get_ID(rubbleStub6Obj);
		Commands->Set_Is_Rendered(rubbleStub6Obj, false);

		GameObject *rubbleStub7Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-117.624f, 73.787f, 3.906f));
		this->rubbleStub7ObjId = Commands->Get_ID(rubbleStub7Obj);
		Commands->Set_Is_Rendered(rubbleStub7Obj, false);
		Commands->Set_Facing(rubbleStub7Obj, -90.0f);

		GameObject *rubbleStub8Obj = Commands->Create_Object("M08_Rubble_Stub", Vector3(-122.458f, 78.303f, 3.906f));
		this->rubbleStub8ObjId = Commands->Get_ID(rubbleStub8Obj);
		Commands->Set_Is_Rendered(rubbleStub8Obj, false);

		Commands->Static_Anim_Phys_Goto_Frame(1300881, 0.0f, "cave_lift.cave_lift");
	}
}

void M10_Elevator_All_Controller::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 5 seconds after custom type 40022 is received
	if (number == 333)
	{
		GameObject *elevatorAllScriptZoneObj = Commands->Find_Object(1148346);
		Commands->Destroy_Object(elevatorAllScriptZoneObj);

		GameObject *rubbleStub1Obj = Commands->Find_Object(this->rubbleStub1ObjId);
		Commands->Destroy_Object(rubbleStub1Obj);

		GameObject *rubbleStub2Obj = Commands->Find_Object(this->rubbleStub2ObjId);
		Commands->Destroy_Object(rubbleStub2Obj);

		GameObject *rubbleStub3Obj = Commands->Find_Object(this->rubbleStub3ObjId);
		Commands->Destroy_Object(rubbleStub3Obj);

		GameObject *rubbleStub4Obj = Commands->Find_Object(this->rubbleStub4ObjId);
		Commands->Destroy_Object(rubbleStub4Obj);

		GameObject *rubbleStub5Obj = Commands->Find_Object(this->rubbleStub5ObjId);
		Commands->Destroy_Object(rubbleStub5Obj);

		GameObject *rubbleStub6Obj = Commands->Find_Object(this->rubbleStub6ObjId);
		Commands->Destroy_Object(rubbleStub6Obj);

		GameObject *rubbleStub7Obj = Commands->Find_Object(this->rubbleStub7ObjId);
		Commands->Destroy_Object(rubbleStub7Obj);

		GameObject *rubbleStub8Obj = Commands->Find_Object(this->rubbleStub8ObjId);
		Commands->Destroy_Object(rubbleStub8Obj);
	}
}

ScriptRegistrant<M10_Elevator_All_Controller> M10_Elevator_All_ControllerRegistrant("M10_Elevator_All_Controller", "");