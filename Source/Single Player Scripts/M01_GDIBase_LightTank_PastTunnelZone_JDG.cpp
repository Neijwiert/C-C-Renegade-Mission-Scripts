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
#include "M01_GDIBase_LightTank_PastTunnelZone_JDG.h"

// This script is never used
void M01_GDIBase_LightTank_PastTunnelZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	if (enterer == Commands->Find_Object(102435)) // openAreaNodLightTankObj
	{
		Commands->Send_Custom_Event(obj, enterer, 0, 24, 0.0f);

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_GDIBase_LightTank_PastTunnelZone_JDG> M01_GDIBase_LightTank_PastTunnelZone_JDGRegistrant("M01_GDIBase_LightTank_PastTunnelZone_JDG", "");