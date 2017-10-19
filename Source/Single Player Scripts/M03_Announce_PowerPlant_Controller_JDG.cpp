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
#include "M03_Announce_PowerPlant_Controller_JDG.h"

/*
M03 -> 1150003
*/
void M03_Announce_PowerPlant_Controller_JDG::Register_Auto_Save_Variables()
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

void M03_Announce_PowerPlant_Controller_JDG::Created(GameObject *obj)
{
	this->soundIndexerMaxRandom = 15;
	this->minSound2Delay = 20.0f;
	this->maxSound2Delay = 30.0f;
	this->startupParam = 20;
	this->field_2C = 21;
	this->soundIndexerParam = 22;
	this->sound2Param = 23;
	this->sound1Param = 24;
	this->soundLocation1 = Vector3(-60.55f, 13.55f, -9.69f);
	this->soundLocation2 = Vector3(-65.74f, 15.17f, -8.71f);
	this->soundLocation3 = Vector3(-63.75f, 22.49f, -7.81f);
	this->soundLocation4 = Vector3(-63.0f, 22.05f, -0.7f);
	this->soundLocation5 = Vector3(-75.75f, 28.92f, -14.77f);
	this->soundLocation6 = Vector3(-64.98f, 20.41f, -13.62f);
	this->soundLocation7 = Vector3(-58.05f, 7.45f, -14.2f);

	if (obj)
	{
		Commands->Send_Custom_Event(obj, obj, 0, this->startupParam, 0.0f);
	}
}

void M03_Announce_PowerPlant_Controller_JDG::Killed(GameObject *obj, GameObject *killer)
{

}

// TODO
void M03_Announce_PowerPlant_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param != this->field_2C)
	{
		if (param == this->startupParam)
		{
			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, this->soundIndexerParam, 0.0f);
			}
		}
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
		else if (param == this->sound2Param)
		{
			static const char *sounds2[19] =
			{
				"01-i000e",
				"01-i002e",
				"01-i004e",
				"01-i006e",
				"01-i008e",
				"01-i010e",
				"01-i012e",
				"01-i014e",
				"01-i016e",
				"01-i018e",
				"01-i020e",
				"01-i024e",
				"01-i026e",
				"01-i050e",
				"01-i062e",
				"01-i066e",
				"01-i076e",
				"01-i078e",
				"01-i022e"
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
		else if (param == this->soundIndexerParam)
		{
			float maxRand = static_cast<float>(this->soundIndexerMaxRandom) + 0.5f;
			float rand = Commands->Get_Random(0.5f, maxRand);
			int sendParam;	

			if (rand >= 0.5f && rand < 1.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 0;
				sendParam = this->sound1Param;
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				this->sound2Index = 1;
				sendParam = this->sound2Param;
			}
			else if (rand >= 2.5f && rand < 3.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 2;
				sendParam = this->sound1Param;
			}
			else if (rand >= 3.5f && rand < 4.5f)
			{
				this->sound1Index = 0;
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
				this->sound2Index = 5;
				sendParam = this->sound2Param;
			}
			else if (rand >= 6.5f && rand < 7.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 6;
				sendParam = this->sound1Param;
			}
			else if (rand >= 7.5f && rand < 8.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 7;
				sendParam = this->sound1Param;
			}
			else if (rand >= 8.5f && rand < 9.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 8;
				sendParam = this->sound1Param;
			}
			else if (rand >= 9.5f && rand < 10.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 9;
				sendParam = this->sound1Param;
			}
			else if (rand >= 10.5f && rand < 11.5f)
			{
				this->sound2Index = 10;
				sendParam = this->sound2Param;
			}
			else if (rand >= 11.5f && rand < 12.5f)
			{
				this->sound2Index = 11;
				sendParam = this->sound2Param;
			}
			else if (rand >= 12.5f && rand < 13.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 12;
				sendParam = this->sound2Param;
			}
			else if (rand >= 13.5f && rand < 14.5f)
			{
				this->sound2Index = 13;
				sendParam = this->sound2Param;
			}
			else if (rand >= 14.5f && rand < 15.5f)
			{
				this->sound2Index = 14;
				sendParam = this->sound2Param;
			}
			else if (rand >= 15.5f && rand < 16.5f)
			{
				this->sound2Index = 15;
				sendParam = this->sound2Param;
			}
			else if (rand >= 16.5f && rand < 17.5f)
			{
				this->sound2Index = 16;
				sendParam = this->sound2Param;
			}
			else if (rand >= 17.5f && rand < 18.5f)
			{
				this->sound2Index = 17;
				sendParam = this->sound2Param;
			}
			else
			{
				this->sound2Index = 18;
				sendParam = this->sound2Param;
			}
									
			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, sendParam, 0.0f);
			}
		}
	}
}

ScriptRegistrant<M03_Announce_PowerPlant_Controller_JDG> M03_Announce_PowerPlant_Controller_JDGRegistrant("M03_Announce_PowerPlant_Controller_JDG", "");