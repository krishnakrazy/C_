/*
 * sample module to read packets using NF hooks
 * struct necessary for our program
 * struct nf_hook_ops {
	 User fills in from here down. 
	nf_hookfn		*hook;
	struct net_device	*dev;
	void			*priv;
	u_int8_t		pf;
	unsigned int		hooknum;
	// Hooks are ordered in ascending priority. 
	int			priority;
   };
 * different hooks supported are:  
 *  IP Hooks from <linux/netfilter_ipv4.h>
    After promisc drops, checksum checks. 
    #define NF_IP_PRE_ROUTING	0
    If the packet is destined for this box. 
    #define NF_IP_LOCAL_IN		1
    If the packet is destined for another interface. 
    #define NF_IP_FORWARD		2
    Packets coming from a local process. 
    #define NF_IP_LOCAL_OUT		3
    Packets about to hit the wire. 
    #define NF_IP_POST_ROUTING	4
    #define NF_IP_NUMHOOKS		5
 *
 * Program taken from: 
 */

/*****************************************************
 * This code was compiled and tested on Ubuntu 20.04
 * with kernel version 5.12.11
 *****************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>


static struct nf_hook_ops *nfho=NULL;

static unsigned int hfunc(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	struct iphdr *iph;
	struct udphdr *udph;
	if (!skb)
		return NF_ACCEPT;

	iph = ip_hdr(skb);
	if (iph->protocol == IPPROTO_UDP) {
		udph = udp_hdr(skb);
		if (ntohs(udph->dest) == 53) {
			return NF_ACCEPT;
		}
	}
	else if (iph->protocol == IPPROTO_TCP) {
		return NF_ACCEPT;
	}
	
	return NF_ACCEPT;
}

static int __init preroute_init(void)
{
	nfho = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
	
/* Initialize netfilter hook */
	nfho->hook 	    = (nf_hookfn*)hfunc;		/* hook function */
	nfho->pf 	    = PF_INET;			        /* IPv4 */
	nfho->hooknum 	= NF_INET_PRE_ROUTING;		/* received packets */
	nfho->priority 	= NF_IP_PRI_FIRST;		    /* max hook priority */

	nf_register_net_hook(&init_net, nfho);
    printk(KERN_INFO "preroute_hook registered");
    return 0;
}

static void __exit preroute_exit(void)
{
	nf_unregister_net_hook(&init_net, nfho);
	kfree(nfho);
    printk(KERN_INFO "preroute_hook unregistered");
}

module_init(preroute_init);
module_exit(preroute_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anonymous");
MODULE_DESCRIPTION("A Simple netfilter module");
