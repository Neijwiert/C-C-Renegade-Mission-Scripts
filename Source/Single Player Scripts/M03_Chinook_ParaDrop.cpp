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
#include "M03_Chinook_ParaDrop.h"

void M03_Chinook_ParaDrop::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->nodTransportHelicopterObjId, sizeof(this->nodTransportHelicopterObjId), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
}

// See M03_Beach_Reinforce::Reinforce_Beach, M03_Beach_Reinforce::Reinforce_Inlet and M03_Beach_Reinforce::Reinforce_Base
void M03_Chinook_ParaDrop::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	float facing = Commands->Get_Facing(obj);

	GameObject *trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
	Commands->Set_Model(trajectoryObj, "X5D_Chinookfly");
	Commands->Set_Facing(trajectoryObj, facing);
	Commands->Set_Animation(trajectoryObj, "X5D_Chinookfly.X5D_Chinookfly", false, NULL, 0.0f, -1.0f, false);

	GameObject *nodTransportHelicopterObj = Commands->Create_Object("Nod_Transport_Helicopter", pos);
	Commands->Set_Facing(nodTransportHelicopterObj, facing);
	Commands->Set_Animation(nodTransportHelicopterObj, "v_Nod_trnspt.XG_RTN_TrnsptA", true, NULL, 0.0f, -1.0f, false);
	Commands->Attach_To_Object_Bone(nodTransportHelicopterObj, trajectoryObj, "BN_Chinook_1");

	this->field_20 = false;
	this->field_24 = 0;

	int objId = Commands->Get_ID(obj);
	char buffer[16];
	sprintf(buffer, "%d", objId);
	Commands->Attach_Script(nodTransportHelicopterObj, "M03_Reinforcement_Chinook", buffer);

	this->nodTransportHelicopterObjId = Commands->Get_ID(nodTransportHelicopterObj);

	Commands->Start_Timer(obj, this, 280 / 30.0f, 0);
	Commands->Start_Timer(obj, this, 169 / 30.0f, 1);
	Commands->Start_Timer(obj, this, 179 / 30.0f, 2);
	Commands->Start_Timer(obj, this, 198 / 30.0f, 3);
	Commands->Start_Timer(obj, this, 145 / 30.0f, 4);
	Commands->Start_Timer(obj, this, 155 / 30.0f, 5);
	Commands->Start_Timer(obj, this, 165 / 30.0f, 6);
}

// TODO
void M03_Chinook_ParaDrop::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 23000 && param == 23000)
	{
		this->field_20 = true;
	}
}

