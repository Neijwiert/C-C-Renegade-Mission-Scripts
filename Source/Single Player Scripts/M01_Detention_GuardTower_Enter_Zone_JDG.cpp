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
#include "M01_Detention_GuardTower_Enter_Zone_JDG.h"

/*
M01 -> 103009
*/
void M01_Detention_GuardTower_Enter_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *detentionCiv1Obj = Commands->Find_Object(101929);
		if (detentionCiv1Obj)
		{
			Commands->Send_Custom_Event(obj, detentionCiv1Obj, 0, 16, 0.0f);
		}

		GameObject *detentionCiv2Obj = Commands->Find_Object(101930);
		if (detentionCiv2Obj)
		{
			Commands->Send_Custom_Event(obj, detentionCiv2Obj, 0, 16, 0.0f);
		}

		GameObject *detentionCiv3Obj = Commands->Find_Object(101931);
		if (detentionCiv3Obj)
		{
			Commands->Send_Custom_Event(obj, detentionCiv3Obj, 0, 16, 0.0f);
		}

		GameObject *detentionSoldier1Obj = Commands->Find_Object(101926);
		if (detentionSoldier1Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier1Obj, 0, 16, 0.0f);
		}

		GameObject *detentionSoldier2Obj = Commands->Find_Object(101927);
		if (detentionSoldier2Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier2Obj, 0, 16, 0.0f);
		}

		GameObject *detentionSoldier3Obj = Commands->Find_Object(101928);
		if (detentionSoldier3Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier3Obj, 0, 16, 0.0f);
		}
	}
}

void M01_Detention_GuardTower_Enter_Zone_JDG::Exited(GameObject *obj, GameObject *exiter)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (exiter == Commands->Get_A_Star(pos))
	{
		GameObject *detentionCiv1Obj = Commands->Find_Object(101929);
		if (detentionCiv1Obj)
		{
			Commands->Send_Custom_Event(obj, detentionCiv1Obj, 0, 24, 0.0f);
		}

		GameObject *detentionCiv2Obj = Commands->Find_Object(101930);
		if (detentionCiv2Obj)
		{
			Commands->Send_Custom_Event(obj, detentionCiv2Obj, 0, 24, 0.0f);
		}

		GameObject *detentionCiv3Obj = Commands->Find_Object(101931);
		if (detentionCiv3Obj)
		{
			Commands->Send_Custom_Event(obj, detentionCiv3Obj, 0, 24, 0.0f);
		}

		GameObject *detentionSoldier1Obj = Commands->Find_Object(101926);
		if (detentionSoldier1Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier1Obj, 0, 24, 0.0f);
		}

		GameObject *detentionSoldier2Obj = Commands->Find_Object(101927);
		if (detentionSoldier2Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier2Obj, 0, 24, 0.0f);
		}

		GameObject *detentionSoldier3Obj = Commands->Find_Object(101928);
		if (detentionSoldier3Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier3Obj, 0, 24, 0.0f);
		}
	}
}

ScriptRegistrant<M01_Detention_GuardTower_Enter_Zone_JDG> M01_Detention_GuardTower_Enter_Zone_JDGRegistrant("M01_Detention_GuardTower_Enter_Zone_JDG", "");