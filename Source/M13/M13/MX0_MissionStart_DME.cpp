/*
 * A Command & Conquer: Renegade SSGM Plugin, containing all the M13 (or M00) mission scripts
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
#include "MX0_MissionStart_DME.h"

void MX0_MissionStart_DME::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 4);
	Auto_Save_Variable(&this->sniper1ObjId, sizeof(this->sniper1ObjId), 5);
	Auto_Save_Variable(&this->sniper2ObjId, sizeof(this->sniper2ObjId), 6);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 7);
	Auto_Save_Variable(&this->field_38, sizeof(this->field_38), 8);
}

void MX0_MissionStart_DME::Created(GameObject *obj)
{
	// Initialize member variables
	this->field_34 = 0;
	this->sniper2ObjId = 0;
	this->sniper1ObjId = 0;
	this->field_28 = 0;
	this->field_24 = 0;
	this->field_20 = 0;
	this->field_1C = 0;
	this->field_38 = 0;

	// Deselect weapon of the star
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);
	Commands->Select_Weapon(starObj, NULL);

	// Fade into intro music
	Commands->Fade_Background_Music("renegade_intro_no_vox.mp3", 0, 1);

	// Start intro cinematic
	GameObject *cinObj = Commands->Create_Object("Invisible_Object", Vector3(0, 0, 0));
	Commands->Attach_Script(cinObj, "Test_Cinematic", "X00_Intro.txt");
}

void MX0_MissionStart_DME::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{

}

void MX0_MissionStart_DME::Timer_Expired(GameObject *obj, int number)
{

}

void MX0_MissionStart_DME::Custom(GameObject *obj, int type, int param, GameObject *sender)
{

}