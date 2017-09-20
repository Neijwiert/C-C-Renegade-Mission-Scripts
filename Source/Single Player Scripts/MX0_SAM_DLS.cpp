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
#include "MX0_SAM_DLS.h"

/*
M13 -> 1500016 1500015
*/
void MX0_SAM_DLS::Register_Auto_Save_Variables()
{

}

void MX0_SAM_DLS::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
}

void MX0_SAM_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from MX0_Area4_Controller_DLS when custom type 445007 is received
	if (type == 445007)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 95.0f, 1);

		GameObject *orcaSAMStrikeLocObj = Commands->Find_Object(1500081);

		params.Set_Attack(orcaSAMStrikeLocObj, 250.0f, 0.0f, true);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<MX0_SAM_DLS> MX0_SAM_DLSRegistrant("MX0_SAM_DLS", "");