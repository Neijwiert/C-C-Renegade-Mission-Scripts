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
#include "M03_Announce_CommCenter_Controller_JDG.h"

/*
M03 -> 1100009
*/
void M03_Announce_CommCenter_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->soundIndexerMaxRandom, sizeof(this->soundIndexerMaxRandom), 1);
	Auto_Save_Variable(&this->minSound2Delay, sizeof(this->minSound2Delay), 2);
	Auto_Save_Variable(&this->maxSound2Delay, sizeof(this->maxSound2Delay), 3);
	Auto_Save_Variable(&this->startupParam, sizeof(this->startupParam), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->soundIndexerParam, sizeof(this->soundIndexerParam), 6);
	Auto_Save_Variable(&this->sound2Param, sizeof(this->sound2Param), 7);
	Auto_Save_Variable(&this->sound1Param, sizeof(this->sound1Param), 8);
	Auto_Save_Variable(&this->sound2Index, sizeof(this->sound2Index), 9);
	Auto_Save_Variable(&this->sound1Index, sizeof(this->sound1Index), 10);
	Auto_Save_Variable(&this->soundLocation1, sizeof(this->soundLocation1), 11);
	Auto_Save_Variable(&this->soundLocation2, sizeof(this->soundLocation2), 12);
	Auto_Save_Variable(&this->soundLocation3, sizeof(this->soundLocation3), 13);
	Auto_Save_Variable(&this->soundLocation4, sizeof(this->soundLocation4), 14);
	Auto_Save_Variable(&this->soundLocation5, sizeof(this->soundLocation5), 15);
	Auto_Save_Variable(&this->soundLocation6, sizeof(this->soundLocation6), 16);
	Auto_Save_Variable(&this->soundLocation7, sizeof(this->soundLocation7), 17);
}

void M03_Announce_CommCenter_Controller_JDG::Created(GameObject *obj)
{
	this->soundIndexerMaxRandom = 15;
	this->minSound2Delay = 20.0f;
	this->maxSound2Delay = 30.0f;
	this->startupParam = 20;
	this->field_2C = 21;
	this->soundIndexerParam = 22;
	this->sound2Param = 23;
	this->sound1Param = 24;
	this->soundLocation1 = Vector3(-104.35f, 47.83f, -3.54f);
	this->soundLocation2 = Vector3(116.1f, 46.42f, -3.15f);
	this->soundLocation3 = Vector3(-117.04f, 41.42f, -5.37f);
	this->soundLocation4 = Vector3(-127.57f, 65.42f, -5.75f);
	this->soundLocation5 = Vector3(-121.95f, 59.32f, -4.91f);
	this->soundLocation6 = Vector3(-117.48f, 45.83f, -3.87f);
	this->soundLocation7 = Vector3(-111.25f, 43.69f, -2.27f);

	if (obj)
	{
		Commands->Send_Custom_Event(obj, obj, 0, this->startupParam, 0.0f);
	}
}

void M03_Announce_CommCenter_Controller_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *nonExisting1Obj = Commands->Find_Object(600042);
	Commands->Destroy_Object(nonExisting1Obj);

	GameObject *nonExisting2Obj = Commands->Find_Object(600056);
	Commands->Destroy_Object(nonExisting2Obj);

	GameObject *nonExisting3Obj = Commands->Find_Object(600057);
	Commands->Destroy_Object(nonExisting3Obj);

	GameObject *nonExisting4Obj = Commands->Find_Object(600058);
	Commands->Destroy_Object(nonExisting4Obj);

	GameObject *nonExisting5Obj = Commands->Find_Object(600059);
	Commands->Destroy_Object(nonExisting5Obj);

	GameObject *nonExisting6Obj = Commands->Find_Object(600060);
	Commands->Destroy_Object(nonExisting6Obj);

	GameObject *nonExisting7Obj = Commands->Find_Object(600061);
	Commands->Destroy_Object(nonExisting7Obj);

	GameObject *nonExisting8Obj = Commands->Find_Object(600062);
	Commands->Destroy_Object(nonExisting8Obj);

	GameObject *nonExisting9Obj = Commands->Find_Object(600063);
	Commands->Destroy_Object(nonExisting9Obj);

	GameObject *nonExisting10Obj = Commands->Find_Object(600064);
	Commands->Destroy_Object(nonExisting10Obj);

	GameObject *nonExisting11Obj = Commands->Find_Object(600065);
	Commands->Destroy_Object(nonExisting11Obj);
}

