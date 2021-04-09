/**
  ******************************************************************************
  * @file    LwIP/LwIP_TCP_Echo_Server/Src/ethernetif.c
  * @author  MCD Application Team
  * @brief   This file implements Ethernet network interface drivers for lwIP
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "lwip/timeouts.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include <string.h>
#include <usbd_cdc_rndis_if.h>
#include <stm32412g_discovery.h>
#include <usbd_rxBuffer.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH) 
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
static void low_level_init(struct netif *netif)
{ 
  
  /* Set netif link flag */
  netif->flags |= NETIF_FLAG_LINK_UP;

  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;
  
  /* set MAC hardware address */
  netif->hwaddr[0] =  ETHNETIF_MAC_ADDR0;
  netif->hwaddr[1] =  ETHNETIF_MAC_ADDR1;
  netif->hwaddr[2] =  ETHNETIF_MAC_ADDR2;
  netif->hwaddr[3] =  ETHNETIF_MAC_ADDR3;
  netif->hwaddr[4] =  ETHNETIF_MAC_ADDR4;
  netif->hwaddr[5] =  ETHNETIF_MAC_ADDR5;
  
  /* maximum transfer unit */
  netif->mtu = 1500;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
  
}

/**
  * @brief This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t low_level_output(struct netif *netif, struct pbuf *p) {

	err_t errval = ERR_OK;

	struct pbuf *q;
	uint32_t MAXBufsize = CDC_USERBUFFER_SIZE;

	USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *) (USBD_Device.pClassData);
#if 1

	uint32_t bufferoffset = sizeof(USBD_CDC_RNDIS_PacketMsgTypeDef);
	uint32_t dataLength = 0;

	for (q = p; q != NULL; q = q->next) {
		//q->tot_len
		if((bufferoffset + q->len) >MAXBufsize ) {
			errval = ERR_MEM;
			break;
		}

		memcpy(hcdc_cdc_rndis->TxBuffer + bufferoffset,q->payload,q->len);
		bufferoffset += q->len;
		dataLength += q->len;
	}

	if(errval == ERR_OK) {
		hcdc_cdc_rndis->TxLength = bufferoffset;
		USBD_CDC_RNDIS_TransmitPacket(&USBD_Device);
	}
#else



	uint32_t framelength = 0;
	uint32_t bufferoffset = 0;
	uint32_t byteslefttocopy = 0;
	uint32_t payloadoffset = 0;

	bufferoffset = 0;

	//uint32_t MAXBufsize = CDC_RNDIS_ETH_MAX_SEGSZE + 100;

	/* copy frame from pbufs to driver buffers */
	for (q = p; q != NULL; q = q->next) {
		/* Get bytes in current lwIP buffer */
		byteslefttocopy = q->len;
		payloadoffset = 0;

		/* Check if the length of data to copy is bigger than Tx buffer size*/
		while ((byteslefttocopy + bufferoffset) > MAXBufsize) {
			/* Copy data to Tx buffer*/
			memcpy(hcdc_cdc_rndis->TxBuffer + bufferoffset, (uint8_t*) ((uint8_t*) q->payload + payloadoffset), (MAXBufsize - bufferoffset));

			byteslefttocopy = byteslefttocopy - (MAXBufsize - bufferoffset);
			payloadoffset = payloadoffset + (MAXBufsize - bufferoffset);
			framelength = framelength + (MAXBufsize - bufferoffset);
			bufferoffset = 0;
		}

		/* Copy the remaining bytes */
		memcpy((uint8_t*) (hcdc_cdc_rndis->TxBuffer + bufferoffset), (uint8_t*) ((uint8_t*) q->payload + payloadoffset), byteslefttocopy);
		bufferoffset = bufferoffset + byteslefttocopy;
		framelength = framelength + byteslefttocopy;
	}

	hcdc_cdc_rndis->TxLength = framelength;
	USBD_CDC_RNDIS_TransmitPacket(&USBD_Device);

	errval = ERR_OK;

#endif



	BSP_LED_Toggle(LED_GREEN);
	return errval;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */

