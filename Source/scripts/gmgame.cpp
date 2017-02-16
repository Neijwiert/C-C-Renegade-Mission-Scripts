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
#include "gmgame.h"
#include "engine.h"
#include "ScriptableGameObj.h"
#include "VehicleGameObjDef.h"
#include "VehicleGameObj.h"
#include "SoldierGameObj.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"
#include "ArmorWarheadManager.h"
#include "HashTemplateIterator.h"
#include "RawFileClass.h"
#include "gmvehicle.h"
#include "gmsoldier.h"
#include "VehicleFactoryGameObj.h"
#include "PowerUpGameObjDef.h"
#include "gmlog.h"
#include "GameObjManager.h"
#include "C4GameObj.h"

SimpleDynVecClass<Plugin *> RegisteredEvents[EVENT_COUNT];
SimpleDynVecClass<Plugin *> Plugins;
SimpleDynVecClass<HINSTANCE> PluginDLLs;

HashTemplateClass<StringClass,DynamicVectorClass<StringClass> *> WeaponGrants;
HashTemplateClass<StringClass,DynamicVectorClass<StringClass> *> WeaponDrops;
char *GDISpawnChar;
char *NodSpawnChar;
DynamicVectorClass<int> DisablePresets;
DynamicVectorClass<int> MapDisablePresets;
HashTemplateClass<StringClass,StringClass> VehicleWreckages;
SCRIPTS_API bool InvincibleBuildings = false;
SCRIPTS_API bool DisableBeacons = false;
SCRIPTS_API bool DisableBaseDefenses = false;
SCRIPTS_API bool DisablePowerPlants = false;
SCRIPTS_API bool DisableRefineries = false;
SCRIPTS_API bool DisableSoldierFactories = false;
SCRIPTS_API bool DisableVehicleFactories = false;
SCRIPTS_API bool DisableRepairPads = false;
SCRIPTS_API bool DisableCommCenters = false;
SCRIPTS_API bool FreePurchases = false;
SCRIPTS_API bool DisableExtraWeapons = false;
SCRIPTS_API bool BuildingDeathPages = false;
SCRIPTS_API bool VehicleOwnership = true;
SCRIPTS_API bool VehicleLockIcons = true;
SCRIPTS_API bool DisableConstructionYards = false;
SCRIPTS_API bool DisableShrines = false;
SCRIPTS_API bool DisableHelipads = false;
SCRIPTS_API bool DisableSpecials = false;
SCRIPTS_API bool DisableTechCenters = false;
SCRIPTS_API bool DisableNavalFactories = false;
bool gamelog = false;
SCRIPTS_API unsigned int RefillLimit = 0;
unsigned int ConfigModTime = 0;
SCRIPTS_API int WeatherType = 0;
SCRIPTS_API int MapWeatherType = 0;
SCRIPTS_API int ForceTeam = -1;
SCRIPTS_API int MapForceTeam = -1;
SCRIPTS_API bool ExtraKillMessages = 0;
SCRIPTS_API float PowerupExpireTime = 10.f;
SCRIPTS_API bool PlayPowerupSounds = 0;
SCRIPTS_API float WreckDestroySelfTime = 60.f;
SCRIPTS_API bool PlayQuakeSounds = 0;
SCRIPTS_API bool SBHCanPickupDropWeapons = 1;
SCRIPTS_API bool CharactersDropDNA = 1;
SCRIPTS_API bool ShowExtraMessages = 1;

SCRIPTS_API bool IsPresetDisabled(int preset)
{
	for (int i = 0;i < DisablePresets.Count();i++)
	{
		if (DisablePresets[i] == preset)
		{
			return true;
		}
	}
	for (int i = 0;i < MapDisablePresets.Count();i++)
	{
		if (MapDisablePresets[i] == preset)
		{
			return true;
		}
	}
	return false;
}

const char *Get_Translated_Preset_Name_Part(GameObject *obj)
{
	if (!obj)
	{
		return newstr("None");
	}
	const char *preset = Get_Translated_Preset_Name_Ini(obj);
	StringClass preset2 = preset;
	if (obj->As_BuildingGameObj() || (obj->As_PhysicalGameObj() && obj->As_PhysicalGameObj()->Peek_Physical_Object() && obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_DecorationPhysClass()) || Is_Harvester_Preset(obj))
	{
		const char *str = Get_Team_Name(Get_Object_Type(obj));
		if (!stristr(preset,str))
		{
			preset2.Format("%s %s",str,preset);
		}
		delete[] str;
	}
	delete[] preset;
	return newstr(preset2);
}

HashTemplateClass<StringClass,StringClass> Translation_Overrides;

SCRIPTS_API const char *Get_Translated_Preset_Name_Ini(GameObject *obj)
{
	if (!obj)
	{
		return newstr("");
	}
	if (obj->As_PhysicalGameObj() && obj->As_PhysicalGameObj()->As_C4GameObj())
	{
		if (Get_C4_Mode(obj) == 1)
		{
			return newstr("Remote C4");
		}
		else if (Get_C4_Mode(obj) == 3)
		{
			return newstr("Proximity C4");
		}
		else if (Get_C4_Mode(obj) == 2)
		{
			return newstr("Timed C4");
		}
	}
	if (!Translation_Overrides.Get(Commands->Get_Preset_Name(obj),0).Is_Empty())
	{
		return newstr(Translation_Overrides.Get(Commands->Get_Preset_Name(obj),0).Peek_Buffer());
	}
	return Get_Translated_Preset_Name(obj);
}

SCRIPTS_API const char *Get_Translated_Definition_Name_Ini(const char *preset)
{
	if (!Translation_Overrides.Get(preset,0).Is_Empty())
	{
		return newstr(Translation_Overrides.Get(preset,0).Peek_Buffer());
	}
	return Get_Translated_Definition_Name(preset);
}

SCRIPTS_API const char *Get_Translated_Preset_Name_Ex(GameObject *obj)
{
	if (!obj)
	{
		return newstr("None");
	}
	if (Commands->Is_A_Star(obj))
	{
		if (Get_Vehicle(obj))
		{
			return Get_Translated_Preset_Name_Part(Get_Vehicle(obj));
		}
		else
		{
			StringClass str;
			const char *str2 = Get_Current_Translated_Weapon(obj);
			const char *str3 = Get_Translated_Preset_Name_Part(obj);
			str.Format("%s/%s",str3,str2);
			delete[] str3;
			delete[] str2;
			return newstr(str);
		}
	}
	return Get_Translated_Preset_Name_Part(obj);
}

SCRIPTS_API void RegisterEvent(EventType evt,Plugin *plugin)
{
	RegisteredEvents[evt].Add(plugin);
}

SCRIPTS_API void UnregisterEvent(EventType evt,Plugin *plugin)
{
	RegisteredEvents[evt].Delete(plugin);
}

