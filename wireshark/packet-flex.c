#include "config.h"
#include <epan/packet.h>
#include <epan/exceptions.h>
#include <epan/ipproto.h>
#include <epan/addr_resolv.h>
#include "packet-flex.h"
#include "packet-tcp.h"
#include <epan/prefs.h>
#include <epan/strutil.h>
#include <epan/expert.h>
#include <epan/afn.h>
#include <epan/tap.h>
#include <epan/stats_tree.h>

void proto_register_flex(void);
void proto_reg_handoff_flex(void);

static int proto_flex = -1;

static gint ett_flex = -1;

static expert_field ei_flex_wrong_value = EI_INIT;

/* Test Codes */
#define TEST_QUERY	0
#define TEST_ANSWER	1
#define TEST_INFO	2

static const value_string test_code_vals[] = {
	{ TEST_QUERY, 	"Test query"	},
	{ TEST_ANSWER, 	"Test answer"	},
	{ TEST_INFO, 	"Test info"		},
	{ 0,			NULL			} };

static int flex_tap = -1;

static int hf_flex_test = -1; 					// Header Field (hf)
static gint ett_flex = -1;						// Ethereal Tree Type (ett)
static expert_field ei_flex_wrong_value = -1;	// Error Indication (ei)
static dissector_handle_t flex_handle = -1;		// Dessector
static range_t *global_flex_port_range;			// Range

/* Ports used for Flex. */
#define DEFAULT_FLEX_PORT_RANGE		"3333"

/* Offsets of fields within an Flex packet. */
#define FLEX_TEST					0

static int
dissect_flex(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void * data)
{
	guint16		flex_test;
	col_set_str(pinfo->cinfo, COL_PROTOCOL, "FLEX");
	col_clear(pinfo->cinfo, COL_INFO);

	/* Test Value */
	flex_test = tvb_get_ntohs(tvb, FLEX_TEST);
}

void
proto_register_flex(void)
{
	static hf_register_info hf[] = {
		{ &hf_flex_test,
			{ "Test", "flex.test",
				FT_UINT16, BASE_HEX, VALS(test_code_vals), 0x0,
				"Test code", HFILL }}};

	static ei_register_info ei[] = {
		{ &ei_flex_wrong_value, { "flex.wrong_value", PI_MALFORMED, PI_ERROR, "Wrong value", EXPFILL}},
	};

	static gint *ett[] = {
		&ett_flex,
	};

	module_t *flex_module;
	expert_module_t *expert_flex;

	proto_flex = proto_register_protocol("Flex ID", "FLEX", "flex");
	proto_register_field_array(proto_flex, hf, array_length(hf));
	proto_register_subtree_array(ett, array_length(ett));
	expert_flex = expert_register_protocol(proto_flex);
	expert_register_field_array(expert_flex, ei, array_length(ei));

	range_convert_str(&global_flex_port_range, DEFAULT_FLEX_PORT_RANGE, MAX_TCP_PORT);

	flex_module = prefs_register_protocol(proto_flex, proto_reg_handoff_flex);
	prefs_register_range_preference(flex_module, "flex.ports", "FLEX TCP Ports",
									"TCP ports to be decoded as FLEX (default: "
									DEFAULT_FLEX_PORT_RANGE ")",
									&global_flex_port_range, MAX_TCP_PORT);

	flex_handle = register_dissector("flex", dissect_flex, proto_flex);
	flex_tap = register_tap("flex");
}

void
proto_reg_handoff_flex(void)
{
	static range_t *flex_tcp_port_range;
	static gboolean initialized = FALSE;

	if (!initialized)
	{
		dissector_handle_t flex_handle;
		flex_handle = create_dissector_handle(dissect_flex, proto_flex);
		dissector_add_uint("tcp.port", DEFAULT_FLEX_PORT_RANGE, flex_handle);
		initialized = TRUE;
	}
	else
	{
		dissector_delete_uint_range("tcp.port", flex_tcp_port_range, flex_handle);
		g_free(dns_tcp_port_range);
	}

	flex_tcp_port_range = range_copy(global_flex_tcp_port_range);
	dissector_add_uint_range("tcp.port", flex_tcp_port_range, flex_handle);
}