struct pbuf * low_level_input(struct netif *netif) {
	struct pbuf *p = NULL;


	struct pbuf *q;
	rxBufferElem_t* rxElem;

	uint32_t bufferOffset = 0;
	uint32_t payloadoffset = 0;
	uint32_t byteslefttocopy = 0;


	uint32_t RXBufSize = CDC_USERBUFFER_SIZE;

	USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *) (USBD_Device.pClassData);

	/* get received frame */
	if(usbdRxBufferEmpty()) {
		return NULL;
	}

	rxElem = usbdRxBufferPop();

	if (rxElem->len > 0) {
		/* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
		p = pbuf_alloc(PBUF_RAW, rxElem->len, PBUF_POOL);
	}

#if 0
	if (p != NULL) {
		byteslefttocopy = q->len;
		bufferOffset = 0;
		for(q = p; q!=NULL && byteslefttocopy>0; q = q->next) {
			memcpy(q->payload, buffer+payloadoffset, )
		}

	}
#else
	if (p != NULL) {
		bufferOffset = 0;

		for (q = p; q != NULL; q = q->next) {
			byteslefttocopy = q->len;
			payloadoffset = 0;

			/* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
			while ((byteslefttocopy + bufferOffset) > RXBufSize) {
				/* Copy data to pbuf */
				memcpy((uint8_t*) ((uint8_t*) q->payload + payloadoffset), (uint8_t*) ((uint8_t*) rxElem->buf + bufferOffset), (RXBufSize - bufferOffset));

				byteslefttocopy = byteslefttocopy - (RXBufSize - bufferOffset);
				payloadoffset = payloadoffset + (RXBufSize - bufferOffset);
				bufferOffset = 0;
			}

			/* Copy remaining data in pbuf */
			memcpy((uint8_t*) ((uint8_t*) q->payload + payloadoffset), (uint8_t*) ((uint8_t*) rxElem->buf + bufferOffset), byteslefttocopy);
			bufferOffset = bufferOffset + byteslefttocopy;
		}
	} else {
		while(1);
	}

	/* Clear Segment_Count */
	hcdc_cdc_rndis->RxState = 0;

	USBD_CDC_RNDIS_ReceivePacket(&USBD_Device);
//	BSP_LED_Toggle(LED_RED);
	//(void) USBD_LL_PrepareReceive(&USBD_Device, CDC_RNDIS_OUT_EP, (uint8_t *) hcdc_cdc_rndis->RxBuffer, hcdc_cdc_rndis->MaxPcktLen);

	/* When Rx Buffer unavailable flag is set: clear it and resume reception */
#endif


	return p;
}

/**
  * @brief This function should be called when a packet is ready to be read
  * from the interface. It uses the function low_level_input() that
  * should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param netif the lwip network interface structure for this ethernetif
  */

void ethernetif_input(struct netif *netif)
{
  err_t err;
  struct pbuf *p;
  
  /* move received packet into a new pbuf */
  p = low_level_input(netif);
    
  /* no packet could be read, silently ignore this */
  if (p == NULL) {
	  return;
  }

  /* entry point to the LwIP stack */
  err = netif->input(p, netif);
    
  if (err != ERR_OK)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
    pbuf_free(p);
    p = NULL;
  }
}

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));
  
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

/**
  * @brief  Returns the current time in milliseconds
  *         when LWIP_TIMERS == 1 and NO_SYS == 1
  * @param  None
  * @retval Current Time value
  */
u32_t sys_now(void)
{
  return HAL_GetTick();
}


/**
  * @brief  Link callback function, this function is called on change of link status
  *         to update low level driver configuration.
* @param  netif: The network interface
  * @retval None
  */
void ethernetif_update_config(struct netif *netif)
{
  //__IO uint32_t tickstart = 0;
  //uint32_t regvalue = 0;
  
  if(netif_is_link_up(netif))
  { 


  }
  else
  {

  }

  ethernetif_notify_conn_changed(netif);
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
__weak void ethernetif_notify_conn_changed(struct netif *netif)
{
  /* NOTE : This is function could be implemented in user file 
            when the callback is needed,
  */  
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