void SSGMGameManager::Init()
{
	SSGMGameLog::Log_Message("Loading global SSGM.ini settings\n","_GENERAL");
	INIClass *SSGMIni = 0;
	RawFileClass file("SSGM.ini");
	if (file.Is_Available(0))
	{
		SSGMIni = new INIClass(file);
	}
	
	if (SSGMIni)
	{
		ReadConfig(SSGMIni);
		unsigned short port = (unsigned short)SSGMIni->Get_Int("General","Port",0);
		SSGMGameLog::Init(port);
		int count = SSGMIni->Entry_Count("Plugins");
		for (int i = 0;i < count;i++)
		{
			const char *Entry = SSGMIni->Get_Entry("Plugins",i);
			StringClass pluginName;
			SSGMIni->Get_String(pluginName,"Plugins",Entry);
			HINSTANCE h = LoadLibrary(pluginName.Peek_Buffer());
			if (h)
			{
				Plugin_Init_Type init = (Plugin_Init_Type)GetProcAddress(h,"Plugin_Init");
				if (init)
				{
					Plugin *plugin = init();
					if (plugin && plugin->Get_Version() >= INTERFACE_VERSION)
					{
						StringClass str;
						str.Format("Loaded plugin %s\n",pluginName);
						Console_Output ("[SSGM] Loaded plugin %s\n", pluginName);
						SSGMGameLog::Log_Message(str,"_GENERAL");
						Plugins.Add(plugin);
						PluginDLLs.Add(h);
					}
					else
					{
						StringClass str;
						str.Format("Unable to load plugin %s\n",pluginName);
						Console_Output ("[SSGM] Failed to load plugin %s\n", pluginName);
						SSGMGameLog::Log_Message(str,"_GENERAL");
						FreeLibrary(h);
					}
				}
			}
		}
	}
	AddChatHook(ChatHook);
	AddRadioHook(RadioHook);
	AddStockDamageHook(StockDamageHook);
	AddTtDamageHook(TtDamageHook);
	ObjectCreateHookStruct *h = new ObjectCreateHookStruct;
	h->data = 0;
	h->hook = ObjectCreateHook;
	AddObjectCreateHook(h);
	AddLoadLevelHook(LoadLevelHook);
	AddPreLoadLevelHook(PreLoadLevelHook);
	AddGameOverHook(GameOverHook);
	AddPlayerJoinHook(PlayerJoinHook);
	AddPlayerLeaveHook(PlayerLeaveHook);
	AddPowerupPurchaseHook(PowerupPurchaseHook,0);
	AddVehiclePurchaseHook(VehiclePurchaseHook,0);
	AddCharacterPurchaseHook(CharacterPurchaseHook,0);
	AddThinkHook(ThinkHook);
	AddConsoleOutputHook(ConsoleOutputHook);
	AddLogFileHook(LogFileHook);
	AddRefillHook(RefillHook);
}

void SSGMGameManager::Shutdown()
{
	SSGMGameLog::Shutdown();
	FreeData();
	FreeMapData();
	for (int i = 0;i < Plugins.Count();i++)
	{
		Plugin_Shutdown_Type shutdown = (Plugin_Shutdown_Type)GetProcAddress(PluginDLLs[i],"Plugin_Shutdown");
		if (shutdown)
		{
			shutdown();
		}
		FreeLibrary(PluginDLLs[i]);
	}
}

SCRIPTS_API void Print_C4(GameObject *Player)
{
	int Remote = 0,Prox = 0;
	int Team = Get_Object_Type(Player);
	SLNode<C4GameObj> *x = GameObjManager::C4GameObjList.Head();
	while (x != 0)
	{
		C4GameObj *o3 = x->Data();
		if (o3 != 0)
		{
			if (Get_Object_Type(o3) == Team)
			{
				if (Get_C4_Mode(o3) == 3)
				{
					++Prox;
				}
				else if (Get_C4_Mode(o3) == 1)
				{
					++Remote;
				}
			}
		}
		x = x->Next();
	}
	StringClass msg;
	msg.Format("ppage %d Remote: %d  - Proximity: %d - Total: %d - Limit: %d",Get_Player_ID(Player),Remote,Prox,Remote+Prox,Get_Mine_Limit());
	Console_Input(msg);
}

bool SSGMGameManager::ChatHook(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID)
{
	if (Message[0] == L'!')
	{
		if (VehicleOwnership)
		{
			GameObject *obj = Get_GameObj(PlayerID);
			if (wcsistr(Message,L"!bind") == Message)
			{
				Bind_Vehicle(obj);
				return false;
			}
			if (wcsistr(Message,L"!lock") == Message)
			{
				Lock_Vehicle(obj);
				return false;
			}
			if (wcsistr(Message,L"!bl") == Message)
			{
				Bind_Vehicle(obj);
				Lock_Vehicle(obj);
				return false;
			}
			if (wcsistr(Message,L"!unbind") == Message)
			{
				Unbind_Vehicle(obj);
				return false;
			}
			if (wcsistr(Message,L"!unlock") == Message)
			{
				Unlock_Vehicle(obj);
				return false;
			}
			if (wcsistr(Message,L"!vkick") == Message)
			{
				Kick_Vehicle_Driver(obj);
				return false;
			}
		}
		if (wcsistr(Message,L"!c4") == Message)
		{
			GameObject *Player = Get_GameObj(PlayerID);
			Print_C4(Player);
			return false;
		}
	}
	GameObject *player = Get_GameObj(PlayerID);
	char type[8];
	if (Type ==	0)
	{
		sprintf(type,"PUBLIC");
	}
	else if (Type == 1)
	{
		sprintf(type,"TEAM");
	}
	SSGMGameLog::Log_Gamelog("CHAT;%s;%d;%ls", type, Commands->Get_ID(player), Message);
	for (int i = 0;i < RegisteredEvents[EVENT_CHAT_HOOK].Count();i++)
	{
		if (!RegisteredEvents[EVENT_CHAT_HOOK][i]->OnChat(PlayerID,Type,Message,recieverID))
		{
			return false;
		}
	}
	return true;
}

bool SSGMGameManager::RadioHook(int PlayerType, int PlayerID, int AnnouncementID, int IconID, AnnouncementEnum AnnouncementType)
{
	for (int i = 0;i < RegisteredEvents[EVENT_RADIO_HOOK].Count();i++)
	{
		if (!RegisteredEvents[EVENT_RADIO_HOOK][i]->OnRadioCommand(PlayerType,PlayerID,AnnouncementID,IconID,AnnouncementType))
		{
			return false;
		}
	}
	return true;
}

RENEGADE_FUNCTION
void Destroy_Pending_Scripts()
AT2(0x006A83D0,0x006A7C70);

void SSGMGameManager::ObjectCreateHook(void *data,GameObject *obj)
{
	if (obj->As_BuildingGameObj())
	{
		Attach_Script_Once(obj,"SSGM_Building","");
	} 
	else if (obj->As_SoldierGameObj())
	{
		if (DisableExtraWeapons)
		{
			const SimpleDynVecClass<GameObjObserverClass *> &observers = obj->Get_Observers();
			SimpleDynVecClass<ScriptImpClass *> deleteobservers;
			for (int i = 0;i < observers.Count();i++)
			{
				if (!_stricmp(observers[i]->Get_Name(),"M00_GrantPowerup_Created"))
				{
					deleteobservers.Add((ScriptImpClass *)observers[i]);
				}
			}
			for (int i = 0;i < deleteobservers.Count();i++)
			{
				deleteobservers[i]->Destroy_Script();
			}
			Destroy_Pending_Scripts();
		}
		Attach_Script_Once(obj,"SSGM_Soldier","");
	}
	else if (obj->As_VehicleGameObj())
	{
		Attach_Script_Once(obj,"SSGM_Vehicle","");
		if (DisableRefineries && Is_Harvester_Preset(obj))
		{
			Commands->Apply_Damage(obj,99999,"Death",0);
		}
	}
	else if (obj->As_PhysicalGameObj() && obj->As_PhysicalGameObj()->As_PowerUpGameObj())
	{
		if (PlayPowerupSounds)
		{
			Attach_Script_Once(obj,"SSGM_Powerup","");
		}
	}
	else if (obj->As_PhysicalGameObj() && obj->As_PhysicalGameObj()->As_BeaconGameObj())
	{
		Attach_Script_Once(obj,"SSGM_Log_Beacon","");
	}
	else if (obj->As_PhysicalGameObj() && obj->As_PhysicalGameObj()->As_C4GameObj())
	{
		Attach_Script_Once(obj,"SSGM_Log_Mine","");
	}
	for (int i = 0;i < RegisteredEvents[EVENT_OBJECT_CREATE_HOOK].Count();i++)
	{
		RegisteredEvents[EVENT_OBJECT_CREATE_HOOK][i]->OnObjectCreate(data,obj);
	}
}

bool SSGMGameManager::StockDamageHook(PhysicalGameObj* damager, PhysicalGameObj* target, float damage, uint warheadId)
{
	for (int i = 0;i < RegisteredEvents[EVENT_STOCK_DAMAGE_HOOK].Count();i++)
	{
		if (!RegisteredEvents[EVENT_STOCK_DAMAGE_HOOK][i]->OnStockDamage(damager, target, damage, warheadId))
		{
			return false;
		}
	}
	return true;
}

