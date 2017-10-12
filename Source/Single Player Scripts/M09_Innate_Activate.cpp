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
#include "M09_Innate_Activate.h"

/*
M03 -> 1144675
M09 -> 2002276 2002270 2000896 2000709 1101845 1101846 1200233 1202994
*/
void M09_Innate_Activate::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->zoneEntered, sizeof(this->zoneEntered), 1);
	Auto_Save_Variable(&this->targetObjIds[0], sizeof(this->targetObjIds[0]), 2);
	Auto_Save_Variable(&this->targetObjIds[1], sizeof(this->targetObjIds[1]), 3);
	Auto_Save_Variable(&this->targetObjIds[2], sizeof(this->targetObjIds[2]), 4);
	Auto_Save_Variable(&this->targetObjIds[3], sizeof(this->targetObjIds[3]), 5);
	Auto_Save_Variable(&this->targetObjIds[4], sizeof(this->targetObjIds[4]), 6);
	Auto_Save_Variable(&this->targetObjIds[5], sizeof(this->targetObjIds[5]), 7);
	Auto_Save_Variable(&this->targetObjIds[6], sizeof(this->targetObjIds[6]), 8);
	Auto_Save_Variable(&this->targetObjIds[7], sizeof(this->targetObjIds[7]), 9);
	Auto_Save_Variable(&this->targetObjIds[8], sizeof(this->targetObjIds[8]), 10);
	Auto_Save_Variable(&this->targetObjIds[9], sizeof(this->targetObjIds[9]), 11);
}

void M09_Innate_Activate::Created(GameObject *obj)
{
	this->zoneEntered = false;
	this->targetObjIds[0] = Get_Int_Parameter("Target0");
	this->targetObjIds[1] = Get_Int_Parameter("Target1");
	this->targetObjIds[2] = Get_Int_Parameter("Target2");
	this->targetObjIds[3] = Get_Int_Parameter("Target3");
	this->targetObjIds[4] = Get_Int_Parameter("Target4");
	this->targetObjIds[5] = Get_Int_Parameter("Target5");
	this->targetObjIds[6] = Get_Int_Parameter("Target6");
	this->targetObjIds[7] = Get_Int_Parameter("Target7");
	this->targetObjIds[8] = Get_Int_Parameter("Target8");
	this->targetObjIds[9] = Get_Int_Parameter("Target9");
}

void M09_Innate_Activate::Entered(GameObject *obj, GameObject *enterer)
{
	if (!this->zoneEntered)
	{
		this->zoneEntered = true;

		for (int x = 0; x < 10; x++)
		{
			GameObject *targetObj = Commands->Find_Object(this->targetObjIds[x]);
			Commands->Send_Custom_Event(obj, targetObj, 40057, 0, 0.0f);
		}
	}
}

ScriptRegistrant<M09_Innate_Activate> M09_Innate_ActivateRegistrant("M09_Innate_Activate", "Target0=0:int, Target1=0:int, Target2=0:int, Target3=0:int, Target4=0:int, Target5=0:int, Target6=0:int, Target7=0:int, Target8=0:int, Target9=0:int");