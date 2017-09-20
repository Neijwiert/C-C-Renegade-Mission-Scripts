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
#include "M01_Base_GDI_Fodder_JDG.h"

/*
M01 -> 119225
*/
void M01_Base_GDI_Fodder_JDG::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 16);
	params.Set_Attack(Vector3(72.223f, -24.751f, -0.296f), 1000.0f, 0.0f, true);
	params.AttackCrouched = true;
	params.AttackCheckBlocked = false;

	Commands->Action_Attack(obj, params);

	float randDelay = Commands->Get_Random(10.0f, 15.0f);
	Commands->Send_Custom_Event(obj, obj, 0, 27, randDelay);
}

void M01_Base_GDI_Fodder_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves, see created
	if (!type && param == 27)
	{
		Commands->Apply_Damage(obj, 10000.0f, "BlamoKiller", NULL);
	}
}

ScriptRegistrant<M01_Base_GDI_Fodder_JDG> M01_Base_GDI_Fodder_JDGRegistrant("M01_Base_GDI_Fodder_JDG", "");