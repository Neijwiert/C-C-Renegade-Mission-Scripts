/*
 * A Command & Conquer: Renegade SSGM Plugin, containing all the M13 (or M00) mission scripts
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
#include "M13.h"
#include <engine_tt.h>
#include <engine_io.h>
#include <gmgame.h>

M13::M13()
{
	
}

M13::~M13()
{
	
}

extern "C" __declspec(dllexport) Plugin *Plugin_Init()
{
	static M13 INSTANCE;

	return &INSTANCE;
}
