/* packet-gryphon.h
 * Definitions for Flex disassembly structures and routines
 * By Hyunwoo Lee <hwlee2014@mmlab.snu.ac.kr>, Hyeonmin Lee <hmlee@mmlab.snu.ac.kr>
 * Copyright 2017 Hyunwoo Lee, Hyeonmin Lee
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* Default port for Flex ID over TCP/IP */
#define FLEX_PORT			1234

/* Version Definition
 * Version is composed of Major and Minor version
 * High 4 bits for Major, Low 4 bits for Minor
 */

#define FLEX_1_0			0x01
#define FLEX_1_0_MAJOR 		0x1
#define FLEX_1_0_MINOR 		0x0

/* Flags */
#define FLEX_PTC			0x8000
#define FLEX_DF				0x4000
#define FLEX_MF				0x2000
#define FLEX_OFFSET			0x1FFF

/* Packet Type 
 * begin with 00 is for control plane messages, 01 is for control plane ack
 * begin with ff is for data plane messages
 */

// Control Plane
#define FLEX_JOIN			0x01	// Join the device with interfaces to Edge network
#define FLEX_STATUS			0x02	// Update the status of interfaces
#define FLEX_LEAVE			0x03 	// Leave the device from Edge network
#define FLEX_REGISTER		0x04	// Register the service/content/resource
#define FLEX_UPDATE			0x05	// Update the service/content/resource
#define FLEX_QUERY			0x06	// Query ID by keywords
#define FLEX_REQUEST		0x07	// Request for resources

#define FLEX_JOIN_ACK		0x11	// ACK for Join
#define FLEX_STATUS_ACK		0x12	// ACK for Status
#define FLEX_LEAVE_ACK		0x13	// ACK for Leave
#define FLEX_REGISTER_ACK	0x14	// ACK for Register
#define FLEX_UPDATE_ACK		0x15	// ACK for Update
#define FLEX_REPLY			0x16	// Reply to Query binding by Packet ID?
#define FLEX_RESPONSE		0x17	// Response for Request by Packet ID?

// Data Plane
#define FLEX_INTEREST		0xf1	// Interest for the service/content
#define FLEX_DATA			0xf2	// Data
#define FLEX_DATA_ACK		0xf3	// ACK for Data

/* Hash Type */
#define SHA1				0x01	// SHA1, 160 bits (20 bytes)
#define SHA192				0x02	// 192 bits (24 bytes)
#define SHA224				0x03	// 224 bits (28 bytes)
#define SHA256				0x04	// 256 bits (32 bytes)
