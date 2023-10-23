
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
// get interface index
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdlib.h>

int ret_if_index(void);
static void print_sockaddr_ll(struct sockaddr_ll *sa);
static void print_packet(unsigned char *p, ssize_t len);

static void print_mac_address(unsigned char *mac);
static void process_eth_packet(unsigned char *p, ssize_t len);

int main(void)
{
  int skt_fd;
  int opt  = 1;
  struct sockaddr_ll sa_ll;
//  unsigned char buf[1200];
//  unsigned long int len;
  int flags;
// 
// skt_fd = socket(AF_PACKET, socket_type, protocol);
// Socket type should be SOCK_DGRAM to get upper layer packets
// Protocol type should be ETH_P_IP for Internet Protocol packets(ipV4) only
// struct sockaddr_ll {
//      unsigned short sll_family;   /* Always AF_PACKET */
//      unsigned short sll_protocol; /* Physical-layer protocol */
//      int            sll_ifindex;  /* Interface number */
//      unsigned short sll_hatype;   /* ARP hardware type */
//      unsigned char  sll_pkttype;  /* Packet type */
//      unsigned char  sll_halen;    /* Length of address */
//      unsigned char  sll_addr[8];  /* Physical-layer address */
// };


    skt_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    
    printf("\n skt_fd : %d \n", skt_fd);
    // get interface index
    sa_ll.sll_ifindex  = ret_if_index(); // there are existing lib functions to get interface index, for ref: man if_nametoindex
    sa_ll.sll_family   = AF_PACKET;
    sa_ll.sll_protocol = htons(ETH_P_ALL);  
    // bind 
/*
    // set socket options
    if (setsockopt(skt_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }     

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
   
 */
    // binding to 127.0.0.1 address
    if (bind(skt_fd, (struct sockaddr *)&sa_ll,
                                sizeof(sa_ll))<0)
    {
        perror("bind failed");
        close(skt_fd);
        exit(EXIT_FAILURE);
    }

    // recv packets
    // recvfrom(sockfd, buf, len, flags, NULL, NULL);
    // ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
    //                        struct sockaddr *src_addr, socklen_t *addrlen);

    unsigned char buffer[1024];
    struct sockaddr sa;
    socklen_t sl;
    ssize_t len;
   
	for (;;) {
		if ((len = recvfrom(skt_fd, &buffer, 1024, 0, NULL, NULL)) == -1){
			close(skt_fd);
			exit(EXIT_FAILURE);
		}
		//printf("\n len is %d \n", sl);
	    //print_sockaddr_ll((struct sockaddr_ll *)&sa);
	    print_packet(buffer, len);
        process_eth_packet(buffer, len);
	}

    close(skt_fd);
    return 0;
}

int ret_if_index(void)
{
    struct if_nameindex *if_ni, *i;
    int index;

    if_ni = if_nameindex();  // returns array of if_nameindex structures
    if (NULL == if_ni)
    {
        printf("\n if_nameindex() returned NULL\n");
        return -1;
    }

    for (i = if_ni;  ! ( (i->if_index == 0) && (i->if_index == NULL)); i++)
    {
        if( strcmp( i->if_name, "lo") != 0)
         {
             printf("\n found non-loopback interface %s, with index %u \n", i->if_name, i->if_index);
             index = i->if_index;
             break;
         }
    }
    if_freenameindex(if_ni); // free all structures recvd
    return index;
}

static void print_sockaddr_ll(struct sockaddr_ll *sa)
{
	printf("sll_family=%u\n", sa->sll_family);
	printf("sll_protocol=%u\n", sa->sll_protocol);
	printf("sll_ifindex=%d\n", sa->sll_ifindex);
	printf("sll_hatype=%u\n", sa->sll_hatype);
	printf("sll_pkttype=%u\n", sa->sll_pkttype);
	printf("sll_halen=%u\n", sa->sll_halen);
	printf("sll_addr=%02x:%02x:%02x:%02x:%02x:%02x\n",
	       sa->sll_addr[0],
	       sa->sll_addr[1],
	       sa->sll_addr[2],
	       sa->sll_addr[3],
	       sa->sll_addr[4],
	       sa->sll_addr[5]);
}

static void process_eth_packet(unsigned char *p, ssize_t len)
{
/*
 * struct ethhdr {
    unsigned char   h_dest[ETH_ALEN];    //destination eth addr 
    unsigned char   h_source[ETH_ALEN];  //source ether addr    
    __be16      h_proto;        // packet type ID field 
} __attribute__((packed));
 * */
   uint16_t eth_type;
   struct ethhdr *eth_pkt; 
   eth_pkt = (struct ethhdr*)p;
   eth_type = ntohs(eth_pkt->h_proto);
   printf("\n type of packet recvd is %x \n", eth_type);
   printf("\n Dst mac address: ");
   print_mac_address(&(eth_pkt->h_dest[0]));
   printf("\n Cst mac address: ");
   print_mac_address(&(eth_pkt->h_source[0]));
   switch( eth_type )
   {
     case 0x800: printf("  IP Packet\n");
                 break;
     case 0x806: printf("  ARP/RARP/GARP packet\n");
                 break;
     case 0x86dd: printf ("  ICMPv6 packet\n");
                  break;
     default:
                   printf(" non IP/ARP/IPv6 Packet \n");
                   break; 
   }
}

static void print_packet(unsigned char *p, ssize_t len)
{
	int i;
	printf("len=%ld\n", len);
	for (i = 0; i < len; i++) {
		if (i % 16 == 0)
			printf("        0x%04x: ", i);
		if (i % 2 == 0)
			printf(" ");
		printf("%02x", p[i]);
		if ((i+1) % 16 == 0)
			printf("\n");
	}
	if (!(i % 16 == 0))
		printf("\n");
    return;
}


static void print_mac_address(unsigned char *mac)
{
   printf("%02x:%02x:%02x:%02x:%02x:%02x \n", mac[0],
                                              mac[1],
                                              mac[2],
                                              mac[3],
                                              mac[4],
                                              mac[5]);
}
