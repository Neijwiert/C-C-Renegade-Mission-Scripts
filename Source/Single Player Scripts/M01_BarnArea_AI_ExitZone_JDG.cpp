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
#include "M01_BarnArea_AI_ExitZone_JDG.h"

void M01_BarnArea_AI_ExitZone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M01_BarnArea_AI_ExitZone_JDG::Created(GameObject *obj)
{
	this->field_1C = false;
}

// TODO
void M01_BarnArea_AI_ExitZone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 12)
	{
		this->field_1C = param;
	}
}

void M01_BarnArea_AI_ExitZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *paramObj = Commands->Find_Object(this->field_1C);
	if (paramObj)
	{
		if (enterer == paramObj)
		{
			Commands->Send_Custom_Event(obj, enterer, 0, 23, 0.0f);
		}
	}
}

ScriptRegistrant<M01_BarnArea_AI_ExitZone_JDG> M01_BarnArea_AI_ExitZone_JDGRegistrant("M01_BarnArea_AI_ExitZone_JDG", "");