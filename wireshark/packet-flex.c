#include "config.h"
#include "packet-flex.h"

#include <epan/packet.h>

#define FLEX_PORT 			1234

static int proto_flex = -1;
static int hf_flex_version = -1;
static int hf_flex_packet_type = -1;
static int hf_flex_hash_type = -1;
static int hf_flex_hop_limit = -1;
static int hf_flex_header_len = -1;
static int hf_flex_check = -1;
static int hf_flex_packet_id = -1;
static int hf_flex_ptc = -1;
static int hf_flex_df = -1;
static int hf_flex_mf = -1;
static int hf_flex_frag_off = -1;
static int hf_flex_sflex_id = -1;
static int hf_flex_dflex_id = -1;
static int hf_flex_packet_len = -1;
static int hf_flex_seq = -1;
static int hf_flex_ack = -1;

static gint ett_flex = -1;

static const value_string version_names[] = {
	{ FLEX_1_0, "Flex v1.0" },
	{ 0, NULL }
};

static const value_string packet_type_names[] = {
	{ FLEX_JOIN, "Join" },
	{ FLEX_STATUS, "Status" },
	{ FLEX_LEAVE, "Leave" },
	{ FLEX_REGISTER, "Register" },
	{ FLEX_UPDATE, "Update" },
	{ FLEX_QUERY, "Query" },
	{ FLEX_REQUEST, "Request" },
	{ FLEX_JOIN_ACK, "Join ACK" },
	{ FLEX_STATUS_ACK, "Status ACK" },
	{ FLEX_LEAVE_ACK, "Leave ACK" },
	{ FLEX_REGISTER_ACK, "Register ACK" },
	{ FLEX_UPDATE_ACK, "Update ACK" },
	{ FLEX_REPLY, "Reply" },
	{ FLEX_RESPONSE, "Response" },
	{ FLEX_INTEREST, "Interest" },
	{ FLEX_DATA, "Data" },
	{ FLEX_DATA_ACK, "Data ACK" },
	{ 0, NULL }
};

static const value_string hash_type_names[] = {
	{ SHA1, "SHA1" },
	{ SHA192, "SHA192" },
	{ SHA224, "SHA224" },
	{ SHA256, "SHA256" },
	{ 0, NULL }
};

