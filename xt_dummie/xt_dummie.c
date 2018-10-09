#include <linux/ip.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/udp.h>
#include <net/ip.h>
#include <linux/netfilter/x_tables.h>
#include "xt_dummie.h"

static struct xt_match dummie_mt_reg __read_mostly = {
	.name       = "dummie",
	.revision   = 0,
	.family     = NFPROTO_UNSPEC,
	.match      = dummie_match,
	.checkentry = dummie_checkentry,
	.destroy    = dummie_destroy,
	.matchsize  = sizeof(struct dummie_info),
	.me         = THIS_MODULE,
};
