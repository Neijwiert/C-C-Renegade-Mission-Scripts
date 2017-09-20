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
#include "MX0_Explosive_Barrels_DLS.h"

/*
M13 -> 1500099 1500098 1500096 1500095 1500094 1500016 1500015
*/
void MX0_Explosive_Barrels_DLS::Register_Auto_Save_Variables()
{

}

void MX0_Explosive_Barrels_DLS::Created(GameObject *obj)
{

}

void MX0_Explosive_Barrels_DLS::Killed(GameObject *obj, GameObject *killer)
{
	float radius = Get_Float_Parameter("Radius");

	Vector3 pos = Commands->Get_Position(obj);
	Commands->Create_Explosion("Air Explosions Twiddler", pos, obj);

	int logicalSound = Get_Int_Parameter("Logical_Sound");
	Commands->Create_Logical_Sound(obj, logicalSound, pos, radius);
}

void MX0_Explosive_Barrels_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{

}

void MX0_Explosive_Barrels_DLS::Sound_Heard(GameObject *obj, const CombatSound &sound)
{
	if (sound.sound == Get_Int_Parameter("Logical_Sound"))
	{
		Commands->Apply_Damage(obj, 50000.0f, "STEEL", NULL);
	}
}

ScriptRegistrant<MX0_Explosive_Barrels_DLS> MX0_Explosive_Barrels_DLSRegistrant("MX0_Explosive_Barrels_DLS", "Logical_Sound=0:int, Radius:float");