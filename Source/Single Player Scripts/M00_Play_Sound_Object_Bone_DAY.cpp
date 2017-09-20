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

/*
M01 -> 103101 101299 101167 101166 101298 106907 106908 106909
M02 -> 401193 401191 401190 401189 401097 401088 401087 401086 401085 400505 400560 400984 401076 406724 406725
M03 -> 300052 1204952 1204951 1147532 1147527 1144715 1144653 300032 1141162 1144500
M04 -> 103320 103318 102638 101277 101201
M05 -> 100158 100109 100100 100099 100606 100605 100103 100647 100264 100052 100057 100058 100108 100110 100169
M06 -> 101400 101399 101398 101397 101391 101385 101384 101356 101105 101097 101098 101099 101037 101153 101154 101369 101393
M07 -> 113833 113832 101086 100672 100671 100732 100733 100902 100907 101052 101053 101092
M08 -> 100649 100648 100772 100771 100770 100537 100536 100534 100191 100192 100226 100231 100233 100236 100237 100245 100247 100255 109597 100612 100613 100672 100673 100674
M09 -> 2005480 2000632 2006877 2006876 2000662 2000660 2005740 2005739 2005738 2000557 2000558 2000559 2004492 2004494 2004497 2000570 2000568 2000569 2005475 2005486 2005499 2005500 2006392 2006393 1101818 1101819 1101822 1101823 1100730
M10 -> 2008186 2008185 1100134 2009686 2009685 1100257 1100226 1111329 2000890
M11 -> 101996 101995 100394 101100 100667 100666 100665 100664 100403 100258 100257 100256 100227 100225 100400 100536 100537 100538 100662 100663 100186 100189 101402 101403
*/
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