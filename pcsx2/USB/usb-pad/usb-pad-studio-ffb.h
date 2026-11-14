// SPDX-FileCopyrightText: 2026
// SPDX-License-Identifier: GPL-3.0+

#pragma once

#include "USB/usb-pad/usb-pad.h"

namespace usb_pad
{
	// Send one original 8-byte Logitech wheel USB output report to the local
	// Lenkrad Studio listener as an armsx2.ffb.v1 JSON datagram.
	void SendFFBReportToStudio(const u8* data, int len);
} // namespace usb_pad
