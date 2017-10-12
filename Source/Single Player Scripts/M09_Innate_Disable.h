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

#pragma once

#include <scripts.h>

/*
M03 -> 1144672 1144673
M09 -> 1201321 2005480 2000893 2000892 2000891 2006877 2006876 2000662 2000660 2000659 2000663 2000661 2005742 2005741 2005740 2005739 2005738 2005737 2000561 2000562 2000557 2000558 2000559 2000564 2000565 2000140 2000142 2000139 2000141 2000136 2000137 2000135 2004492 2004493 2004494 2004497 2000534 2004262 2004495 2000538 2004496 2004519 2004520 2000124 2000125 2000123 2000128 2000129 2000127 2000130 2000133 2000132 2000131 2000570 2000568 2000569 2005475 2005486 2005500 2005506 2000637 2005505 2000640 2006392 2006393 2006394 2006397 2006398 2006399 2000638 1101310 1101311 1101312 1101807 1101808 1101834 1101838 1101839 1101840 1101841 1101842 1101843
*/
class M09_Innate_Disable : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Sound_Heard(GameObject *obj, const CombatSound & sound);

		float newHealth;
		float oldHealth;
		float lastDamage;
};