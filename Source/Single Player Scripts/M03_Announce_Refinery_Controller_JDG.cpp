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
#include "M03_Announce_Refinery_Controller_JDG.h"

/*
M03 -> 1144606
*/
void M03_Announce_Refinery_Controller_JDG::Register_Auto_Save_Variables()
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
	Auto_Save_Variable(&this->soundLocation8, sizeof(this->soundLocation8), 18);
	Auto_Save_Variable(&this->soundLocation8, sizeof(this->soundLocation8), 19); // soundLocation9 is never saved
	Auto_Save_Variable(&this->soundLocation10, sizeof(this->soundLocation10), 20);
	Auto_Save_Variable(&this->soundLocation11, sizeof(this->soundLocation11), 21);
	Auto_Save_Variable(&this->soundLocation12, sizeof(this->soundLocation12), 22);
	Auto_Save_Variable(&this->soundLocation13, sizeof(this->soundLocation13), 23);
	Auto_Save_Variable(&this->soundLocation14, sizeof(this->soundLocation14), 24);
	Auto_Save_Variable(&this->soundLocation15, sizeof(this->soundLocation15), 25);
	Auto_Save_Variable(&this->soundLocation16, sizeof(this->soundLocation16), 26);
	Auto_Save_Variable(&this->soundLocation17, sizeof(this->soundLocation17), 27);
}

void M03_Announce_Refinery_Controller_JDG::Created(GameObject *obj)
{
	this->soundIndexerMaxRandom = 28;
	this->minSound2Delay = 20.0f;
	this->maxSound2Delay = 30.0f;
	this->startupParam = 20;
	this->field_2C = 21;
	this->soundIndexerParam = 22;
	this->sound2Param = 23;
	this->sound1Param = 24;
	this->soundLocation1 = Vector3(-156.3f, 28.4f, -10.07f);
	this->soundLocation2 = Vector3(-172.19f, 28.91f, -10.08f);
	this->soundLocation3 = Vector3(-144.46f, 17.18f, -10.0f);
	this->soundLocation4 = Vector3(-136.33f, 1.47f, -10.63f);
	this->soundLocation5 = Vector3(-139.38f, -14.86f, -10.79f);
	this->soundLocation6 = Vector3(-146.76f, -27.75f, -10.24f);
	this->soundLocation7 = Vector3(-134.95f, -33.21f, -10.7f);
	this->soundLocation8 = Vector3(-181.62f, 16.33f, -10.65f);
	this->soundLocation9 = Vector3(-180.04f, 3.15f, -10.16f);
	this->soundLocation10 = Vector3(-178.31f, -13.79f, -10.84f);
	this->soundLocation11 = Vector3(-177.85f, -30.11f, -10.99f);
	this->soundLocation12 = Vector3(-168.09f, -33.71f, -10.01f);
	this->soundLocation13 = Vector3(-156.28f, -39.59f, -10.43f);
	this->soundLocation14 = Vector3(-160.06f, -18.86f, -10.99f);
	this->soundLocation15 = Vector3(-143.87f, -1.17f, -10.99f);
	this->soundLocation16 = Vector3(-159.78f, -1.17f, -10.99f);
	this->soundLocation17 = Vector3(-166.61f, 11.61f, -10.99f);

	if (obj)
	{
		Commands->Send_Custom_Event(obj, obj, 0, this->startupParam, 0.0f);
	}
}

void M03_Announce_Refinery_Controller_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *nonExisting1Obj = Commands->Find_Object(600067);
	Commands->Destroy_Object(nonExisting1Obj);

	GameObject *nonExisting2Obj = Commands->Find_Object(600068);
	Commands->Destroy_Object(nonExisting2Obj);

	GameObject *nonExisting3Obj = Commands->Find_Object(600069);
	Commands->Destroy_Object(nonExisting3Obj);

	GameObject *nonExisting4Obj = Commands->Find_Object(600070);
	Commands->Destroy_Object(nonExisting4Obj);

	GameObject *nonExisting5Obj = Commands->Find_Object(600071);
	Commands->Destroy_Object(nonExisting5Obj);
}