void M03_Announce_CommCenter_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param != this->field_2C)
	{
		// Received by ourselves after 0 seconds, see create
		if (param == this->startupParam)
		{
			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, this->soundIndexerParam, 0.0f);
			}
		}

		// Received by ourselves after 0 seconds, see custom soundIndexerParam
		else if (param == this->sound1Param)
		{
			static const char *sounds1[2] =
			{
				"Klaxon Warning",
				"Klaxon Info"
			};

			const char *sound1 = sounds1[this->sound1Index];
			Commands->Create_Sound(sound1, this->soundLocation1, obj);
			Commands->Create_Sound(sound1, this->soundLocation2, obj);
			Commands->Create_Sound(sound1, this->soundLocation3, obj);
			Commands->Create_Sound(sound1, this->soundLocation4, obj);
			Commands->Create_Sound(sound1, this->soundLocation5, obj);
			Commands->Create_Sound(sound1, this->soundLocation6, obj);
			Commands->Create_Sound(sound1, this->soundLocation7, obj);

			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, this->sound2Param, 1.25f);
			}
		}

		// Received by ourselves after 1.25 seconds, see custom sound1Param
		// Received by ourselves after 0 seconds, see custom soundIndexerParam
		else if (param == this->sound2Param)
		{
			static const char *sounds2[15] =
			{
				"01-I022E",
				"01-I028E",
				"01-I030E",
				"01-I032E",
				"01-I034E",
				"01-I036E",
				"01-I038E",
				"01-I040E",
				"01-I042E",
				"01-I044E",
				"01-I046E",
				"01-I048E",
				"01-I050E",
				"01-I062E",
				"01-I066E"
			};

			const char *sound2 = sounds2[this->sound2Index];
			Commands->Create_Sound(sound2, this->soundLocation1, obj);
			Commands->Create_Sound(sound2, this->soundLocation2, obj);
			Commands->Create_Sound(sound2, this->soundLocation3, obj);
			Commands->Create_Sound(sound2, this->soundLocation4, obj);
			Commands->Create_Sound(sound2, this->soundLocation5, obj);
			Commands->Create_Sound(sound2, this->soundLocation6, obj);
			Commands->Create_Sound(sound2, this->soundLocation7, obj);

			float randDelay = Commands->Get_Random(this->minSound2Delay, this->maxSound2Delay);
			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, this->soundIndexerParam, randDelay);
			}
		}

		// Received by ourselves after 0 seconds, see custom startupParam or minSound2Delay to maxSound2Delay seconds, see custom sound2Param
		else if (param == this->soundIndexerParam)
		{
			float maxRand = static_cast<float>(this->soundIndexerMaxRandom) + 0.5f;
			float rand = Commands->Get_Random(0.5f, maxRand);
			int sendParam;

			if (rand >= 0.5f && rand < 1.5f)
			{
				this->sound2Index = 0;
				sendParam = this->sound2Param;
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				this->sound2Index = 1;
				sendParam = this->sound2Param;
			}
			else if (rand >= 2.5f && rand < 3.5f)
			{
				this->sound2Index = 2;
				sendParam = this->sound2Param;
			}
			else if (rand >= 3.5f && rand < 4.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 3;
				sendParam = this->sound1Param;
			}
			else if (rand >= 4.5f && rand < 5.5f)
			{
				this->sound2Index = 4;
				sendParam = this->sound2Param;
			}
			else if (rand >= 5.5f && rand < 6.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 5;
				sendParam = this->sound1Param;
			}
			else if (rand >= 6.5f && rand < 7.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 6;
				sendParam = this->sound1Param;
			}
			else if (rand >= 7.5f && rand < 8.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 7;
				sendParam = this->sound1Param;
			}
			else if (rand >= 8.5f && rand < 9.5f)
			{
				this->sound2Index = 8;
				sendParam = this->sound2Param;
			}
			else if (rand >= 9.5f && rand < 10.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 9;
				sendParam = this->sound1Param;
			}
			else if (rand >= 10.5f && rand < 11.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 10;
				sendParam = this->sound1Param;
			}
			else if (rand >= 11.5f && rand < 12.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 11;
				sendParam = this->sound1Param;
			}
			else if (rand >= 12.5f && rand < 13.5f)
			{
				this->sound2Index = 12;
				sendParam = this->sound2Param;
			}
			else if (rand >= 13.5f && rand < 14.5f)
			{
				this->sound2Index = 13;
				sendParam = this->sound2Param;
			}
			else
			{
				this->sound2Index = 14;
				sendParam = this->sound2Param;
			}

			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, sendParam, 0.0f);
			}
		}
	}
}

ScriptRegistrant<M03_Announce_CommCenter_Controller_JDG> M03_Announce_CommCenter_Controller_JDGRegistrant("M03_Announce_CommCenter_Controller_JDG", "");