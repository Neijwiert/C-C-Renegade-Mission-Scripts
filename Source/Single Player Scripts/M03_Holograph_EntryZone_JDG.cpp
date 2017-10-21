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
#include "M03_Holograph_EntryZone_JDG.h"

/*
M03 -> 2016366
*/
void M03_Holograph_EntryZone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->starEnteredZone, sizeof(this->starEnteredZone), 1);
}

void M03_Holograph_EntryZone_JDG::Created(GameObject *obj)
{
	this->starEnteredZone = false;
}

void M03_Holograph_EntryZone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 0.25 to 1 second, see entered
	if (param == 8000)
	{
		GameObject *hologramProjector = Commands->Find_Object(2017192);
		if (hologramProjector)
		{
			GameObject *kaneHologramHead = Commands->Create_Object_At_Bone(hologramProjector, "Nod_Kane_HologramHead", "O_ARROW");
			Commands->Attach_To_Object_Bone(kaneHologramHead, hologramProjector, "O_ARROW");
			Commands->Disable_All_Collisions(kaneHologramHead);

			float projectorFacing = Commands->Get_Facing(hologramProjector);
			Commands->Set_Facing(kaneHologramHead, projectorFacing);

			Commands->Set_Loiters_Allowed(kaneHologramHead, false);
			Commands->Attach_Script(kaneHologramHead, "M03_KaneHead_JDG", "");
		}
	}
}

void M03_Holograph_EntryZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos) && !this->starEnteredZone)
	{
		this->starEnteredZone = true;

		float randDelay = Commands->Get_Random(0.25f, 1.0f);
		Commands->Send_Custom_Event(obj, obj, 0, 8000, randDelay);
	}
}

ScriptRegistrant<M03_Holograph_EntryZone_JDG> M03_Holograph_EntryZone_JDGRegistrant("M03_Holograph_EntryZone_JDG", "");