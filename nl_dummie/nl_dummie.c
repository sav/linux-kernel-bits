
static struct sock *sock;

static void nl_dummie_input(struct sk_buff *skb)
{
	struct net *net = sock_net(skb->sk);
	struct nlmsghdr *nlh = nlmsg_hdr(skb);
	const char *data = nlmsg_data(nlh);
	pr_info("received: %s\n", data);
}

static int __init nl_dummie_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.groups = 0,
		.input = nl_dummie_input,
	};

	sock = netlink_kernel_create(&init_net, NETLINK_USERSOCK, &cfg);
	if (sock == NULL)
		return -ENONET;

	return 0;
}	
