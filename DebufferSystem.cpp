// DebufferSystem.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


namespace NETWORK {

#pragma region "DeBuffer Main Class"
	DeBuffer::DeBuffer() {
		ZeroMemory(Buffer, MAX_BUFFER); //initial void
		BufferLength = 0;
	}

	DeBuffer::~DeBuffer() {	}

	void DeBuffer::ResetBuffer(void) {
		if (BufferLength > 0) {
			ZeroMemory(Buffer, MAX_BUFFER);
			BufferLength = 0;
		}
	}

	void DeBuffer::MoveBuffer(const USHORT uLen) {
		if ((BufferLength - uLen) == 0) { 
			ResetBuffer(); 
		}
		else {
			memmove(Buffer, Buffer + uLen, BufferLength - uLen);
			BufferLength -= uLen;
		}
	}

	USHORT DeBuffer::BufferLen() {
		return BufferLength;
	}

	USHORT DeBuffer::StringLength() {
		return (strlen((const char*)(Buffer)) & 0xFFFF);
	}

#pragma region "Insertions"
	void DeBuffer::Insert(const void *Data, UINT32 DataLength) {
		memcpy(Buffer+BufferLength, Data, DataLength);
		BufferLength += DataLength;
	}

	void DeBuffer::InsertNonString(const std::string Data) {
		Insert(Data.c_str(), Data.length());
	}

	void DeBuffer::InsertString(const std::string Data) {
		Insert(Data.c_str(), Data.length() + 1);
	}

	void DeBuffer::InsertShort(const USHORT Data) {
		Insert(&Data, 2);
	}

	void DeBuffer::InsertUInt32(const UINT32 Data) {
		Insert(&Data, 4);
	}

	void DeBuffer::InsertUInt64(const UINT64 Data) {
		Insert(&Data, 8);
	}
#pragma endregion

#pragma region "Extractions"
	void DeBuffer::ExtractString(UINT32 Address, std::string &Data) {
		Data = std::string((const char*)(Buffer+Address));
	}
	void DeBuffer::ExtractByte(UINT32 Address, UCHAR &Data) {
		Data = Buffer[Address];
	}
	void DeBuffer::ExtractShort(UINT32 Address, USHORT &Data) {
		Data = *(USHORT*)(Buffer + Address);
	}
	void DeBuffer::ExtractUInt32(UINT32 Address, UINT32 &Data) {
		Data = *(UINT32*)(Buffer + Address);
	}
	void DeBuffer::ExtractUInt64(UINT32 Address, UINT64 &Data) {
		Data = *(UINT64*)(Buffer + Address);
	}
	void DeBuffer::ExtractFileTime(UINT32 Address, FILETIME &Data) {
		memcpy(&Data, (Buffer + Address), 8);
	}
	void DeBuffer::ExtractSHABuffer(UINT32 Address, t_hashbuf &Data) {
		memcpy(&Data, (Buffer + Address), MAX_SHA);
	}
	void DeBuffer::ExtractKEYBuffer(UINT32 Address, t_keyhash &Data) {
		memcpy(&Data, (Buffer + Address), KEY_BUF);
	}
#pragma endregion

	void DeBuffer::HexDump(UINT32 len, UCHAR *buf) {
		char tmp[128];

		for (UINT32 offset = 0; offset < len; offset += 16) {
			sprintf_s(tmp, "%08X   ", offset); OutputDebugString(tmp); //BotnetInterfaceData.AddChat(BotnetInterfaceData.RTB_CHAT, myColors::white, "%s", tmp);
			UCHAR *p = (UCHAR *)(buf + offset);
			int stop = ((offset + 16) > len) ? len - offset : 16;

			UINT32 i = 0;
			for (; i < stop; i++) {
				if (i == 8) OutputDebugString("- "); //BotnetInterfaceData.AddChat(BotnetInterfaceData.RTB_CHAT, myColors::white, "%s", "- ");
				sprintf_s(tmp, "%02X ", p[i]);
				OutputDebugString(tmp); //BotnetInterfaceData.AddChat(BotnetInterfaceData.RTB_CHAT, myColors::white, "%s", tmp);
			}

			while (i < 16) {
				if (i++ == 8) OutputDebugString("- "); //BotnetInterfaceData.AddChat(BotnetInterfaceData.RTB_CHAT, myColors::white, "%s", "- ");
				OutputDebugString("   "); //BotnetInterfaceData.AddChat(BotnetInterfaceData.RTB_CHAT, myColors::white, "%s", "   ");
			}

			OutputDebugString("  "); //BotnetInterfaceData.AddChat(BotnetInterfaceData.RTB_CHAT, myColors::white, "%s", "  ");

			for (i = 0; i < stop; i++)
				tmp[i] = isgraph(p[i]) ? p[i] : p[i] == ' ' ? ' ' : '.';
			tmp[i] = 0;
			OutputDebugString(tmp); //BotnetInterfaceData.AddChat(BotnetInterfaceData.RTB_CHAT, myColors::white, "%s\r\n", tmp);
			OutputDebugString("\r\n");
		}
	}

#pragma endregion

#pragma region "Telnet Debuffer"

