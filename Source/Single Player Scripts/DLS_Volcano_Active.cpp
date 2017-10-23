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
#include "DLS_Volcano_Active.h"

/*
M03 -> 1300001
*/
void DLS_Volcano_Active::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->receiveType, sizeof(this->receiveType), 1);
	Auto_Save_Variable(&this->receiveParam, sizeof(this->receiveParam), 2);
	Auto_Save_Variable(&this->volcanoTimerId, sizeof(this->volcanoTimerId), 3);
	Auto_Save_Variable(&this->volcanoDelay, sizeof(this->volcanoDelay), 4);
	Auto_Save_Variable(&this->explosionDelayMin, sizeof(this->explosionDelayMin), 5);
	Auto_Save_Variable(&this->explosionDelayMax, sizeof(this->explosionDelayMax), 6);
	Auto_Save_Variable(&this->rumbleDelayMin, sizeof(this->rumbleDelayMin), 7);
	Auto_Save_Variable(&this->rumbleDelayMax, sizeof(this->rumbleDelayMax), 8);
	Auto_Save_Variable(&this->field_3C, sizeof(this->field_3C), 9);
	Auto_Save_Variable(&this->field_3D, sizeof(this->field_3D), 10);
	Auto_Save_Variable(&this->explosionDelay, sizeof(this->explosionDelay), 11);
	Auto_Save_Variable(&this->rumbleDelay, sizeof(this->rumbleDelay), 12);
	Auto_Save_Variable(this->explosionLocations, sizeof(this->explosionLocations), 13);
}

void DLS_Volcano_Active::Created(GameObject *obj)
{
	this->receiveType = Get_Int_Parameter("Receive_Type");
	this->receiveParam = Get_Int_Parameter("Receive_Param");
	this->volcanoTimerId = Get_Int_Parameter("Volcano_Timer_Id");
	this->volcanoDelay = static_cast<float>(Get_Int_Parameter("Volcano_Delay")); // Yes, this is read as an int
	this->explosionDelayMin = Get_Float_Parameter("Explosion_Delay_Min");
	this->explosionDelayMax = Get_Float_Parameter("Explosion_Delay_Max");
	this->rumbleDelayMin = Get_Float_Parameter("Rumble_Delay_Min");
	this->rumbleDelayMax = Get_Float_Parameter("Rumble_Delay_Max");
	this->field_3C = false;
	this->explosionLocations[0] = Vector3(11.53f, 20.95f, 50.73f);
	this->explosionLocations[1] = Vector3(9.8f, 11.69f, 53.99f);
	this->explosionLocations[2] = Vector3(4.24f, 8.67f, 40.85f);
	this->explosionLocations[3] = Vector3(-15.75f, 14.05f, 30.02f);
	this->explosionLocations[4] = Vector3(9.05f, -2.1f, 45.83f);
	this->explosionLocations[5] = Vector3(-57.34f, 22.78f, 16.88f);
	this->explosionLocations[6] = Vector3(-151.27f, 18.56f, 17.59f);
	this->explosionLocations[7] = Vector3(-98.43f, -7.34f, 8.65f);
	this->explosionLocations[8] = Vector3(-103.61f, 17.34f, 9.21f);
	this->explosionLocations[9] = Vector3(-117.75f, 0.22f, 8.95f);
	this->explosionLocations[10] = Vector3(-171.99f, 71.87f, 14.4f);
	this->explosionLocations[11] = Vector3(-196.42f, 87.29f, 11.5f);
	this->explosionLocations[12] = Vector3(-269.45f, 40.87f, 4.93f);
	this->explosionLocations[13] = Vector3(-260.96f, 132.15f, 8.45f);
	this->explosionLocations[14] = Vector3(-144.31f, 44.4f, 9.33f);
	this->explosionLocations[15] = Vector3(-68.42f, 22.18f, 20.73f);
}

// TODO
void DLS_Volcano_Active::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == this->receiveType && param == this->receiveParam)
	{
		Commands->Start_Timer(obj, this, 5.0f, 60001);

		this->field_3C = true;

		this->explosionDelay = Commands->Get_Random(this->explosionDelayMin, this->explosionDelayMax);
		Commands->Start_Timer(obj, this, this->explosionDelay, 6701);

		this->rumbleDelay = Commands->Get_Random(this->rumbleDelayMin, this->rumbleDelayMax);
		Commands->Start_Timer(obj, this, this->rumbleDelay, 6702);

		if (this->volcanoTimerId)
		{
			Commands->Start_Timer(obj, this, this->volcanoDelay, this->volcanoTimerId);
		}
	}
}

// TODO
void DLS_Volcano_Active::Timer_Expired(GameObject *obj, int number)
{
	if (number == 60001)
	{
		GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03MissionControllerObj, 40001, 0, 0.0f);

		Commands->Set_Clouds(1.0f, 1.0f, 20.0f);

		Commands->Start_Timer(obj, this, 55.0f, 40005);
		Commands->Start_Timer(obj, this, 10.0f, 40007);
	}
	else if (number == 40005)
	{
		// You don't have much time! Get to the submarine and get off the island!
		// Just doing some house cleaning before I go.
		int conversationId = Commands->Create_Conversation("M03CON011", 99, 2000.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100011);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (number == 40007)
	{
		int conversationId = Commands->Create_Conversation("M03CON048", 99, 2000.0f, true); // Warning - Unstable seizmic activity detected. All personnel evacuate immediately.
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100048);
		Commands->Monitor_Conversation(obj, conversationId);

		Commands->Start_Timer(obj, this, 30.0f, 40007);
	}
	else if (number == 6701)
	{
		if (this->field_3C)
		{
			int randExplosionIndex = static_cast<int>(Commands->Get_Random(0.0f, 15.9999f));
			if (randExplosionIndex > 15)
			{
				randExplosionIndex = 15;
			}

			Commands->Create_Explosion("Ground Explosions - Harmless", this->explosionLocations[randExplosionIndex], obj);

			Commands->Start_Timer(obj, this, this->explosionDelay, 6701);
		}
	}
	else if (number == 6702)
	{
		if (this->field_3C)
		{
			float cameraShakeIntensity = Commands->Get_Random(0.050000001f, 0.1f);
			float cameraShakeDuration = Commands->Get_Random(4.0f, 8.0f);
			Commands->Shake_Camera(Vector3(0.0f, 0.0f, 0.0f), 5000.0f, cameraShakeIntensity, cameraShakeDuration);

			Commands->Start_Timer(obj, this, this->rumbleDelay, 6702);

			Commands->Create_Logical_Sound(obj, 1001, Vector3(0.0f, 0.0f, 0.0f), 5000.0f);

			Commands->Create_Sound("earthquake_large_01", Vector3(0.0f, 0.0f, 0.0f), obj);
		}
	}
	else if (number == this->volcanoTimerId)
	{
		if (this->field_3C)
		{
			this->field_3C = false;
		}
	}
}

void DLS_Volcano_Active::Entered(GameObject *obj, GameObject *enterer)
{
	this->field_3C = false;
}

ScriptRegistrant<DLS_Volcano_Active> DLS_Volcano_ActiveRegistrant("DLS_Volcano_Active", "Receive_Type=0:int, Receive_Param=0:int, Volcano_Timer_Id=0:int, Volcano_Delay=0.0:float, Explosion_Delay_Min=0.0:float, Explosion_Delay_Max=0.0:float, Rumble_Delay_Min=0.0:float, Rumble_Delay_Max=0.0:float, Debug_Mode=0:int");