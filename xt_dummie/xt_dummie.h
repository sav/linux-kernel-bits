#ifndef _LINUX_NETFILTER_XT_DUMMIE_H
#define _LINUX_NETFILTER_XT_DUMMIE_H 1

#define DUMMIE_TAG_MAXLEN (1024)

struct dummie_info {
	unsigned char tag[DUMMIE_TAG_MAXLEN];
};

#endif /* _LINUX_NETFILTER_XT_DUMMIE_H */
