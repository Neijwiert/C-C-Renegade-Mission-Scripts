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
#include "MX0_NOD_TroopDrop.h"

void MX0_NOD_TroopDrop::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->isZoneTriggered, sizeof(this->isZoneTriggered), 1);
}

// On level start
void MX0_NOD_TroopDrop::Created(GameObject *obj)
{
	this->isZoneTriggered = false;
}

void MX0_NOD_TroopDrop::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 3 seconds after entered
	if (number == 109)
	{
		GameObject *cinObj = Commands->Find_Object(1200018);
		Vector3 cinObjPos = Commands->Get_Position(cinObj);
		float cinObjFacing = Commands->Get_Facing(cinObj);

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", cinObjPos);
		Commands->Set_Facing(invisObj, cinObjFacing);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "MX0_C130Troopdrop.txt");

		GameObject *cinObj2 = Commands->Find_Object(1200020);
		Vector3 cinObj2Pos = Commands->Get_Position(cinObj2);
		float cinObj2Facing = Commands->Get_Facing(cinObj2);

		GameObject *invisObj2 = Commands->Create_Object("Invisible_Object", cinObj2Pos);
		Commands->Set_Facing(invisObj2, cinObj2Facing);
		Commands->Attach_Script(invisObj2, "Test_Cinematic", "MX0_C130Troopdrop2.txt");
	}
}

void MX0_NOD_TroopDrop::Entered(GameObject *obj, GameObject *enterer)
{
	if (!this->isZoneTriggered)
	{
		this->isZoneTriggered = true;

		GameObject *cinObj = Commands->Find_Object(1200018);
		Vector3 cinObjPos = Commands->Get_Position(cinObj);
		float cinObjFacing = Commands->Get_Facing(cinObj);

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", cinObjPos);
		Commands->Set_Facing(invisObj, cinObjFacing);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "MX0_C130Troopdrop.txt");

		GameObject *cinObj2 = Commands->Find_Object(1200020);
		Vector3 cinObj2Pos = Commands->Get_Position(cinObj2);
		float cinObj2Facing = Commands->Get_Facing(cinObj2);

		GameObject *invisObj2 = Commands->Create_Object("Invisible_Object", cinObj2Pos);
		Commands->Set_Facing(invisObj2, cinObj2Facing);
		Commands->Attach_Script(invisObj2, "Test_Cinematic", "MX0_C130Troopdrop2.txt");

		Commands->Start_Timer(obj, this, 3.0f, 109);

		GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
		Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 114, 0, 0.0f);
	}
}

ScriptRegistrant<MX0_NOD_TroopDrop> MX0_NOD_TroopDropRegistrant("MX0_NOD_TroopDrop", "");