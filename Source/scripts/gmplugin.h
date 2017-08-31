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
#ifndef TT_INCLUDE_GMPLUGIN_H
#define TT_INCLUDE_GMPLUGIN_H
#include "scripts.h"
#include "engine.h"
#define INTERFACE_VERSION 4.5f

enum EventType {
	EVENT_GLOBAL_INI = 0,
	EVENT_MAP_INI,
	EVENT_CHAT_HOOK,
	EVENT_OBJECT_CREATE_HOOK,
	EVENT_LOAD_LEVEL_HOOK,
	EVENT_GAME_OVER_HOOK,
	EVENT_PLAYER_JOIN_HOOK,
	EVENT_PLAYER_LEAVE_HOOK,
	EVENT_REFILL_HOOK,
	EVENT_POWERUP_PURCHASE_HOOK,
	EVENT_VEHICLE_PURCHASE_HOOK,
	EVENT_CHARACTER_PURCHASE_HOOK,
	EVENT_THINK_HOOK,
	EVENT_RADIO_HOOK,
	EVENT_STOCK_DAMAGE_HOOK,
	EVENT_TT_DAMAGE_HOOK,
	EVENT_PRE_LOAD_LEVEL_HOOK,
	EVENT_COUNT
};

class Plugin {
public:
	virtual ~Plugin() {};
	virtual float Get_Version() {return INTERFACE_VERSION;} //returns the version of the SSGM interface this plugin is intended to work with
	virtual void OnLoadGlobalINISettings(INIClass *SSGMIni) {}; //called when ssgm.ini is parsed to read global settings
	virtual void OnFreeData() {}; //called when data allocated for global settings is freed (i.e. on shutdown and before global settings are re-loaded)
	virtual void OnLoadMapINISettings(INIClass *SSGMIni) {}; //called when ssgm.ini is parsed to read per-map settings
	virtual void OnFreeMapData() {}; //called when data allocated for per-map settings is freed (i.e. on shutdown and before per-map settings are re-loaded)
	virtual bool OnChat(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID) {return true;}; //called on chat message
	virtual void OnObjectCreate(void *data,GameObject *obj) {}; //called on object create
	virtual void OnLoadLevel() {}; //called on level load
	virtual void OnGameOver() {}; //called on game over
	virtual void OnPlayerJoin(int PlayerID,const char *PlayerName) {}; //called on player join
	virtual void OnPlayerLeave(int PlayerID) {}; //called on player leave
	virtual bool OnRefill(GameObject *purchaser) {return true;}; //called on refill
	virtual int OnPowerupPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data) {return -1;}; //called on beacon purchase
	virtual int OnVehiclePurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data) {return -1;}; //called on vehicle purchase
	virtual int OnCharacterPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data) {return -1;}; //called on character purchase
	virtual void OnThink() {}; //called once per frame
	virtual bool OnRadioCommand(int PlayerType, int PlayerID, int AnnouncementID, int IconID, AnnouncementEnum AnnouncementType) {return true;} //called on radio command
	virtual bool OnStockDamage(PhysicalGameObj* damager, PhysicalGameObj* target, float damage, uint warheadId) { return true; } //called on damage from clients with version <4.0
	virtual bool OnTtDamage(PhysicalGameObj* damager, PhysicalGameObj* target, const AmmoDefinitionClass* ammo, const char* bone) { return true; } //called on damage from clients with version >=4.0
	virtual void OnPreLoadLevel() {}; //called on level load but before the client is sent any network updates
};

typedef Plugin *(*Plugin_Init_Type) ();
typedef void (*Plugin_Shutdown_Type) ();

#endif
