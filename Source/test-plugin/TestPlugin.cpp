/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#include "General.h"
#include "TestPlugin.h"
#include "engine_tt.h"
#include "engine_io.h"
#include "gmgame.h"

#include <DefinitionMgrClass.h>
#include <RawFileClass.h>

typedef int(*gsc) ();
typedef char *(*gsn) (int);
typedef char *(*gspd) (int);

gsc GetScriptCount;
gsn GetScriptName;
gspd GetScriptParamDescription;

TestPlugin::TestPlugin()
{
	RegisterEvent(EVENT_GLOBAL_INI,this);
	RegisterEvent(EVENT_MAP_INI,this);
	RegisterEvent(EVENT_CHAT_HOOK,this);
	RegisterEvent(EVENT_OBJECT_CREATE_HOOK,this);
	RegisterEvent(EVENT_LOAD_LEVEL_HOOK,this);
	RegisterEvent(EVENT_GAME_OVER_HOOK,this);
	RegisterEvent(EVENT_PLAYER_JOIN_HOOK,this);
	RegisterEvent(EVENT_PLAYER_LEAVE_HOOK,this);
	RegisterEvent(EVENT_REFILL_HOOK,this);
	RegisterEvent(EVENT_POWERUP_PURCHASE_HOOK,this);
	RegisterEvent(EVENT_VEHICLE_PURCHASE_HOOK,this);
	RegisterEvent(EVENT_CHARACTER_PURCHASE_HOOK,this);
	RegisterEvent(EVENT_THINK_HOOK,this);
}

TestPlugin::~TestPlugin()
{
	UnregisterEvent(EVENT_GLOBAL_INI,this);
	UnregisterEvent(EVENT_MAP_INI,this);
	UnregisterEvent(EVENT_CHAT_HOOK,this);
	UnregisterEvent(EVENT_OBJECT_CREATE_HOOK,this);
	UnregisterEvent(EVENT_LOAD_LEVEL_HOOK,this);
	UnregisterEvent(EVENT_GAME_OVER_HOOK,this);
	UnregisterEvent(EVENT_PLAYER_JOIN_HOOK,this);
	UnregisterEvent(EVENT_PLAYER_LEAVE_HOOK,this);
	UnregisterEvent(EVENT_REFILL_HOOK,this);
	UnregisterEvent(EVENT_POWERUP_PURCHASE_HOOK,this);
	UnregisterEvent(EVENT_VEHICLE_PURCHASE_HOOK,this);
	UnregisterEvent(EVENT_CHARACTER_PURCHASE_HOOK,this);
	UnregisterEvent(EVENT_THINK_HOOK,this);
}

void TestPlugin::OnLoadGlobalINISettings(INIClass *SSGMIni)
{
	
}

void TestPlugin::OnFreeData()
{

	
}

void TestPlugin::OnLoadMapINISettings(INIClass *SSGMIni)
{
	
}

void TestPlugin::OnFreeMapData()
{
	
}

bool TestPlugin::OnChat(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID)
{
	return true;
}

void TestPlugin::OnObjectCreate(void *data,GameObject *obj)
{
	
}

