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
#include "M03_Reinforcement_Chinook.h"

void M03_Reinforcement_Chinook::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->chinookIdleSoundId, sizeof(this->chinookIdleSoundId), 1);
}

// When M03_Chinook_ParaDrop is created
void M03_Reinforcement_Chinook::Created(GameObject *obj)
{
	this->chinookIdleSoundId = Commands->Create_3D_Sound_At_Bone("Chinook_Idle_01", obj, "V_FUSELAGE");
}

void M03_Reinforcement_Chinook::Destroyed(GameObject *obj)
{
	Commands->Stop_Sound(this->chinookIdleSoundId, true);
}

void M03_Reinforcement_Chinook::Killed(GameObject *obj, GameObject *killer)
{
	const char *strControllerId = Get_Parameter(0); // Yes, it is converting it here
	int controllerId = atoi(strControllerId);
	
	GameObject *controllerObj = Commands->Find_Object(controllerId);
	Commands->Send_Custom_Event(obj, controllerObj, 23000, 23000, 0.0f);
}

ScriptRegistrant<M03_Reinforcement_Chinook> M03_Reinforcement_ChinookRegistrant("M03_Reinforcement_Chinook", "Controller_ID:int");