static int
dissect_flex(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
{
	gint offset = 0;
	gint8 version = tvb_get_guint8(tvb, 0);
	gint8 packet_type = tvb_get_guint8(tvb, 1);
	proto_item *ti;
	proto_tree *flex_tree;

	col_set_str(pinfo->cinfo, COL_PROTOCOL, "FLEX");
	col_clear(pinfo->cinfo, COL_INFO);
	col_add_fstr(pinfo->cinfo, COL_INFO, "Version: %s, Type: %s",
			val_to_str(version, version_names, "Unknown (0x%02x)"),
			val_to_str(packet_type, packet_type_names, "Unknown (0x%02x)"));

	ti = proto_tree_add_item(tree, proto_flex, tvb, 0, -1, ENC_NA);
	proto_item_append_text(ti, ", Version: %s, Type: %s",
			val_to_str(version, version_names, "Unknown (0x%02x)"),
			val_to_str(packet_type, packet_type_names, "Unknown (0x%02x)"));
	flex_tree = proto_item_add_subtree(ti, ett_flex);
	proto_tree_add_item(flex_tree, hf_flex_version, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(flex_tree, hf_flex_packet_type, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(flex_tree, hf_flex_hash_type, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(flex_tree, hf_flex_hop_limit, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(flex_tree, hf_flex_header_len, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(flex_tree, hf_flex_check, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(flex_tree, hf_flex_packet_id, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(flex_tree, hf_flex_frag_off, tvb, offset, 2, ENC_BIG_ENDIAN);
	proto_tree_add_item(flex_tree, hf_flex_ptc, tvb, offset, 2, ENC_BIG_ENDIAN);
	proto_tree_add_item(flex_tree, hf_flex_df, tvb, offset, 2, ENC_BIG_ENDIAN);
	proto_tree_add_item(flex_tree, hf_flex_mf, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(flex_tree, hf_flex_sflex_id, tvb, offset, FLEX_ID_LENGTH, ENC_BIG_ENDIAN);
	offset += FLEX_ID_LENGTH;
	proto_tree_add_item(flex_tree, hf_flex_dflex_id, tvb, offset, FLEX_ID_LENGTH, ENC_BIG_ENDIAN);
	offset += FLEX_ID_LENGTH; 
	proto_tree_add_item(flex_tree, hf_flex_packet_len, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(flex_tree, hf_flex_seq, tvb, offset, 4, ENC_BIG_ENDIAN);
	offset += 4;
	proto_tree_add_item(flex_tree, hf_flex_ack, tvb, offset, 4, ENC_BIG_ENDIAN);
	offset += 4;

	return tvb_captured_length(tvb);
}

void 
proto_register_flex(void)
{
	static hf_register_info hf[] = {
		{ &hf_flex_version,
			{ "Version", "flex.version", FT_UINT8, BASE_HEX,
			VALS(version_names), 0x0, NULL, HFILL }
		},
		{ &hf_flex_packet_type,
			{ "Packet Type", "flex.packet_type", FT_UINT8, BASE_HEX,
			VALS(packet_type_names), 0x0, NULL, HFILL }
		},
		{ &hf_flex_hash_type,
			{ "Hash Type", "flex.hash_type", FT_UINT8, BASE_HEX,
			VALS(hash_type_names), 0x0, NULL, HFILL}
		},
		{ &hf_flex_hop_limit,
			{ "Hop Limit", "flex.hop_limit", FT_UINT8, BASE_DEC,
			NULL, 0x0, NULL, HFILL}
		},
		{ &hf_flex_header_len,
			{ "Header Length", "flex.header_len", FT_UINT16, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_check,
			{ "Checksum", "flex.check", FT_UINT16, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_packet_id,
			{ "Checksum", "flex.packet_id", FT_UINT16, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_ptc,
			{ "Reliable/Unreliable", "flex.flags.ptc", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), FLEX_PTC, NULL, HFILL }
		},
		{ &hf_flex_df,
			{ "Don't Fragment", "flex.flags.df", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), FLEX_DF, NULL, HFILL }
		},
		{ &hf_flex_mf,
			{ "More Fragments", "flex.flags.mf", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), FLEX_MF, NULL, HFILL }
		},
		{ &hf_flex_frag_off,
			{ "Fragment Offset", "flex.frag_off", FT_UINT16, BASE_DEC,
			NULL, 0x0, "Fragment Offset (13 bits)", HFILL }
		},
		{ &hf_flex_sflex_id, 
			{ "Source Flex ID", "flex.sflex_id", FT_STRING, BASE_NONE,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_dflex_id, 
			{ "Destination Flex ID", "flex.dflex_id", FT_STRING, BASE_NONE,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_packet_len,
			{ "Packet Length", "flex.packet_len", FT_UINT16, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_seq,
			{ "Sequence Number", "flex.seq", FT_UINT32, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_ack,
			{ "Acknowledgement Number", "flex.ack", FT_UINT32, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
	};

	static gint *ett[] = {
		&ett_flex
	};

	proto_flex = proto_register_protocol (
		"Flex Protocol",
		"FLEX",
		"flex"
		);

	proto_register_field_array(proto_flex, hf, array_length(hf));
	proto_register_subtree_array(ett, array_length(ett));
}

void 
proto_reg_handoff_flex(void)
{
	static dissector_handle_t flex_handle;

	flex_handle = create_dissector_handle(dissect_flex, proto_flex);
	dissector_add_uint("ethertype", ETH_P_FLEX, flex_handle);
	dissector_add_uint("tcp.port", FLEX_PORT, flex_handle);
}
