/*
 * dhcpServer.h
 *
 *  Created on: 9 Apr 2021
 *      Author: Rolf S. Adelsberger
 *
 *
 * MIT License
 * -----------
 * Copyright 2021 Rolf S. Adelsberger
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef DHCPSERVER_H_
#define DHCPSERVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <lwip/err.h>
#include "lwip/udp.h"
#include "netif/etharp.h"
#include <macros.h>


// we provide a maximum of that many addresses
#define NUM_DHCP_ENTRY 			3

/*
 * see http://www.tcpipguide.com/free/t_SummaryOfDHCPOptionsBOOTPVendorInformationFields-6.htm
 */
#define DHCP_MIN_MSG_SIZE 		(300)
#define DHCP_MAX_MSG_SIZE 		(548)


//Hardware type
typedef enum {
	DHCP_HARDWARE_TYPE_ETH = 1,
} DHCPHardwareType_t;

typedef struct {
	uint8_t code;    //0
	uint8_t length;  //1
	uint8_t value[]; //2
}_ATTR_PACKED DhcpOption_t;

typedef union {
	uint8_t 	addr_a[4];
	uint32_t 	addr_v;
} _ATTR_PACKED IPAddress_t;


typedef struct
{
   union
   {
      uint8_t b[6];
      uint16_t w[3];
   };
} _ATTR_PACKED MacAddr;

#define MAC_EQUAL(M1,M2) ((M1.w[0]==M2.w[0])&&(M1.w[1]==M2.w[1])&&(M1.w[2]==M2.w[2]))
#define MAC_EMPTY(M) 	((M.w[0]==0)&&(M.w[1]==0)&&(M.w[2]==0))


typedef struct {
	uint8_t 		op; /* packet opcode type */
	uint8_t 		htype; /* hardware addr type */
	uint8_t 		hlen; /* hardware addr length */
	uint8_t 		hops; /* gateway hops */
	uint32_t 		xid; /* transaction ID */
	uint16_t 		secs; /* seconds since boot began */
	uint16_t 		flags;
	IPAddress_t		ciaddr; /* client IP address */
	IPAddress_t		yiaddr; /* 'your' IP address */
	IPAddress_t		siaddr; /* server IP address */
	IPAddress_t		giaddr; /* gateway IP address */
	MacAddr 		chaddr; /* client hardware address */
	uint8_t 		unused[10];
	uint8_t 		sname[64];
	uint8_t 		file[128];
	uint32_t 		magic;
	uint8_t 		options[]; /* options area */
} _ATTR_PACKED DHCP_TYPE;

