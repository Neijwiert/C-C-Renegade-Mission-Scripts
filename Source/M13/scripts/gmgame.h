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
#ifndef TT_INCLUDE_GMGAME_H
#define TT_INCLUDE_GMGAME_H
#include "gmplugin.h"
#include "engine_ttdef.h"
class SSGMGameManager {
public:
	static void Init();
	static void Shutdown();
	static bool ChatHook(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID);
	static bool RadioHook(int PlayerType, int PlayerID, int AnnouncementID, int IconID, AnnouncementEnum AnnouncementType);
	static void ObjectCreateHook(void *data,GameObject *obj);
	static void LoadLevelHook();
	static void PreLoadLevelHook();
	static void GameOverHook();
	static void ConsoleOutputHook(const char *output);
	static void LogFileHook(const char *output);
	static bool RefillHook(GameObject *purchaser);
	static void PlayerJoinHook(int PlayerID,const char *PlayerName);
	static void PlayerLeaveHook(int PlayerID);
	static int PowerupPurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
	static int VehiclePurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
	static int CharacterPurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
	static void ReadConfig(INIClass *SSGMIni = NULL);
	static void ReadMapConfig();
	static void FreeData();
	static void FreeMapData();
	static void ThinkHook();
	static bool StockDamageHook(PhysicalGameObj* damager, PhysicalGameObj* target, float damage, uint warheadId);
	static bool TtDamageHook(PhysicalGameObj* damager, PhysicalGameObj* target, const AmmoDefinitionClass* ammo, const char* bone);
};

class SSGM_Random_Weather : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	float Range, CurrentClouds, CurrentRain, CurrentLightning, CurrentSnow, CurrentAsh, CurrentWind, FogEnd;
};

class SSGM_Log_Scores : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class SSGM_Log_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Killed(GameObject *obj, GameObject *shooter);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool triggerrepairing;
	bool killed;
};

class SSGM_Log_Mine : public ScriptImpClass {
	void Destroyed(GameObject *obj);
	void Killed(GameObject *obj, GameObject *shooter);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Created(GameObject *obj);
	bool WasDisarmed;
	bool WasDetonated;
};

class SSGM_Powerup_Expire : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class SSGM_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Drop_DNA_Powerup : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class SSGM_Powerup_SBH : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Havoc : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Deadeye : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_BHSniper : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Sakura : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_SydneyPower : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Raveshaw : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Mobius : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Mendoza : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_GDI_RSoldier : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Nod_RSoldier : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Hotwire : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Technician : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Gunner : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_GDI_Sydney : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_GDI_Officer : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Nod_Officer : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_LCG : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Patch : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class SSGM_Powerup_Chemwar : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};


SCRIPTS_API const char *Get_Translated_Preset_Name_Ex(GameObject *obj);
SCRIPTS_API const char *Get_Translated_Preset_Name_Ini(GameObject *obj);
SCRIPTS_API const char *Get_Translated_Definition_Name_Ini(const char *preset);
SCRIPTS_API void RegisterEvent(EventType evt,Plugin *plugin);
SCRIPTS_API void UnregisterEvent(EventType evt,Plugin *plugin);
SCRIPTS_API bool IsPresetDisabled(int preset);
SCRIPTS_API void Print_C4(GameObject *Player);
SCRIPTS_API extern bool BuildingDeathPages;
SCRIPTS_API extern unsigned int RefillLimit;
SCRIPTS_API extern int WeatherType;
SCRIPTS_API extern int MapWeatherType;
SCRIPTS_API extern int ForceTeam;
SCRIPTS_API extern int MapForceTeam;
SCRIPTS_API extern bool InvincibleBuildings;
SCRIPTS_API extern bool DisableBeacons;
SCRIPTS_API extern bool DisableBaseDefenses;
SCRIPTS_API extern bool DisablePowerPlants;
SCRIPTS_API extern bool DisableRefineries;
SCRIPTS_API extern bool DisableSoldierFactories;
SCRIPTS_API extern bool DisableVehicleFactories;
SCRIPTS_API extern bool DisableRepairPads;
SCRIPTS_API extern bool DisableCommCenters;
SCRIPTS_API extern bool FreePurchases;
SCRIPTS_API extern bool DisableExtraWeapons;
SCRIPTS_API extern bool BuildingDeathPages;
SCRIPTS_API extern bool VehicleOwnership;
SCRIPTS_API extern bool DisableConstructionYards;
SCRIPTS_API extern bool DisableShrines;
SCRIPTS_API extern bool DisableHelipads;
SCRIPTS_API extern bool DisableSpecials;
SCRIPTS_API extern bool DisableTechCenters;
SCRIPTS_API extern bool DisableNavalFactories;
SCRIPTS_API extern bool ExtraKillMessages;
SCRIPTS_API extern bool PlayPowerupSounds;
SCRIPTS_API extern bool PlayQuakeSounds;
SCRIPTS_API extern bool SBHCanPickupDropWeapons;
SCRIPTS_API extern bool CharactersDropDNA;
SCRIPTS_API extern bool ShowExtraMessages;
SCRIPTS_API extern float PowerupExpireTime;
SCRIPTS_API extern float WreckDestroySelfTime;
SCRIPTS_API extern bool VehicleLockIcons;
#endif
