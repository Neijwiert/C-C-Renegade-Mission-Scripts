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
#include "M00_Play_Sound.h"

void M00_Play_Sound::Created(GameObject *obj)
{
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

void M00_Play_Sound::Timer_Expired(GameObject *obj, int number)
{
	const char *soundPreset = Get_Parameter("Sound_Preset");
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

	int soundId;
	if (is3D)
	{
		Commands->Debug_Message("Playing 3D Sound\n");

		soundId = Commands->Create_Sound(soundPreset, pos, obj);
	}
	else
	{
		Commands->Debug_Message("Playing 2D Sound\n");

		soundId = Commands->Create_2D_Sound(soundPreset);
	}

	Commands->Monitor_Sound(obj, soundId);
}

void M00_Play_Sound::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Triggered when sound has ended
	if (type == CUSTOM_EVENT_SOUND_ENDED && Get_Int_Parameter("Frequency_Min") != -1)
	{
		float frequencyMax = Get_Float_Parameter("Frequency_Max");
		float frequencyMin = Get_Float_Parameter("Frequency_Min");

		float randFrequency = Commands->Get_Random(frequencyMin, frequencyMax);

		Commands->Start_Timer(obj, this, randFrequency, 0);
	}
}

ScriptRegistrant<M00_Play_Sound> M00_Play_SoundRegistrant("M00_Soldier_Powerup_Disable", "Sound_Preset:string, Is_3D=1:int, Offset:vector3, Offset_Randomness:vector3, Frequency_Min=-1:float, Frequency_Max:float");