typedef enum
{
   DHCP_OPT_PAD                          = 0, // 0x00
   DHCP_OPT_SUBNET_MASK                  = 1, // 0x01
   DHCP_OPT_TIME_OFFSET                  = 2, // 0x02
   DHCP_OPT_ROUTER                       = 3, // 0x03
   DHCP_OPT_TIME_SERVER                  = 4, // 0x04
   DHCP_OPT_NAME_SERVER                  = 5, // 0x05
   DHCP_OPT_DNS_SERVER                   = 6, // 0x06
   DHCP_OPT_LOG_SERVER                   = 7, // 0x07
   DHCP_OPT_COOKIE_SERVER                = 8, // 0x08
   DHCP_OPT_LPR_SERVER                   = 9, // 0x09
   DHCP_OPT_IMPRESS_SERVER               = 10,// 0x0a
   DHCP_OPT_RESOURCE_LOCATION_SERVER     = 11,// 0x0b
   DHCP_OPT_HOST_NAME                    = 12,// 0x0c
   DHCP_OPT_BOOT_FILE_SIZE               = 13,// 0x0d
   DHCP_OPT_MERIT_DUMP_FILE              = 14,// 0x0e
   DHCP_OPT_DOMAIN_NAME                  = 15,// 0x0f
   DHCP_OPT_SWAP_SERVER                  = 16,// 0x10
   DHCP_OPT_ROOT_PATH                    = 17,// 0x11
   DHCP_OPT_EXTENSIONS_PATH              = 18,// 0x12
   DHCP_OPT_IP_FORWARDING                = 19,// 0x13
   DHCP_OPT_NON_LOCAL_SOURCE_ROUTING     = 20,// 0x14
   DHCP_OPT_POLICY_FILTER                = 21,// 0x15
   DHCP_OPT_MAX_DATAGRAM_REASSEMBLY_SIZE = 22,// 0x16
   DHCP_OPT_DEFAULT_IP_TTL               = 23,// 0x17
   DHCP_OPT_PATH_MTU_AGING_TIMEOUT       = 24,// 0x18
   DHCP_OPT_PATH_MTU_PLATEAU_TABLE       = 25,// 0x19
   DHCP_OPT_INTERFACE_MTU                = 26,// 0x1a
   DHCP_OPT_ALL_SUBNETS_ARE_LOCAL        = 27,// 0x1b
   DHCP_OPT_BROADCAST_ADDRESS            = 28,// 0x1c
   DHCP_OPT_PERFORM_MASK_DISCOVERY       = 29,// 0x1d
   DHCP_OPT_MASK_SUPPLIER                = 30,// 0x1e
   DHCP_OPT_PERFORM_ROUTER_DISCOVERY     = 31,// 0x1f
   DHCP_OPT_ROUTER_SOLICITATION_ADDRESS  = 32,// 0x20
   DHCP_OPT_STATIC_ROUTE                 = 33,// 0x21
   DHCP_OPT_TRAILER_ENCAPSULATION        = 34,// 0x22
   DHCP_OPT_ARP_CACHE_TIMEOUT            = 35,// 0x23
   DHCP_OPT_ETHERNET_ENCAPSULATION       = 36,// 0x24
   DHCP_OPT_TCP_DEFAULT_TTL              = 37,// 0x25
   DHCP_OPT_TCP_KEEPALIVE_INTERVAL       = 38,// 0x26
   DHCP_OPT_TCP_KEEPALIVE_GARBAGE        = 39,// 0x27
   DHCP_OPT_NIS_DOMAIN                   = 40,// 0x28
   DHCP_OPT_NIS_SERVER                   = 41,// 0x29
   DHCP_OPT_NTP_SERVER                   = 42,// 0x2a
   DHCP_OPT_VENDOR_SPECIFIC_INFO         = 43,// 0x2b
   DHCP_OPT_NETBIOS_NBNS_SERVER          = 44,// 0x2c
   DHCP_OPT_NETBIOS_NBDD_SERVER          = 45,// 0x2d
   DHCP_OPT_NETBIOS_NODE_TYPE            = 46,// 0x2e
   DHCP_OPT_NETBIOS_SCOPE                = 47,// 0x2f
   DHCP_OPT_X11_FONT_SERVER              = 48,// 0x30
   DHCP_OPT_X11_DISPLAY_MANAGER          = 49,// 0x31
   DHCP_OPT_REQUESTED_IP_ADDRESS         = 50,// 0x32
   DHCP_OPT_IP_ADDRESS_LEASE_TIME        = 51,// 0x33
   DHCP_OPT_OPTION_OVERLOAD              = 52,// 0x34
   DHCP_OPT_DHCP_MESSAGE_TYPE            = 53,// 0x35
   DHCP_OPT_SERVER_IDENTIFIER            = 54,// 0x36
   DHCP_OPT_PARAM_REQUEST_LIST           = 55,// 0x37
   DHCP_OPT_MESSAGE                      = 56,// 0x38
   DHCP_OPT_MAX_DHCP_MESSAGE_SIZE        = 57,// 0x39
   DHCP_OPT_RENEWAL_TIME_VALUE           = 58,// 0x3a
   DHCP_OPT_REBINDING_TIME_VALUE         = 59,// 0x3b
   DHCP_OPT_VENDOR_CLASS_IDENTIFIER      = 60,// 0x3c
   DHCP_OPT_CLIENT_IDENTIFIER            = 61,// 0x3d
   DHCP_OPT_NISP_DOMAIN                  = 64,// 0x40
   DHCP_OPT_NISP_SERVER                  = 65,// 0x41
   DHCP_OPT_TFTP_SERVER_NAME             = 66,// 0x42
   DHCP_OPT_BOOTFILE_NAME                = 67,// 0x43
   DHCP_OPT_MOBILE_IP_HOME_AGENT         = 68,// 0x44
   DHCP_OPT_SMTP_SERVER                  = 69,// 0x45
   DHCP_OPT_POP3_SERVER                  = 70,// 0x46
   DHCP_OPT_NNTP_SERVER                  = 71,// 0x47
   DHCP_OPT_DEFAULT_WWW_SERVER           = 72,// 0x48
   DHCP_OPT_DEFAULT_FINGER_SERVER        = 73,// 0x49
   DHCP_OPT_DEFAULT_IRC_SERVER           = 74,// 0x4a
   DHCP_OPT_STREETTALK_SERVER            = 75,// 0x4b
   DHCP_OPT_STDA_SERVER                  = 76,// 0x4c
   DHCP_OPT_RAPID_COMMIT                 = 80,// 0x50
   DHCP_OPT_CAPTIVE_PORTAL               = 160,// 0xa0
   DHCP_OPT_END                          = 255// 0xff
} DhcpOptionCode;

