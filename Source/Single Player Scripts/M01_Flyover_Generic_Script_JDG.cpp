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
#include "M01_Flyover_Generic_Script_JDG.h"

// After 1 cinematic frame in x1d_gdipowerupdrop.txt
// After 1 cinematic frame in x1a_solo_orca_03/04/05/06/07/08/14/15/18/19/20/21.txt
// After 1 cinematic frame in x1a_solo_apache_03/04/05/06/07/08/14/15/18/19/20/21.txt
// After 1 cinematic frame in x1a_solo_nod_chinook_03/05/06/07/08/14/15/18/19/20/21.txt
// After 1 cinematic frame in x1a_solo_nodcargo_03/04/05/06/07/08/14/15/18/19.txt
// After 1 cinematic frame in x1a_solo_a10_03/04/05/06/07/08/14/15/18/19/20/21.txt
// After 1 cinematic frame in x1a_solo_gdi_chinook_04/05/06/07/08/14/15/18/19/20/21.txt
void M01_Flyover_Generic_Script_JDG::Created(GameObject *obj)
{
	Commands->Disable_Physical_Collisions(obj);
	Commands->Enable_Hibernation(obj, false);
	Commands->Enable_Cinematic_Freeze(obj, false);

	float maxHealth = Commands->Get_Max_Health(obj);
	float randHealth = Commands->Get_Random(0.050000001f, 1.0f) * maxHealth;
	
	float maxShieldStrength = Commands->Get_Max_Shield_Strength(obj);
	float randMaxShieldStrength = Commands->Get_Random(0.050000001f, 1.0f) * maxShieldStrength;

	Commands->Set_Health(obj, randHealth);
	Commands->Set_Shield_Strength(obj, randMaxShieldStrength);
}

ScriptRegistrant<M01_Flyover_Generic_Script_JDG> M01_Flyover_Generic_Script_JDGRegistrant("M01_Flyover_Generic_Script_JDG", "");