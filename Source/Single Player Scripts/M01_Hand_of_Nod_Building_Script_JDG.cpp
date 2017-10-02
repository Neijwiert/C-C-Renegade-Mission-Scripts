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
#include "M01_Hand_of_Nod_Building_Script_JDG.h"

/*
M01 -> 153909
*/
void M01_Hand_of_Nod_Building_Script_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->soundId, sizeof(this->soundId), 1);
}

void M01_Hand_of_Nod_Building_Script_JDG::Created(GameObject *obj)
{
	this->soundId = Commands->Create_Sound("m01eval_dsgn0254r1ncxk_snd", Vector3(-195.053f, 533.737f, 10.953f), obj);
}

void M01_Hand_of_Nod_Building_Script_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Stop_Sound(this->soundId, true);

	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 122, 0.0f);

	GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
	Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 122, 0.0f);

	GameObject *nonExistingObj = Commands->Find_Object(103371);
	Commands->Send_Custom_Event(obj, nonExistingObj, 0, 122, 0.0f);

	Commands->Create_Object("POW_Medal_Armor", Vector3(-180.312f, 541.4f, 3.518f));
	Commands->Create_Object("POW_Medal_Health", Vector3(-178.349f, 541.471f, 3.518f));
}

void M01_Hand_of_Nod_Building_Script_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		GameObject *honRepairEngineerObj = Commands->Find_Object(101948);
		if (honRepairEngineerObj)
		{
			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 26, 0.0f);

			if (damager == honRepairEngineerObj)
			{
				float maxHealth = Commands->Get_Max_Health(obj);
				float maxShieldStrength = Commands->Get_Max_Shield_Strength(obj);
				Commands->Set_Health(obj, maxHealth);
				Commands->Set_Shield_Strength(obj, maxShieldStrength);
			}
		}
	}
}

ScriptRegistrant<M01_Hand_of_Nod_Building_Script_JDG> M01_Hand_of_Nod_Building_Script_JDGRegistrant("M01_Hand_of_Nod_Building_Script_JDG", "");