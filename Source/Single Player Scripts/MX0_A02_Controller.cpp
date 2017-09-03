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
#include "MX0_A02_Controller.h"

void MX0_A02_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_40, sizeof(this->field_40), 2);
	Auto_Save_Variable(&this->field_4C, sizeof(this->field_4C), 3);
	Auto_Save_Variable(&this->destroyedHumveeObjId, sizeof(this->destroyedHumveeObjId), 4);
	Auto_Save_Variable(&this->destroyedMedTankObjId, sizeof(this->destroyedMedTankObjId), 5);
	Auto_Save_Variable(&this->apcObjId, sizeof(this->apcObjId), 6);
	Auto_Save_Variable(&this->field_5C, sizeof(this->field_5C), 7);
	Auto_Save_Variable(&this->field_60, sizeof(this->field_60), 8);
	Auto_Save_Variable(&this->field_42, sizeof(this->field_42), 9);
	Auto_Save_Variable(&this->field_68, sizeof(this->field_68), 10);
	Auto_Save_Variable(&this->field_43, sizeof(this->field_43), 11);
	Auto_Save_Variable(&this->field_6C, sizeof(this->field_6C), 12);
	Auto_Save_Variable(&this->field_44, sizeof(this->field_44), 13);
	Auto_Save_Variable(&this->blockageObjId, sizeof(this->blockageObjId), 14);
	Auto_Save_Variable(&this->field_45, sizeof(this->field_45), 15);
	Auto_Save_Variable(&this->field_46, sizeof(this->field_46), 16);
	Auto_Save_Variable(&this->field_48, sizeof(this->field_48), 17);
	Auto_Save_Variable(&this->field_70, sizeof(this->field_70), 18);
	Auto_Save_Variable(&this->field_74, sizeof(this->field_74), 19);
	Auto_Save_Variable(&this->field_78, sizeof(this->field_78), 20);
	Auto_Save_Variable(&this->field_7C, sizeof(this->field_7C), 21);
	Auto_Save_Variable(&this->field_47, sizeof(this->field_47), 22);
	Auto_Save_Variable(&this->field_80, sizeof(this->field_80), 23);
	Auto_Save_Variable(&this->field_84, sizeof(this->field_84), 24);
	Auto_Save_Variable(&this->field_41, sizeof(this->field_41), 25);
}

void MX0_A02_Controller::Created(GameObject *obj)
{
	this->field_41 = 0;
	this->field_40 = 0;
	this->field_42 = 0;
	this->field_43 = 1;
	this->field_44 = 0;
	this->field_45 = 0;
	this->field_46 = 0;
	this->field_47 = 0;
	this->field_48 = 0;
	this->field_4C = 0;
	this->destroyedHumveeObjId = 0;
	this->destroyedMedTankObjId = 0;
	this->apcObjId = 0;
	this->field_5C = 0;
	this->field_60 = 0;
	this->blockageObjId = 0;
	this->field_68 = 0;
	this->field_6C = 0;
	this->field_70 = 0;
	this->field_74 = 0;
	this->field_78 = 0;
	this->field_7C = 0;
	this->field_80 = 0;
	this->field_84 = 1;
}

void MX0_A02_Controller::Timer_Expired(GameObject *obj, int number)
{

}

void MX0_A02_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{

}

void MX0_A02_Controller::MX0_A02_Fill_Empty_Slot(GameObject *a2, bool a3)
{
	
}

ScriptRegistrant<MX0_A02_Controller> MX0_A02_ControllerRegistrant("MX0_A02_Controller", "");
