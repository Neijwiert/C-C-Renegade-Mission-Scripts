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
#include "M00_Play_Sound_Object_Bone_DAY.h"

void M00_Play_Sound_Object_Bone_DAY::Created(GameObject *obj)
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

void M00_Play_Sound_Object_Bone_DAY::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_SOUND_ENDED && Get_Int_Parameter("Frequency_Min") < 0)
	{
		float frequencyMax = Get_Float_Parameter("Frequency_Max");
		float frequencyMin = Get_Float_Parameter("Frequency_Min");

		float randFrequency = Commands->Get_Random(frequencyMin, frequencyMax);
		Commands->Start_Timer(obj, this, randFrequency, 0);
	}
}

void M00_Play_Sound_Object_Bone_DAY::Timer_Expired(GameObject *obj, int number)
{
	const char *soundPreset = Get_Parameter("Sound_Preset");

	/*
	This is here, but it doesn't do anything

	Vector3 pos = Commands->Get_Position(obj);
	Vector3 offset = Get_Vector3_Parameter("Offset");

	pos.X += offset.X;
	pos.Y += offset.Y;
	pos.Z += offset.Z;
	*/

	Commands->Debug_Message("Playing 3D Sound\n");

	int soundId = Commands->Create_3D_Sound_At_Bone(soundPreset, obj, "ROOTTRANSFORM");
	Commands->Monitor_Sound(obj, soundId);
}

ScriptRegistrant<M00_Play_Sound_Object_Bone_DAY> M00_Play_Sound_Object_Bone_DAYRegistrant("M00_Play_Sound_Object_Bone_DAY", "Sound_Preset:string, Frequency_Min=-1.0:float, Frequency_Max:float");