void TestPlugin::OnLoadLevel()
{
	HMODULE wwscripts = LoadLibrary("scripts2.dll");

	GetScriptCount = (gsc)GetProcAddress(wwscripts, "Get_Script_Count");
	GetScriptName = (gsn)GetProcAddress(wwscripts, "Get_Script_Name");
	GetScriptParamDescription = (gspd)GetProcAddress(wwscripts, "Get_Script_Param_Description");

	DynamicVectorClass<StringClass> knownScripts;

	RawFileClass files[2] =
	{
		RawFileClass("C:\\Users\\guido\\Documents\\GitHub\\C-C-Renegade-Mission-Scripts\\Source\\Single Player Scripts\\Completed.txt"),
		RawFileClass("C:\\Users\\guido\\Documents\\GitHub\\C-C-Renegade-Mission-Scripts\\Source\\Single Player Scripts\\Uncompleted.txt")
	};

	for (int x = 0; x < 2; x++)
	{
		files[x].Open(1);

		char buffer[512];
		StringClass line;

		int bytesRead = files[x].Read(buffer, sizeof(buffer));
		while (bytesRead != 0)
		{
			for (int y = 0; y < bytesRead; y++)
			{
				if (buffer[y] != '\n' && buffer[y] != '\r')
				{
					line += buffer[y];
				}
				else
				{
					if (!line.Is_Empty())
					{
						line.ToUpper();
						knownScripts.Add(line);

						line = "";
					}
				}
			}

			bytesRead = files[x].Read(buffer, sizeof(buffer));
		}

		if (!line.Is_Empty())
		{
			line.ToUpper();
			knownScripts.Add(line);
		}
	}	

	//Console_Output("%ls\n", Get_Wide_Translated_String(7587));

	//RawFileClass newScriptsFile("C:\\Users\\guido\\Desktop\\newscripts.txt");
	//newScriptsFile.Open(2);

	int scriptCount = GetScriptCount();
	for (int x = 0; x < scriptCount; x++)
	{
		StringClass scriptName = GetScriptName(x);
		StringClass scriptNameUpper = scriptName;

		scriptNameUpper.ToUpper();

		bool isAKnownScript = false;
		for (int y = 0; y < knownScripts.Count(); y++)
		{
			if (knownScripts[y].Compare(scriptNameUpper) == 0)
			{
				isAKnownScript = true;

				break;
			}
		}

		if (!isAKnownScript && (strstr(scriptNameUpper.Peek_Buffer(), "M03")))
		{
			//Console_Output("%s\n", scriptName);

			static char NEW_LINE[] = { '\r', '\n' };

			//newScriptsFile.Write(scriptName.Peek_Buffer(), scriptName.Get_Length() * sizeof(char) + sizeof(char));
			//newScriptsFile.Write(NEW_LINE, sizeof(NEW_LINE));
		}
	}

	for (DefinitionClass *def = DefinitionMgrClass::Get_First(); def; def = DefinitionMgrClass::Get_Next(def))
	{
		/*
		if (strcmp(def->Get_Name(), "Chinook Troop Drop - Beach") == 0)
		{
			Console_Output("%u\n", def->Get_Class_ID());
		}
		*/

		if (def->Get_Class_ID() == 12292)
		{
			Console_Output("%s\n", def->Get_Name());
		}

		// 20480 - sounds
		// 45057 - weapons
		// 12289 - soldiers
		// 12291 - powerups
		// 12301 - spawners
		// 12304 - vehicles
		// 45058 - ammo
		// 45059 - explosions
		// 53249 - buildings

		uint classId = def->Get_Class_ID();

		/*
		switch (classId)
		{
			case 20480:
			case 45057:
			case 12289:
			case 12291:
			case 12301:
			case 12304:
			case 45058:
			case 45059:
			case 53249:
				break;
			default:
				Console_Output("%s: %u\n", def->Get_Name(), classId);

				break;
		}
		*/

		if (classId == 53249)
		{
			DamageableGameObjDef *damageableDef = static_cast<DamageableGameObjDef *>(def);
			int encyId = damageableDef->Get_Encyclopedia_ID();

			if (encyId == 12)
			{
				//Console_Output("%s: %d\n", def->Get_Name(), damageableDef->Get_Encyclopedia_ID());
			}
		}
	}

	FreeLibrary(wwscripts);
}

void TestPlugin::OnGameOver()
{
	
}

void TestPlugin::OnPlayerJoin(int PlayerID,const char *PlayerName)
{
	
}

void TestPlugin::OnPlayerLeave(int PlayerID)
{
	
}

bool TestPlugin::OnRefill(GameObject *purchaser)
{
	return true;
}

int TestPlugin::OnPowerupPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	return -1;
}

int TestPlugin::OnVehiclePurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	return -1;
}

int TestPlugin::OnCharacterPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	return -1;
}

void TestPlugin::OnThink()
{
	
}

TestPlugin testPlugin;

extern "C" __declspec(dllexport) Plugin* Plugin_Init()
{
	return &testPlugin;
}
