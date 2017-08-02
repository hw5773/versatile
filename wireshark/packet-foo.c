#include "config.h"

#include <epan/packet.h>

#define FOO_PORT 1234

static int proto_foo = -1;
static int hf_foo_pdu_type = -1;
static gint ett_foo = -1;

static int
dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
{
	col_set_str(pinfo->cinfo, COL_PROTOCOL, "FOO");
	col_clear(pinfo->cinfo, COL_INFO);

	proto_item *ti = proto_tree_add_item(tree, proto_foo, tvb, 0, -1, ENC_NA);
	proto_tree *foo_tree = proto_item_add_subtree(ti, ett_foo);
	proto_tree_add_item(foo_tree, hf_foo_pdu_type, tvb, 0, 1, ENC_BIG_ENDIAN);

	return tvb_captured_length(tvb);
}

void 
proto_register_foo(void)
{
	static hf_register_info hf[] = {
		{ &hf_foo_pdu_type,
			{ "FOO Test field", "foo.type",
			FT_UINT32, BASE_HEX,
			NULL, 0x0,
			NULL, HFILL }
		}
	};

	static gint *ett[] = {
		&ett_foo
	};

	proto_foo = proto_register_protocol (
		"Foo Protocol",
		"FOO",
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
