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
#include "M03_Destroyed_Chinook.h"

// When M03_Alternate_Sam_Site receives any custom
void M03_Destroyed_Chinook::Killed(GameObject *obj, GameObject *killer)
{
	int controllerId = Get_Int_Parameter("Controller_ID");
	GameObject *controllerObj = Commands->Find_Object(controllerId);

	int simpleId = Get_Int_Parameter("Simple_ID");
	GameObject *simpleObj = Commands->Find_Object(simpleId);

	if (controllerObj)
	{
		Commands->Destroy_Object(controllerObj);
	}

	if (simpleObj)
	{
		Commands->Destroy_Object(simpleObj);
	}

	Commands->Create_Explosion_At_Bone("Air Explosions Twiddler", obj, "SEAT1", killer);
	Commands->Create_Explosion_At_Bone("Air Explosions Twiddler", obj, "ROTOR01", killer);
}

ScriptRegistrant<M03_Destroyed_Chinook> M03_Destroyed_ChinookRegistrant("M03_Destroyed_Chinook", "Controller_ID:int, Simple_ID:int");