typedef struct dhcp_entry {
	MacAddr 	mac;				// hardware address of the client
	IPAddress_t	addr;				// IP address assigned to the client
	IPAddress_t	subnet;				// subnet mask
	uint32_t 	lease;				// lease time
}_ATTR_PACKED DhcpEntry_t;

typedef struct dhcp_config {
	IPAddress_t 	addr;				// server address
	uint16_t 		port;				// server port
	IPAddress_t		dns;				// DNS server address
	const char 		*domain;			// DNS suffix
	int 			num_entries;		// number of entries
	DhcpEntry_t 	*entries;			// the array of entries.
}_ATTR_PACKED DhcpConfig_t;

/* DHCP op codes */
typedef enum {
	DHCP_OPCODE_BOOTREQUEST			= 1,
	DHCP_OPCODE_DHCP_BOOTREPLY      = 2,
} DhcpOpcode_t;

typedef enum {
	DHCP_MESSAGE_TYPE_DISCOVER 		= 1,
	DHCP_MESSAGE_TYPE_OFFER 		= 2,
	DHCP_MESSAGE_TYPE_REQUEST 		= 3,
	DHCP_MESSAGE_TYPE_DECLINE 		= 4,
	DHCP_MESSAGE_TYPE_ACK 			= 5,
	DHCP_MESSAGE_TYPE_NAK 			= 6,
	DHCP_MESSAGE_TYPE_RELEASE 		= 7,
	DHCP_MESSAGE_TYPE_INFORM 		= 8
} DhcpMessageType;


typedef struct {
	MacAddr 			macAddr;     	///<Client's MAC address
	ip_addr_t 			ipAddr;     	///<Client's IPv4 address
	bool 				validLease;   	///<Valid lease
	uint32_t 			timestamp; 		///<Timestamp
} DhcpServerBinding_t;


//////////////////////////////////////

extern DhcpConfig_t gDhcpConfig;


err_t dhcpSrvInit(void);

void dhcpSrvFree(void);

void dhcpSrvHandleDiscover(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen);
void dhcpSrvHandleRequest(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen);
void dhcpSrvHandleDecline(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen);
void dhcpSrvHandleRelease(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen);
void dhcpSrvHandleInform(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen);
void dhcpSrvSendReply(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp, uint32_t msgLen);

DhcpOption_t* dhcpSrvGetOption(DHCP_TYPE* dhcp,DhcpOptionCode opCode, uint32_t optLen);
bool dhcpSrvClearOptions(DHCP_TYPE* dhcp, uint32_t optLen);
bool dhcpSrvAddOption(DHCP_TYPE* dhcp,DhcpOptionCode opCode, uint32_t optLen, uint8_t* optValues, uint32_t optTotalLen);
int dhcpSrvGetOptionLength(DHCP_TYPE* dhcp, uint32_t optMaxLen);

#endif /* DHCPSERVER_H_ */
