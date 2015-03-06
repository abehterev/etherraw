/*
   * The EtherRAW application
   *
   * This is small application that generate raw-packets with 
   * different MAC addresses.
   *
   * (C) 2014 by Andrey I. Bekhterev (info@behterev.su)
   * Released under the GPL
   *
   */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <time.h>

#include "send_pkt.h"
 
#ifdef DEBUG
#define DEBUG_TRACE(x) fprintf(stderr,"DEBUG: %s\n", (x))
#else
#define DEBUG_TRACE(x)
#endif

void print_usage(void){
	fprintf(stderr,"Usage: send_pkt -i <iface> -n <mac_num>\n\n"
			"\t-i <iface>\tinterface for packet sending\n"
			"\t-n <mac_num>\tnumber of MAC's\n"
			"\t-s \t\twork in slow mode\n"
			"\t-r \t\tgenerate random Src MAC for each new packet\n"
			"\t-a \t\tAll field of MAC set as random\n\n"
	       );
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int opt;
	char debug[255];
	uint32_t macnum;
	char* iface;
	int workslow = 0;
	int macrandom = 0;
	int allrandom = 0;
	
	snprintf(debug, sizeof(debug), "Options number: %d\n", argc);
	DEBUG_TRACE(debug);

	if (argc<3){
		DEBUG_TRACE("No argv parameters");
		print_usage();
	}

	while ((opt = getopt(argc, argv, "i:n:sra")) != -1) {
		switch (opt) {
			case 'n':
				macnum = atol(optarg);
				snprintf(debug, sizeof(debug),
					  "Set MAC's number: %d\n", macnum);
				DEBUG_TRACE(debug);
				break;
			case 'i':
				iface = optarg;
				snprintf(debug, sizeof(debug),
					  "Set iface: %s\n", iface);
				DEBUG_TRACE(debug);
				break;
			case 's':
				workslow = 1;
				DEBUG_TRACE("Set work too slow\n");
				break;
			case 'r':
				macrandom = 1;
				DEBUG_TRACE("Set random MAC\n");
				break;
			case 'a':
				allrandom = 1;
				DEBUG_TRACE("Set all random MAC\n");
				break;
			default:
				print_usage();
		}
	}

	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;

	char sendbuf[BUF_SIZE];

	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
	struct sockaddr_ll socket_address;

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
		perror("socket");
	}
	 
	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, iface, strlen(iface));
	
	snprintf(debug, sizeof(debug), "if_idx.ifr_name: %s\n", if_idx.ifr_name);
	DEBUG_TRACE(debug);

	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
		perror("SIOCGIFINDEX");

	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, iface, strlen(iface));

	snprintf(debug, sizeof(debug), "if_mac.ifr_name: %s\n", if_mac.ifr_name);
	DEBUG_TRACE(debug);
     
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
		perror("SIOCGIFHWADDR");

	uint32_t i;
	for ( i=1; i <= macnum; i++ ){

	    int tx_len = 0;

	    /* Construct the Ethernet header */
	    memset(sendbuf, 0, BUF_SIZE); // fill zero buffer

	    uint8_t mac1, mac2;
	    mac1 = 0;
	    mac2 = 0;
	    mac2 = i & 0xff;
	    mac1 = (i >> 8);

	    if_mac.ifr_hwaddr.sa_data[0] = (uint8_t)MY_S_MAC0;

	    if (macrandom == 1){
		    srand ( clock() );
		    if (allrandom == 1){
			  if_mac.ifr_hwaddr.sa_data[0] = (uint8_t)rand();
		    }
		    if_mac.ifr_hwaddr.sa_data[1] = (uint8_t)rand();
		    if_mac.ifr_hwaddr.sa_data[2] = (uint8_t)rand();
		    if_mac.ifr_hwaddr.sa_data[3] = (uint8_t)rand();
		    if_mac.ifr_hwaddr.sa_data[4] = (uint8_t)rand();
		    if_mac.ifr_hwaddr.sa_data[5] = (uint8_t)rand();
	    }else{
		    if_mac.ifr_hwaddr.sa_data[1] = (uint8_t)MY_S_MAC1;
		    if_mac.ifr_hwaddr.sa_data[2] = (uint8_t)MY_S_MAC2;
		    if_mac.ifr_hwaddr.sa_data[3] = (uint8_t)MY_S_MAC3;
		    if_mac.ifr_hwaddr.sa_data[4] = (uint8_t)mac1;
		    if_mac.ifr_hwaddr.sa_data[5] = (uint8_t)mac2;
	    }

	    /* Ethernet header */
	    memcpy(eh->ether_shost,if_mac.ifr_hwaddr.sa_data,
			    sizeof(if_mac.ifr_hwaddr.sa_data));

	    eh->ether_dhost[0] = MY_DEST_MAC0;
	    eh->ether_dhost[1] = MY_DEST_MAC1;
	    eh->ether_dhost[2] = MY_DEST_MAC2;
	    eh->ether_dhost[3] = MY_DEST_MAC3;
	    eh->ether_dhost[4] = MY_DEST_MAC4;
	    eh->ether_dhost[5] = MY_DEST_MAC5;

	    /* Ethertype field */
	    eh->ether_type = htons(ETH_P_IP);
	    tx_len += sizeof(struct ether_header);
	    
	    /* Packet data payload*/
	    sendbuf[tx_len++] = 0xaa;
	    sendbuf[tx_len++] = 0xbb;
	    sendbuf[tx_len++] = mac1;
	    sendbuf[tx_len++] = mac2;
	    // other bytes are zero
	    
	    /* Index of the network device */
	    socket_address.sll_ifindex = if_idx.ifr_ifindex;

	    /* Address length*/
	    socket_address.sll_halen = ETH_ALEN;

	    fprintf(stderr,"i: %d\tmac_end[%02X:%02X] ",i,mac1,mac2);

	    char *d =if_mac.ifr_hwaddr.sa_data;
	    fprintf(stderr,"full_mac: %02X:%02X:%02X:%02X:%02X:%02X\n",
			    (uint8_t)d[0], (uint8_t)d[1], (uint8_t)d[2],
			    (uint8_t)d[3], (uint8_t)d[4], (uint8_t)d[5]);

	    /* Send packet */
	    if (sendto(sockfd, sendbuf, tx_len, 0, 
				    (struct sockaddr*)&socket_address, 
				    sizeof(struct sockaddr_ll)) < 0)
	    fprintf(stderr,"Send failed\n");

	    /* Wait timeout */
	    if(workslow == 1) usleep(SleepTime);
	}
	return 0;

}
