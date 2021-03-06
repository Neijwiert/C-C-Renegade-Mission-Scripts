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
#include "M00_BuildingStateSoundSpeaker.h"

/*
M02 -> 102416 102415 101535 101534 101533 101532 101531 101529 101528 101526 101522 101521 101520 101519 101010 101012 101013 101016 101018 101020 101022 101024 101026 101027 101029 101030 101031 101033 101036 101039 101041 101043 101045 101047 101049 101051 101054 101058 101060 101062 101063 101064 101065 101066 101067 101068 101069 101070 101071 101072 101073 101074 101075 101083 101084 101085 101086 101089 101090 101091 101092 101093 101094 101095 101096 101097 101098 101099 101100 101101 101105 101107 101109 101110 101111 101112 101113 101115 101116 101118 101119
M13 -> 1700013 1700011 1700009 1700007
*/
void M00_BuildingStateSoundSpeaker::Register_Auto_Save_Variables()
{
	// Yes, doExplosions is forgotten
	Auto_Save_Variable(&this->soundId, sizeof(this->soundId), 1);
	Auto_Save_Variable(&this->isDestroyed, sizeof(this->isDestroyed), 3);
}

void M00_BuildingStateSoundSpeaker::Created(GameObject *obj)
{
	this->isDestroyed = false;
	this->doExplosions = true;

	int buildingControllerId = Get_Int_Parameter("BuildingController_ID");
	GameObject *buildingControllerObj = Commands->Find_Object(buildingControllerId);

	if (buildingControllerObj)
	{
		int objId = Commands->Get_ID(obj);

		char buffer[20];
		sprintf(buffer, "%d", objId);

		Commands->Attach_Script(buildingControllerObj, "M00_BuildingStateSoundController", buffer);
	}

	if (Get_Int_Parameter("Frequency_Min") == -1)
	{
		Timer_Expired(obj, 0);
	}
	else
	{
		float frequencyMax = Get_Float_Parameter("Frequency_Max");
		float frequencyMin = Get_Float_Parameter("Frequency_Min");

		float randFrequency = Commands->Get_Random(frequencyMin, frequencyMax);

		Commands->Start_Timer(obj, this, randFrequency, 0);
	}
}

void M00_BuildingStateSoundSpeaker::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received when the sound has ended
	if (type == CUSTOM_EVENT_SOUND_ENDED)
	{
		if (this->isDestroyed)
		{
			if (Get_Int_Parameter("Frequency_Min_Destroyed") != -1)
			{
				float frequencyMaxDestroyed = Get_Float_Parameter("Frequency_Max_Destroyed");
				float frequencyMinDestroyed = Get_Float_Parameter("Frequency_Min_Destroyed");

				float randFrequency = Commands->Get_Random(frequencyMinDestroyed, frequencyMaxDestroyed);

				Commands->Start_Timer(obj, this, randFrequency, 0);
			}
		}
		else if (Get_Int_Parameter("Frequency_Min") != -1)
		{
			float frequencyMax = Get_Float_Parameter("Frequency_Max");
			float frequencyMin = Get_Float_Parameter("Frequency_Min");

			float randFrequency = Commands->Get_Random(frequencyMin, frequencyMax);

			Commands->Start_Timer(obj, this, randFrequency, 0);
		}
	}

	// Received when the building controller has died
	else if (type == 9026)
	{
		if (param == 1)
		{
			Commands->Stop_Sound(this->soundId, true);

			Timer_Expired(obj, 0);
		}
	}

	// Received when the building controller has died or the random interval has expired
	else if (type == 9027)
	{
		if (this->doExplosions)
		{
			Vector3 pos = Commands->Get_Position(obj);
			pos.X += Commands->Get_Random(2.0, 5.0);
			pos.Y += Commands->Get_Random(2.0, 5.0);
			pos.Z += Commands->Get_Random(2.0, 5.0);

			const char *explosionName = Get_Parameter("Explosion_Name");
			Commands->Create_Explosion(explosionName, pos, NULL);

			float randInterval = Commands->Get_Random(3.0, 6.0);
			Commands->Send_Custom_Event(obj, obj, 9027, 1, randInterval);
		}
	}

	// Received from M00_BuildingStateSoundController when custom type 9028 is received
	else if (type == 9028)
	{
		this->doExplosions = false;
	}

	// Received from M00_BuildingStateSoundController when custom type 9029 is received
	else if (type == 9029)
	{
		this->doExplosions = true;
	}
}

