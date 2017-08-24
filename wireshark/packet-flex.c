#include "config.h"
#include "packet-flex.h"
#include <epan/packet.h>

static int proto_flex = -1;
static int hf_flex_version = -1;
static int hf_flex_packet_type = -1;
static int hf_flex_hash_type = -1;
static int hf_flex_hop_limit = -1;
static int hf_flex_header_length = -1;
static int hf_flex_checksum = -1;
static int hf_flex_packet_id = -1;
static int hf_flex_ptc = -1;
static int hf_flex_df = -1;
static int hf_flex_mf = -1;
static int hf_flex_frag_offset = -1;
static int hf_flex_src_id = -1;
static int hf_flex_dst_id = -1;
static int hf_flex_packet_length = -1;
static int hf_flex_sequence = -1;
static int hf_flex_acknowledge = -1;

static gint ett_foo = -1;

static const value_string version[] = {
	{ FLEX_1_0, "Flex v1.0"}
};

static const value_string packet_type[] = {
	{ JOIN, "Join" },
	{ STATUS, "Status" },
	{ LEAVE, "Leave" },
	{ REGISTER, "Register" },
	{ UPDATE, "Update" },
	{ QUERY, "Query" },
	{ REQUEST, "Request" },
	{ JOIN_ACK, "Join ACK" },
	{ STATUS_ACK, "Status ACK" },
	{ LEAVE_ACK, "Leave ACK" },
	{ REGISTER_ACK, "Register ACK" },
	{ UPDATE_ACK, "Update ACK" },
	{ REPLY, "Reply" },
	{ RESPONSE, "Response" },
	{ INTEREST, "Interest" },
	{ DATA, "Data" },
	{ DATA_ACK, "Data ACK" }
};

static int
dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
{
	gint offset = 0;
	gint8 packet_type = tvb_get_guint8(tvb, 0);

	col_set_str(pinfo->cinfo, COL_PROTOCOL, "FLEX");
	col_clear(pinfo->cinfo, COL_INFO);
	col_add_fstr(pinfo->cinfo, COL_INFO, "Type %s",
			val_to_str(packet_type, packettypenames, "Unknown (0x%02x)"));

	proto_item *ti = proto_tree_add_item(tree, proto_foo, tvb, 0, -1, ENC_NA);
	proto_item_append_text(ti, ", Type %s",
			val_to_str(packet_type, packettypenames, "UNknown (0x%02x)"));
	proto_tree *foo_tree = proto_item_add_subtree(ti, ett_foo);
	proto_tree_add_item(foo_tree, hf_foo_pdu_type, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(foo_tree, hf_foo_flags, tvb, offset, 1, ENC_BIG_ENDIAN);
	proto_tree_add_item(foo_tree, hf_foo_startflag, tvb, offset, 1, ENC_BIG_ENDIAN);
	proto_tree_add_item(foo_tree, hf_foo_endflag, tvb, offset, 1, ENC_BIG_ENDIAN);
	proto_tree_add_item(foo_tree, hf_foo_priorityflag, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(foo_tree, hf_foo_sequenceno, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(foo_tree, hf_foo_initialip, tvb, offset, 4, ENC_BIG_ENDIAN);
	offset += 4; 

	return tvb_captured_length(tvb);
}

void 
proto_register_foo(void)
{
	static hf_register_info hf[] = {
		{ &hf_flex_version,
			{ "Version", "flex.version", FT_UINT8, BASE_HEX,
			VALS(version), 0x0, NULL, HFILL }
		},
		{ &hf_flex_packet_type
			{ "Packet Type", "flex.packet_type", FT_UINT8, BASE_HEX,
			VALS(packet_type), 0x0, NULL, HFILL}
		},
		{ &hf_flex_hash_type,
			{ "Hash Type", "flex.hash_type", FT_UINT8, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_hop_limit,
			{ "Hop limit", "flex.hop_limit", FT_UINT8, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_header_length,
			{ "Header Length", "flex.header_length", FT_UINT8, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_checksum,
			{ "Checksum", "flex.checksum", FT_UINT8, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_packet_id,
			{ "Packet ID", "flex.packet_id", FT_UINT16, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_ptc,
			{ "PTC", "flex.flags.ptc", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), 0x0, NULL, HFILL }
		},
		{ &hf_flex_df,
			{ "Don't fragment", "flex.flags.df", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), 0x0, NULL, HFILL }
		},
		{ &hf_flex_mf,
			{ "More fragments", "flex.flags.mf", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), 0x0, NULL, HFILL }
		},
		{ &hf_flex_frag_offset,
			{ "Fragment offset", "flex.frag_offset", FT_UINT16, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_src_id,
			{ "Flex ID Source", "flex.src_id", FT_STRING, BASE_NONE,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_dst_id,
			{ "Flex ID Destination", "flex.dst_id", FT_STRING, BASE_NONE,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_packet_length,
			{ "Packet Length", "flex.packet_length", FT_UINT16, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_sequence,
			{ "Sequence", "flex.sequence", FT_UINT32, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_flex_acknowledge,
			{ "Acknowledge", "flex.acknowledge", FT_UINT32, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		}
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
	dissector_add_uint("tcp.port", FLEX_PORT, flex_handle);
//	dissector_add_uint("ethertype", F
}
