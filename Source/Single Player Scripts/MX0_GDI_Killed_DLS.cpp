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
#include "MX0_GDI_Killed_DLS.h"

void MX0_GDI_Killed_DLS::Register_Auto_Save_Variables()
{

}

// After 401 cinematic frames in mx0_gdi_reinforce_area4
// After 489 cinematic frames in mx0_gdi_reinforce_area4
// When MX0_Area4_Controller_DLS has received custom type 445000
void MX0_GDI_Killed_DLS::Created(GameObject *obj)
{

}

void MX0_GDI_Killed_DLS::Killed(GameObject *obj, GameObject *killer)
{
	int unitId = Get_Int_Parameter("Unit_ID");
	if (unitId == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);

		GameObject *gdiMinigunner10FFObj = Commands->Create_Object("GDI_Minigunner_1Off", pos);
		Commands->Set_Facing(gdiMinigunner10FFObj, 0.0f);
		Commands->Attach_Script(gdiMinigunner10FFObj, "M00_Send_Object_ID", "1500020, 2, 0.0f");
		Commands->Attach_Script(gdiMinigunner10FFObj, "MX0_GDI_Soldier_DLS", "0,0,0,0,0.0f");

		Commands->Send_Custom_Event(obj, gdiMinigunner10FFObj, 445002, 445011, 0.0f);

		GameObject *MX0NodObeliskObj = Commands->Find_Object(1500020);
		Commands->Send_Custom_Event(obj, MX0NodObeliskObj, 445004, 1, 1.0f);
	}
	else if (unitId == 2)
	{
		Vector3 pos = Commands->Get_Position(obj);
		
		GameObject *MX0GDIMediumTankDestroyedObj = Commands->Create_Object("MX0_GDI_Medium_Tank_Destroyed", pos);

		float MX0GDIMediumTankDestroyedObjFacing = Commands->Get_Facing(obj);
		Commands->Set_Facing(MX0GDIMediumTankDestroyedObj, MX0GDIMediumTankDestroyedObjFacing);
	}
	else if (unitId == 3)
	{
		GameObject *MX0A04ControllerObj = Commands->Find_Object(1500020);
		Commands->Send_Custom_Event(obj, MX0A04ControllerObj, 445019, 1, 1.0f);
	}
}

void MX0_GDI_Killed_DLS::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	GameObject *MX0NodObeliskObj = Commands->Find_Object(1500020);
	if (damager == MX0NodObeliskObj && MX0NodObeliskObj)
	{
		Commands->Apply_Damage(obj, 50000.0f, "STEEL", NULL);
	}
}

ScriptRegistrant<MX0_GDI_Killed_DLS> MX0_GDI_Killed_DLSRegistrant("MX0_GDI_Killed_DLS", "Unit_ID=0:int");