bool SSGMGameManager::TtDamageHook(PhysicalGameObj* damager, PhysicalGameObj* target, const AmmoDefinitionClass* ammo, const char* bone)
{
	for (int i = 0;i < RegisteredEvents[EVENT_TT_DAMAGE_HOOK].Count();i++)
	{
		if (!RegisteredEvents[EVENT_TT_DAMAGE_HOOK][i]->OnTtDamage(damager, target, ammo, bone))
		{
			return false;
		}
	}
	return true;
}

void SSGMGameManager::FreeMapData()
{
	DisablePresets.Clear();
	MapDisablePresets.Clear();
	MapWeatherType = 0;
	for (int i = 0;i < RegisteredEvents[EVENT_MAP_INI].Count();i++)
	{
		RegisteredEvents[EVENT_MAP_INI][i]->OnFreeMapData();
	}
}

void SSGMGameManager::FreeData()
{
	for (HashTemplateIterator<StringClass, DynamicVectorClass<StringClass>*> iter(WeaponGrants); iter; ++iter)
	{
		delete iter.getValue();
	}
	WeaponGrants.Remove_All();
	for (HashTemplateIterator<StringClass, DynamicVectorClass<StringClass>*> iter(WeaponDrops); iter; ++iter)
	{
		delete iter.getValue();
	}
	WeaponDrops.Remove_All();
	VehicleWreckages.Remove_All();
	if (GDISpawnChar)
	{
		delete[] GDISpawnChar;
	}
	if (NodSpawnChar)
	{
		delete[] NodSpawnChar;
	}
	for (int i = 0;i < RegisteredEvents[EVENT_GLOBAL_INI].Count();i++)
	{
		RegisteredEvents[EVENT_GLOBAL_INI][i]->OnFreeData();
	}
}

void SSGMGameManager::ReadConfig(INIClass *SSGMIni)
{
	// On init the ini is already open so no need to open it again. Otherwise we
	// can go ahead and open it ourselves
	bool freeIni = false;
	if ( SSGMIni == NULL )
	{
		RawFileClass file("SSGM.ini");
		if (file.Is_Available(0))
		{
			SSGMIni = new INIClass(file);
		}
		freeIni = true;
	}

	if (SSGMIni)
	{
		InvincibleBuildings = SSGMIni->Get_Bool("General","InvincibleBuildings",false);
		ForceTeam = SSGMIni->Get_Int("General","ForceTeam",-1);
		RefillLimit = SSGMIni->Get_Int("General","RefillLimit",0);
		DisableBeacons = SSGMIni->Get_Bool("General","DisableBeacons",false);
		DisableBaseDefenses = SSGMIni->Get_Bool("General","DisableBaseDefenses",false);
		DisablePowerPlants = SSGMIni->Get_Bool("General","DisablePowerPlants",false);
		DisableRefineries = SSGMIni->Get_Bool("General","DisableRefineries",false);
		DisableSoldierFactories = SSGMIni->Get_Bool("General","DisableSoldierFactories",false);
		DisableVehicleFactories = SSGMIni->Get_Bool("General","DisableVehicleFactories",false);
		DisableRepairPads = SSGMIni->Get_Bool("General","DisableRepairPads",false);
		DisableCommCenters = SSGMIni->Get_Bool("General","DisableCommCenters",false);
		DisableConstructionYards = SSGMIni->Get_Bool("General","DisableConstructionYards",false);
		DisableShrines = SSGMIni->Get_Bool("General","DisableShrines",false);
		DisableHelipads = SSGMIni->Get_Bool("General","DisableHelipads",false);
		DisableSpecials = SSGMIni->Get_Bool("General","DisableSpecials",false);
		DisableTechCenters = SSGMIni->Get_Bool("General","DisableTechCenters",false);
		DisableNavalFactories = SSGMIni->Get_Bool("General","DisableNavalFactories",false);
		FreePurchases = SSGMIni->Get_Bool("General","FreePurchases",false);
		DisableExtraWeapons = SSGMIni->Get_Bool("General","DisableExtraWeapons",false);
		BuildingDeathPages = SSGMIni->Get_Bool("General","BuildingDeathPages",false);
		VehicleOwnership = SSGMIni->Get_Bool("General","VehicleOwnership",true);
		VehicleLockIcons = SSGMIni->Get_Bool("General","VehicleLockIcons",true);
		gamelog = SSGMIni->Get_Bool("General","EnableGamelog",false);
		ExtraKillMessages = SSGMIni->Get_Bool("General","ExtraKillMessages",false);
		PowerupExpireTime = SSGMIni->Get_Float("General","PowerupExpireTime",10.0f);
		PlayPowerupSounds = SSGMIni->Get_Bool("General","PlayPowerupSounds",false);
		PlayQuakeSounds = SSGMIni->Get_Bool("General","PlayQuakeSounds",false);
		SBHCanPickupDropWeapons = SSGMIni->Get_Bool("General","SBHCanPickupDropWeapons",true);
		CharactersDropDNA = SSGMIni->Get_Bool("General","CharactersDropDNA",true);
		ShowExtraMessages = SSGMIni->Get_Bool("General","ShowExtraMessages",true);
		WreckDestroySelfTime = SSGMIni->Get_Float("General","WreckDestroySelfTime",60.0f);
		StringClass Weather;
		SSGMIni->Get_String(Weather,"General","WeatherType","None");
		if (Weather == "None")
		{
			WeatherType = 0;
		}
		else if (Weather == "Snow")
		{
			WeatherType = 1;
		}
		else if (Weather == "Ash")
		{
			WeatherType = 2;
		}
		else if (Weather == "Rain")
		{
			WeatherType = 3;
		}
		int count = SSGMIni->Entry_Count("WeaponGrant");
		for (int i = 0;i < count;i++)
		{
			const char *Entry = SSGMIni->Get_Entry("WeaponGrant",i);
			StringClass weapons;
			SSGMIni->Get_String(weapons,"WeaponGrant",Entry);
			DynamicVectorClass<StringClass> *vector = new DynamicVectorClass<StringClass>;
			for (const char* token = strtok(weapons.Peek_Buffer(), ","); token; token = strtok(NULL, ","))
			{
				vector->Add(token);
			}
			WeaponGrants.Insert(Entry,vector);
		}
		int countx = SSGMIni->Entry_Count("WeaponDrop");
		for (int i = 0;i < countx;i++)
		{
			const char *Entry = SSGMIni->Get_Entry("WeaponDrop",i);
			StringClass weapons;
			SSGMIni->Get_String(weapons,"WeaponDrop",Entry);
			DynamicVectorClass<StringClass> *vector = new DynamicVectorClass<StringClass>;
			for (const char* token = strtok(weapons.Peek_Buffer(), ","); token; token = strtok(NULL, ","))
			{
				vector->Add(token);
			}
			WeaponDrops.Insert(Entry,vector);
		}
		int count2 = SSGMIni->Entry_Count("VehicleWreckage");
		for (int i = 0;i < count2;i++)
		{
			const char *Entry = SSGMIni->Get_Entry("VehicleWreckage",i);
			StringClass weapons;
			SSGMIni->Get_String(weapons,"VehicleWreckage",Entry);
			VehicleWreckages.Insert(Entry,weapons);
		}
		int count3 = SSGMIni->Entry_Count("Translation_Overrides");
		for (int i = 0;i < count3;i++)
		{
			const char *Entry = SSGMIni->Get_Entry("Translation_Overrides",i);
			StringClass translation;
			SSGMIni->Get_String(translation,"Translation_Overrides",Entry);
			Translation_Overrides.Insert(Entry,translation);
		}
		for (int i = 0;i < RegisteredEvents[EVENT_GLOBAL_INI].Count();i++)
		{
			RegisteredEvents[EVENT_GLOBAL_INI][i]->OnLoadGlobalINISettings(SSGMIni);
		}
	}
	if ( freeIni )
		Release_INI(SSGMIni);
}

