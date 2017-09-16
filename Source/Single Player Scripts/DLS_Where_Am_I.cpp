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
#include "DLS_Where_Am_I.h"

void DLS_Where_Am_I::Register_Auto_Save_Variables()
{

}

void DLS_Where_Am_I::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	const char *presetName = Commands->Get_Preset_Name(obj);

	Commands->Debug_Message("%s at: X:%f Y:%f Z:%f \n", presetName, pos.X, pos.Y, pos.Z);

	Commands->Start_Timer(obj, this, 3.0f, 0);
}

void DLS_Where_Am_I::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 3 seconds on creation or after 3 seconds in this block
	if (!number)
	{
		Vector3 pos = Commands->Get_Position(obj);
		const char *presetName = Commands->Get_Preset_Name(obj);

		Commands->Debug_Message("%s at: X:%f Y:%f Z:%f \n", presetName, pos.X, pos.Y, pos.Z);

		Commands->Start_Timer(obj, this, 3.0f, 0);
	}
}

ScriptRegistrant<DLS_Where_Am_I> DLS_Where_Am_IRegistrant("DLS_Where_Am_I", "");