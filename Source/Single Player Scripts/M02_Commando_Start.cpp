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
#include "M02_Commando_Start.h"

// Level start script
void M02_Commando_Start::Created(GameObject *obj)
{
	Commands->Give_PowerUp(obj, "POW_MineRemote_Player", false);
	Commands->Give_PowerUp(obj, "POW_Chaingun_Player", false);
	Commands->Give_PowerUp(obj, "POW_SniperRifle_Player", false);
}

ScriptRegistrant<M02_Commando_Start> M02_Commando_StartRegistrant("M02_Commando_Start", "");