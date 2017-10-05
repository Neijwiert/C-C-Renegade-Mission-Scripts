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
#include "M01_Ambient_Sound_Controller_JDG.h"

/*
M01 -> 100253
*/
void M01_Ambient_Sound_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->ccAmbientM01SoundId, sizeof(this->ccAmbientM01SoundId), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->klaxonSoundIndex, sizeof(this->klaxonSoundIndex), 6);
	Auto_Save_Variable(&this->ambientSoundIndex, sizeof(this->ambientSoundIndex), 7);
	Auto_Save_Variable(&this->field_38, sizeof(this->field_38), 8);
	Auto_Save_Variable(&this->field_39, sizeof(this->field_39), 9);
	Auto_Save_Variable(&this->field_3A, sizeof(this->field_3A), 10);
}

void M01_Ambient_Sound_Controller_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	this->field_1C = 0;
	this->field_20 = 0;
	this->klaxonSoundIndex = 0;
	this->ambientSoundIndex = 0;
	this->field_38 = false;
	this->field_39 = false;
	this->field_3A = false;
	this->field_24 = 5;

	Commands->Send_Custom_Event(obj, obj, 0, 158, 0.0f);
}

// TODO
void M01_Ambient_Sound_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after create
	// Received from M01_mission_Controller_JDG when param 138 or 142 or 144 or 146 is received
	if (param == 158)
	{
		if (!this->field_38)
		{
			this->field_24 = 5;
			this->field_38 = true;

			if (this->field_39)
			{
				Commands->Stop_Sound(this->ccAmbientM01SoundId, true);
			}

			this->field_39 = false;
			this->field_3A = false;
		}
	}

	// Received by M01_mission_Controller_JDG when param 135 or 141 or 143 or 145 is received
	else if (param == 159)
	{
		if (this->field_38)
		{
			this->field_38 = false;
		}
	}

	// Received from M01_Hand_of_Nod_Building_Script_JDG when killed
	else if (param == 122)
	{
		this->field_1C = 1;
	}

	// Received from M01_Comm_Center_Building_Script_JDG when killed
	else if (param == 124)
	{
		this->field_20 = 1;
	}
	else if (param == 130)
	{
		this->field_24 = 0;
	}
	else if (param == 132)
	{
		this->field_24 = 1;
	}

	// Received from M01_mission_Controller_JDG when param 141 is received
	else if (param == 141)
	{
		this->field_24 = 4;

		if (!this->field_20 && !this->field_39)
		{

			Commands->Debug_Message("**********************turning on active Comm Center ambient noise\n");

			this->ccAmbientM01SoundId = Commands->Create_Sound("CC_Ambient_M01", Vector3(0.0f, 0.0f, 0.0f), obj);
			this->field_39 = true;

			float randDelay = Commands->Get_Random(5.0, 10.0);
			Commands->Send_Custom_Event(obj, obj, 0, 105, randDelay);
		}
	}

	// Received by M01_mission_Controller_JDG when param 135 is received
	else if (param == 135)
	{
		this->field_24 = 2;

		if (!this->field_1C && !this->field_39)
		{
			this->field_39 = true;
		}
	}

	// Received from ourselves when param 105 is received
	else if (param == 106)
	{
		if (this->field_24 == 4 && !this->field_20)
		{
			GameObject *starObj = Commands->Get_A_Star(Vector3(0.0f, 0.0f, 0.0f));
			if (starObj)
			{
				Vector3 starPos = Commands->Get_Position(starObj);

				float randXOffset = Commands->Get_Random(-5.0, 5.0);
				float randYOffset = Commands->Get_Random(-5.0, 5.0);

				starPos.X += randXOffset;
				starPos.Y += randYOffset;
				starPos.Z += 3.0f;

				static const char *klaxonSounds[2] = 
				{
					"Klaxon Warning",
					"Klaxon Info"
				};

				Commands->Create_Sound(klaxonSounds[this->klaxonSoundIndex], starPos, obj);

				Commands->Send_Custom_Event(obj, obj, 0, 107, 1.5f);
			}
		}
	}

	// Received from ourselves after 1.5 seconds when param 106 is received or param 105
	else if (param == 107)
	{
		if (this->field_24 == 4 && !this->field_20)
		{
			GameObject *starObj = Commands->Get_A_Star(Vector3(0.0f, 0.0f, 0.0f));
			if (starObj)
			{
				Vector3 starPos = Commands->Get_Position(starObj);

				float randXOffset = Commands->Get_Random(-5.0, 5.0);
				float randYOffset = Commands->Get_Random(-5.0, 5.0);

				starPos.X += randXOffset;
				starPos.Y += randYOffset;
				starPos.Z += 3.0f;

				static const char *ambientSounds[15] =
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

				Commands->Create_Sound(ambientSounds[this->ambientSoundIndex], starPos, obj);

				float randDelay = Commands->Get_Random(10.0f, 25.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 105, randDelay);
			}
		}
	}

	// Received from ourselves after 5 to 10 seconds when param 141 is received or when param 107 is received
	else if (param == 105)
	{
		if (this->field_24 == 4 && !this->field_20)
		{
			float rand = Commands->Get_Random(0.5, 15.5);

			if (rand >= 0.5f && rand < 1.5f)
			{
				this->ambientSoundIndex = 0;

				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				this->ambientSoundIndex = 1;

				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
			else if (rand >= 2.5f && rand < 3.5f)
			{
				this->ambientSoundIndex = 2;

				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
			else if (rand >= 3.5f && rand < 4.5f)
			{
				this->klaxonSoundIndex = 1;
				this->ambientSoundIndex = 3;

				Commands->Send_Custom_Event(obj, obj, 0, 106, 0.0f);
			}
			else if (rand >= 4.5f && rand < 5.5f)
			{
				this->ambientSoundIndex = 4;
				
				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
			else if (rand >= 5.5f && rand < 6.5f)
			{
				this->klaxonSoundIndex = 1;
				this->ambientSoundIndex = 5;

				Commands->Send_Custom_Event(obj, obj, 0, 106, 0.0f);
			}
			else if (rand >= 6.5f && rand < 7.5f)
			{
				this->klaxonSoundIndex = 1;
				this->ambientSoundIndex = 6;
				
				Commands->Send_Custom_Event(obj, obj, 0, 106, 0.0f);
			}
			else if (rand >= 7.5f && rand < 8.5f)
			{
				this->klaxonSoundIndex = 0;
				this->ambientSoundIndex = 7;

				Commands->Send_Custom_Event(obj, obj, 0, 106, 0.0f);
			}
			else if (rand >= 8.5f && rand < 9.5f)
			{
				this->ambientSoundIndex = 8;

				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
			else if (rand >= 9.5f && rand < 10.5f)
			{
				this->klaxonSoundIndex = 0;
				this->ambientSoundIndex = 9;

				Commands->Send_Custom_Event(obj, obj, 0, 106, 0.0f);
			}
			else if (rand >= 10.5f && rand < 11.5f)
			{
				this->klaxonSoundIndex = 0;
				this->ambientSoundIndex = 10;

				Commands->Send_Custom_Event(obj, obj, 0, 106, 0.0f);
			}
			else if (rand >= 11.5f && rand < 12.5f)
			{
				this->klaxonSoundIndex = 1;
				this->ambientSoundIndex = 11;

				Commands->Send_Custom_Event(obj, obj, 0, 106, 0.0f);
			}
			else if (rand >= 12.5f && rand < 13.5f)
			{
				// I did not forget this->ambientSoundIndex = 12 here 

				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
			else if (rand >= 13.5f && rand < 14.5f)
			{
				this->ambientSoundIndex = 13;

				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
			else
			{
				this->ambientSoundIndex = 14;

				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}
		}
	}
}

ScriptRegistrant<M01_Ambient_Sound_Controller_JDG> M01_Ambient_Sound_Controller_JDGRegistrant("M01_Ambient_Sound_Controller_JDG", "");