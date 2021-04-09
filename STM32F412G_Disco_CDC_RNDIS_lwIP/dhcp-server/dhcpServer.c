/*
 * dhcpServer.c
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

#include <dhcpServer.h>
#include <stm32412g_discovery.h>
#include <etharp.h>


#define DHCP_MAGIC_COOKIE 0x63825363

static DhcpEntry_t DhcpEntries[NUM_DHCP_ENTRY] =
{
    /* mac    ip address        subnet mask        lease time */
    {
    		.mac.w 			= {0,0,0},
			.addr.addr_a 	= {192, 168, 7, 2},
			.subnet.addr_a 	= {255, 255, 255, 0},
			.lease 			= 60 * 60
    },
    {
    		.mac.w 			= {0,0,0},
			.addr.addr_a 	= {192, 168, 7, 3},
			.subnet.addr_a 	= {255, 255, 255, 0},
			.lease 			= 60 * 60
    },
    {
    		.mac.w 			= {0,0,0},
			.addr.addr_a 	= {192, 168, 7, 4},
			.subnet.addr_a 	= {255, 255, 255, 0},
			.lease 			= 60 * 60
    }
};

DhcpConfig_t gDhcpConfig =
{
    .addr.addr_a 			= {192, 168, 7, 1}, // server address
	.port 					= 67, 				// server port
    .dns.addr_a 			= {192, 168, 7, 1}, // dns server
    .domain 				= "srx",            // dns suffix
    .num_entries 			= NUM_DHCP_ENTRY,   // how many entries?
    .entries               	= DhcpEntries		// the actual entries
};



DHCP_TYPE*				 	dhcp_data;

static struct udp_pcb 		*dhcpSrvProcPCB = NULL;
static DhcpConfig_t 		*dhcpConfig = NULL;

char magic_cookie[] = { 0x63, 0x82, 0x53, 0x63 };


DhcpEntry_t* _dhcpSrvGetEntryByIP(IPAddress_t* IP);
DhcpEntry_t* _dhcpSrvGetEntryByMAC(MacAddr *mac);
DhcpEntry_t* _dhcpSrvGetNextVacantEntry();
void _dhcpSrvClearEntry(DhcpEntry_t* entry);

static void dhcpSrvProc(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {

	if(p->len > DHCP_MAX_MSG_SIZE) {
		return;
	}
	if(p->len < DHCP_MIN_MSG_SIZE) {
		return;
	}

	uint8_t l_tempBuf[DHCP_MAX_MSG_SIZE];
	memcpy(l_tempBuf,p->payload,p->len);

	uint32_t optLen = p->len - sizeof(DHCP_TYPE);

	dhcp_data = (DHCP_TYPE*)p->payload;

	if(dhcp_data->op != DHCP_OPCODE_BOOTREQUEST) {
		return;
	}
	if(dhcp_data->htype != DHCP_HARDWARE_TYPE_ETH) {
		return;
	}
	if(dhcp_data->hlen != NETIF_MAX_HWADDR_LEN) {
		  return;
	}
	if(dhcp_data->magic != HTONL(DHCP_MAGIC_COOKIE)) {
		return;
	}

	DhcpOption_t* l_opt = dhcpSrvGetOption(dhcp_data,DHCP_OPT_DHCP_MESSAGE_TYPE,optLen);

	if(l_opt == NULL || l_opt->length != 1) {
		return;
	}

	switch (l_opt->value[0]) {
	   case DHCP_MESSAGE_TYPE_DISCOVER :
		   dhcpSrvHandleDiscover(upcb,port,dhcp_data,optLen);
		   break;
	   //case DHCP_MESSAGE_TYPE_OFFER    :
		   //break;
	   case DHCP_MESSAGE_TYPE_REQUEST  :
		   dhcpSrvHandleRequest(upcb,port,dhcp_data,optLen);
		   break;
	   case DHCP_MESSAGE_TYPE_DECLINE  :
		   dhcpSrvHandleDecline(upcb,port,dhcp_data,optLen);
		   break;
	   //case DHCP_MESSAGE_TYPE_ACK      :
		//   break;
	   //case DHCP_MESSAGE_TYPE_NAK      :
	//	   break;
	   case DHCP_MESSAGE_TYPE_RELEASE  :
		   dhcpSrvHandleRelease(upcb,port,dhcp_data,optLen);
		   break;
	   case DHCP_MESSAGE_TYPE_INFORM   :
		   dhcpSrvHandleInform(upcb,port,dhcp_data,optLen);
		   break;
	   default:
		   break;
	}

	pbuf_free(p);

}

err_t dhcpSrvInit() {

	err_t err;

	DhcpConfig_t *c = &gDhcpConfig;

	dhcpSrvFree();

	dhcpSrvProcPCB = udp_new();

	if (dhcpSrvProcPCB == NULL) {
		return ERR_MEM;
	}

	err = udp_bind(dhcpSrvProcPCB, IP_ADDR_ANY, c->port);

	if (err != ERR_OK) {
		dhcpSrvFree();
		return err;
	}

	udp_recv(dhcpSrvProcPCB, dhcpSrvProc, NULL);

	dhcpConfig = c;

	return ERR_OK;
}

void dhcpSrvFree(void) {

	if (dhcpSrvProcPCB == NULL) {
		return;
	}

	udp_remove(dhcpSrvProcPCB);

	dhcpSrvProcPCB = NULL;
}

void dhcpSrvHandleDiscover(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp_data,uint32_t optLen) {

	DhcpEntry_t 	*entry;
	uint8_t 		replyMsgType;
	uint32_t 		dataLen;

	entry = _dhcpSrvGetEntryByMAC(&(dhcp_data->chaddr));

	if (entry == NULL) {
		entry = _dhcpSrvGetNextVacantEntry();
	}

	if (entry == NULL) {
		return;
	}

	dhcp_data->op 		= DHCP_OPCODE_DHCP_BOOTREPLY; /* reply */
	dhcp_data->secs 	= 0;
	dhcp_data->flags 	= 0;
	// copy the suggested address to the reply structure...
	dhcp_data->yiaddr.addr_v = entry->addr.addr_v;
	memcpy(&(dhcp_data->magic), magic_cookie, 4);

	// clear the options...
	dhcpSrvClearOptions(dhcp_data, optLen);
	replyMsgType 	= DHCP_MESSAGE_TYPE_OFFER;
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_DHCP_MESSAGE_TYPE, 	1, &replyMsgType, 				optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_SERVER_IDENTIFIER, 	4, dhcpConfig->addr.addr_a, 		optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_IP_ADDRESS_LEASE_TIME, 4, (uint8_t*) &(entry->lease), 	optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_SUBNET_MASK, 			4, entry->subnet.addr_a,		optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_END, 					0, NULL, 						optLen);

	optLen = dhcpSrvGetOptionLength(dhcp_data,optLen);

	dataLen = sizeof(DHCP_TYPE) + optLen;
	if (dataLen < DHCP_MIN_MSG_SIZE) {
		dataLen = DHCP_MIN_MSG_SIZE;
	}

	dhcpSrvSendReply(upcb,port,dhcp_data,dataLen);

}

