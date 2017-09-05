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
#include "M08_Petra_C_Helo.h"

void M08_Petra_C_Helo::Register_Auto_Save_Variables()
{

}

// After 0 cinematic frames in mx0_a03_nod_ledgedrop
void M08_Petra_C_Helo::Created(GameObject *obj)
{

}

void M08_Petra_C_Helo::Killed(GameObject *obj, GameObject *killer)
{
	int unitId = Get_Int_Parameter("Unit_ID");

	GameObject *unknownObj = Commands->Find_Object(100389); // TODO (where is this used? M13 does not have this object, in M08 it's a speaker)
	Commands->Send_Custom_Event(obj, unknownObj, 8033, unitId, 0.0f);
}

ScriptRegistrant<M08_Petra_C_Helo> M08_Petra_C_HeloRegistrant("M08_Petra_C_Helo", "Unit_ID=0:int");