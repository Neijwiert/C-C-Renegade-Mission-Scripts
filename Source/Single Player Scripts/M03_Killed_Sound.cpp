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
#include "M03_Killed_Sound.h"

/*
M03 -> 1147534 1147533 1147532 1147531 1147530 1147529 1147527 1146701 1146700 300009 300028 300032 1141162 1144451 1144448 1144731 1144732 300010
*/
void M03_Killed_Sound::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M03BeachReinforceObj = Commands->Find_Object(2018061);

	int location = Get_Int_Parameter("Location");
	Commands->Send_Custom_Event(obj, M03BeachReinforceObj, 40021, location, 0.0f);

	int officer = Get_Int_Parameter("Officer");
	Commands->Send_Custom_Event(obj, M03BeachReinforceObj, 40008, officer, 0.0f);
}

ScriptRegistrant<M03_Killed_Sound> M03_Killed_SoundRegistrant("M03_Killed_Sound", "Officer=0:int, Location=0:int");