void SSGMGameManager::ReadMapConfig()
{
	if (GDISpawnChar)
	{
		delete[] GDISpawnChar;
	}
	if (NodSpawnChar)
	{
		delete[] NodSpawnChar;
	}
	INIClass *SSGMIni = 0;
	RawFileClass file("SSGM.ini");
	if (file.Is_Available(0))
	{
		SSGMIni = new INIClass(file);
	}
	if (SSGMIni)
	{
		StringClass GDISpawnCharx;
		StringClass NodSpawnCharx;
		SSGMIni->Get_String(GDISpawnCharx,"General","GDISpawnChar",Get_GDI_Soldier_Name());
		SSGMIni->Get_String(NodSpawnCharx,"General","NodSpawnChar",Get_Nod_Soldier_Name());
		GDISpawnChar = newstr(GDISpawnCharx);
		NodSpawnChar = newstr(NodSpawnCharx);
		int count2 = SSGMIni->Entry_Count("PresetDisable");
		for (int i = 0;i < count2;i++)
		{
			const char *Entry = SSGMIni->Get_Entry("PresetDisable",i);
			DisablePresets.Add(Get_Definition_ID(Entry));
		}
		char *s = newstr(The_Game()->Get_Map_Name());
		_strlwr(s);
		StringClass str = s;
		str += "_PresetDisable";
		int count = SSGMIni->Entry_Count(str);
		for (int i = 0;i < count;i++)
		{
			const char *Entry = SSGMIni->Get_Entry(str,i);
			MapDisablePresets.Add(Get_Definition_ID(Entry));
		}
		MapForceTeam = SSGMIni->Get_Int(s,"ForceTeam",-1);
		if (MapForceTeam == -1)
		{
			MapForceTeam = ForceTeam;
		}
		StringClass Weather;
		SSGMIni->Get_String(Weather,s,"WeatherType","None");
		if (Weather == "None")
		{
			MapWeatherType = WeatherType;
		}
		else if (Weather == "Snow")
		{
			MapWeatherType = 1;
		}
		else if (Weather == "Ash")
		{
			MapWeatherType = 2;
		}
		else if (Weather == "Rain")
		{
			MapWeatherType = 3;
		}
		if (MapWeatherType)
		{
			GameObject *weather = Commands->Create_Object("Invisible_Object",Vector3(0,0,0));
			Commands->Attach_Script(weather,"SSGM_Random_Weather","");
		}
		for (int i = 0;i < RegisteredEvents[EVENT_MAP_INI].Count();i++)
		{
			RegisteredEvents[EVENT_MAP_INI][i]->OnLoadMapINISettings(SSGMIni);
		}
		delete[] s;
	}
	else
	{
		GDISpawnChar = newstr(Get_GDI_Soldier_Name());
		NodSpawnChar = newstr(Get_Nod_Soldier_Name());
	}
	Release_INI(SSGMIni);
}

void SSGMGameManager::PreLoadLevelHook()
{
	for (int i = 0;i < RegisteredEvents[EVENT_PRE_LOAD_LEVEL_HOOK].Count();i++)
	{
		RegisteredEvents[EVENT_PRE_LOAD_LEVEL_HOOK][i]->OnPreLoadLevel();
	}
}

void SSGMGameManager::LoadLevelHook()
{
	RawFileClass file("SSGM.ini");
	if (file.Is_Available(0))
	{
		file.Open(1);
		unsigned int time = file.Get_Date_Time();
		file.Close();
		if (time != ConfigModTime)
		{
			ConfigModTime = time;
			SSGMGameLog::Log_Message("Reloading global SSGM.ini settings\n","_GENERAL");
			FreeData();
			ReadConfig();
		}
		SSGMGameLog::Log_Message("Loading per-map SSGM.ini settings\n","_GENERAL");
		FreeMapData();
		ReadMapConfig();
	}
	if (GDISpawnChar) Set_GDI_Soldier_Name(GDISpawnChar);
	if (NodSpawnChar) Set_Nod_Soldier_Name(NodSpawnChar);
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		BuildingGameObj *o = x->Data();
		if (o)
		{
			if (DisablePowerPlants && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_POWER_PLANT)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableSoldierFactories && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_SOLDIER_FACTORY)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableVehicleFactories && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_VEHICLE_FACTORY)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableRefineries && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_REFINERY)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableCommCenters && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_COM_CENTER)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableRepairPads && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_REPAIR_BAY)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableShrines && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_SHRINE)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableHelipads && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_HELIPAD)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableConstructionYards && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_CONYARD)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableBaseDefenses && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_BASE_DEFENSE)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableTechCenters && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_TECH_CENTER)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableNavalFactories && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_NAVAL_FACTORY)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (DisableSpecials && o->Get_Definition().Get_Type() == BuildingConstants::TYPE_SPECIAL)
			{
				Commands->Apply_Damage(o,99999,"Death",0);
			}
			else if (InvincibleBuildings)
			{
				o->Get_Defense_Object()->Set_Shield_Type(ArmorWarheadManager::Get_Armor_Type("Blamo"));
			}
		}
		x = x->Next();
	}
	SLNode<VehicleGameObj> *x2 = GameObjManager::VehicleGameObjList.Head();
	while (x2)
	{
		VehicleGameObj *o = x2->Data();
		if (o)
		{
			if (o)
			{
				if (o->Is_Turret())
				{
					if (DisableBaseDefenses)
					{
						Commands->Apply_Damage(o,99999,"Death",0);
					}
					else if (InvincibleBuildings)
					{
						o->Get_Defense_Object()->Set_Shield_Type(ArmorWarheadManager::Get_Armor_Type("Blamo"));
					}
				}
				else if (Is_Harvester_Preset(o))
				{
					if (DisableRefineries)
					{
						Commands->Apply_Damage(o,99999,"Death",0);
					}
					else if (InvincibleBuildings)
					{
						o->Get_Defense_Object()->Set_Shield_Type(ArmorWarheadManager::Get_Armor_Type("Blamo"));
					}
				}
			}
		}
		x2 = x2->Next();
	}
	SSGMGameLog::Log_Gamelog("2.03;%s",The_Game()->Get_Map_Name());
	SSGMGameLog::Log_Gamelog("CONFIG;%u;%ls",The_Game()->Get_Time_Limit_Minutes(),The_Game()->Get_Game_Title().Peek_Buffer());
	Vector3	loc;
	loc.X =	0.0f;
	loc.Y =	0.0f;
	loc.Z =	0.0f;
	GameObject *gamelog_obj	= Commands->Create_Object("Invisible_Object",loc);
	Commands->Attach_Script(gamelog_obj,"SSGM_Log_Scores","");
	for (int i = 0;i < RegisteredEvents[EVENT_LOAD_LEVEL_HOOK].Count();i++)
	{
		RegisteredEvents[EVENT_LOAD_LEVEL_HOOK][i]->OnLoadLevel();
	}
}

void SSGM_Log_Scores::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,60.0f,1);
}

void SSGM_Log_Scores::Timer_Expired(GameObject *obj,int	number)
{
	if (number == 1)
	{
		Commands->Start_Timer(obj,this,60.0f,1);
		int	x =	The_Game()->Get_Max_Players();
		for	(int j = 1;j <=	x;j++)
		{
			GameObject *player = Get_GameObj(j);
			if (player)
			{
				SSGMGameLog::Log_Gamelog("SCORE;%d;%d;%d",Commands->Get_ID(player), (int)Commands->Get_Points(player), (int)Commands->Get_Money(player));
			}
		}
	}
}

void SSGMGameManager::GameOverHook()
{
	StringClass WinType;
	if (The_Game()->Get_Win_Type() == 3) WinType = "Building Destruction";
	else if (The_Game()->Get_Win_Type() == 2) WinType = "High score when time limit expired";
	else if (The_Game()->Get_Win_Type() == 4) WinType = "Pedestal Beacon";
	else if (The_Game()->Get_Win_Type() == 0) WinType = "Server Shutdown";
	else WinType = "Unknown";
	StringClass str;
	const char *str2 = Get_Team_Name(The_Game()->Get_Winner_ID());
	str.Format("Current game on map %s has ended. Game was won by %s by %s.",The_Game()->Get_Map_Name(),str2,WinType);
	delete[] str2;
	SSGMGameLog::Log_Message(str,"_GENERAL");
	const char *str3 = Get_Team_Name(The_Game()->Get_Winner_ID());
	SSGMGameLog::Log_Gamelog("WIN;%s;%s;%d;%d",str3,WinType,(int)Get_Team_Score(0),(int)Get_Team_Score(1));
	delete[] str3;
	for (int i = 0;i < RegisteredEvents[EVENT_GAME_OVER_HOOK].Count();i++)
	{
		RegisteredEvents[EVENT_GAME_OVER_HOOK][i]->OnGameOver();
	}
}

