// SPDX-FileCopyrightText: 2026
// SPDX-License-Identifier: GPL-3.0+

#include "USB/usb-pad/usb-pad-studio-ffb.h"

#include "fmt/format.h"

#if defined(__APPLE__) || defined(__linux__)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace usb_pad
{
	void SendFFBReportToStudio(const u8* data, int len)
	{
		static_assert(sizeof(ff_data) == 8);
		if (!data || len < static_cast<int>(sizeof(ff_data)))
			return;

#if defined(__APPLE__) || defined(__linux__)
		static const int socket_fd = ::socket(AF_INET, SOCK_DGRAM, 0);
		if (socket_fd < 0)
			return;

		const unsigned int cmdslot = data[0];
		const unsigned int command = cmdslot == CMD_EXTENDED_CMD ? CMD_EXTENDED_CMD : (cmdslot & 0x0F);
		const unsigned int slots = cmdslot == CMD_EXTENDED_CMD ? 0 : ((cmdslot >> 4) & 0x0F);
		const std::string json = fmt::format(
			R"({{"protocol":"armsx2.ffb.v1","source":"ARMSX2","type":"usb_ff_report","length":8,"cmdslot":{},"command":{},"slots":{},"type_byte":{},"data":[{},{},{},{},{},{},{},{}]}})",
			cmdslot, command, slots, static_cast<unsigned int>(data[1]),
			static_cast<unsigned int>(data[0]), static_cast<unsigned int>(data[1]),
			static_cast<unsigned int>(data[2]), static_cast<unsigned int>(data[3]),
			static_cast<unsigned int>(data[4]), static_cast<unsigned int>(data[5]),
			static_cast<unsigned int>(data[6]), static_cast<unsigned int>(data[7]));

		sockaddr_in address{};
		address.sin_family = AF_INET;
		address.sin_port = htons(41772);
		if (::inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) != 1)
			return;

		(void)::sendto(socket_fd, json.data(), json.size(), 0,
			reinterpret_cast<sockaddr*>(&address), sizeof(address));
#else
		(void)data;
		(void)len;
#endif
	}
} // namespace usb_pad
