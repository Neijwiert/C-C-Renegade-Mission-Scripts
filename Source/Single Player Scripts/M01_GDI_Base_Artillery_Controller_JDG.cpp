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
#include "M01_GDI_Base_Artillery_Controller_JDG.h"

/*
M01 -> 102294
*/
void M01_GDI_Base_Artillery_Controller_JDG::Register_Auto_Save_Variables()
{
	// Yes, explosionLocations is not saved. This means that when the map is save/loaded it will be broken?
	Auto_Save_Variable(&this->explosionLocation, sizeof(this->explosionLocation), 1);
	Auto_Save_Variable(&this->explosionLocationIndex, sizeof(this->explosionLocationIndex), 2);
	Auto_Save_Variable(&this->soundId, sizeof(this->soundId), 3);
	Auto_Save_Variable(&this->canDoExplosions, sizeof(this->canDoExplosions), 4);
	Auto_Save_Variable(&this->minExplosionInterval, sizeof(this->minExplosionInterval), 5);
	Auto_Save_Variable(&this->maxExplosionInterval, sizeof(this->maxExplosionInterval), 6);
}

void M01_GDI_Base_Artillery_Controller_JDG::Created(GameObject *obj)
{
	this->explosionLocations[0] = Vector3(96.983f, -75.433f, -7.894f);
	this->explosionLocations[1] = Vector3(64.778f, -70.692f, -7.917f);
	this->explosionLocations[2] = Vector3(39.923f, -55.085f, -6.986f);
	this->explosionLocations[3] = Vector3(64.088f, -51.238f, -1.13f);
	this->explosionLocations[4] = Vector3(90.243f, -42.054f, 7.82f);
	this->explosionLocations[5] = Vector3(101.932f, -22.724f, 3.892f);
	this->explosionLocations[6] = Vector3(110.26f, 6.87f, 11.455f);
	this->explosionLocations[7] = Vector3(104.993f, 34.865f, 7.102f);
	this->explosionLocations[8] = Vector3(88.132f, 31.827f, 9.156f);
	this->explosionLocations[9] = Vector3(82.538f, -2.162f, 7.26f);
	this->explosionLocations[10] = Vector3(73.697f, 10.886f, 10.622f);
	this->explosionLocations[11] = Vector3(61.373f, -9.328f, 9.156f);
	this->explosionLocations[12] = Vector3(45.354f, -11.552f, 7.243f);
	this->explosionLocations[13] = Vector3(53.74f, -1.58f, 13.105f);
	this->explosionLocations[14] = Vector3(53.059f, 17.069f, 13.105f);
	this->explosionLocations[15] = Vector3(24.016f, 8.417f, 7.113f);
	this->explosionLocations[16] = Vector3(22.19f, -4.718f, 10.245f);
	this->explosionLocations[17] = Vector3(1.074f, -48.981f, -1.604f);
	this->explosionLocations[18] = Vector3(-17.956f, -48.856f, 3.441f);
	this->explosionLocations[19] = Vector3(-46.404f, -26.827f, 6.827f);
	this->explosionLocations[20] = Vector3(-40.934f, -20.838f, 2.387f);
	this->explosionLocations[21] = Vector3(9.578f, -17.054f, 7.419f);
	this->explosionLocations[22] = Vector3(-6.474f, -17.783f, 5.052f);
	this->explosionLocations[23] = Vector3(-8.36f, -6.935f, 11.862f);
	this->explosionLocations[24] = Vector3(5.836f, -4.874f, 17.664f);
	this->explosionLocations[25] = Vector3(-40.13f, -1.333f, 6.052f);
	this->explosionLocations[26] = Vector3(-43.953f, 5.812f, 14.117f);
	this->explosionLocations[27] = Vector3(-50.261f, 32.361f, 5.41f);
	this->explosionLocations[28] = Vector3(-50.813f, 48.439f, 11.585f);
	this->explosionLocations[29] = Vector3(-40.847f, 60.449f, 5.454f);
	this->explosionLocations[30] = Vector3(-36.209f, 68.892f, 14.059f);
	this->explosionLocations[31] = Vector3(-14.355f, 40.254f, 3.19f);
	this->explosionLocations[32] = Vector3(9.023f, 39.898f, 4.112f);
	this->explosionLocations[33] = Vector3(-5.525f, 41.241f, 12.043f);
	this->explosionLocations[34] = Vector3(24.496f, 52.051f, 9.968f);

	this->explosionLocationIndex = 40;
	this->minExplosionInterval = 20.0f;
	this->maxExplosionInterval = 30.0f;
	this->canDoExplosions = true;

	Commands->Send_Custom_Event(obj, obj, 0, 67, 2.0f);
}

// TODO
void M01_GDI_Base_Artillery_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_SOUND_ENDED)
	{
		if (param == this->soundId)
		{
			Commands->Create_Explosion("Ground Explosions Twiddler", Vector3(this->explosionLocation.X, this->explosionLocation.Y, this->explosionLocation.Z - 10.0f), NULL);
			
			float randInterval = Commands->Get_Random(this->minExplosionInterval, this->maxExplosionInterval);
			Commands->Send_Custom_Event(obj, obj, 0, 67, randInterval);
		}
	}
	else
	{
		// Received from M01_GDI_GuardTower_NOD_Commander_JDG when killed
		if (param == 24)
		{
			this->canDoExplosions = false;

			if (sender == Commands->Find_Object(102360)) // Entrance barn area tower Nod officer
			{
				Commands->Destroy_Object(obj);
			}
		}

		// Received from ourselves after 2 seconds on create or when explosion sound is complete or param 16 is received
		else if (param == 67)
		{
			if (this->canDoExplosions)
			{
				int randomExplosionLocationIndex = static_cast<int>(Commands->Get_Random(0.0f, 34.999901f));
				while (this->explosionLocationIndex == randomExplosionLocationIndex)
				{
					randomExplosionLocationIndex = static_cast<int>(Commands->Get_Random(0.0f, 34.999901f));
				}

				this->explosionLocationIndex = randomExplosionLocationIndex;

				this->explosionLocation = Vector3(this->explosionLocations[this->explosionLocationIndex].X, this->explosionLocations[this->explosionLocationIndex].Y, this->explosionLocations[this->explosionLocationIndex].Z + 10.0f);

				this->soundId = Commands->Create_Sound("Bomb_Falling_Sound_Twiddler", this->explosionLocation, obj);
				Commands->Monitor_Sound(obj, this->soundId);
			}
		}
		else if (param == 16)
		{
			this->canDoExplosions = true;

			Commands->Send_Custom_Event(obj, obj, 0, 67, 2.0f);
		}
	}	
}

ScriptRegistrant<M01_GDI_Base_Artillery_Controller_JDG> M01_GDI_Base_Artillery_Controller_JDGRegistrant("M01_GDI_Base_Artillery_Controller_JDG", "");