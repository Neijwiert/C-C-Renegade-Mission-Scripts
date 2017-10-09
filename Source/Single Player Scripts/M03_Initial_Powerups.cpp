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
#include "M03_Initial_Powerups.h"

/*
M03 -> 1100004
*/
void M03_Initial_Powerups::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 0.5f, 0);
}

// TODO
void M03_Initial_Powerups::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 12176 && param == 12176)
	{
		Commands->Start_Timer(obj, this, 2.0f, 0);
	}
}

// TODO
void M03_Initial_Powerups::Timer_Expired(GameObject *obj, int number)
{
	if (!number)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		if (starObj)
		{
			Commands->Give_PowerUp(starObj, "Shotgun Weapon 1 Clip PU", false);
			Commands->Give_PowerUp(starObj, "Sniper Weapon 1 Clip PU", false);
			Commands->Give_PowerUp(starObj, "Remote Mine Weapon 1 Clip PU", false);

			Commands->Grant_Key(starObj, 5, true);

			int objId = Commands->Get_ID(obj);
			char buffer[32];
			sprintf(buffer, "%d", objId);

			Commands->Attach_Script(starObj, "M03_Commando_Script", buffer);
		}
		else
		{
			Commands->Start_Timer(obj, this, 1.0f, 0);
		}
	}
}

ScriptRegistrant<M03_Initial_Powerups> M03_Initial_PowerupsRegistrant("M03_Initial_Powerups", "");