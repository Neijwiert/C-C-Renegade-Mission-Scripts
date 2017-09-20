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
#include "MX0_Area4_Zone_DLS.h"

/*
M13 -> 1500005 1500004 1500003 1500002 1500001 1500006
*/
void MX0_Area4_Zone_DLS::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canSendCustom, sizeof(this->canSendCustom), 1);
}

void MX0_Area4_Zone_DLS::Created(GameObject *obj)
{
	this->canSendCustom = true;
}

void MX0_Area4_Zone_DLS::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		if (this->canSendCustom)
		{
			int area = Get_Int_Parameter("Area");

			GameObject *MX0A04ControllerObj = Commands->Find_Object(1500017);
			Commands->Send_Custom_Event(obj, MX0A04ControllerObj, 445000, area, 0.0f);

			this->canSendCustom = false;
		}
	}
}

ScriptRegistrant<MX0_Area4_Zone_DLS> MX0_Area4_Zone_DLSRegistrant("MX0_Area4_Zone_DLS", "Area=0:int");