// TODO
void M03_Announce_Refinery_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
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
			Commands->Create_Sound(sound1, this->soundLocation8, obj);
			Commands->Create_Sound(sound1, this->soundLocation9, obj);
			Commands->Create_Sound(sound1, this->soundLocation10, obj);
			Commands->Create_Sound(sound1, this->soundLocation11, obj);
			Commands->Create_Sound(sound1, this->soundLocation12, obj);
			Commands->Create_Sound(sound1, this->soundLocation13, obj);
			Commands->Create_Sound(sound1, this->soundLocation14, obj);
			Commands->Create_Sound(sound1, this->soundLocation15, obj);
			Commands->Create_Sound(sound1, this->soundLocation16, obj);
			Commands->Create_Sound(sound1, this->soundLocation17, obj);

			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, this->sound2Param, 1.25f);
			}
		}
		else if (param == this->sound2Param)
		{
			static const char *sounds2[28] =
			{
				"01-i010E",
				"01-i022E",
				"01-i032E",
				"01-i034E",
				"01-i052E",
				"01-i054E",
				"01-i056E",
				"01-i058E",
				"01-i060E",
				"01-i062E",
				"01-i064E",
				"01-i066E",
				"01-i068E",
				"01-i070E",
				"01-i072E",
				"01-i074E",
				"01-i076E",
				"01-i078E",
				"01-i080E",
				"01-i082E",
				"01-i084E",
				"01-i086E",
				"01-i088E",
				"01-i090E",
				"01-i092E",
				"01-i094E",
				"01-i096E",
				"01-i098E"
			};

			const char *sound2 = sounds2[this->sound2Index];
			Commands->Create_Sound(sound2, this->soundLocation1, obj);
			Commands->Create_Sound(sound2, this->soundLocation2, obj);
			Commands->Create_Sound(sound2, this->soundLocation3, obj);
			Commands->Create_Sound(sound2, this->soundLocation4, obj);
			Commands->Create_Sound(sound2, this->soundLocation5, obj);
			Commands->Create_Sound(sound2, this->soundLocation6, obj);
			Commands->Create_Sound(sound2, this->soundLocation7, obj);
			Commands->Create_Sound(sound2, this->soundLocation8, obj);
			Commands->Create_Sound(sound2, this->soundLocation9, obj);
			Commands->Create_Sound(sound2, this->soundLocation10, obj);
			Commands->Create_Sound(sound2, this->soundLocation11, obj);
			Commands->Create_Sound(sound2, this->soundLocation12, obj);
			Commands->Create_Sound(sound2, this->soundLocation13, obj);
			Commands->Create_Sound(sound2, this->soundLocation14, obj);
			Commands->Create_Sound(sound2, this->soundLocation15, obj);
			Commands->Create_Sound(sound2, this->soundLocation16, obj);
			Commands->Create_Sound(sound2, this->soundLocation17, obj);

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
				this->sound2Index = 0;
				sendParam = this->sound2Param;
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 1;
				sendParam = this->sound1Param;
			}
			else if (rand >= 2.5f && rand < 3.5f)
			{
				this->sound2Index = 2;
				sendParam = this->sound2Param;
			}
			else if (rand >= 3.5f && rand < 4.5f)
			{
				this->sound2Index = 3;
				sendParam = this->sound2Param;
			}
			else if (rand >= 4.5f && rand < 5.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 4;
				sendParam = this->sound1Param;
			}
			else if (rand >= 5.5f && rand < 6.5f)
			{
				this->sound2Index = 5;
				sendParam = this->sound2Param;
			}
			else if (rand >= 6.5f && rand < 7.5f)
			{
				this->sound2Index = 6;
				sendParam = this->sound2Param;
			}
			else if (rand >= 7.5f && rand < 8.5f)
			{
				this->sound2Index = 7;
				sendParam = this->sound2Param;
			}
			else if (rand >= 8.5f && rand < 9.5f)
			{
				this->sound2Index = 8;
				sendParam = this->sound2Param;
			}
			else if (rand >= 9.5f && rand < 10.5f)
			{
				this->sound2Index = 9;
				sendParam = this->sound2Param;
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
				this->sound2Index = 12;
				sendParam = this->sound2Param;
			}
			else if (rand >= 13.5f && rand < 14.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 13;
				sendParam = this->sound1Param;
			}
			else if (rand >= 14.5f && rand < 15.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 14;
				sendParam = this->sound1Param;
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
				this->sound1Index = 1;
				this->sound2Index = 17;
				sendParam = this->sound1Param;
			}
			else if(rand >= 18.5f && rand < 19.5f)
			{
				this->sound1Index = 0;
				this->sound2Index = 18;
				sendParam = this->sound1Param;
			}
			else if (rand >= 19.5f && rand < 20.5f)
			{
				this->sound2Index = 19;
				sendParam = this->sound2Param;
			}
			else if (rand >= 20.5f && rand < 21.5f)
			{
				this->sound1Index = 1;
				this->sound2Index = 20;
				sendParam = this->sound1Param;
			}
			else if (rand >= 21.5f && rand < 22.5f)
			{
				this->sound2Index = 21;
				sendParam = this->sound2Param;
			}
			else if (rand >= 22.5f && rand < 23.5f)
			{
				this->sound2Index = 22;
				sendParam = this->sound2Param;
			}
			else if (rand >= 23.5f && rand < 24.5f)
			{
				this->sound2Index = 23;
				sendParam = this->sound2Param;
			}
			else if (rand >= 24.5f && rand < 25.5f)
			{
				this->sound2Index = 24;
				sendParam = this->sound2Param;
			}
			else if (rand >= 25.5f && rand < 26.5f)
			{
				this->sound2Index = 25;
				sendParam = this->sound2Param;
			}
			else if (rand >= 26.5f && rand < 27.5f)
			{
				this->sound2Index = 26;
				sendParam = this->sound2Param;
			}
			else
			{
				this->sound2Index = 27;
				sendParam = this->sound2Param;
			}

			if (obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, sendParam, 0.0f);
			}
		}
	}
}

ScriptRegistrant<M03_Announce_Refinery_Controller_JDG> M03_Announce_Refinery_Controller_JDGRegistrant("M03_Announce_Refinery_Controller_JDG", "");