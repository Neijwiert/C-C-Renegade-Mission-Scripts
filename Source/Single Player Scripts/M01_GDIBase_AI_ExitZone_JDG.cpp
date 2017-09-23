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
#include "M01_GDIBase_AI_ExitZone_JDG.h"

/*
M01 -> 112014
*/
void M01_GDIBase_AI_ExitZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *nonExisting1Obj = Commands->Find_Object(104686);
	GameObject *captainDuncanObj = Commands->Find_Object(106050);
	GameObject *nonExisting2Obj = Commands->Find_Object(104685);
	GameObject *nonExisting3Obj = Commands->Find_Object(103327);
	GameObject *nonExisting4Obj = Commands->Find_Object(103328);

	if (enterer == nonExisting1Obj)
	{
		if (enterer)
		{
			Commands->Send_Custom_Event(obj, enterer, 0, 23, 0.0f);
		}
	}
	else if (enterer == captainDuncanObj)
	{
		if (enterer)
		{
			Commands->Send_Custom_Event(obj, enterer, 0, 9, 0.0f);
		}

		if (nonExisting1Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 9, 0.0f);
		}

		if (nonExisting2Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 9, 0.0f);
		}
	}
	else if (enterer == nonExisting3Obj)
	{
		if (nonExisting3Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting3Obj, 0, 9, 0.0f);
		}

		if (nonExisting4Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting4Obj, 0, 9, 0.0f);
		}
	}
}

ScriptRegistrant<M01_GDIBase_AI_ExitZone_JDG> M01_GDIBase_AI_ExitZone_JDGRegistrant("M01_GDIBase_AI_ExitZone_JDG", "");