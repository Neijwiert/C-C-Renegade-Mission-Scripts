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
#include "M01_Airstrike_Controller_JDG.h"

/*
M01 -> 121041
*/
void M01_Airstrike_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->decoNodLightTank1ObjId, sizeof(this->decoNodLightTank1ObjId), 1);
	Auto_Save_Variable(&this->decoNodLightTank2ObjId, sizeof(this->decoNodLightTank2ObjId), 2);
	Auto_Save_Variable(&this->decoNodLightTank3ObjId, sizeof(this->decoNodLightTank3ObjId), 3);
}

void M01_Airstrike_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received after 95 cinematic frames in x1g_a-10_strike.txt
		if (!param)
		{
			GameObject *M01GDIBaseRealLightTankJDGObj = Commands->Find_Object(120022);
			if (M01GDIBaseRealLightTankJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseRealLightTankJDGObj, 0, 17, 0.5f);
			}

			GameObject *decoNodLightTank1Obj = Commands->Find_Object(this->decoNodLightTank1ObjId);
			GameObject *decoNodLightTank2Obj = Commands->Find_Object(this->decoNodLightTank2ObjId);
			GameObject *decoNodLightTank3Obj = Commands->Find_Object(this->decoNodLightTank3ObjId);
			if (decoNodLightTank1Obj)
			{
				Commands->Send_Custom_Event(obj, decoNodLightTank1Obj, 0, 18, 0.5f);
			}

			if (decoNodLightTank2Obj)
			{
				Commands->Send_Custom_Event(obj, decoNodLightTank2Obj, 0, 18, 0.75f);
			}

			if (decoNodLightTank3Obj)
			{
				Commands->Send_Custom_Event(obj, decoNodLightTank3Obj, 0, 18, 1.0f);
			}
		}

		// Received from M01_GDIBase_RealLightTank_JDG when custom param 16 is received
		else if (param == 16)
		{
			GameObject *decoNodLightTank1Obj = Commands->Create_Object("Nod_Light_Tank_Dec", Vector3(-12.175f, 68.28f, 3.009f));
			Commands->Set_Facing(decoNodLightTank1Obj, -145.0f);

			GameObject *decoNodLightTank2Obj = Commands->Create_Object("Nod_Light_Tank_Dec", Vector3(-14.948f, 61.305f, 3.259f));
			Commands->Set_Facing(decoNodLightTank2Obj, -150.0f);

			GameObject *decoNodLightTank3Obj = Commands->Create_Object("Nod_Light_Tank_Dec", Vector3(-22.011f, 66.635f, 2.959f));
			Commands->Set_Facing(decoNodLightTank3Obj, -115.0f);

			Commands->Attach_Script(decoNodLightTank1Obj, "M01_Deco_LightTanks_JDG", "");
			Commands->Attach_Script(decoNodLightTank2Obj, "M01_Deco_LightTanks_JDG", "");
			Commands->Attach_Script(decoNodLightTank3Obj, "M01_Deco_LightTanks_JDG", "");

			this->decoNodLightTank1ObjId = Commands->Get_ID(decoNodLightTank1Obj);
			this->decoNodLightTank2ObjId = Commands->Get_ID(decoNodLightTank2Obj);
			this->decoNodLightTank3ObjId = Commands->Get_ID(decoNodLightTank3Obj);
		}

		// Received from ourselves after we received custom param 27
		// Received from M01_GDIBase_RealLightTank_JDG after 15 seconds when custom param 16 is received
		else if (param == 17)
		{
			GameObject *decoNodLightTank1Obj = Commands->Find_Object(this->decoNodLightTank1ObjId);
			GameObject *decoNodLightTank2Obj = Commands->Find_Object(this->decoNodLightTank2ObjId);
			GameObject *decoNodLightTank3Obj = Commands->Find_Object(this->decoNodLightTank3ObjId);
			if (decoNodLightTank1Obj)
			{
				Commands->Send_Custom_Event(obj, decoNodLightTank1Obj, 0, 17, 0.5f);
			}

			if (decoNodLightTank2Obj)
			{
				Commands->Send_Custom_Event(obj, decoNodLightTank2Obj, 0, 17, 0.5f);
			}

			if (decoNodLightTank3Obj)
			{
				Commands->Send_Custom_Event(obj, decoNodLightTank3Obj, 0, 17, 0.5f);
			}
		}

		// Received from M01_GDIBase_RealLightTank_JDG after 15 seconds when custom param 16 is received
		else if (param == 27)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 17, 0.0f);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1G_A-10_Strike.txt");
		}
	}
}

ScriptRegistrant<M01_Airstrike_Controller_JDG> M01_Airstrike_Controller_JDGRegistrant("M01_Airstrike_Controller_JDG", "");