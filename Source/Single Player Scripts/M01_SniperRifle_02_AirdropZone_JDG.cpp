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
#include "M01_SniperRifle_02_AirdropZone_JDG.h"

/*
M01 -> 118832
*/
void M01_SniperRifle_02_AirdropZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		Commands->Create_Sound("M01EVAG_DSGN0268I1EVAG_SND", Vector3(0.0f, 0.0f, 0.0f), obj);

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-12.031f, -42.15f, -1.252f));
		Commands->Set_Facing(invisObj, -45.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_GDIBasePrimaryNodChinook.txt");

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_SniperRifle_02_AirdropZone_JDG> M01_SniperRifle_02_AirdropZone_JDGRegistrant("M01_SniperRifle_02_AirdropZone_JDG", "");