	TelnetDebuf::TelnetDebuf() { }

	TelnetDebuf::~TelnetDebuf() { }

	USHORT TelnetDebuf::Length() {
		if (BufferLen() == 0) {
			return 0;
		}
		USHORT sLen = (StringLength() + 1);
		return sLen;
	}

	void TelnetDebuf::GetString(std::string &message) {
		ExtractString(0, message);
	}

	bool TelnetDebuf::IsValid() {
		if (BufferLen() == 0) { return false; }
		return true;
	}

	void TelnetDebuf::DeleteCurrent(void) {
		MoveBuffer(Length());
	}

	void TelnetDebuf::DumpPacket(void) {
		if (Length() == 0) {
			OutputDebugString("\r\nNo data to dump\r\n");
			return;
		}
		HexDump(Length(), Buffer);
	}

#pragma endregion

#pragma region "BNCS Debuffer"
	
	BNCSDebuf::BNCSDebuf() { }

	BNCSDebuf::~BNCSDebuf() { }

	USHORT BNCSDebuf::Length(void) {
		if (BufferLen() < 4) { return 0; }
		USHORT uLen = 0;
		ExtractShort(2, uLen);
		return uLen;
	}

	bool BNCSDebuf::IsValid(void) {
		if (BufferLen() < 4) { return false; }
		if (BufferLen() < Length()) { return false; }
		return true;
	}

	UINT BNCSDebuf::PacketID(void) {
		return Buffer[1];
	}

	void BNCSDebuf::DeleteCurrent(void) {
		MoveBuffer(Length());
	}

	void BNCSDebuf::DumpPacket(void) {
		if (Length() == 0) {
			OutputDebugString("\r\nNo data to dump\r\n");
			return;
		}
		HexDump(Length(), Buffer);
	}

#pragma endregion

#pragma region "MCP Debuffer"

	MCPDebuf::MCPDebuf() { }

	MCPDebuf::~MCPDebuf() { }

	USHORT MCPDebuf::Length(void) {
		if (BufferLen() < 2) { return 0; }
		USHORT uLen = 0;
		ExtractShort(0, uLen);
		return uLen;
	}

	bool MCPDebuf::IsValid(void) {
		if (BufferLen() < 2) { return false; }
		if (BufferLen() < Length()) { return false; }
		return true;
	}

	UINT MCPDebuf::PacketID(void) {
		return Buffer[2];
	}

	void MCPDebuf::DeleteCurrent(void) {
		MoveBuffer(Length());
	}

	void MCPDebuf::DumpPacket(void) {
		if (Length() == 0) {
			OutputDebugString("\r\nNo data to dump\r\n");
			return;
		}
		HexDump(Length(), Buffer);
	}

#pragma endregion

#pragma region "FTP Debuffer"

	FTPDebuf::FTPDebuf() { }

	FTPDebuf::~FTPDebuf() { }

	USHORT FTPDebuf::Length(void) {
		if (BufferLen() < 2) { return 0; }
		USHORT uLen = 0;
		ExtractShort(0, uLen);
		return uLen;
	}

	bool FTPDebuf::IsValid(void) {
		if (BufferLen() < 2) { return false; }
		if (BufferLen() < Length()) { return false; }
		return true;
	}

	USHORT FTPDebuf::VersionID(void) {
		USHORT uLen = 0;
		ExtractShort(2, uLen);
		return uLen;
	}

	void FTPDebuf::DeleteCurrent(void) {
		MoveBuffer(Length());
	}

	void FTPDebuf::DumpPacket(void) {
		if (Length() == 0) {
			OutputDebugString("\r\nNo data to dump\r\n");
			return;
		}
		HexDump(Length(), Buffer);
	}

#pragma endregion

}