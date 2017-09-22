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
#include "M01_GDIBaseCommander_EvacController_JDG.h"

/*
M01 -> 106694
*/
void M01_GDIBaseCommander_EvacController_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->waypathId, sizeof(this->waypathId), 2);
	Auto_Save_Variable(&this->chopperObjId, sizeof(this->chopperObjId), 3);
	Auto_Save_Variable(&this->ropeObjId, sizeof(this->ropeObjId), 4);
}

void M01_GDIBaseCommander_EvacController_JDG::Created(GameObject *obj)
{
	this->field_1C = false;

	Commands->Enable_Hibernation(obj, false);
}

// TODO
void M01_GDIBaseCommander_EvacController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 16)
		{
			GameObject *captainDuncanObj = Commands->Find_Object(106050);
			GameObject *gdiMinigunnerObj = Commands->Find_Object(116382);
			GameObject *gdiGrenadierObj = Commands->Find_Object(116383);

			if (captainDuncanObj)
			{
				Commands->Send_Custom_Event(obj, captainDuncanObj, 0, 9, 0.0f);
			}

			if (gdiMinigunnerObj)
			{
				Commands->Send_Custom_Event(obj, gdiMinigunnerObj, 0, 9, 0.0f);
			}

			if (gdiGrenadierObj)
			{
				Commands->Send_Custom_Event(obj, gdiGrenadierObj, 0, 9, 0.0f);
			}
		}
		else if (param == 27)
		{
			if (!this->field_1C)
			{
				this->field_1C = true;
				
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *invisObj = Commands->Create_Object("Invisible_Object", pos);
				Commands->Attach_Script(invisObj, "Test_Cinematic", "XG_M01_GDIBaseCommander_EvacAnim.txt");
			}
		}
		else if (param == 28)
		{
			if (this->field_1C)
			{
				GameObject *chopperObj = Commands->Find_Object(this->chopperObjId);
				GameObject *waypath = Commands->Find_Object(this->waypathId);
				GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);

				if (chopperObj)
				{
					Commands->Debug_Message("**********************sending custom to chopper to leave.....%d\n", this->chopperObjId);

					Commands->Send_Custom_Event(obj, chopperObj, 0, 27, 0.0f);
				}

				if (waypath)
				{
					Commands->Debug_Message("**********************sending custom to waypath to leave.....%d\n", this->waypathId);

					Commands->Send_Custom_Event(obj, waypath, 0, 27, 0.0f);
				}
				if (ropeObj)
				{
					Commands->Debug_Message("**********************sending custom to rope to leave.....%d\n", this->ropeObjId);
					
					Commands->Send_Custom_Event(obj, ropeObj, 0, 27, 0.0f);
				}
			}
		}
		else if (param == 29)
		{
			this->chopperObjId = Commands->Get_ID(sender);

			Commands->Debug_Message("**********************chopper id received.....%d\n", this->chopperObjId);
		}
		else if (param == 30)
		{
			this->waypathId = Commands->Get_ID(sender);

			Commands->Debug_Message("**********************waypath id received.....%d\n", this->waypathId);
		}
		else if (param == 31)
		{
			this->ropeObjId = Commands->Get_ID(sender);

			Commands->Debug_Message("**********************rope id received.....%d\n", this->ropeObjId);
		}
		else if (param == 10)
		{
			Vector3 pos = Commands->Get_Position(obj);
			
			GameObject *troopBone = Commands->Create_Object("Invisible_Object", pos);
			Commands->Set_Model(troopBone, "XG_EV5_troopBN");
			Commands->Attach_Script(troopBone, "M01_Evac_TroopBone_JDG", "");

			Commands->Innate_Disable(sender);
			Commands->Attach_To_Object_Bone(sender, troopBone, "Troop_L");

			if (troopBone)
			{
				Commands->Set_Animation(troopBone, "XG_EV5_troopBN.XG_EV5_troopBN", false, NULL, 0.0f, -1.0f, false);
			}
			else
			{
				Commands->Debug_Message("troopbone not created yet");
			}

			Commands->Set_Animation(sender, "S_A_Human.XG_EV5_troop", false, NULL, 0.0f, -1.0f, false);
		}
	}
}

ScriptRegistrant<M01_GDIBaseCommander_EvacController_JDG> M01_GDIBaseCommander_EvacController_JDGRegistrant("M01_GDIBaseCommander_EvacController_JDG", "");