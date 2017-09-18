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
#pragma once

#include "gmplugin.h"

class TestPlugin : public Plugin
{
	public:
		TestPlugin();
		~TestPlugin();
	
		virtual void OnLoadGlobalINISettings(INIClass *SSGMIni);
		virtual void OnFreeData();
		virtual void OnLoadMapINISettings(INIClass *SSGMIni);
		virtual void OnFreeMapData();
		virtual bool OnChat(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID);
		virtual void OnObjectCreate(void *data,GameObject *obj);
		virtual void OnLoadLevel();
		virtual void OnGameOver();
		virtual void OnPlayerJoin(int PlayerID,const char *PlayerName);
		virtual void OnPlayerLeave(int PlayerID);
		virtual bool OnRefill(GameObject *purchaser);
		virtual int OnPowerupPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
		virtual int OnVehiclePurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
		virtual int OnCharacterPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
		virtual void OnThink();

};
