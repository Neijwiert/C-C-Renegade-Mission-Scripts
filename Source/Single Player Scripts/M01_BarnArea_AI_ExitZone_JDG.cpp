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
	Auto_Save_Variable(&this->turretBeachGDIGuy1ObjId, sizeof(this->turretBeachGDIGuy1ObjId), 1);
}

void M01_BarnArea_AI_ExitZone_JDG::Created(GameObject *obj)
{
	this->turretBeachGDIGuy1ObjId = 0;
}

void M01_BarnArea_AI_ExitZone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_TurretBeach_GDI_Guy_01_JDG when created
	if (type == 12)
	{
		this->turretBeachGDIGuy1ObjId = param;
	}
}

void M01_BarnArea_AI_ExitZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *turretBeachGDIGuy1Obj = Commands->Find_Object(this->turretBeachGDIGuy1ObjId);
	if (turretBeachGDIGuy1Obj)
	{
		if (enterer == turretBeachGDIGuy1Obj)
		{
			Commands->Send_Custom_Event(obj, enterer, 0, 23, 0.0f);
		}
	}
}

ScriptRegistrant<M01_BarnArea_AI_ExitZone_JDG> M01_BarnArea_AI_ExitZone_JDGRegistrant("M01_BarnArea_AI_ExitZone_JDG", "");