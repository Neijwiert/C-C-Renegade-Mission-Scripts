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
#include "M03_Mission_Complete_Zone.h"

/*
M03 -> 2000817
*/
void M03_Mission_Complete_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->zoneEntered, sizeof(this->zoneEntered), 1);
}

void M03_Mission_Complete_Zone::Created(GameObject *obj)
{
	this->zoneEntered = false;
}

void M03_Mission_Complete_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (!this->zoneEntered)
	{
		this->zoneEntered = true;

		Commands->Mission_Complete(true);
	}
}

ScriptRegistrant<M03_Mission_Complete_Zone> M03_Mission_Complete_ZoneRegistrant("M03_Mission_Complete_Zone", "");