void SSGMGameManager::PlayerJoinHook(int PlayerID,const char *PlayerName)
{
	for (int i = 0;i < RegisteredEvents[EVENT_PLAYER_JOIN_HOOK].Count();i++)
	{
		RegisteredEvents[EVENT_PLAYER_JOIN_HOOK][i]->OnPlayerJoin(PlayerID,PlayerName);
	}
}

void SSGMGameManager::PlayerLeaveHook(int PlayerID)
{
	for (int i = 0;i < RegisteredEvents[EVENT_PLAYER_LEAVE_HOOK].Count();i++)
	{
		RegisteredEvents[EVENT_PLAYER_LEAVE_HOOK][i]->OnPlayerLeave(PlayerID);
	}
}

bool SSGMGameManager::RefillHook(GameObject *purchaser)
{
	SSGM_Soldier *script = (SSGM_Soldier *)Find_Script_On_Object(purchaser,"SSGM_Soldier");
  if ( script )
  {
	  if (RefillLimit)
	  {
		  if (The_Game()->Get_Game_Duration_S() - script->RefillTime < RefillLimit)
		  {
			  return false;
		  }
	  }
	  script->RefillTime = The_Game()->Get_Game_Duration_S();
  }
	for (int i = 0;i < RegisteredEvents[EVENT_REFILL_HOOK].Count();i++)
	{
		if (!RegisteredEvents[EVENT_REFILL_HOOK][i]->OnRefill(purchaser))
		{
			return false;
		}
	}
	return true;
}

int SSGMGameManager::PowerupPurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	if (IsPresetDisabled(preset) || DisableBeacons)
	{
		return 4;
	}
	StringClass str;
	const char *str2 = Get_Translated_Definition_Name_Ini(Get_Definition_Name(preset));
	const char *str3 = Get_Player_Name(purchaser);
	str.Format("Purchase: %s - %s",str3,str2);
	delete[] str3;
	delete[] str2;
	SSGMGameLog::Log_Message(str,"_PURCHASE");
	bool fp = FreePurchases;
	for (int i = 0;i < RegisteredEvents[EVENT_POWERUP_PURCHASE_HOOK].Count();i++)
	{
		int ret = RegisteredEvents[EVENT_POWERUP_PURCHASE_HOOK][i]->OnPowerupPurchase(base,purchaser,cost,preset,data);
		if (ret == -2)
		{
			fp = true;
		}
		else if (ret != -1)
		{
			return ret;
		}
	}
	if (fp)
	{
		return -2;
	}
	return -1;
}

int SSGMGameManager::VehiclePurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	if (IsPresetDisabled(preset))
	{
		return 4;
	}
	StringClass str;
	const char *str2 = Get_Translated_Definition_Name_Ini(Get_Definition_Name(preset));
	const char *str3 = Get_Player_Name(purchaser);
	str.Format("Purchase: %s - %s",str3,str2);
	delete[] str3;
	delete[] str2;
	SSGMGameLog::Log_Message(str,"_PURCHASE");
	bool fp = FreePurchases;
	for (int i = 0;i < RegisteredEvents[EVENT_VEHICLE_PURCHASE_HOOK].Count();i++)
	{
		int ret = RegisteredEvents[EVENT_VEHICLE_PURCHASE_HOOK][i]->OnVehiclePurchase(base,purchaser,cost,preset,data);
		if (ret == -2)
		{
			fp = true;
		}
		else if (ret != -1)
		{
			return ret;
		}
	}
	if (fp)
	{
		return -2;
	}
	return -1;
}

int SSGMGameManager::CharacterPurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	SSGM_Soldier *script = (SSGM_Soldier *)Find_Script_On_Object(purchaser,"SSGM_Soldier");
  if (script && RefillLimit)
	{
		if (The_Game()->Get_Game_Duration_S() - script->RefillTime < RefillLimit)
		{
			return 4;
		}
  }
	if (IsPresetDisabled(preset))
	{
		return 4;
	}
	StringClass str;
	const char *str2 = Get_Translated_Definition_Name_Ini(Get_Definition_Name(preset));
	const char *str3 = Get_Player_Name(purchaser);
	str.Format("Purchase: %s - %s",str3,str2);
	delete[] str3;
	delete[] str2;
	SSGMGameLog::Log_Message(str,"_PURCHASE");
	bool fp = FreePurchases;
	for (int i = 0;i < RegisteredEvents[EVENT_CHARACTER_PURCHASE_HOOK].Count();i++)
	{
		int ret = RegisteredEvents[EVENT_CHARACTER_PURCHASE_HOOK][i]->OnCharacterPurchase(base,purchaser,cost,preset,data);
		if (ret == -2)
		{
			fp = true;
		}
		else if (ret != -1)
		{
			return ret;
		}
	}
	if (fp)
	{
		return -2;
	}
	return -1;
}

void SSGMGameManager::ConsoleOutputHook(const char *output)
{
	SSGMGameLog::Send_Console(output);
}

void SSGMGameManager::LogFileHook(const char *output)
{
	SSGMGameLog::Log_RenLog(output);
}

void SSGMGameManager::ThinkHook()
{
	SSGMGameLog::Think();
	for (int i = 0;i < RegisteredEvents[EVENT_THINK_HOOK].Count();i++)
	{
		RegisteredEvents[EVENT_THINK_HOOK][i]->OnThink();
	}
}

void SSGM_Random_Weather::Created(GameObject *obj)
{
	Range = Commands->Get_Random(0,1);
	CurrentClouds = Commands->Get_Random(0,1);
	Commands->Set_Clouds(CurrentClouds,CurrentClouds,true);
	Commands->Start_Timer(obj,this,0.5f,0);
	CurrentWind = 0.0f;
}

void SSGM_Random_Weather::Timer_Expired(GameObject *obj, int number)
{
	if (number == 0)
	{
		if (CurrentClouds < Range)
		{
			if ((CurrentClouds + 0.001f) >= Range) CurrentClouds = Range;
			else CurrentClouds += 0.001f;
		}
		else if (CurrentClouds > Range)
		{
			if ((CurrentClouds - 0.001f) <= Range) CurrentClouds = Range;
			else CurrentClouds -= 0.001f;
		}
		Commands->Set_Clouds(CurrentClouds,CurrentClouds,true);
		CurrentWind = 10 * CurrentClouds;
		Commands->Set_Wind(0.0f,CurrentWind,1.0f,true);
		if (MapWeatherType == 1)
		{
			if (CurrentClouds >= 0.625f)
			{
				CurrentSnow = (((8 * CurrentClouds) - 5) / 3);
				Commands->Set_Snow(CurrentSnow,CurrentSnow,true);
			}
			else
			{
				Commands->Set_Snow(0.0f,0.0f,true);
			}
		}
		else if (MapWeatherType == 2)
		{
			if (CurrentClouds >= 0.625f)
			{
				CurrentAsh = (((8 * CurrentClouds) - 5) / 3);
				Commands->Set_Ash(CurrentAsh,CurrentAsh,true);
			}
			else
			{
				Commands->Set_Ash(0.0f,0.0f,true);
			}
		}
		else
		{
			if (CurrentClouds >= 0.625f)
			{
				CurrentRain = ((1.875f * CurrentClouds) - 0.125f);
				Commands->Set_Rain(CurrentRain,CurrentRain,true);
			}
			else
			{
				Commands->Set_Rain(0.0f,0.0f,true);
			}
			if (CurrentClouds >= 0.875f)
			{
				CurrentLightning = (8 * (CurrentClouds + 0.125f)) - 8;
				Commands->Set_Lightning(CurrentLightning,0.0f,1.0f,0.0f,1.0f,true);
			}
			else
			{
				Commands->Set_Lightning(0.0f,0.0f,1.0f,0.0f,1.0f,true);
			}
		}
		if (CurrentClouds == Range)
		{
			Range = Commands->Get_Random(0,1);
			Commands->Start_Timer(obj,this,0.5f,0);
		}
		else
		{
			Commands->Start_Timer(obj,this,0.5f,0);
		}
	}
}

