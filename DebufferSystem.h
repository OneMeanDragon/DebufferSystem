#pragma once

namespace NETWORK {

	class DeBuffer {
	public:
		t_Buffer Buffer;
		UINT32 BufferLength;

		DeBuffer(void);
		~DeBuffer(void);

		void ResetBuffer(void);
		void MoveBuffer(const USHORT uLen);

		USHORT BufferLen(void);
		USHORT StringLength(void);

#pragma region "Insertions"
		void Insert(const void *Data, UINT32 DataLength);
		void InsertNonString(const std::string Data);
		void InsertString(const std::string Data);
		void InsertShort(const USHORT Data);
		void InsertUInt32(const UINT32 Data);
		void InsertUInt64(const UINT64 Data);
#pragma endregion

#pragma region "Extractions"
		void ExtractString(UINT32 Address, std::string &Data);
		void ExtractByte(UINT32 Address, UCHAR &Data);
		void ExtractShort(UINT32 Address, USHORT &Data);
		void ExtractUInt32(UINT32 Address, UINT32 &Data);
		void ExtractUInt64(UINT32 Address, UINT64 &Data);
		void ExtractFileTime(UINT32 Address, FILETIME &Data);
		void ExtractSHABuffer(UINT32 Address, t_hashbuf &Data);
		void ExtractKEYBuffer(UINT32 Address, t_keyhash &Data);
#pragma endregion

		//Debugging
		void HexDump(UINT32 len, UCHAR *buf);
	};

	class TelnetDebuf : public DeBuffer {
	public:
		TelnetDebuf(void);
		~TelnetDebuf(void);

		USHORT Length(void);
		void GetString(std::string &message);
		bool IsValid(void);
		void DeleteCurrent(void);
		void DumpPacket(void);
	};

	class BNCSDebuf : public DeBuffer {
	public:
		BNCSDebuf(void);
		~BNCSDebuf(void);

		USHORT Length(void);
		bool IsValid(void);
		UINT PacketID(void);
		void DeleteCurrent(void);
		void DumpPacket(void);
	};

	class MCPDebuf : public DeBuffer {
	public:
		MCPDebuf(void);
		~MCPDebuf(void);

		USHORT Length(void);
		bool IsValid(void);
		UINT PacketID(void);
		void DeleteCurrent(void);
		void DumpPacket(void);
	};

	class FTPDebuf : public DeBuffer {
	public:
		FTPDebuf(void);
		~FTPDebuf(void);

		USHORT Length(void);
		bool IsValid(void);
		USHORT VersionID(void);
		void DeleteCurrent(void);
		void DumpPacket(void);
	};

}