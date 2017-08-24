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
#define FLEX_PORT		1234

/* Version Definition
 * Version is composed of Major and Minor version
 * High 4 bits for Major, Low 4 bits for Minor
 */

#define FLEX_1_0		0x0100
#define FLEX_1_0_MAJOR 	0x01
#define FLEX_1_0_MINOR 	0x00

/* Flags */
#define FLEX_PTC		0x8000
#define FLEX_DF			0x4000
#define FLEX_MF			0x2000
#define FLEX_OFFSET		0x1FFF

/* Packet Type 
 * begin with 00 is for control plane messages, 01 is for control plane ack
 * begin with ff is for data plane messages
 */

// Control Plane
#define JOIN			0x0001	// Join the device with interfaces to Edge network
#define STATUS			0x0002	// Update the status of interfaces
#define LEAVE			0x0003 	// Leave the device from Edge network
#define REGISTER		0x0004	// Register the service/content/resource
#define UPDATE			0x0005	// Update the service/content/resource
#define QUERY			0x0006	// Query ID by keywords
#define REQUEST			0x0007	// Request for resources

#define JOIN_ACK		0x0101	// ACK for Join
#define STATUS_ACK		0x0102	// ACK for Status
#define LEAVE_ACK		0x0103	// ACK for Leave
#define REGISTER_ACK	0x0104	// ACK for Register
#define UPDATE_ACK		0x0105	// ACK for Update
#define REPLY			0x0106	// Reply to Query binding by Packet ID?
#define RESPONSE		0x0107	// Response for Request by Packet ID?

// Data Plane
#define INTEREST		0xff01	// Interest for the service/content
#define DATA			0xff02	// Data
#define DATA_ACK		0xff03	// ACK for Data

/* Hash Type */
#define SHA1			0x0001	// SHA1, 160 bits (20 bytes)
#define SHA192			0x0002	// 192 bits (24 bytes)
#define SHA224			0x0003	// 224 bits (28 bytes)
#define SHA256			0x0004	// 256 bits (32 bytes)