void SSGM_Log_Mine::Created(GameObject *obj)
{
	WasDetonated = false;
	WasDisarmed = false;
	Vector3 pos = Commands->Get_Position(obj);
	SSGMGameLog::Log_Gamelog("CREATED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y),int(pos.X),int(pos.Z), int(Commands->Get_Facing(obj)), int(Commands->Get_Max_Health(obj)), int(Commands->Get_Max_Shield_Strength(obj)), Get_Object_Type(obj),Commands->Get_ID(Get_C4_Planter(obj)));
}

void SSGM_Log_Mine::Destroyed(GameObject *obj)
{
	Vector3	pos = Commands->Get_Position(obj);
	if (!WasDisarmed && WasDetonated)
	{
		StringClass str;
		const char *str4 = Get_Team_Name(Get_Object_Type(obj));
		const char *str5 = Get_Translated_Preset_Name_Ini(obj);
		const char *str6 = Get_Translated_Preset_Name_Ini(Get_C4_Attached(obj));
		str.Format("%s %s has detonated (Owner: %ls - Attached to: %s)",str4,str5,Get_Wide_Player_Name(Get_C4_Planter(obj)),str6);
		delete[] str4;
		delete[] str5;
		delete[] str6;
		SSGMGameLog::Log_Message(str,"_C4");
		SSGMGameLog::Log_Gamelog("DETONATED;OBJECT;%d;%s;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y), int(pos.X), int(pos.Z));
	}
	SSGMGameLog::Log_Gamelog("DESTROYED;OBJECT;%d;%s;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y), int(pos.X), int(pos.Z));
}

void SSGM_Log_Mine::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (amount == 0 && damager == Get_C4_Planter(obj))
	{
		WasDetonated = true;
	}
	if (amount != 0)
	{
		Vector3	victimpos =	Commands->Get_Position(obj);
		Vector3	damagerpos = Commands->Get_Position(damager);
		SSGMGameLog::Log_Gamelog("DAMAGED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%f;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y),	int(victimpos.X), int(victimpos.Z),	int(Commands->Get_Facing(obj)),	Commands->Get_ID(damager), Commands->Get_Preset_Name(damager), int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z),	int(Commands->Get_Facing(damager)),	amount,	int(Commands->Get_Health(obj)),	int(Commands->Get_Shield_Strength(obj)));
	}
}

void SSGM_Log_Mine::Killed(GameObject *obj, GameObject *killer)
{
	WasDisarmed = true;
	Vector3	victimpos =	Commands->Get_Position(obj);
	Vector3	damagerpos = Commands->Get_Position(killer);
	const char *str = Get_Translated_Preset_Name_Ex(obj);
	const char *str2 = Get_Translated_Preset_Name_Ex(killer);
	SSGMGameLog::Log_Gamelog("KILLED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s",Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(killer),	Commands->Get_Preset_Name(killer),	int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(killer)),Get_Current_Weapon(killer),str,str2);
	StringClass str3;
	const char *str4 = Get_Team_Name(Get_Object_Type(obj));
	const char *str5 = Get_Translated_Preset_Name_Ini(obj);
	const char *str6 = Get_Translated_Preset_Name_Ini(Get_C4_Attached(obj));
	str3.Format("%s %s has been disarmed by %ls (Owner: %ls - Attached to: %s)",str4,str5,Get_Wide_Player_Name(killer),Get_Wide_Player_Name(Get_C4_Planter(obj)),str6);
	delete[] str4;
	delete[] str5;
	delete[] str6;
	SSGMGameLog::Log_Message(str3,"_C4");
	delete[] str;
	delete[] str2;

	if (ShowExtraMessages)
	{
		if (Commands->Get_Player_Type(killer) == 0)
		{
			char disarmer[250];
			const char *s1 = Get_Translated_Preset_Name_Ex(obj);
			sprintf(disarmer,"%ls disarmed a %s",Get_Wide_Player_Name(killer),s1);
			delete[] s1;
			Send_Message_Team(0,104,234,40,disarmer);
		}
		else if (Commands->Get_Player_Type(killer) == 1)
		{
			char disarmer[250];
			const char *s1 = Get_Translated_Preset_Name_Ex(obj);
			sprintf(disarmer,"%ls disarmed a %s",Get_Wide_Player_Name(killer),s1);
			delete[] s1;
			Send_Message_Team(1,104,234,40,disarmer);
		}
	}
}

void SSGM_Log_Beacon::Created(GameObject *obj)
{
	killed = false;
	triggerrepairing = true;
	Vector3	pos	= Commands->Get_Position(obj);
	SSGMGameLog::Log_Gamelog("CREATED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y),int(pos.X),int(pos.Z),	int(Commands->Get_Facing(obj)),	int(Commands->Get_Max_Health(obj)),int(Commands->Get_Max_Shield_Strength(obj)),	Commands->Get_Player_Type(obj),Commands->Get_ID(Get_Beacon_Planter(obj)));
}

void SSGM_Log_Beacon::Destroyed(GameObject *obj)
{
	if (!killed)
	{
		StringClass str3;
		const char *str4 = Get_Translated_Preset_Name_Ini(obj);
		str3.Format("%s has detonated",str4);
		delete[] str4;
		SSGMGameLog::Log_Message(str3,"_BEACON");
	}
	Vector3	pos	= Commands->Get_Position(obj);
	SSGMGameLog::Log_Gamelog("DESTROYED;OBJECT;%d;%s;%d;%d;%d", Commands->Get_ID(obj),Commands->Get_Preset_Name(obj), int(pos.Y), int(pos.X), int(pos.Z));
}

void SSGM_Log_Beacon::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&triggerrepairing,sizeof(triggerrepairing),1);
}

void SSGM_Log_Beacon::Timer_Expired(GameObject *obj, int number)
{
	if (number == 222222)
	{
		triggerrepairing = true;
	}
}

void SSGM_Log_Beacon::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (amount != 0)
	{
		Vector3	victimpos =	Commands->Get_Position(obj);
		Vector3	damagerpos = Commands->Get_Position(damager);
		SSGMGameLog::Log_Gamelog("DAMAGED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%f;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(victimpos.Y), int(victimpos.X),	int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(damager), Commands->Get_Preset_Name(damager), int(damagerpos.Y),	int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(damager)), amount, int(Commands->Get_Health(obj)), int(Commands->Get_Shield_Strength(obj)));
	}
	if (ShowExtraMessages)
	{
		if (triggerrepairing)
		{
			if (amount > 0.0f)
			{
				triggerrepairing = false;
				int team = Commands->Get_Player_Type(damager);
				char disarmer[250];
				const char *s1 = Get_Translated_Preset_Name_Ex(obj);
				sprintf(disarmer,"%ls is disarming a %s",Get_Wide_Player_Name(damager),s1);
				delete[] s1;
				Send_Message_Team(team,104,234,40,disarmer);
				Commands->Start_Timer(obj,this,15.0f,222222);
			}
		}
	}
}

void SSGM_Log_Beacon::Killed(GameObject *obj, GameObject *killer)
{
	killed = true;
	Vector3	victimpos =	Commands->Get_Position(obj);
	Vector3	damagerpos = Commands->Get_Position(killer);
	const char *str = Get_Translated_Preset_Name_Ex(obj);
	const char *str2 = Get_Translated_Preset_Name_Ex(killer);
	SSGMGameLog::Log_Gamelog("KILLED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s",Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(killer),	Commands->Get_Preset_Name(killer),	int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(killer)),Get_Current_Weapon(killer),str,str2);
	delete[] str;
	delete[] str2;
	StringClass str3;
	const char *str4 = Get_Translated_Preset_Name_Ini(obj);
	str3.Format("%ls disarmed a %s",Get_Wide_Player_Name(killer),str4);
	SSGMGameLog::Log_Message(str3,"_BEACON");
	delete[] str4;

	if (ShowExtraMessages)
	{
		char disarmer[250];
		const char *s1 = Get_Translated_Preset_Name_Ex(obj);
		sprintf(disarmer,"%ls disarmed a %s",Get_Wide_Player_Name(killer),s1);
		delete[] s1;
		Send_Message(104,234,40,disarmer);
	}
}

