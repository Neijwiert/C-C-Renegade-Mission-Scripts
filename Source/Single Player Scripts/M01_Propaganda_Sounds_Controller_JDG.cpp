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
#include "M01_Propaganda_Sounds_Controller_JDG.h"

/*
M01 -> 103099
*/
void M01_Propaganda_Sounds_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->propagandaScreenObjId, sizeof(this->propagandaScreenObjId), 2);
	Auto_Save_Variable(&this->propagandaSound3Id, sizeof(this->propagandaSound3Id), 3);
	Auto_Save_Variable(&this->movieProjectorDestroyed, sizeof(this->movieProjectorDestroyed), 4);
	Auto_Save_Variable(&this->propagandaIndex, sizeof(this->propagandaIndex), 5);
	Auto_Save_Variable(&this->playingPropaganda, sizeof(this->playingPropaganda), 6);
	Auto_Save_Variable(&this->propagandaSound2Id, sizeof(this->propagandaSound2Id), 7);
	Auto_Save_Variable(&this->propagandaSound1Id, sizeof(this->propagandaSound1Id), 8);
}

void M01_Propaganda_Sounds_Controller_JDG::Created(GameObject *obj)
{
	this->field_1C = 0;
	this->propagandaScreenObjId = 0;
	this->propagandaSound3Id = 0;
	this->propagandaSound2Id = 0;
	this->propagandaSound1Id = 0;
	this->movieProjectorDestroyed = false;
	this->propagandaIndex = 0;
	this->playingPropaganda = false;
}

void M01_Propaganda_Sounds_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from ourselves after 10 seconds when param 16 is received
		if (param == 27)
		{
			if (obj)
			{
				this->playingPropaganda = false;

				GameObject *propagandaScreenObj = Commands->Find_Object(this->propagandaScreenObjId);
				if (propagandaScreenObj)
				{
					Commands->Destroy_Object(propagandaScreenObj);
				}

				Commands->Send_Custom_Event(obj, obj, 0, 16, 2.0f);
			}
		}

		// Received from ourselves after 2 seconds when param 27 is received
		// Received from M01_mission_Controller_JDG when param 201 is received
		else if (param == 16)
		{
			if (!this->movieProjectorDestroyed)
			{
				static const char *propagandaScreens[10] =
				{
					"M01_Propaganda_Screen_01",
					"M01_Propaganda_Screen_02",
					"M01_Propaganda_Screen_03",
					"M01_Propaganda_Screen_04",
					"M01_Propaganda_Screen_05",
					"M01_Propaganda_Screen_06",
					"M01_Propaganda_Screen_07",
					"M01_Propaganda_Screen_08",
					"M01_Propaganda_Screen_10",
					"M01_Propaganda_Screen_11"
				};

				static const char *propagandaSounds[10] =
				{
					"M01NCXS_DSGN0270I1NCXS_SND",
					"M01NCXS_DSGN0271I1NCXS_SND",
					"M01NCXS_DSGN0272I1NCXS_SND",
					"M01NCXS_DSGN0273I1NCXS_SND",
					"M01NCXS_DSGN0279I1NCXS_SND",
					"M01NCXS_DSGN0275I1NCXS_SND",
					"M01NCXS_DSGN0276I1NCXS_SND",
					"M01NCXS_DSGN0277I1NCXS_SND",
					"M01NCXS_DSGN0278I1NCXS_SND",
					"M01NCXS_DSGN0274I1NCXS_SND"
				};

				if (++this->propagandaIndex > 9)
				{
					this->propagandaIndex = 0;
				}

				GameObject *propagandaScreenObj = Commands->Create_Object(propagandaScreens[this->propagandaIndex], Vector3(0.0f, 0.0f, 0.0f));
				this->propagandaScreenObjId = Commands->Get_ID(propagandaScreenObj);

				this->propagandaSound1Id = Commands->Create_Sound(propagandaSounds[this->propagandaIndex], Vector3(-319.283f, 589.265f, 35.385f), obj);
				this->propagandaSound2Id = Commands->Create_Sound(propagandaSounds[this->propagandaIndex], Vector3(-277.73f, 581.935f, 35.724f), obj);
				this->propagandaSound3Id = Commands->Create_Sound(propagandaSounds[this->propagandaIndex], Vector3(-296.269f, 612.473f, 35.468f), obj);

				this->playingPropaganda = true;

				Commands->Send_Custom_Event(obj, obj, 0, 27, 10.0f);
			}
		}

		// Received from M01_MovieProjector_JDG when damaged by star
		// Received from M01_mission_Controller_JDG when param 5 is received
		else if (param == 22)
		{
			this->movieProjectorDestroyed = true;

			GameObject *propagandaScreenObj = Commands->Find_Object(this->propagandaScreenObjId);
			if (propagandaScreenObj)
			{
				Commands->Destroy_Object(propagandaScreenObj);
			}

			if (this->playingPropaganda)
			{
				Commands->Stop_Sound(this->propagandaSound3Id, true);
				Commands->Stop_Sound(this->propagandaSound2Id, true);
				Commands->Stop_Sound(this->propagandaSound1Id, true);
			}

			Commands->Destroy_Object(obj);
		}
	}
}

ScriptRegistrant<M01_Propaganda_Sounds_Controller_JDG> M01_Propaganda_Sounds_Controller_JDGRegistrant("M01_Propaganda_Sounds_Controller_JDG", "");