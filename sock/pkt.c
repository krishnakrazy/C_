
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
// get interface index
#include <sys/ioctl.h>
#include <net/if.h>


int main(void)
{
  int skt_fd;

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


    skt_fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
    
    printf("\n skt_fd : %d \n", skt_fd);
    // get interface index
    // bind 
    // recv packets

    close(skt_fd);

    return 0;
}

