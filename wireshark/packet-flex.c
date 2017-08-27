#include "config.h"
#include "packet-foo.h"

#include <epan/packet.h>

#define FOO_PORT 			1234

static int proto_foo = -1;
static int hf_foo_version = -1;
static int hf_foo_packet_type = -1;
static int hf_foo_hash_type = -1;
static int hf_foo_hop_limit = -1;
static int hf_foo_header_len = -1;
static int hf_foo_check = -1;
static int hf_foo_packet_id = -1;
static int hf_foo_ptc = -1;
static int hf_foo_df = -1;
static int hf_foo_mf = -1;
static int hf_foo_frag_off = -1;
static int hf_foo_sflex_id = -1;
static int hf_foo_dflex_id = -1;
static int hf_foo_packet_len = -1;
static int hf_foo_seq = -1;
static int hf_foo_ack = -1;

static gint ett_foo = -1;

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
dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
{
	gint offset = 0;
	gint8 version = tvb_get_guint8(tvb, 0);
	gint8 packet_type = tvb_get_guint8(tvb, 1);
	proto_item *ti;
	proto_tree *foo_tree;

	col_set_str(pinfo->cinfo, COL_PROTOCOL, "FLEX");
	col_clear(pinfo->cinfo, COL_INFO);
	col_add_fstr(pinfo->cinfo, COL_INFO, "Version: %s, Type: %s",
			val_to_str(version, version_names, "Unknown (0x%02x)"),
			val_to_str(packet_type, packet_type_names, "Unknown (0x%02x)"));

	ti = proto_tree_add_item(tree, proto_foo, tvb, 0, -1, ENC_NA);
	proto_item_append_text(ti, ", Version: %s, Type: %s",
			val_to_str(version, version_names, "Unknown (0x%02x)"),
			val_to_str(packet_type, packet_type_names, "Unknown (0x%02x)"));
	foo_tree = proto_item_add_subtree(ti, ett_foo);
	proto_tree_add_item(foo_tree, hf_foo_version, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(foo_tree, hf_foo_packet_type, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(foo_tree, hf_foo_hash_type, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(foo_tree, hf_foo_hop_limit, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
	proto_tree_add_item(foo_tree, hf_foo_header_len, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(foo_tree, hf_foo_check, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(foo_tree, hf_foo_packet_id, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(foo_tree, hf_foo_frag_off, tvb, offset, 2, ENC_BIG_ENDIAN);
	proto_tree_add_item(foo_tree, hf_foo_ptc, tvb, offset, 2, ENC_BIG_ENDIAN);
	proto_tree_add_item(foo_tree, hf_foo_df, tvb, offset, 2, ENC_BIG_ENDIAN);
	proto_tree_add_item(foo_tree, hf_foo_mf, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(foo_tree, hf_foo_sflex_id, tvb, offset, FLEX_ID_LENGTH, ENC_BIG_ENDIAN);
	offset += FLEX_ID_LENGTH;
	proto_tree_add_item(foo_tree, hf_foo_dflex_id, tvb, offset, FLEX_ID_LENGTH, ENC_BIG_ENDIAN);
	offset += FLEX_ID_LENGTH; 
	proto_tree_add_item(foo_tree, hf_foo_packet_len, tvb, offset, 2, ENC_BIG_ENDIAN);
	offset += 2;
	proto_tree_add_item(foo_tree, hf_foo_seq, tvb, offset, 4, ENC_BIG_ENDIAN);
	offset += 4;
	proto_tree_add_item(foo_tree, hf_foo_ack, tvb, offset, 4, ENC_BIG_ENDIAN);
	offset += 4;

	return tvb_captured_length(tvb);
}

void 
proto_register_foo(void)
{
	static hf_register_info hf[] = {
		{ &hf_foo_version,
			{ "Version", "foo.version", FT_UINT8, BASE_HEX,
			VALS(version_names), 0x0, NULL, HFILL }
		},
		{ &hf_foo_packet_type,
			{ "Packet Type", "foo.packet_type", FT_UINT8, BASE_HEX,
			VALS(packet_type_names), 0x0, NULL, HFILL }
		},
		{ &hf_foo_hash_type,
			{ "Hash Type", "foo.hash_type", FT_UINT8, BASE_HEX,
			VALS(hash_type_names), 0x0, NULL, HFILL}
		},
		{ &hf_foo_hop_limit,
			{ "Hop Limit", "foo.hop_limit", FT_UINT8, BASE_DEC,
			NULL, 0x0, NULL, HFILL}
		},
		{ &hf_foo_header_len,
			{ "Header Length", "foo.header_len", FT_UINT16, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_foo_check,
			{ "Checksum", "foo.check", FT_UINT16, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_foo_packet_id,
			{ "Checksum", "foo.packet_id", FT_UINT16, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_foo_ptc,
			{ "Reliable/Unreliable", "foo.flags.ptc", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), FLEX_PTC, NULL, HFILL }
		},
		{ &hf_foo_df,
			{ "Don't Fragment", "foo.flags.df", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), FLEX_DF, NULL, HFILL }
		},
		{ &hf_foo_mf,
			{ "More Fragments", "foo.flags.mf", FT_BOOLEAN, BASE_NONE,
			TFS(&tfs_set_notset), FLEX_MF, NULL, HFILL }
		},
		{ &hf_foo_frag_off,
			{ "Fragment Offset", "foo.frag_off", FT_UINT16, BASE_DEC,
			NULL, 0x0, "Fragment Offset (13 bits)", HFILL }
		},
		{ &hf_foo_sflex_id, 
			{ "Source Flex ID", "foo.sflex_id", FT_STRING, BASE_NONE,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_foo_dflex_id, 
			{ "Destination Flex ID", "foo.dflex_id", FT_STRING, BASE_NONE,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_foo_packet_len,
			{ "Packet Length", "foo.packet_len", FT_UINT16, BASE_DEC,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_foo_seq,
			{ "Sequence Number", "foo.seq", FT_UINT32, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
		{ &hf_foo_ack,
			{ "Acknowledgement Number", "foo.ack", FT_UINT32, BASE_HEX,
			NULL, 0x0, NULL, HFILL }
		},
	};

	static gint *ett[] = {
		&ett_foo
	};

	proto_foo = proto_register_protocol (
		"Flex Protocol",
		"FLEX",
		"flex"
		);

	proto_register_field_array(proto_foo, hf, array_length(hf));
	proto_register_subtree_array(ett, array_length(ett));
}

void 
proto_reg_handoff_foo(void)
{
	static dissector_handle_t foo_handle;

	foo_handle = create_dissector_handle(dissect_foo, proto_foo);
	dissector_add_uint("ethertype", ETH_P_FLEX, foo_handle);
	dissector_add_uint("tcp.port", FOO_PORT, foo_handle);
}
