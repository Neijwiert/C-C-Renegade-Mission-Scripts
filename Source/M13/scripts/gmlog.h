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
#ifndef TT_INCLUDE_GMLOG_H
#define TT_INCLUDE_GMLOG_H

class SCRIPTS_API SSGMGameLog {
public:
	static void Init(unsigned short port);
	static void Log_Message(const char *message,const char *category);
	static void Log_Gamelog(const char *format,...);
	static void Log_RenLog(const char *message);
	static void Log_Custom(int id,const char *format,...);
	static void Send_Console(const char *message);
	static void Shutdown();
	static void Think();
	static void Send(const char *data);
};

#endif