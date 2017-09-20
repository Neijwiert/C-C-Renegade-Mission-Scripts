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
#include "M02_Data_Disk.h"

/*
M02 -> 407528 407529 407530
*/
void M02_Data_Disk::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int diskId = Get_Int_Parameter("Disk_ID");

		Vector3 mapRegionPos;
		int pixelRadius;

		switch (diskId)
		{
			case 1:
				mapRegionPos = Vector3(425.76f, 840.78f, 8.19f);
				pixelRadius = 15;

				break;
			case 2:
				mapRegionPos = Vector3(1016.43f, 885.96f, 35.41f);
				pixelRadius = 30;

				break;
			default:
				mapRegionPos = Vector3(1204.46f, 565.35f, 37.84f);
				pixelRadius = 15;

				break;
		}

		Commands->Clear_Map_Region_By_Pos(mapRegionPos, pixelRadius);

		Vector3 color(0.196f, 0.882f, 0.196f);
		Commands->Set_HUD_Help_Text(7587, color); // Data Link Updated - Satellite Image Enhanced\n
	}
}

ScriptRegistrant<M02_Data_Disk> M02_Data_DiskRegistrant("M02_Data_Disk", "Disk_ID:int");