void dhcpSrvHandleRequest(struct udp_pcb *upcb, u16_t port, DHCP_TYPE* dhcp, uint32_t optLen) {
	DhcpEntry_t 	*entry;
	uint8_t 		replyMsgType;
	uint32_t 		dataLen;
	DhcpOption_t* l_opt = dhcpSrvGetOption(dhcp_data, DHCP_OPT_REQUESTED_IP_ADDRESS, optLen);

	if ((l_opt == NULL) | (l_opt->length != 4)) {
		return;
	}

	entry = _dhcpSrvGetEntryByMAC(&(dhcp_data->chaddr));
	if (entry != NULL) {
		_dhcpSrvClearEntry(entry);
	}

	// get the entry to the requested IP
	entry = _dhcpSrvGetEntryByIP((IPAddress_t*)l_opt->value);

	// if we don't have such an entry, abort!
	if (entry == NULL) {
		return;
	}

	// if it is already assigned, abort!
	if(!MAC_EMPTY(entry->mac)) {
		return;
	}

	// OK, it seems that we are successful
	// --> copy over the MAC address to the entry
	memcpy(entry->mac.b,(uint8_t*)&(dhcp_data->chaddr),sizeof(MacAddr));

	// fill in the fields in the reply..
	memcpy(dhcp_data->yiaddr.addr_a, l_opt->value, 4);
	dhcp_data->op 			= DHCP_OPCODE_DHCP_BOOTREPLY;
	dhcp_data->secs 		= 0;
	dhcp_data->flags 		= 0;
	memcpy(&(dhcp_data->magic), magic_cookie, 4);

	// let the ARP table know of the assignment.
	err_t arpError = etharp_add_static_entry((ip4_addr_t*) &(entry->addr.addr_v) , (struct eth_addr*) &(entry->mac));

	UNUSED(arpError);

	dhcpSrvClearOptions(dhcp_data, optLen);
	replyMsgType 			= DHCP_MESSAGE_TYPE_ACK;
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_DHCP_MESSAGE_TYPE, 		1, &replyMsgType, 				optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_SERVER_IDENTIFIER, 		4, dhcpConfig->addr.addr_a,		optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_IP_ADDRESS_LEASE_TIME, 	4, (uint8_t*) &(entry->lease),	optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_SUBNET_MASK, 				4, entry->subnet.addr_a,		optLen);
	dhcpSrvAddOption(dhcp_data, DHCP_OPT_END, 						0, NULL, 						optLen);
	optLen = dhcpSrvGetOptionLength(dhcp_data, optLen);

	dataLen = sizeof(DHCP_TYPE) + optLen;
	if (dataLen < DHCP_MIN_MSG_SIZE) {
		dataLen = DHCP_MIN_MSG_SIZE;
	}

	dhcpSrvSendReply(upcb,port,dhcp_data,dataLen);
}