void SSGM_Log_Beacon::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_BEACON_DEPLOYED)
	{
		StringClass str3;
		const char *str4 = Get_Translated_Preset_Name_Ini(obj);
		str3.Format("%ls deployed a %s",Get_Wide_Player_Name(sender),str4);
		SSGMGameLog::Log_Message(str3,"_BEACON");
		delete[] str4;
	}
}


void SSGM_Powerup_Expire::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,PowerupExpireTime,100);
}
void SSGM_Powerup_Expire::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Expire_Powerup(obj);
		Commands->Destroy_Object(obj);
	}	
}

void SSGM_Powerup::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_MineRemote_02"))
		{
			Send_Message_Player(sender,104,234,40,"Remote C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pac4_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"CnC_Weapon_MineRemote_Player_2Max");
				}
			}
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"POW_Chaingun_Player")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Chaingun_Player_Nod")) )
		{
			Send_Message_Player(sender,104,234,40,"Chaingun acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwcg_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_Chaingun_Player");
					Remove_Weapon(sender,"Weapon_Chaingun_Player_Nod");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_MineProximity_05"))
		{
			Send_Message_Player(sender,104,234,40,"Proximity C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pacp_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_MineProximity_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_ChemSprayer_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Chemsprayer acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwcs_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_ChemSprayer_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_MineTimed_Player_02"))
		{
			Send_Message_Player(sender,104,234,40,"Timed C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pact_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"CnC_Weapon_MineTimed_Player_2Max");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Flamethrower_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Flamethrower acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwft_aqob0001i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_Flamethrower_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_GrenadeLauncher_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Grenade Launcher acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwgl_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_GrenadeLauncher_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_IonCannonBeacon_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Ion Cannon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paib_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_IonCannonBeacon_Player"))
		{
			Send_Message_Player(sender,104,234,40,"10 Second Superweapon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paib_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Nuclear_Missle_Beacon"))
		{
			Send_Message_Player(sender,104,234,40,"10 Second Superweapon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"nukeavail.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_LaserChaingun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Laser Chaingun acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwlc_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_LaserChaingun_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_LaserRifle_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Laser Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwlr_aqob0004i1evag_snd.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"POW_VoltAutoRifle_Player")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_VoltAutoRifle_Player_Nod")))
		{
			Send_Message_Player(sender,104,234,40,"Volt Auto Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwvr_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_VoltAutoRifle_Player");
					Remove_Weapon(sender,"Weapon_VoltAutoRifle_Player_Nod");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Railgun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Railgun acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwrg_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_Railgun_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_RamjetRifle_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Ramjet Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwrj_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"CnC_Weapon_RamjetRifle_Player");
					Remove_Weapon(sender,"Weapon_RamjetRifle_Player");
				}
			}
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"POW_RocketLauncher_Player")) || 
			     (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_RocketLauncher_Player")) || 
				 (strstr(Commands->Get_Preset_Name(obj),"M02_POW_Rocket_FullAmmo")) )
		{
			Send_Message_Player(sender,104,234,40,"Rocket Launcher acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwrl_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_RocketLauncher_Player");
					Remove_Weapon(sender,"CnC_Weapon_RocketLauncher_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_RepairGun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Maintenance Tool acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwrp_aqob0001i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_RepairGun_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_RepairGun_Player"))		
		{
			Send_Message_Player(sender,104,234,40,"Maintenance Tool acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwrp_aqob0001i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"CnC_Weapon_RepairGun_Player_Special");
				}
			}
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"POW_SniperRifle_Player")) || (strstr(Commands->Get_Preset_Name(obj),"POW_SniperRifle_Player_Nod")))
		{
			Send_Message_Player(sender,104,234,40,"Sniper Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwsr_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_SniperRifle_Player");
					Remove_Weapon(sender,"Weapon_SniperRifle_Player_Nod");
					Remove_Weapon(sender,"CnC_Weapon_SniperRifle_Player");
					Remove_Weapon(sender,"CnC_Weapon_SniperRifle_Player_Nod");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Shotgun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Shotgun acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwps_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_Shotgun_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_TiberiumFlechetteGun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Tiberium Flechette Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwtf_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_TiberiumFlechetteGun_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_TiberiumAutoRifle_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Tiberium Auto Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwtr_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_TiberiumAutoRifle_Player");
				}
			}
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_Armor_Max")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Armor_100")) )
		{
			Send_Message_Player(sender,104,234,40,"Full Body Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prba_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_Armor_Max"))
		{
			Send_Message_Player(sender,104,234,40,"Full Body Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prba_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Armor_100"))
		{
			Send_Message_Player(sender,104,234,40,"Kevlar Vest Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prkv_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Armor_050"))
		{
			Send_Message_Player(sender,104,234,40,"Breast Plate Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prbp_aqob0004i1evag_snd.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_Health_Max")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Health_025")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Health_050")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Health_100")) )
		{
			Send_Message_Player(sender,104,234,40,"Health Supplement acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00ph25_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_AutoRifle_Player"))		
		{
			Send_Message_Player(sender,104,234,40,"Automatic Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwar_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_AutoRifle_Player");
				}
			}
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_AutoRifle_Player_Nod"))		
		{
			Send_Message_Player(sender,104,234,40,"Automatic Rifle acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pwar_aqob0004i1evag_snd.wav");
			if (!SBHCanPickupDropWeapons)
			{
				if (strcmp(Commands->Get_Preset_Name(sender),"CnC_Nod_FlameThrower_2SF") == 0)
				{
					Remove_Weapon(sender,"Weapon_AutoRifle_Player_Nod");
				}
			}
		}
	}
}

void SSGM_Drop_DNA_Powerup::Destroyed(GameObject *obj)
{
	Vector3 DeathPlace = Commands->Get_Position(obj);
	int RandomNum = Commands->Get_Random_Int(1,10);
	GameObject *BackpackScript = 0;
	if (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_1Off")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_GDI_Officer","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	}
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_MiniGunner_1Off")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Nod_Officer","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	}
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_Mutant_0_Mutant")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Chemwar","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_RocketSoldier_2SF")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_LCG","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	}
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_FlameThrower_2SF")) {

		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Stealth_Suit",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_SBH","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	}
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_Technician_0")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Technician","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	}
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_Engineer_2SF")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Hotwire","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if ((strstr(Commands->Get_Preset_Name(obj),"CnC_Sydney_PowerSuit")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_Sydney_PowerSuit_ALT2"))) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_SydneyPower","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_RocketSoldier_3Boss")) || 
			  (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_RocketSoldier_3Boss_ALT2")) || 
			  (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_RocketSoldier_3Boss_Secret")) || 
			  (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_RocketSoldier_3Boss_Secret2")) ){
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Raveshaw","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_3Boss")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_MiniGunner_3Boss_ALT2")) ) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Sakura","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_3Boss")) ||
			  (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_3Boss_ALT2")) ||
			  (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_3Boss_ALT3")) ||
			  (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_3Boss_ALT4")) || 
			  (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_2SF_Logan")) ){
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Havoc","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_RocketSoldier_1Off")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_GDI_RSoldier","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_RocketSoldier_1Off")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Nod_RSoldier","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else 
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_RocketSoldier_2SF_Secret")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Gunner","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_2SF")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Deadeye","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_2SF")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_BHSniper","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_Sydney")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_GDI_Sydney","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if (strstr(Commands->Get_Preset_Name(obj),"CnC_GDI_Grenadier_2SF")) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Patch","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	} 
	else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_Ignatio_Mobius")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_Ignatio_Mobius_ALT2")) ){
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Mobius","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	}
	else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_FlameThrower_3Boss")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_Nod_FlameThrower_3Boss_ALT2")) ) {
		if (RandomNum == 1 || RandomNum == 2) {
			BackpackScript = Commands->Create_Object("POW_Tissue_Nanites",DeathPlace);
			Set_Powerup_Always_Allow_Grant(BackpackScript,true);
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Mendoza","");
			Attach_Script_Once(BackpackScript,"SSGM_Powerup_Expire","");
		}
		else
		{
			Destroy_Script();
		}
	}
}

