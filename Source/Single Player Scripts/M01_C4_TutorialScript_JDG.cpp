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
#include "M01_C4_TutorialScript_JDG.h"

/*
M01 -> 1200196 117621 117617 117421
*/
void M01_C4_TutorialScript_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		GameObject *M01DataDiscTextControllerJDGObj = Commands->Find_Object(117188);
		if (M01DataDiscTextControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01DataDiscTextControllerJDGObj, 1, 1, 0.0f);
		}
	}
}

ScriptRegistrant<M01_C4_TutorialScript_JDG> M01_C4_TutorialScript_JDGRegistrant("M01_C4_TutorialScript_JDG", "");