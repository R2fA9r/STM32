/**
  ******************************************************************************
  * @file    ethernetif.h
  * @author  MCD Application Team
  * @brief   Ethernet interface header file.
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

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

/*
 * THIS is the MAC address of the 'virtual' device that represents the lwip implementation, i.e. the gateway
 * It is DIFFERENT from the MAC address of the device that is created on the host side
 * (see usbd_cdc_rndis_if.h), e.g CDC_RNDIS_MAC_ADDRX
 */
#define ETHNETIF_MAC_ADDR0	0x00
#define ETHNETIF_MAC_ADDR1	0x01
#define ETHNETIF_MAC_ADDR2	0x02
#define ETHNETIF_MAC_ADDR3	0x03
#define ETHNETIF_MAC_ADDR4	0x04
#define ETHNETIF_MAC_ADDR5	0x05

/* Exported types ------------------------------------------------------------*/
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void ethernetif_set_link(struct netif *netif);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);
#endif