void dhcpSrvHandleDecline(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen) {

}

void dhcpSrvHandleRelease(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen) {

}

void dhcpSrvHandleInform(struct udp_pcb *upcb,u16_t port,DHCP_TYPE* dhcp,uint32_t optLen) {

}

void dhcpSrvSendReply(struct udp_pcb *upcb, u16_t port, DHCP_TYPE* dhcp_data, uint32_t msgLen) {
	struct pbuf *pp;

	pp = pbuf_alloc(PBUF_TRANSPORT, msgLen, PBUF_POOL);

	if (pp == NULL) {
		return;
	}

	memcpy(pp->payload, dhcp_data, msgLen);
	err_t err = udp_sendto(upcb, pp, IP_ADDR_BROADCAST, port);
	UNUSED(err);
	pbuf_free(pp);
}

DhcpOption_t* dhcpSrvGetOption(DHCP_TYPE* dhcp,DhcpOptionCode opCode, uint32_t optLen) {
	DhcpOption_t* opt = NULL;
	for(int i = 0; i < optLen; ) {
		opt = (DhcpOption_t*)(dhcp->options + i);

		if(opt->code == DHCP_OPT_PAD) {
			i++;
			continue;
		}

		if(opt->code == DHCP_OPT_END) {
			break;
		}

		// else: all other option codes except PAD and END
		if(opt->code == opCode) {
			return opt;
		} else {
			i += 1 + 1 + opt->length;
		}
	}
	return NULL;
}

bool dhcpSrvClearOptions(DHCP_TYPE* dhcp, uint32_t optLen) {
	memset(dhcp->options,0,optLen);
	return true;
}

int dhcpSrvGetOptionLength(DHCP_TYPE* dhcp, uint32_t optMaxLen) {
	int ret = -1;
	DhcpOption_t* opt = NULL;
	for(int i = 0; i < optMaxLen; i++) {
		opt = (DhcpOption_t*)(dhcp->options + i);

		if (opt->code == DHCP_OPT_PAD) {
			i++;
			continue;
		}

		if (opt->code == DHCP_OPT_END) {
			ret = i;
			break;
		}

		i += 1 + 1 + opt->length;
	}
	return ret;
}

bool dhcpSrvAddOption(DHCP_TYPE* dhcp,DhcpOptionCode opCode, uint32_t optLen, uint8_t* optValues, uint32_t optTotalLen) {

	DhcpOption_t* opt = NULL;
	bool success = false;
	for(int i = 0; i < optTotalLen; ) {

		opt = (DhcpOption_t*)(dhcp->options + i);
		if((opt->code != DHCP_OPT_PAD) && (opt->code != DHCP_OPT_END)) {
			i += 1 + 1 + opt->length;
			continue;
		}

		// see if there is enough space in the option buffer...
		if((i + 1 + 1 + optLen ) > optTotalLen) {
			break;
		}

		// there seems to be enough space AND we have found an empty spot...
		// --> add the option.
		opt->code = opCode;
		if(opCode != DHCP_OPT_END) {
			opt->length = optLen;
			memcpy(opt->value,optValues,optLen);
		}
		success = true;

	}

	return success;
}


DhcpEntry_t* _dhcpSrvGetEntryByIP(IPAddress_t* IP) {
	DhcpEntry_t* ret = NULL;

	if(dhcpConfig != NULL) {

		for (int i = 0; i < dhcpConfig->num_entries; i++) {
			if(dhcpConfig->entries[i].addr.addr_v == IP->addr_v) {
				ret = &(dhcpConfig->entries[i]);
				break;
			}
		}
	}

	return ret;
}

DhcpEntry_t* _dhcpSrvGetEntryByMAC(MacAddr *mac) {
	DhcpEntry_t* ret = NULL;

	if (dhcpConfig != NULL) {
		for (int i = 0; i < dhcpConfig->num_entries; i++) {
			if ( MAC_EQUAL((*mac),dhcpConfig->entries[i].mac) ) {
				ret = &(dhcpConfig->entries[i]);
				break;
			}
		}
	}

	return ret;
}

DhcpEntry_t* _dhcpSrvGetNextVacantEntry() {
	DhcpEntry_t* ret = NULL;

	if (dhcpConfig != NULL) {
		for (int i = 0; i < dhcpConfig->num_entries; i++) {
			if(MAC_EMPTY(dhcpConfig->entries[i].mac)) {
				ret = &(dhcpConfig->entries[i]);
				break;
			}
		}
	}

	return ret;
}

void _dhcpSrvClearEntry(DhcpEntry_t* entry) {
	memset(entry,0,sizeof(DhcpEntry_t));
}