// TODO
void M03_Chinook_ParaDrop::Timer_Expired(GameObject *obj, int number)
{
	Vector3 pos = Commands->Get_Position(obj);
	const char *preset = Get_Parameter("Preset");
	float facing = Commands->Get_Facing(obj);
	
	if (number == 0)
	{
		GameObject *nodTransportHelicopterObj = Commands->Find_Object(this->nodTransportHelicopterObjId);
		Commands->Destroy_Object(nodTransportHelicopterObj);
	}
	else if (number == 1)
	{
		if (this->field_24 > 0)
		{
			GameObject *cinObj = Commands->Create_Object("Generic_Cinematic", pos);
			Commands->Set_Facing(cinObj, facing);
			Commands->Set_Model(cinObj, "X5D_Parachute");
			Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_1", false, NULL, 0.0f, -1.0f, false);
			Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
			Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
		}
	}
	else if (number == 2)
	{
		if (this->field_24 > 1)
		{
			GameObject *cinObj = Commands->Create_Object("Generic_Cinematic", pos);
			Commands->Set_Facing(cinObj, facing);
			Commands->Set_Model(cinObj, "X5D_Parachute");
			Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_2", false, NULL, 0.0f, -1.0f, false);
			Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
			Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
		}
	}
	else if (number == 3)
	{
		if (this->field_24 == 3)
		{
			GameObject *cinObj = Commands->Create_Object("Generic_Cinematic", pos);
			Commands->Set_Facing(cinObj, facing);
			Commands->Set_Model(cinObj, "X5D_Parachute");
			Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_3", false, NULL, 0.0f, -1.0f, false);
			Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
			Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
		}
	}
	else if (number == 4)
	{
		if (!this->field_20)
		{
			GameObject *trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
			Commands->Set_Model(trajectoryObj, "X5D_Box01");
			Commands->Set_Facing(trajectoryObj, facing);
			Commands->Set_Animation(trajectoryObj, "X5D_Box01.X5D_Box01", false, NULL, 0.0f, -1.0f, false);

			GameObject *presetObj = Commands->Create_Object_At_Bone(trajectoryObj, preset, "Box01");
			Commands->Set_Facing(presetObj, facing);
			Commands->Attach_Script(presetObj, "RMV_Trigger_Killed", "1144444, 1000, 1000");
			Commands->Attach_Script(presetObj, "M03_Killed_Sound", "");
			Commands->Attach_Script(presetObj, "M03_Paratrooper_Run", "");
			Commands->Attach_Script(presetObj, "DLS_Volcano_Stumble", "");
			Commands->Attach_To_Object_Bone(presetObj, trajectoryObj, "Box01");
			Commands->Set_Animation(presetObj, "s_a_human.H_A_X5D_ParaT_1", false, NULL, 0.0f, -1.0f, false);

			this->field_24++;
		}
	}
	else if (number == 5)
	{
		if (!this->field_20)
		{
			GameObject *trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
			Commands->Set_Model(trajectoryObj, "X5D_Box02");
			Commands->Set_Facing(trajectoryObj, facing);
			Commands->Set_Animation(trajectoryObj, "X5D_Box02.X5D_Box02", false, NULL, 0.0f, -1.0f, false);

			GameObject *presetObj = Commands->Create_Object_At_Bone(trajectoryObj, preset, "Box02");
			Commands->Set_Facing(presetObj, facing);
			Commands->Attach_Script(presetObj, "RMV_Trigger_Killed", "1144444, 1000, 1000");
			Commands->Attach_Script(presetObj, "M03_Killed_Sound", "");
			Commands->Attach_Script(presetObj, "M03_Paratrooper_Run", "");
			Commands->Attach_Script(presetObj, "DLS_Volcano_Stumble", "");
			Commands->Set_Animation(presetObj, "s_a_human.H_A_X5D_ParaT_2", false, NULL, 0.0f, -1.0f, false);
			Commands->Attach_To_Object_Bone(presetObj, trajectoryObj, "Box02");

			this->field_24++;
		}
	}
	else if (number == 6)
	{
		if (!this->field_20)
		{
			GameObject *trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
			Commands->Set_Model(trajectoryObj, "X5D_Box03");
			Commands->Set_Facing(trajectoryObj, facing);
			Commands->Set_Animation(trajectoryObj, "X5D_Box03.X5D_Box03", false, NULL, 0.0f, -1.0f, false);

			GameObject *presetObj = Commands->Create_Object_At_Bone(trajectoryObj, preset, "Box03");
			Commands->Set_Facing(presetObj, facing);
			Commands->Attach_Script(presetObj, "RMV_Trigger_Killed", "1144444, 1000, 1000");
			Commands->Attach_Script(presetObj, "M03_Killed_Sound", "");
			Commands->Attach_Script(presetObj, "M03_Paratrooper_Run", "");
			Commands->Attach_Script(presetObj, "DLS_Volcano_Stumble", "");
			Commands->Set_Animation(presetObj, "s_a_human.H_A_X5D_ParaT_3", false, NULL, 0.0f, -1.0f, false);
			Commands->Attach_To_Object_Bone(presetObj, trajectoryObj, "Box03");

			this->field_24++;
		}
	}
}

ScriptRegistrant<M03_Chinook_ParaDrop> M03_Chinook_ParaDropRegistrant("M03_Chinook_ParaDrop", "Preset:string");