void SSGM_Powerup_SBH::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Nod_FlameThrower_2SF");
	}
}

void SSGM_Powerup_Havoc::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int RandomNum = Commands->Get_Random_Int(1,4);
		if (RandomNum == 1)
		{
			Change_Character(sender,"CnC_GDI_MiniGunner_3Boss");
		}
		else if (RandomNum == 2)
		{
			Change_Character(sender,"CnC_GDI_MiniGunner_3Boss_ALT2");
		}
		else if (RandomNum == 3)
		{
			Change_Character(sender,"CnC_GDI_MiniGunner_3Boss_ALT3");
		}
		else if (RandomNum == 4)
		{
			Change_Character(sender,"CnC_GDI_MiniGunner_3Boss_ALT4");
		}
	}
}
void SSGM_Powerup_Deadeye::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_GDI_MiniGunner_2SF");
	}
}
void SSGM_Powerup_BHSniper::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Nod_Minigunner_2SF");
	}
}
void SSGM_Powerup_Sakura::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int RandomNum = Commands->Get_Random_Int(1,2);
		if (RandomNum == 1)
		{
			Change_Character(sender,"CnC_Nod_Minigunner_3Boss");
		}
		else if (RandomNum == 2)
		{
			Change_Character(sender,"CnC_Nod_Minigunner_3Boss_ALT2");
		}
	}
}
void SSGM_Powerup_SydneyPower::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int RandomNum = Commands->Get_Random_Int(1,2);
		if (RandomNum == 1)
		{
			Change_Character(sender,"CnC_Sydney_PowerSuit");
		}
		else if (RandomNum == 2)
		{
			Change_Character(sender,"CnC_Sydney_PowerSuit_ALT2");
		}
	}
}
void SSGM_Powerup_Raveshaw::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int RandomNum = Commands->Get_Random_Int(1,2);
		if (RandomNum == 1)
		{
			Change_Character(sender,"CnC_Nod_RocketSoldier_3Boss");
		}
		else if (RandomNum == 2)
		{
			Change_Character(sender,"CnC_Nod_RocketSoldier_3Boss_ALT2");
		}
	}
}
void SSGM_Powerup_Mobius::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int RandomNum = Commands->Get_Random_Int(1,2);
		if (RandomNum == 1)
		{
			Change_Character(sender,"CnC_Ignatio_Mobius_ALT2");
		}
		else if (RandomNum == 2)
		{
			Change_Character(sender,"CnC_Ignatio_Mobius");
		}
	}
}
void SSGM_Powerup_Mendoza::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int RandomNum = Commands->Get_Random_Int(1,2);
		if (RandomNum == 1)
		{
			Change_Character(sender,"CnC_Nod_FlameThrower_3Boss_ALT2");
		}
		else if (RandomNum == 2)
		{
			Change_Character(sender,"CnC_Nod_FlameThrower_3Boss");
		}
	}
}
void SSGM_Powerup_Hotwire::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_GDI_Engineer_2SF");
	}
}
void SSGM_Powerup_Technician::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Nod_Technician_0");
	}
}
void SSGM_Powerup_Gunner::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_GDI_RocketSoldier_2SF");
	}
}
void SSGM_Powerup_LCG::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Nod_RocketSoldier_2SF");
	}
}
void SSGM_Powerup_Patch::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_GDI_Grenadier_2SF");
	}
}
void SSGM_Powerup_Nod_Officer::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Nod_Minigunner_1Off");
	}
}

void SSGM_Powerup_GDI_Officer::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_GDI_MiniGunner_1Off");
	}
}

void SSGM_Powerup_GDI_Sydney::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Sydney");
	}
}

void SSGM_Powerup_Chemwar::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Nod_FlameThrower_1Off");
	}
}

void SSGM_Powerup_GDI_RSoldier::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_GDI_RocketSoldier_1Off");
	}
}

void SSGM_Powerup_Nod_RSoldier::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"CnC_Nod_RocketSoldier_1Off");
	}
}



ScriptRegistrant<SSGM_Powerup_GDI_RSoldier> SSGM_Powerup_GDI_RSoldier_Registrant("SSGM_Powerup_GDI_RSoldier","");
ScriptRegistrant<SSGM_Powerup_Nod_RSoldier> SSGM_Powerup_Nod_RSoldier_Registrant("SSGM_Powerup_Nod_RSoldier","");
ScriptRegistrant<SSGM_Powerup_Gunner> SSGM_Powerup_Gunner_Registrant("SSGM_Powerup_Gunner","");
ScriptRegistrant<SSGM_Powerup_Havoc> SSGM_Powerup_Havoc_Registrant("SSGM_Powerup_Havoc","");
ScriptRegistrant<SSGM_Powerup_Deadeye> SSGM_Powerup_Deadeye_Registrant("SSGM_Powerup_Deadeye","");
ScriptRegistrant<SSGM_Powerup_BHSniper> SSGM_Powerup_BHSniper_Registrant("SSGM_Powerup_BHSniper","");
ScriptRegistrant<SSGM_Powerup_Sakura> SSGM_Powerup_Sakura_Registrant("SSGM_Powerup_Sakura","");
ScriptRegistrant<SSGM_Powerup_SydneyPower> SSGM_Powerup_SydneyPower_Registrant("SSGM_Powerup_SydneyPower","");
ScriptRegistrant<SSGM_Powerup_Raveshaw> SSGM_Powerup_Raveshaw_Registrant("SSGM_Powerup_Raveshaw","");
ScriptRegistrant<SSGM_Powerup_Mobius> SSGM_Powerup_Mobius_Registrant("SSGM_Powerup_Mobius","");
ScriptRegistrant<SSGM_Powerup_Mendoza> SSGM_Powerup_Mendoza_Registrant("SSGM_Powerup_Mendoza","");
ScriptRegistrant<SSGM_Powerup_Hotwire> SSGM_Powerup_Hotwire_Registrant("SSGM_Powerup_Hotwire","");
ScriptRegistrant<SSGM_Powerup_Technician> SSGM_Powerup_Technician_Registrant("SSGM_Powerup_Technician","");
ScriptRegistrant<SSGM_Powerup_LCG> SSGM_Powerup_LCG_Registrant("SSGM_Powerup_LCG","");
ScriptRegistrant<SSGM_Powerup_Nod_Officer> SSGM_Powerup_Nod_Officer_Registrant("SSGM_Powerup_Nod_Officer","");
ScriptRegistrant<SSGM_Powerup_GDI_Officer> SSGM_Powerup_GDI_Officer_Registrant("SSGM_Powerup_GDI_Officer","");
ScriptRegistrant<SSGM_Powerup_GDI_Sydney> SSGM_Powerup_GDI_Sydney_Registrant("SSGM_Powerup_GDI_Sydney","");
ScriptRegistrant<SSGM_Powerup_Patch> SSGM_Powerup_Patch_Registrant("SSGM_Powerup_Patch","");
ScriptRegistrant<SSGM_Powerup_Chemwar> SSGM_Powerup_Chemwar_Registrant("SSGM_Powerup_Chemwar","");
ScriptRegistrant<SSGM_Powerup_SBH> SSGM_Powerup_SBH_Registrant("SSGM_Powerup_SBH","");
ScriptRegistrant<SSGM_Drop_DNA_Powerup> SSGM_Drop_DNA_Powerup_Registrant("SSGM_Drop_DNA_Powerup","");
ScriptRegistrant<SSGM_Powerup> SSGM_Powerup_Registrant("SSGM_Powerup","");
ScriptRegistrant<SSGM_Powerup_Expire> SSGM_Powerup_Expire_Registrant("SSGM_Powerup_Expire","");
ScriptRegistrant<SSGM_Random_Weather> SSGM_Random_Weather_Registrant("SSGM_Random_Weather","");
ScriptRegistrant<SSGM_Log_Scores> SSGM_Log_Scores_Registrant("SSGM_Log_Scores","");
ScriptRegistrant<SSGM_Log_Mine> SSGM_Log_Mine_Registrant("SSGM_Log_Mine","");
ScriptRegistrant<SSGM_Log_Beacon> SSGM_Log_Beacon_Registrant("SSGM_Log_Beacon","");
