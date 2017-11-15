#ifndef __FLEX_HDR__
#define __FLEX_HDR__

#include <flex/flex_const.h>
#include <linux/types.h>

/* Flex Common Header */
struct flexhdr
{
	__u8	version;
	__u8	packet_type;
	__u8	hash_type;
	__u8	hop_limit;
	__be16  header_len;
	__sum16	check;
	__be16	packet_id;
	__be16	frag_off;
};

/* Flex Reliable Header */
struct rflexhdr 
{
	struct flexhdr	common;
	char	  sflex_id[FLEX_ID_LENGTH];
	char    dflex_id[FLEX_ID_LENGTH];
	__be16  packet_len;
	__be32	seq;
	__be32	ack;
};

/* Flex Reliable Ext Header */
struct rflexhdr_ext
{
	struct flexhdr	common;
	char	  sflex_id[FLEX_ID_EXT_LENGTH];
	char    dflex_id[FLEX_ID_EXT_LENGTH];
	__be16  packet_len;
	__be32	seq;
	__be32	ack;
};

/* Flex Unreliable Header */
struct uflexhdr 
{
	struct flexhdr	common;
	char	  sflex_id[FLEX_ID_LENGTH];
	char    dflex_id[FLEX_ID_LENGTH];
	__be16  packet_len;
};

/* Flex Unreliable Ext Header */
struct uflexhdr_ext
{
	struct flexhdr common;
	char	  sflex_id[FLEX_ID_EXT_LENGTH];
	char    dflex_id[FLEX_ID_EXT_LENGTH];
	__be16  packet_len;
};

static inline struct rflexhdr *reliable_flex_hdr(struct flexhdr *hdr)
{
  return (struct rflexhdr *)hdr;
}

static inline struct rflexhdr_ext *reliable_ext_flex_hdr(struct flexhdr *hdr)
{
  return (struct rflexhdr_ext *)hdr;
}

static inline struct uflexhdr *unreliable_flex_hdr(struct flexhdr *hdr)
{
  return (struct uflexhdr *)hdr;
}

static inline struct uflexhdr_ext *unreliable_ext_flex_hdr(struct flexhdr *hdr)
{
  return (struct uflexhdr_ext *)hdr;
}

#endif /* __FLEX_HDR__ */
