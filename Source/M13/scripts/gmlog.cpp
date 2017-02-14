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
#include "general.h"
#include "gmlog.h"
#include "engine.h"
bool init = false;
extern bool gamelog;
struct Connection {
	SOCKET socket;
	char cbuffer[512];
	int cbufferFilled;
};
SimpleDynVecClass<Connection *> Connections;
SOCKET s;
void SSGMGameLog::Init(unsigned short port)
{
	if (port != 0)
	{
		WSADATA wsaData;
#pragma warning(suppress: 6031) //warning C6031: return value ignored
		WSAStartup(MAKEWORD(1,1), &wsaData);
		s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		sockaddr_in SockAddr;
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = INADDR_ANY;
		SockAddr.sin_port = htons(port);
		u_long i = 1;
		ioctlsocket(s,FIONBIO,&i);
		bind(s,(sockaddr *)&SockAddr,sizeof(SockAddr));
		listen(s,3);
		init = true;
	}
	else
	{
		init = false;
	}
}

void SSGMGameLog::Shutdown()
{
	if (init)
	{
		for (int i = 0;i < Connections.Count();i++)
		{
			closesocket(Connections[i]->socket);
		}
		closesocket(s);
		init = false;
	}
}

void SSGMGameLog::Think()
{
	SOCKET so = accept(s,NULL,NULL);
	if (so != INVALID_SOCKET)
	{
		Connection *c = new Connection;
		c->cbufferFilled = 0;
		c->socket = so;
		c->cbuffer[_countof(c->cbuffer)-1] = '\0';
		Connections.Add(c);
	}
	for (int i = 0; i < Connections.Count(); ++i)
	{
		int chars_read = recv(Connections[i]->socket, Connections[i]->cbuffer + Connections[i]->cbufferFilled, _countof(Connections[i]->cbuffer)-1 - Connections[i]->cbufferFilled, 0);
		if (chars_read == 0 || (chars_read == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)) // Graceful close or error. Note that if the buffer is full, 0 bytes are read, and this is interpreted as a graceful close too!
		{
			closesocket(Connections[i]->socket);
			Connections.Delete(Connections[i]);
		}
		else if (chars_read > 0) // Data received
		{
			char* lineStartPos = Connections[i]->cbuffer;
			char* endPos = Connections[i]->cbuffer + Connections[i]->cbufferFilled + chars_read;
			for (;;)
			{
				TT_ASSERT(endPos >= lineStartPos);
				char* lineEndPos = (char*)memchr(lineStartPos, '\n', endPos - lineStartPos);
				if (!lineEndPos) break;
				*lineEndPos = '\0';
				if (lineEndPos - lineStartPos > 0 && *(lineEndPos-1) == '\r') *(lineEndPos-1) = '\0'; // Remove trailing \r if CRLF line endings are used.
				Console_Input(lineStartPos);
				lineStartPos = lineEndPos+1;
			}
			TT_ASSERT(endPos - lineStartPos >= 0);
			Connections[i]->cbufferFilled = endPos - lineStartPos;
			memmove(Connections[i]->cbuffer, lineStartPos, endPos - lineStartPos);
		}
	}
}

void SSGMGameLog::Log_Message(const char *message,const char *category)
{
	if (init)
	{
		char time[256];
		memset(time,0,sizeof(time));
		time[0] = 0x3F;
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_FORCE24HOURFORMAT,0,"'['HH':'mm':'ss'] '",time,0xFF);
		StringClass str = "000";
		str += time;
		str += category;
		str += " ";
		str += message;
		Send(str.Peek_Buffer());
	}
}

void SSGMGameLog::Log_Gamelog(const char *format,...)
{
	if (init && gamelog)
	{
		char time[256];
		memset(time,0,sizeof(time));
		time[0] = 0x3F;
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_FORCE24HOURFORMAT,0,"'['HH':'mm':'ss'] '",time,0xFF);
		StringClass str = "001";
		str += time;
		StringClass str2;
		va_list arg_list;
		va_start(arg_list,format);
		str2.Format_Args(format,arg_list);
		va_end(arg_list);
		str += str2;
		Send(str.Peek_Buffer());
	}
}

void SSGMGameLog::Log_RenLog(const char *message)
{
	if (init)
	{
		char time[256];
		memset(time,0,sizeof(time));
		time[0] = 0x3F;
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_FORCE24HOURFORMAT,0,"'['HH':'mm':'ss'] '",time,0xFF);
		StringClass str = "002";
		str += time;
		str += message;
		Send(str.Peek_Buffer());
	}
}

void SSGMGameLog::Send_Console(const char *message)
{
	if (init)
	{
		StringClass str = "003";
		str += message;
		Send(str.Peek_Buffer());
	}
}

void SSGMGameLog::Log_Custom(int id,const char *format,...)
{
	if (init)
	{
		StringClass str;
		str.Format("%03d\n",id);
		StringClass str2;
		va_list arg_list;
		va_start(arg_list,format);
		str2.Format_Args(format,arg_list);
		va_end(arg_list);
		str += str2;
		Send(str.Peek_Buffer());
	}
}

void SSGMGameLog::Send(const char *data)
{
	for (int i = 0;i < Connections.Count();i++)
	{
		send(Connections[i]->socket,data,strlen(data)+1,0);
	}
}
