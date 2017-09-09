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
#include "M02_Respawn_Controller.h"

void M02_Respawn_Controller::Register_Auto_Save_Variables()
{

}

void M02_Respawn_Controller::Created(GameObject *obj)
{

}

void M02_Respawn_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{

}

void M02_Respawn_Controller::Timer_Expired(GameObject *obj, int number)
{

}

DWORD M02_Respawn_Controller::Check_Respawns(int a2)
{
	return 0;
}

bool M02_Respawn_Controller::Use_Spawners(int a2)
{
	return false;
}

void M02_Respawn_Controller::Use_Heli_Drop_02(int a2)
{
	
}

GameObject *M02_Respawn_Controller::Use_Parachute_Drop(int a2)
{
	return NULL;
}

void M02_Respawn_Controller::Use_Heli_Drop_01(int a2)
{

}

GameObject *M02_Respawn_Controller::Replacement_Vehicle(GameObject *obj, int a3)
{
	return NULL;
}

ScriptRegistrant<M02_Respawn_Controller> M02_Respawn_ControllerRegistrant("M02_Respawn_Controller", "");