void M00_BuildingStateSoundSpeaker::Timer_Expired(GameObject *obj, int number)
{
	if (this->isDestroyed)
	{
		bool is3DDestroyed = (Get_Int_Parameter("Is_3D_Destroyed") == 1);

		Vector3 pos = Commands->Get_Position(obj);
		Vector3 offsetDestroyed = Get_Vector3_Parameter("Offset_Destroyed");

		pos.X += offsetDestroyed.X;
		pos.Y += offsetDestroyed.Y;
		pos.Z += offsetDestroyed.Z;

		Vector3 offsetRandomnessDestroyed = Get_Vector3_Parameter("Offset_Randomness_Destroyed");

		float randX = Commands->Get_Random(-offsetRandomnessDestroyed.X, offsetRandomnessDestroyed.X);
		float randY = Commands->Get_Random(-offsetRandomnessDestroyed.Y, offsetRandomnessDestroyed.Y);
		float randZ = Commands->Get_Random(-offsetRandomnessDestroyed.Z, offsetRandomnessDestroyed.Z);

		pos.X += randX;
		pos.Y += randY;
		pos.Z += randZ;

		if (is3DDestroyed)
		{
			Commands->Debug_Message("Playing 3D Sound\n");

			const char *soundDestroyed = Get_Parameter("Sound_Destroyed");

			this->soundId = Commands->Create_Sound(soundDestroyed, pos, obj);
			Commands->Monitor_Sound(obj, this->soundId);
		}
		else
		{
			Commands->Debug_Message("Playing 2D Sound\n");

			const char *soundDestroyed = Get_Parameter("Sound_Destroyed");

			this->soundId = Commands->Create_2D_Sound(soundDestroyed);
			Commands->Monitor_Sound(obj, this->soundId);
		}
	}
	else
	{
		bool is3D = (Get_Int_Parameter("Is_3D") == 1);

		Vector3 pos = Commands->Get_Position(obj);
		Vector3 offset = Get_Vector3_Parameter("Offset");

		pos.X += offset.X;
		pos.Y += offset.Y;
		pos.Z += offset.Z;

		Vector3 offsetRandomness = Get_Vector3_Parameter("Offset_Randomness");

		float randX = Commands->Get_Random(-offsetRandomness.X, offsetRandomness.X);
		float randY = Commands->Get_Random(-offsetRandomness.Y, offsetRandomness.Y);
		float randZ = Commands->Get_Random(-offsetRandomness.Z, offsetRandomness.Z);

		pos.X += randX;
		pos.Y += randY;
		pos.Z += randZ;

		if (!is3D)
		{
			Commands->Debug_Message("Playing 2D Sound\n");

			const char *soundNormal = Get_Parameter("Sound_Normal");

			this->soundId = Commands->Create_2D_Sound(soundNormal);
			Commands->Monitor_Sound(obj, this->soundId);
		}
		else
		{
			Commands->Debug_Message("Playing 3D Sound\n");

			const char *soundNormal = Get_Parameter("Sound_Normal");

			this->soundId = Commands->Create_Sound(soundNormal, pos, obj);
			Commands->Monitor_Sound(obj, this->soundId);
		}
	}
}

ScriptRegistrant<M00_BuildingStateSoundSpeaker> M00_BuildingStateSoundSpeakerRegistrant("M00_BuildingStateSoundSpeaker", "Sound_Normal:string,Sound_Destroyed:string,BuildingController_ID:int,Is_3D=1:int,Offset:vector3,Offset_Randomness:vector3,Frequency_Min=-1:float,Frequency_Max:float,Is_3D_Destroyed=1:int,Offset_Destroyed:vector3,Offset_Randomness_Destroyed:vector3,Frequency_Min_Destroyed=-1:float,Frequency_Max_Destroyed:float, Explosion_Name:string");