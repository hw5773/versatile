#include "config.h"

#include <epan/packet.h>

#define FOO_PORT 			1234
#define FOO_START_FLAG		0x01
#define FOO_END_FLAG		0x02
#define FOO_PRIORITY_FLAG	0x04

static int proto_foo = -1;
static int hf_foo_pdu_type = -1;
static int hf_foo_sequenceno = -1;
static int hf_foo_initialip = -1;
static int hf_foo_flags = -1;
static int hf_foo_startflag = -1;
static int hf_foo_endflag = -1;
static int hf_foo_priorityflag = -1;

static gint ett_foo = -1;

static const value_string packettypenames[] = {
	{ 1, "Initialise" },
	{ 2, "Terminate" },
	{ 3, "Data" },
	{ 0, NULL }
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
			val_to_str(packet_type, packettypenames, "Unknown (0x%02x)"));
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
		{ &hf_foo_pdu_type,
			{ "FLEX Test field", "foo.type",
			FT_UINT8, BASE_DEC,
			VALS(packettypenames), 0x0,
			NULL, HFILL }
		},
		{ &hf_foo_flags,
			{ "FLEX PDU Flags", "foo.flags",
			FT_UINT8, BASE_HEX,
			NULL, 0x0,
			NULL, HFILL}
		},
		{ &hf_foo_startflag,
			{ "FLEX PDU Start Flags", "foo.flags.start",
			FT_BOOLEAN, 8,
			NULL, FOO_START_FLAG,
			NULL, HFILL }
		},
		{ &hf_foo_endflag,
			{ "FLEX PDU End Flags", "foo.flags.end",
			FT_BOOLEAN, 8,
			NULL, FOO_END_FLAG,
			NULL, HFILL }
		},
		{ &hf_foo_priorityflag,
			{ "FLEX PDU Priority Flags", "foo.flags.priority",
			FT_BOOLEAN, 8,
			NULL, FOO_PRIORITY_FLAG,
			NULL, HFILL }
		},
		{ &hf_foo_sequenceno,
			{ "FLEX PDU Sequence Number", "foo.seqn",
			FT_UINT16, BASE_DEC,
			NULL, 0x0,
			NULL, HFILL }
		},
		{ &hf_foo_initialip,
			{ "FLEX PDU Initial IP", "foo.initialip",
			FT_IPv4, BASE_NONE,
			NULL, 0x0,
			NULL, HFILL }
		}
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
	dissector_add_uint("tcp.port", FOO_PORT, foo_handle);
}
