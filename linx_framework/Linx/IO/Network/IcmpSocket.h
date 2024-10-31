#pragma once

#include "Base/SocketBase.h"
#include "Linx/System/Time.h"
#include "Linx/Math/Check.h"
#include "Linx/System/Process.h"

namespace Linx
{
	struct IPHeader
	{
		uint8_t VerHLen; // 4 bits version + 4 bits head length
		uint8_t TOS; // Service type
		uint16_t TotalLen; // Total length
		uint16_t ID; // ID
		uint16_t FlagFragOffset; // 3 bits flag + 13 bits offset
		uint8_t TTL; // TTL
		uint8_t Protocol; // Protocol
		uint16_t HChecksum; // Head check sum
		uint32_t SrcIP; // Source IP address
		uint32_t DestIP; // Destination IP address
	};

	struct ICMPPackage
	{
		uint8_t Type; // 8 = Request, 0 = Reply
		uint8_t Code;
		uint16_t Checksum; 
		uint16_t ID;
		uint16_t Seq; // Sequence
		uint8_t Data[];

		/**
		 * Make the requested package.
		 * @param InSeq	Sequence of the package.
		 * @param Size	Length of Data.
		 */
		void MakeRequest(uint16_t InSeq, int Size)
		{
			Type = 8;
			Code = 0;
			ID = htons(GetPID());
			Seq = htons(InSeq);
			Checksum = 0;
			Checksum = CheckSum((unsigned short*)this, sizeof(ICMPPackage) + Size);
		}

		/** Check whether the received reply packet is correct. */
		bool CheckReply(uint16_t InSeq)
		{
			return ID == htons(GetPID())
				&& Type == 0
				&& htons(InSeq) == Seq;
		}
	};

	struct PingReply
	{
		uint16_t Seq; // Sequence
		size_t RoundTripTime; // The time interval from sending to receiving
		size_t Bytes; // Length of received data length
		size_t TTL; // TTL
		char SrcIP[16]{}; // Source IP address
	};

	class IcmpSocket : public SocketBase
	{
	public:
		using Super = SocketBase;

		IcmpSocket() { Init(); }

		/**
		 * Execute ping once.
		 * @param Reply		Returns results.
		 * @param InSeq		Sequence of the package.
		 * @param DataSize	Length of data.
		 * @return whether the ping succeeds.
		 */
		bool Ping(PingReply* Reply = nullptr, int InSeq = 1, int DataSize = 32) noexcept;

		/** Implements the ping function from the terminal. */
		static bool PingTest(const char* Target, int Count = 4, int Size = 32, int Interval = 1000) noexcept;

		// Begin SocketBase Interface.
		virtual void Init() override;
		virtual int Recv(char* buf, size_t bufsize) noexcept override;
		virtual int Send(const char* buf, size_t bufsize) noexcept override;
		// End SocketBase Interface.
	};
}
